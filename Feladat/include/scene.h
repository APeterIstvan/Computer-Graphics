#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "car.h"
#include "texture.h"

#include <SDL2/SDL.h>

#include <obj/model.h>

#define ROAD_NUMBERS 0
#define BARRIER_NUMBERS 15

typedef struct Bridge {
    Model model;
    GLuint texture;
    vec3 position;
} Bridge;

typedef struct Barrier {
    GLuint texture;
    vec3 position;
    float minus_size;
    float plus_size;
} Barrier;

typedef struct Skull {
    Model model;
    GLuint texture;
    vec3 position;
} Skull;

typedef struct Light {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float speed;
} Light;

typedef struct Water {
    Model model;
    GLuint texture;
    vec3 position;
    bool motion_up;
} Water;

typedef struct Scene {
    Water water;

    Model road;
    Bridge bridge[ROAD_NUMBERS];
    Model bridge_model;
    GLuint bridge_texture;

    Barrier barrier[BARRIER_NUMBERS];
    Model barrier_model;

    Skull skull;
    Material skull_material;

    Light light;

    Material material;

    Model mountain_model;
    GLuint mountain_texture;

    Car car;

    GLuint skybox_texture;
    bool fog_state;
    float fog_speed;
    bool help_panel_state;
    GLuint help_panel_texture;
    double uptime;
} Scene;

void load_skybox(Scene scene);

void set_light_speed(Scene *scene, float speed);

void set_fog_speed(Scene *scene, float speed);

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(Scene *scene);

/**
 * Set the current material.
 */
void set_material(const Material *material);

/**
 * Update the scene.
 */
void update_scene(Scene *scene, Camera *camera, Car *car, double time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene *scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void rotate_camera(Camera *camera, double horizontal, double vertical);

void draw_help_panel(GLuint help_panel);

void set_fog_state(Scene *scene, bool state);

void display_help_panel(Scene *scene, bool state);

#endif /* SCENE_H */
