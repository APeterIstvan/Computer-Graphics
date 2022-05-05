#include "car.h"
#include <SDL2/SDL.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>

void init_car(Car *car) {
    init_car_objects(car);
    init_car_textures(car);

    //Camera initialization values
    car->camera_follow = true;

    //Car initialization values
    car->car_material.ambient.red = 0.6f;
    car->car_material.ambient.green = 0.6f;
    car->car_material.ambient.blue = 0.6f;

    car->car_material.diffuse.red = 1.0f;
    car->car_material.diffuse.green = 1.0f;
    car->car_material.diffuse.blue = 1.0f;

    car->car_material.specular.red = 1.0f;
    car->car_material.specular.green = 1.0f;
    car->car_material.specular.blue = 1.0f;

    car->car_material.shininess = 300.0f;

    car->position.x = 0.0;
    car->position.y = 0.0;
    car->position.z = 3.0;

    car->rotation.x = 0.0;
    car->rotation.y = 0.0;
    car->rotation.z = 0.0;

    car->speed.x = 0.0;
    car->speed.y = 0.0;
    car->speed.z = 0.0;

    car->rotation_speed.x = 0.0;
    car->rotation_speed.y = 0.0;
    car->rotation_speed.z = 0.0;

    car->body_rotation.x = 0.0;
    car->body_rotation.y = 0.0;
    car->body_rotation.z = 0.0;

    car->front_wheel_rotation.x = 0.0;
    car->front_wheel_rotation.y = 0.0;
    car->front_wheel_rotation.z = 0.0;

    car->front_wheel_rotation_speed.x = 0.0;
    car->front_wheel_rotation_speed.y = 0.0;
    car->front_wheel_rotation_speed.z = 0.0;

    car->back_wheel_rotation.x = 0.0;
    car->back_wheel_rotation.y = 0.0;
    car->back_wheel_rotation.z = 0.0;

    car->back_wheel_rotation_speed.x = 0.0;
    car->back_wheel_rotation_speed.y = 0.0;
    car->back_wheel_rotation_speed.z = 0.0;

    car->acceleration = 0.0;

    car->steering_rotation.z = 0.0;

    car->brake_speed = 0.0;
    car->brake_on = false;
    car->button_brake_on = false;

    car->crash_state = false;
    car->bump = true;

    car->reverse_on = false;

    car->headlight_position.x = -300.0;
    car->headlight_position.y = 10.0;
    car->headlight_position.z = 3.0;

    car->party_lights.x = 0.0f;
    car->party_lights.y = 0.0f;
    car->party_lights.z = 0.0f;

    car->headlight_crashed = 0.0f;

    car->headlights_on = false;

    car->car_started = false;

    //Audio initialization values
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;

    SDL_LoadWAV("assets/audio/car_start.wav", &wav_spec, &wav_buffer, &wav_length);
    car->car_start = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
    SDL_QueueAudio(car->car_start, wav_buffer, wav_length);
}

void init_car_objects(Car *car) {
    load_model(&(car->model), "assets/models/car.obj");
    load_model(&(car->crash_model), "assets/models/porsche_crashed.obj");
    load_model(&(car->front_right_wheel), "assets/models/front_right_wheel.obj");
    load_model(&(car->front_left_wheel), "assets/models/front_left_wheel.obj");
    load_model(&(car->back_wheels), "assets/models/back_wheels.obj");
    car->texture = load_texture("assets/textures/car.jpg");
}

void init_car_textures(Car *car) {
    car->wheel_texture = load_texture("assets/textures/tyre.jpg");
}

void update_car(Car *car, Camera *camera, double time) {
    //Loading car texture according to the car's state of movement
    if (!car->brake_on && !car->button_brake_on && !car->reverse_on) {
        car->texture = load_texture("assets/textures/car.jpg");
    } else if (car->reverse_on && !car->brake_on) {
        car->texture = load_texture("assets/textures/car_reverse.jpg");
    } else if (car->speed.x > 0) {
        car->brake_on = false;
        car->reverse_on = true;
    } else if (car->speed.x < 0 && car->brake_on && car->button_brake_on) {
        car->texture = load_texture("assets/textures/car_brake.jpg");
    }

    //Acceleration forward - car body and wheels
    if (car->acceleration < 0.0) {
        car->brake_on = true;
        car->speed.x += car->acceleration * time;
        if (car->body_rotation.y < 0.2) {
            car->body_rotation.y += 0.2;
        }
        //printf("%d ", (-1) * (int) car->
        //
        //speed.x / 2);
        car->position.x +=
                cos(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
        car->position.y +=
                sin(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
        car->front_wheel_rotation.y += car->speed.x + car->acceleration * time;
        car->back_wheel_rotation.y += car->speed.x + car->acceleration * time;
        if (car->camera_follow) {
            camera->position.x +=
                    cos(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
            camera->position.y +=
                    sin(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;;
        }
    }

    //Acceleration backwards
    if (car->acceleration > 0.0) {
        if (car->button_brake_on && car->speed.x < 0) {
            car->brake_on = true;
        } else {
            car->reverse_on = true;
        }
        if (car->speed.x > 0.0 && car->speed.x / 2 < 50) {
            car->acceleration = 20;
        } else if (car->speed.x > 50.0 && car->speed.x / 2 < 100) {
            car->acceleration = 20;
        } else if (car->speed.x > 100.0 && car->speed.x / 2 < 300) {
            car->acceleration = 20;
        }
        if (car->body_rotation.y > -1) {
            car->body_rotation.y += -0.5;
        }
        car->speed.x += car->acceleration * time;
        car->position.x +=
                cos(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
        car->position.y +=
                sin(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;;
        car->front_wheel_rotation.y += car->speed.x + car->acceleration * time;
        car->back_wheel_rotation.y += car->speed.x + car->acceleration * time;
        if (car->camera_follow) {
            camera->position.x +=
                    cos(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
            camera->position.y +=
                    sin(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
        }
    }

    //Dynamic slow-down
    if (car->acceleration == 0.0) {
        car->reverse_on = false;
        car->brake_on = false;
        if (car->speed.x < 0) {
            car->speed.x += 20 * time;
            car->position.x +=
                    cos(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
            car->position.y +=
                    sin(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
            car->front_wheel_rotation.y += car->speed.x + car->acceleration * time;
            car->back_wheel_rotation.y += car->speed.x + car->acceleration * time;
            if (car->camera_follow) {
                camera->position.x +=
                        cos(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
                camera->position.y +=
                        sin(degree_to_radian(car->steering_rotation.z)) * car->speed.x * time;
            }
            if (car->body_rotation.y > 0.01 && car->body_rotation.y < 0.5) {
                car->body_rotation.y += -0.2;
            } else if (car->body_rotation.y < -0.01 && car->body_rotation.y >= -1.5) {
                car->body_rotation.y += 0.5;
            }
        } else {
            car->speed.x = 0;
        }
    }

    //Setting car's tilt to default if the car isn't moving
    if (car->speed.x == 0.0 && car->body_rotation.y <= -0.1) {
        car->body_rotation.y += 0.5;
    }

    //Wheel rotation - front and back
    if (car->front_wheel_rotation.z > 40) {
        car->front_wheel_rotation.z = 40;
    } else if (car->front_wheel_rotation.z < -40) {
        car->front_wheel_rotation.z = -40;
    } else {
        car->front_wheel_rotation.z += car->front_wheel_rotation_speed.z * time;
    }
    car->steering_rotation.z += car->front_wheel_rotation_speed.z * time;
    car->back_wheel_rotation.x += car->back_wheel_rotation_speed.x * time;
    car->back_wheel_rotation.z += car->back_wheel_rotation_speed.z * time;


    //Collision detection - road
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

    if(car->position.x > 1240){
        car->position.x = 1240;
        if (car->camera_follow) {
            camera->position.x = 1256;
        }
    }

    //Crashed headlights
    car->headlight_crashed = rand() / (RAND_MAX / (0.2 - 0.0));

    //Party headlights
    car->party_lights.x = rand() / (RAND_MAX / (1.0 - 0.0));
    car->party_lights.y = rand() / (RAND_MAX / (1.0 - 0.0));
    car->party_lights.z = rand() / (RAND_MAX / (1.0 - 0.0));

    //AUDIO
    if (car->car_started) {
        SDL_PauseAudioDevice(car->car_start, 0);
    } else {
        SDL_PauseAudioDevice(car->car_start, 1);
    }
}

void render_car(const Car *car) {
    set_car_material(&(car->car_material));

    render_lights(car);

    glTranslatef(car->position.x, car->position.y, car->position.z);
    glRotatef(car->steering_rotation.z, 0, 0, 1);
    glRotatef(car->rotation.y, 0, 1, 0);
    glTranslatef(-car->position.x, -car->position.y, -car->position.z);

    //Car body
    glPushMatrix();
    glPushMatrix();
    if (car->crash_state == false) {
        glTranslatef(car->position.x - 0.2, car->position.y, car->position.z - 0.5);
        glRotatef(car->body_rotation.x, 1, 0, 0);
        glRotatef(car->body_rotation.y, 0, 1, 0);
        glRotatef(car->body_rotation.z, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, car->texture);
        draw_model(&(car->model));
    } else {
        glTranslatef(car->position.x - 0.2, car->position.y, car->position.z - 2.5);
        glRotatef(car->body_rotation.x, 1, 0, 0);
        glRotatef(car->body_rotation.y, 0, 1, 0);
        glRotatef(car->body_rotation.z, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, car->texture);
        draw_model(&(car->crash_model));
    }

    glPopMatrix();

    glTranslatef(car->position.x, car->position.y, car->position.z);
    glRotatef(car->rotation.z, 0, 0, 1);
    glTranslatef(-car->position.x, -car->position.y, -car->position.z);

    //Right front wheel
    glPushMatrix();
    glTranslatef(car->position.x - 5.4, car->position.y + 3.5, car->position.z - 1.5);
    glRotatef(car->front_wheel_rotation.y, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, car->wheel_texture);
    draw_model(&(car->front_right_wheel));
    glPopMatrix();

    //Left front wheel
    glPushMatrix();
    glTranslatef(car->position.x - 5.4, car->position.y - 3.5, car->position.z - 1.5);
    glRotatef(car->front_wheel_rotation.y, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, car->wheel_texture);
    draw_model(&(car->front_left_wheel));
    glPopMatrix();

    //Back wheels
    glPushMatrix();
    glTranslatef(car->position.x + 4.5, car->position.y, car->position.z - 1.5);
    glRotatef(car->back_wheel_rotation.x, 1, 0, 0);
    glRotatef(car->back_wheel_rotation.y, 0, 1, 0);
    glRotatef(car->back_wheel_rotation.z, 0, 0, 1);
    glBindTexture(GL_TEXTURE_2D, car->wheel_texture);
    draw_model(&(car->back_wheels));
    glPopMatrix();
    glPopMatrix();
}

void render_lights(Car *car) {
    //Left headlight of the car
    glPushMatrix();
    glTranslatef(car->position.x - 5.4, car->position.y + 4.0, car->position.z - 2.5);
    glRotatef(car->steering_rotation.z, 0, 0, 1);
    toggle_headlight_left(car);
    glPopMatrix();

    //Right headlight of the car
    glPushMatrix();
    glTranslatef(car->position.x - 5.4, car->position.y - 4.0, car->position.z - 2.5);
    glRotatef(car->steering_rotation.z, 0, 0, 1);
    toggle_headlight_right(car);
    glPopMatrix();
}

void toggle_headlight_left(Car *car) {
    if (car->headlights_on) {
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }

    GLfloat ambient_light[] = {0, 0, 0, 0};
    GLfloat diffuse_light[4];
    if (!car->crash_state) {
        diffuse_light[0] = 0.2f;
        diffuse_light[1] = 0.2f;
        diffuse_light[2] = 0.2f;
        diffuse_light[3] = 1.0f;
    } else if (car->party_lights_on) {
        diffuse_light[0] = car->party_lights.x;
        diffuse_light[1] = car->party_lights.y;
        diffuse_light[2] = car->party_lights.z;
        diffuse_light[3] = 1.0f;
    } else {
        diffuse_light[0] = car->headlight_crashed;
        diffuse_light[1] = car->headlight_crashed;
        diffuse_light[2] = car->headlight_crashed;
        diffuse_light[3] = 0;
    }
    GLfloat specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat spot_direction[] = {car->headlight_position.x, car->headlight_position.y, car->headlight_position.z};

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 5);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();
}

void toggle_headlight_right(Car *car) {
    if (car->headlights_on) {
        glEnable(GL_LIGHT2);
    } else {
        glDisable(GL_LIGHT2);
    }

    GLfloat ambient_light[] = {0, 0, 0, 0};
    GLfloat diffuse_light[4];
    if (!car->crash_state) {
        diffuse_light[0] = 0.2f;
        diffuse_light[1] = 0.2f;
        diffuse_light[2] = 0.2f;
        diffuse_light[3] = 1.0f;
    } else if (car->party_lights_on) {
        diffuse_light[0] = car->party_lights.x;
        diffuse_light[1] = car->party_lights.y;
        diffuse_light[2] = car->party_lights.z;
        diffuse_light[3] = 1.0f;
    } else {
        diffuse_light[0] = car->headlight_crashed;
        diffuse_light[1] = car->headlight_crashed;
        diffuse_light[2] = car->headlight_crashed;
        diffuse_light[3] = 0;
    }
    GLfloat specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat spot_direction[] = {car->headlight_position.x, car->headlight_position.y, car->headlight_position.z};


    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular_light);
    glPushMatrix();
    glLightfv(GL_LIGHT2, GL_POSITION, position);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 4);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();
}

void set_car_material(const Material *material) {
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

void set_car_front_wheel_rotation_z_speed(Car *car, float value) {
    car->front_wheel_rotation_speed.z = value;
}

void set_car_front_wheel_rotation_y_speed(Car *car, float value) {
    car->front_wheel_rotation_speed.y = value;
}

void set_car_back_wheel_rotation_y_speed(Car *car, float value) {
    car->back_wheel_rotation_speed.y = value;
}

void set_car_rotation_speed_z(Car *car, float value) {
    car->rotation_speed.z = value;
}

void set_car_speed_x(Car *car, float speed) {
    car->speed.x = speed;
}

void set_car_speed_y(Car *car, float value) {
    car->speed.y = value;
}

void set_car_speed_z(Car *car, float value) {
    car->speed.z = value;
}

void set_acceleration_speed(Car *car, float speed) {
    car->acceleration = speed;
}

void toggle_headlights(Car *car, bool status) {
    car->headlights_on = status;
}

void set_car_brake_speed(Car *car, float speed) {
    car->brake_speed = speed;
}

void set_camera_follow_state(Car *car, bool state) {
    car->camera_follow = state;
}
