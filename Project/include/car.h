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
    Model windows;
    Model windows_crashed;
    Model headlights;
    Model car_seat;
    Model steering_wheel;
    Model dashboard;
    Model back_seat;

    GLuint texture;
    GLuint wheel_texture;
    GLuint brake_texture;
    GLuint reverse_texture;
    GLuint seat_texture;
    GLuint window_texture;
    GLuint steering_wheel_texture;
    GLuint dashboard_texture;

    GLuint seatList;
    GLuint dashboardList;

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
    float headlight_crashed;

    bool camera_follow;

    bool car_started;
    SDL_AudioDeviceID car_start;
    SDL_AudioDeviceID idling;

    SDL_AudioDeviceID acceleation_sound;
    SDL_AudioDeviceID slow_down_sound;
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;
} Car;

void init_car(Car *car);

void init_car_objects(Car *car);

void init_car_textures(Car *car);

void init_car_values(Car *car);

void init_material_car(Car *car);

void init_car_audio(Car *car);

void init_car_display_lists(Car *car);

void update_car(Car *car, Camera *camera, double time);

void acceleration_forward(Car *car, Camera *camera, double time);

void acceleration_backwards(Car *car, Camera *camera, double time);

void dynamic_slow_down(Car *car, Camera *camera, double time);

void wheel_rotation(Car *car, double time);

void render_car(const Car *car);

void render_body(const Car *car);

void render_wheels(const Car *car);

void render_windows(const Car *car);

void render_headlights(const Car *car);

void render_lights(const Car *car);

void render_car_seat(const Car *car);

void render_back_seat(const Car *car);

void render_steering_wheel(const Car *car);

void render_dashboard(const Car *car);

void toggle_headlight_left(const Car *car);

void toggle_headlight_right(const Car *car);

void set_car_material(const Material *material);

void set_car_front_wheel_rotation_z_speed(Car *car, float value);

void set_car_front_wheel_rotation_y_speed(Car *car, float value);

void set_car_back_wheel_rotation_y_speed(Car *car, float value);

void set_car_rotation_speed_z(Car *car, float value);

void set_car_speed_x(Car *car, float value);

void set_car_speed_y(Car *car, float value);

void set_car_speed_z(Car *car, float value);

void set_acceleration_speed(Car *car, float speed);

void toggle_headlights(Car *car, bool status);

void set_car_brake_speed(Car *car, float speed);

void set_car_wheel_rotation_speed(Car *car, float speed);

void set_camera_follow_state(Car *car, bool state);

#endif /* CAR_H */
