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

void draw_shape(float x, float y, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex3f(-SHAPE_SIZE, -SHAPE_SIZE, 0.f);
    glVertex3f(SHAPE_SIZE, -SHAPE_SIZE, 0.f);
    glVertex3f(SHAPE_SIZE, SHAPE_SIZE, 0.f);
    glVertex3f(-SHAPE_SIZE, SHAPE_SIZE, 0.f);
    glEnd();
    glPopMatrix();
}

float get_distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

void update_position(float *x, float *y, float *x_speed, float *y_speed)
{
    *x += *x_speed;
    *y += *y_speed;
    if (*x + SHAPE_SIZE > 1.f || *x - SHAPE_SIZE < -1.f)
    {
        *x_speed = -*x_speed;
    }
    if (*y + SHAPE_SIZE > 1.f || *y - SHAPE_SIZE < -1.f)
    {
        *y_speed = -*y_speed;
    }
}

void set_shape_size(float size) {
    SHAPE_SIZE = size;
}

int collisions = 0;

void init_position(float *x1, float *y1, float *x2, float *y2);

void handle_collision(float *x1, float *y1, float *x_speed1, float *y_speed1,
                      float *x2, float *y2, float *x_speed2, float *y_speed2)
{
    float dist = get_distance(*x1, *y1, *x2, *y2);
    if (dist < SHAPE_SIZE * 2)
    {
        float temp_x_speed = *x_speed1;
        float temp_y_speed = *y_speed1;
        *x_speed1 = *x_speed2;
        *y_speed1 = *y_speed2;
        *x_speed2 = temp_x_speed;
        *y_speed2 = temp_y_speed;
        collisions++;
    }
}

void init_speeds(float *x_speed1, float *y_speed1, float *x_speed2, float *y_speed2, float speed_coef)
{
    *x_speed1 = speed_coef*.01f;
    *y_speed1 = speed_coef*.015f;
    *x_speed2 = speed_coef*-.015f;
    *y_speed2 = speed_coef*-.01f;
}

void init_position(float *x1, float *y1, float *x2, float *y2) {
    (*x1) = -0.5f + SHAPE_SIZE + ((float)rand() / RAND_MAX) * 0.5f;
    (*y1) = -0.5f + SHAPE_SIZE + ((float)rand() / RAND_MAX) * 0.5f;
    (*x2) = 0.5f + SHAPE_SIZE - ((float)rand() / RAND_MAX) * 0.5f;
    (*y2) = 0.5f + SHAPE_SIZE - ((float)rand() / RAND_MAX) * 0.5f;
}


int main(void)
{
    srand(time(NULL));
    float x_speed1, y_speed1, x_speed2, y_speed2;
    init_speeds(&x_speed1, &y_speed1, &x_speed2, &y_speed2, 0.5f);
    float x1, y1, x2, y2;
    init_position(&x1, &y1, &x2, &y2);
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
        draw_shape(x1, y1, 1.f, 0.f, 0.f);
        draw_shape(x2, y2, 0.f, 0.f, 1.f);
        update_position(&x1, &y1, &x_speed1, &y_speed1);
        update_position(&x2, &y2, &x_speed2, &y_speed2);
        handle_collision(&x1, &y1, &x_speed1, &y_speed1, &x2, &y2, &x_speed2, &y_speed2);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#endif