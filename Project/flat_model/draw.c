#include "draw.h"

#include <GL/glut.h>

#include <stdio.h>

void draw_triangles(const struct Model *model) {
    int i, k;
    int vertex_index, texture_index, normal_index;
    double x, y, z, u, v;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {
        for (k = 0; k < 3; ++k) {
            normal_index = model->triangles[i].points[k].normal_index;
            x = model->normals[normal_index].x;
            y = model->normals[normal_index].y;
            z = model->normals[normal_index].z;
            glNormal3d(x, y, z);

            // DEBUG !
            printf("normal: %d\n", normal_index);
            printf("[%lf, %lf, %lf]\n", x, y, z);

            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3d(x, y, z);
        }
    }

    glEnd();
}

void draw_quads(const struct Model *model) {
    int i, k;
    int vertex_index, texture_index, normal_index;
    double x, y, z, u, v;

    glBegin(GL_QUADS);

    for (i = 0; i < model->n_quads; ++i) {
        for (k = 0; k < 4; ++k) {
            vertex_index = model->quads[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3d(x, y, z);
        }
    }

    glEnd();
}

void draw_model(const struct Model *model) {
    draw_triangles(model);
    draw_quads(model);
}

