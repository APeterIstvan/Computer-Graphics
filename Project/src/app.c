#include "app.h"

#include <SDL2/SDL_image.h>

void init_app(App *app, int width, int height) {
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
            "Fast Car Go Vroom! - A4XIOV",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_car(&(app->car));
    init_scene(&(app->scene));


    app->is_running = true;
    app->uptime = 0.0;
}

void init_opengl() {
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.2, 0.7, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height) {
    int x, y, w, h;
    double ratio;

    ratio = (double) width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int) ((double) height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    } else {
        w = width;
        h = (int) ((double) width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //view distance (last value)
    glFrustum(
            -.08, .08,
            -.06, .06,
            .1, 15000
    );
}

void handle_app_events(App *app) {
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        app->is_running = false;
                        break;
                    case SDL_SCANCODE_W:
                        set_camera_speed(&(app->camera), 20);
                        break;
                    case SDL_SCANCODE_S:
                        set_camera_speed(&(app->camera), -20);
                        break;
                    case SDL_SCANCODE_A:
                        set_camera_side_speed(&(app->camera), 20);
                        break;
                    case SDL_SCANCODE_D:
                        set_camera_side_speed(&(app->camera), -20);
                        break;
                    case SDL_SCANCODE_C:
                        app->camera.position.x = app->car.position.x + 16;
                        app->camera.position.y = app->car.position.y;
                        app->camera.position.z = 8.0;
                        app->camera.rotation.x = -16.0;
                        app->camera.rotation.y = 0.0;
                        app->camera.rotation.z = 180.0;
                        break;
                    case SDL_SCANCODE_E:
                        app->car.car_started = !app->car.car_started;
                        break;
                    case SDL_SCANCODE_Q:
                        toggle_headlights(&(app->car), !(app->car.headlights_on));
                        break;
                    case SDL_SCANCODE_UP:
                        set_acceleration_speed(&(app->car), -25);
                        set_car_front_wheel_rotation_y_speed(&(app->car), -200);
                        set_car_back_wheel_rotation_y_speed(&(app->car), -200);
                        break;
                    case SDL_SCANCODE_DOWN:
                        set_acceleration_speed(&(app->car), 75);
                        set_car_front_wheel_rotation_y_speed(&(app->car), 200.0);
                        set_car_back_wheel_rotation_y_speed(&(app->car), 200);
                        app->car.bump = true;
                        app->car.button_brake_on = true;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        set_car_speed_y(&(app->car), 10);
                        set_car_front_wheel_rotation_z_speed(&(app->car), -20);
                        break;
                    case SDL_SCANCODE_LEFT:
                        set_car_speed_y(&(app->car), -10);
                        set_car_front_wheel_rotation_z_speed(&(app->car), 20);
                        break;
                    case SDL_SCANCODE_SPACE:
                        set_camera_vertical_speed(&(app->camera), 20);
                        set_car_brake_speed(&(app->car), 40);
                        break;
                    case SDL_SCANCODE_LSHIFT:
                        set_camera_vertical_speed(&(app->camera), -20);
                        break;
                    case SDL_SCANCODE_F1:
                        display_help_panel(&(app->scene), !(app->scene.help_panel_state));
                        break;
                    case SDL_SCANCODE_F2:
                        set_light_speed(&(app->scene), -0.1);
                        set_fog_speed(&(app->scene), -0.05);
                        break;
                    case SDL_SCANCODE_F3:
                        set_light_speed(&(app->scene), 0.1);
                        set_fog_speed(&(app->scene), 0.05);
                        break;
                    case SDL_SCANCODE_F4:
                        set_camera_follow_state(&(app->car), !(app->car.camera_follow));
                        break;
                    case SDL_SCANCODE_F5:
                        set_fog_state(&(app->scene), !(app->scene.fog_state));
                        break;
                    case SDL_SCANCODE_R:
                        app->camera.position.x = 16.0;
                        app->camera.position.y = 0.0;
                        app->camera.position.z = 8.0;
                        app->camera.rotation.x = -16.0;
                        app->camera.rotation.y = 0.0;
                        app->camera.rotation.z = 180.0;
                        app->car.position.x = 0.0;
                        app->car.rotation.x = 0.0;
                        app->car.rotation.y = 0.0;
                        app->car.rotation.z = 0.0;
                        app->car.position.y = 0.0;
                        app->car.acceleration = 0.0;
                        app->car.speed.x = 0.0;
                        app->car.speed.y = 0.0;
                        app->car.speed.z = 0.0;
                        app->car.bump = true;
                        app->car.crash_state = false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_S:
                        set_camera_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_D:
                        set_camera_side_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_DOWN:
                        set_acceleration_speed(&(app->car), 0);
                        set_car_back_wheel_rotation_y_speed(&(app->car), 0);
                        set_car_front_wheel_rotation_y_speed(&(app->car), 0.0);
                        app->car.button_brake_on = false;
                        break;
                    case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_LEFT:
                        set_car_speed_y(&(app->car), 0.0);
                        set_car_front_wheel_rotation_z_speed(&(app->car), 0.0);
                        break;
                    case SDL_SCANCODE_LSHIFT:
                    case SDL_SCANCODE_SPACE:
                        set_camera_vertical_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_F2:
                    case SDL_SCANCODE_F3:
                        set_light_speed(&(app->scene), 0.0);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                is_mouse_down = true;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
                mouse_x = x;
                mouse_y = y;
                break;
            case SDL_MOUSEBUTTONUP:
                is_mouse_down = false;
                break;
            case SDL_QUIT:
                app->is_running = false;
                break;
            default:
                break;
        }
    }
}

void update_app(App *app) {
    double current_time;
    double elapsed_time;

    current_time = (double) SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_car(&(app->car), &(app->camera), elapsed_time);
    update_scene(&(app->scene), &(app->camera), &(app->car), elapsed_time);

}

void render_app(App *app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    render_car(&(app->car));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app) {
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
