#ifndef CAR_H
#define CAR_H

#include "texture.h"
#include "utils.h"
#include "camera.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

#include <obj/model.h>

typedef struct Car {
    Model model;
    Model crash_model;
    Model front_right_wheel;
    Model front_left_wheel;
    Model back_wheels;

    GLuint texture;
    GLuint wheel_texture;

    Material car_material;

    vec3 rotation;
    vec3 rotation_speed;
    vec3 position;
    vec3 speed;
    float acceleration;
    bool crash_state;
    bool bump;

    vec3 body_rotation;

    vec3 front_wheel_rotation;
    vec3 front_wheel_rotation_speed;
    vec3 back_wheel_rotation;
    vec3 back_wheel_rotation_speed;

    vec3 steering_rotation;

    bool brake_on;
    bool button_brake_on;
    float brake_speed;

    bool reverse_on;

    vec3 headlight_position;
    bool headlights_on;
    bool party_lights_on;
    vec3 party_lights;
    vec3 headlight_crashed;

    bool camera_follow;

    bool car_started;
    SDL_AudioDeviceID car_start;
} Car;

void init_car(Car *car);

void init_car_objects(Car *car);

void init_car_textures(Car *car);

void update_car(Car *car, Camera *camera, double time);

void render_car(const Car *car);

void render_lights(Car *car);

void toggle_headlight_left(Car *car);

void toggle_headlight_right(Car *car);

void set_car_material(const Material *material);

void set_car_speed_x(Car *car, float value);

void set_car_speed_y(Car *car, float value);

void set_car_speed_z(Car *car, float value);

void set_car_front_wheel_rotation_y_speed(Car *car, float value);

void set_car_front_wheel_rotation_z_speed(Car *car, float value);

void set_car_back_wheel_rotation_z_speed(Car *car, float value);

void set_car_brake_speed(Car *car, float speed);

void set_car_rotation_speed_z(Car *car, float value);

void set_car_wheel_rotation_speed(Car *car, float speed);

void set_car_side_speed(Car *car, float value);

void set_acceleration_speed(Car *car, float speed);

void toggle_headlights(Car *car, bool status);

void set_camera_follow_state(Car *car, bool state);

/**
 * Render the car objects.
 */
void render_car(const Car *car);

#endif /* CAR_H */
