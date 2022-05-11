#include "scene.h"
#include "car.h"
#include <SDL2/SDL.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>

void init_scene(Scene *scene) {
    init_objects(scene);

    init_textures(scene);

    init_bridges(scene);

    init_obstacles(scene);

    init_material_scene(scene);

    init_material_skull(scene);

    init_water(scene);

    init_light(scene);

    init_fog(scene);

    scene->help_panel_state = false;

    init_audio(scene);

    init_display_lists(scene);
}

void init_objects(Scene *scene) {
    load_model(&(scene->bridge_model), "assets/models/tower_bridge.obj");
    load_model(&(scene->barrier_model), "assets/models/barrier.obj");
    load_model(&(scene->road), "assets/models/tower_bridge.obj");
    load_model(&(scene->water.model), "assets/models/water.obj");
    load_model(&(scene->skull.model), "assets/models/skull.obj");
    load_model(&(scene->mountain_model), "assets/models/mountain.obj");
}

void init_textures(Scene *scene) {
    scene->bridge_texture = load_texture("assets/textures/tower_bridge.jpg");
    scene->water.texture = load_texture("assets/textures/water3.jpg");
    scene->help_panel_texture = load_texture("assets/textures/help.jpg");
    scene->skybox_texture = load_texture("assets/textures/sky.jpg");
    scene->skull.texture = load_texture("assets/textures/skull_texture.jpg");
    scene->mountain_texture = load_texture("assets/textures/mountain2.jpg");
}

void init_display_lists(Scene *scene) {
    scene->skullList = glGenLists(1);
    glNewList(scene->skullList, GL_COMPILE);
    render_skull(scene);
    glEndList();

    scene->bridgeList = glGenLists(1);
    glNewList(scene->bridgeList, GL_COMPILE);
    render_init_bridge(scene);
    render_road(scene);
    glEndList();

    scene->barrierList = glGenLists(1);
    glNewList(scene->barrierList, GL_COMPILE);
    render_barrier(scene);
    glEndList();

    scene->waterList = glGenLists(1);
    glNewList(scene->waterList, GL_COMPILE);
    render_water(scene);
    glEndList();

    scene->mountainList = glGenLists(1);
    glNewList(scene->mountainList, GL_COMPILE);
    render_mountain(scene);
    glEndList();
}

void set_lighting(const Scene *scene) {

    if (scene->fog_state) {
        glEnable(GL_FOG);
        GLfloat fogColor[] = {scene->fog_speed, scene->fog_speed, scene->fog_speed, 1};
        glHint(GL_FOG_HINT, GL_NICEST);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_END, 700.0f);
    } else {
        glDisable(GL_FOG);
    }

    float ambient_light[] = {scene->light.ambient[0], scene->light.ambient[1], scene->light.ambient[2],
                             scene->light.ambient[3]};
    float diffuse_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float position[] = {0.0f, 0.0f, 1.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void init_bridges(Scene *scene) {
    float curr_pos_minus = -2499;
    for (int i = 0; i < ROAD_NUMBERS; i++) {
        scene->bridge[i].texture = load_texture("assets/textures/tower_bridge.jpg");
        scene->bridge[i].position.x = curr_pos_minus;
        curr_pos_minus = scene->bridge[i].position.x - 2499;
    }
}

void init_obstacles(Scene *scene) {
    float curr_pos = -50;
    for (int i = 0; i < BARRIER_NUMBERS; i++) {
        scene->barrier[i].texture = load_texture("assets/textures/barrier.jpg");
        scene->barrier[i].position.x = curr_pos;
        scene->barrier[i].minus_size = 30;
        scene->barrier[i].plus_size = -10;
        curr_pos = scene->barrier[i].position.x - rand() % 100 - 70;
        if (i % 2) {
            scene->barrier[i].position.y = 10 - rand() % 3 - 1;
        } else {
            scene->barrier[i].position.y = -10 + rand() % 3 - 1;
        }
    }
}

void init_material_scene(Scene *scene) {
    scene->material.ambient.red = 2.0;
    scene->material.ambient.green = 2.0;
    scene->material.ambient.blue = 2.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 3.0;
}

void init_material_skull(Scene *scene) {
    scene->skull_material.ambient.red = 0.25f;
    scene->skull_material.ambient.green = 0.20725f;
    scene->skull_material.ambient.blue = 0.20725f;

    scene->skull_material.diffuse.red = 1.0;
    scene->skull_material.diffuse.green = 0.829f;
    scene->skull_material.diffuse.blue = 0.829f;

    scene->skull_material.specular.red = 0.296648f;
    scene->skull_material.specular.green = 0.296648f;
    scene->skull_material.specular.blue = 0.296648f;

    scene->skull_material.shininess = 11.264f;
}

void init_water(Scene *scene) {
    scene->water.position.x = -1500;
    scene->water.position.y = 0.0;
    scene->water.position.z = -65;

    scene->water.motion_up = true;
}

void init_light(Scene *scene) {
    scene->light.ambient[0] = 0.15f;
    scene->light.ambient[1] = 0.15f;
    scene->light.ambient[2] = 0.15f;
    scene->light.ambient[3] = 1.0f;

    scene->light.speed = 0.0;
}

void init_audio(Scene *scene) {
    SDL_LoadWAV("assets/audio/crash.wav", &scene->wav_spec, &scene->wav_buffer, &scene->wav_length);
    scene->crash_sound = SDL_OpenAudioDevice(NULL, 0, &scene->wav_spec, NULL, 0);
    //SDL_QueueAudio(scene->crash_sound, scene->wav_buffer, scene->wav_length);
}

void init_fog(Scene *scene) {
    scene->fog_speed = 0.8;
    scene->fog_state = true;
}

void set_material(const Material *material) {
    float ambient_material_color[] = {
            material->ambient.red,
            material->ambient.green,
            material->ambient.blue
    };

    float diffuse_material_color[] = {
            material->diffuse.red,
            material->diffuse.green,
            material->diffuse.blue
    };

    float specular_material_color[] = {
            material->specular.red,
            material->specular.green,
            material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene *scene, Camera *camera, Car *car, double time) {

    //Light control
    for (int i = 0; i < 4; i++) {
        scene->light.ambient[i] += scene->light.speed * time;
    }

    road_collision_detection(car, camera);

    obstacle_collision_detection(scene, car, time);

    /*if(car->acceleration != 0 || car->speed.x != 0){
        SDL_ClearQueuedAudio(scene->crash_sound);
    }*/

    finish_line(scene, car, camera);

    water_motion(scene, time);
}

void road_collision_detection(Car *car, Camera *camera) {
    if (car->position.y < -12) {
        car->position.y = -12;
        if (car->camera_follow) {
            camera->position.y = -12;
        }
    } else if (car->position.y > 12) {
        car->position.y = 12;
        if (car->camera_follow) {
            camera->position.y = 12;
        }
    }

    if (car->position.x > 1240) {
        car->position.x = 1240;
        car->speed.x = 0;
        car->acceleration = 0;
        if (car->camera_follow) {
            camera->position.x = 1256;
        }
    }
}

void obstacle_collision_detection(Scene *scene, Car *car, double time) {
    float curr_pos_y;
    for (int i = 0; i < BARRIER_NUMBERS; i++) {
        if (i % 2) {
            if (scene->barrier[i].position.x + scene->barrier[i].plus_size <= car->position.x &&
                scene->barrier[i].position.x + scene->barrier[i].minus_size >= car->position.x &&
                car->position.x - 10 <= scene->barrier[i].position.x
                && scene->barrier[i].position.y + 16 >= car->position.y &&
                car->position.y + 8 >= scene->barrier[i].position.y) {
                /*SDL_QueueAudio(scene->crash_sound, scene->wav_buffer, scene->wav_length);
                SDL_PauseAudioDevice(scene->crash_sound, 0);
               // SDL_ClearQueuedAudio(scene->crash_sound);*/
                car->speed.x = 0;

                car->crash_state = true;
                curr_pos_y = car->position.y;
                car->position.y = curr_pos_y;
                if (car->rotation.y >= -2 && car->bump == true) {
                    car->rotation.y += -25 * time;
                } else {
                    car->bump = false;
                    car->rotation.y += 10 * time;
                    if (car->rotation.y > 0 && car->bump == false) {
                        car->rotation.y = 0.0;
                    }
                }
                if (car->position.x < scene->barrier[i].position.x + scene->barrier[i].minus_size) {
                    car->position.x = scene->barrier[i].position.x + 10;

                    car->acceleration = 0;

                    /*if (camera->position.x > car->position.x + 16) {
                        camera->position.x += camera->speed.x * time;
                    } else {
                        camera->position.x = car->position.x + 16;
                    }*/
                }
            }
        } else if (scene->barrier[i].position.x + scene->barrier[i].plus_size <= car->position.x &&
                   scene->barrier[i].position.x + scene->barrier[i].minus_size >= car->position.x &&
                   car->position.x - 10 <= scene->barrier[i].position.x
                   && scene->barrier[i].position.y - 16 <= car->position.y &&
                   car->position.y - 8 <= scene->barrier[i].position.y) {
            /* SDL_QueueAudio(scene->crash_sound, scene->wav_buffer, scene->wav_length);
             SDL_PauseAudioDevice(scene->crash_sound, 0);
             SDL_ClearQueuedAudio(scene->crash_sound);*/
            car->speed.x = 0;

            car->crash_state = true;
            curr_pos_y = car->position.y;
            car->position.y = curr_pos_y;
            if (car->rotation.y >= -2 && car->bump == true) {
                car->rotation.y += -25 * time;
            } else {
                car->bump = false;
                car->rotation.y += 10 * time;
                if (car->rotation.y > 0 && car->bump == false) {
                    car->rotation.y = 0.0;
                }
            }
            if (car->position.x < scene->barrier[i].position.x + scene->barrier[i].minus_size) {
                car->position.x = scene->barrier[i].position.x + 10;

                car->acceleration = 0;
            }
            //camera->position.x += camera->speed.x * time;
            /*if(car->camera_follow) {
                printf("%f ", camera->position.x);
                camera->position.x = cam_cur_pos;
                camera->position.y += 0;
                camera->position.z += 0;
            }*/
        }
    }
}

void finish_line(Scene *scene, Car *car, Camera *camera) {
    if (car->position.x <= scene->bridge[ROAD_NUMBERS - 1].position.x - 1240 - 10) {
        car->position.x = 0;
        car->position.y = 0;
        car->speed.x = 0;
        camera->position.x = 16;
        camera->position.y = 0;
        camera->speed.x = 0;
        camera->speed.y = 0;
        car->crash_state = false;
    }
}

void water_motion(Scene *scene, double time) {
    if (scene->water.position.z <= -40 && scene->water.motion_up == true) {
        scene->water.position.z += 4 * time;
    } else {
        scene->water.motion_up = false;
        scene->water.position.z += -4 * time;
        if (scene->water.position.z <= -50 && scene->water.motion_up == false) {
            scene->water.motion_up = true;
            scene->water.position.z += 4 * time;
        }
    }
}

void render_scene(const Scene *scene) {
    //Help panel status
    if (scene->help_panel_state) {
        draw_help_panel(scene->help_panel_texture);
    }

    //General lighting of the scene
    set_lighting(scene);

    set_material(&(scene->material));

    //Skybox
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->skybox_texture);
    load_skybox();
    glPopMatrix();

    glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f);
    glTranslatef(scene->water.position.x, scene->water.position.y, scene->water.position.z);
    glCallList(scene->waterList);
    glPopMatrix();

    set_material(&(scene->skull_material));

    glCallList(scene->skullList);
    set_material(&(scene->material));

    glPushMatrix();
    glTranslatef(-20000.0f, -15000.0f, 1000.0f);
    glCallList(scene->mountainList);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20000.0f, 15000.0f, 800.0f);
    glRotatef(180, 0.0f, 0.0f, 1.0f);
    glCallList(scene->mountainList);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -25000.0f, 800.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glCallList(scene->mountainList);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15000.0f, 25000.0f, 200.0f);
    glRotatef(225, 0.0f, 0.0f, 1.0f);
    glCallList(scene->mountainList);
    glPopMatrix();

    glCallList(scene->bridgeList);
    glCallList(scene->barrierList);

    set_material(&(scene->material));
}

void render_barrier(const Scene *scene) {
    glTranslatef(0, 0, 0);
    for (int i = 0; i < BARRIER_NUMBERS; i++) {
        glPushMatrix();
        glTranslatef(scene->barrier[i].position.x, scene->barrier[i].position.y, 1.70);
        glRotatef(180, 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, scene->barrier[i].texture);
        draw_model(&(scene->barrier_model));
        glPopMatrix();
    }
}

void render_skull(const Scene *scene) {
    glPushMatrix();
    glTranslatef(scene->bridge[ROAD_NUMBERS - 1].position.x - 1260, 0, 40);
    glBindTexture(GL_TEXTURE_2D, scene->skull.texture);
    //glBegin(GL_TRIANGLES);
    draw_model(&(scene->skull.model));
    //glEnd();
    glPopMatrix();
}

void render_water(const Scene *scene) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->water.texture);
    glBegin(GL_TRIANGLES);
    draw_model(&(scene->water.model));
    glEnd();
    glPopMatrix();
}

void render_road(const Scene *scene) {
    glTranslatef(0, 0, 0);
    for (int i = 0; i < ROAD_NUMBERS; i++) {
        glPushMatrix();
        glTranslatef(scene->bridge[i].position.x, 0, 95);
        glBegin(GL_TRIANGLES);
        draw_model(&(scene->bridge_model));
        glPopMatrix();
    }
}

void render_init_bridge(const Scene *scene) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->bridge_texture);
    glTranslatef(0, 0, 95);
    draw_model(&(scene->road));
    glPopMatrix();
}

void render_mountain(const Scene *scene) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->mountain_texture);
    draw_model(&(scene->mountain_model));
    glPopMatrix();
}

void draw_help_panel(GLuint help_panel) {
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, help_panel);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3d(-4, 3, -5);

    glTexCoord2f(1, 0);
    glVertex3d(4, 3, -5);

    glTexCoord2f(1, 1);
    glVertex3d(4, -3, -5);

    glTexCoord2f(0, 1);
    glVertex3d(-4, -3, -5);
    glEnd();

    glEnable(GL_LIGHTING);
}

void load_skybox() {
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;
    int n_slices, n_stacks;
    double radius;
    int i, k;
    n_slices = 60;
    n_stacks = 60;
    radius = 40000;

    glPushMatrix();

    glScaled(radius, radius, radius);

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double) i / n_stacks;
        v2 = (double) (i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double) k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1) - 0.25;
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2) - 0.25;
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);
        }
    }

    glEnd();

    glPopMatrix();
}

void draw_origin() {
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 20);

    glEnd();
}

void set_light_speed(Scene *scene, float speed) {
    scene->light.speed = speed;
}

void set_fog_speed(Scene *scene, float speed) {
    scene->fog_speed += speed;
}

void set_fog_state(Scene *scene, bool state) {
    scene->fog_state = state;
}

void display_help_panel(Scene *scene, bool state) {
    scene->help_panel_state = state;
}