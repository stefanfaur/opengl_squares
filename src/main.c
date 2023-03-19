#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

float SHAPE_SIZE = 0.2f;

void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

typedef struct shape_t {
    float x;
    float y;
    float x_speed;
    float y_speed;
}shape_t;

shape_t create_shape(float x, float y, float x_speed, float y_speed) {
    struct shape_t new_shape;
    new_shape.x = x;
    new_shape.y = y;
    new_shape.x_speed = x_speed;
    new_shape.y_speed = y_speed;
    return new_shape;
}


void set_window_size(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width / (float)height, width / (float)height, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void draw_shape(shape_t sh, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(sh.x, sh.y, 0.f);
    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex3f(-SHAPE_SIZE, -SHAPE_SIZE, 0.f);
    glVertex3f(SHAPE_SIZE, -SHAPE_SIZE, 0.f);
    glVertex3f(SHAPE_SIZE, SHAPE_SIZE, 0.f);
    glVertex3f(-SHAPE_SIZE, SHAPE_SIZE, 0.f);
    glEnd();
    glPopMatrix();
}

float get_distance(shape_t sh1, shape_t sh2)
{
    float dx = sh2.x - sh1.x;
    float dy = sh2.y - sh1.y;
    return sqrt(dx * dx + dy * dy);
}

void update_position(shape_t *sh)
{
    sh->x += sh->x_speed;
    sh->y += sh->y_speed;
    if (sh->x + SHAPE_SIZE > 1.f || sh->x - SHAPE_SIZE < -1.f)
    {
        sh->x_speed = -sh->x_speed;
    }
    if (sh->y + SHAPE_SIZE > 1.f || sh->y - SHAPE_SIZE < -1.f)
    {
        sh->y_speed = -sh->y_speed;
    }
}

void set_shape_size(float size) {
    SHAPE_SIZE = size;
}

int collisions = 0;


void handle_collisions(shape_t *sh1, shape_t *sh2) {
    float dist = get_distance(*sh1, *sh2);
    if (dist < SHAPE_SIZE * 2)
    {
        float temp_x_speed = sh1->x_speed;
        float temp_y_speed = sh1->y_speed;
        sh1->x_speed = sh2->x_speed;
        sh1->y_speed = sh2->y_speed;
        sh2->x_speed = temp_x_speed;
        sh2->y_speed = temp_y_speed;
        collisions++;
    }
}

void init_speeds(shape_t *sh1, shape_t *sh2, float multiplier) {
    sh1->x_speed = multiplier*.01f;
    sh1->y_speed = multiplier*.015f;
    sh2->x_speed = multiplier*-.015f;
    sh2->y_speed = multiplier*-.01f;
}

void init_position(shape_t *sh1, shape_t *sh2) {
    sh1->x = -0.5f + SHAPE_SIZE + ((float)rand() / RAND_MAX) * 0.5f;
    sh1->y = -0.5f + SHAPE_SIZE + ((float)rand() / RAND_MAX) * 0.5f;
    sh2->x = 0.5f + SHAPE_SIZE - ((float)rand() / RAND_MAX) * 0.5f;
    sh2->y = 0.5f + SHAPE_SIZE - ((float)rand() / RAND_MAX) * 0.5f;
}


int main(void)
{
    srand(time(NULL));
    shape_t sh1 = create_shape(0.f, 0.f, 0.01f, 0.015f);
    shape_t sh2 = create_shape(0.f, 0.f, -0.015f, -0.01f);
    init_speeds(&sh1, &sh2, 1.f);
    init_speeds(&sh1, &sh2, 1.f);
    init_position(&sh1, &sh2);
    set_shape_size(0.1f);

    GLFWwindow *window = NULL;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(1000, 1000, "Mergi oare?", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        set_window_size(window, width, height);
        draw_shape(sh1, 1.f, 0.f, 0.f);
        draw_shape(sh2, 0.f, 0.f, 1.f);
        update_position(&sh1);
        update_position(&sh2);
        handle_collisions(&sh1, &sh2);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#endif