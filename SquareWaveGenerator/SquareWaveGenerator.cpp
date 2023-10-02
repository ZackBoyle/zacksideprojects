#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <deque>

#define PI 3.14159265
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define NUM_CIRCLES 5

using namespace std; // haha std

// i dont remember what this one does chatgpt wrote it
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// What you all came here for
void drawCircle(float centerX, float centerY, float radius)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float x = centerX + radius * cos(i * PI / 180.0f);
        float y = centerY + radius * sin(i * PI / 180.0f);
        glVertex2f(x, y);
    }
    glEnd();
}

int main(void)
{
    // kick chicken
    GLFWwindow* window;
    float time = 0;
    float circleXCenter = -0.5;
    float circleYCenter = 0;
    float circleRadius = (4 / PI) / (1.5 * PI);
    deque<float> wave;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Square Wave", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw Initial Circle
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float x = circleXCenter + circleRadius * cos(i * PI / 180.0f);
            float y = circleYCenter + circleRadius * sin(i * PI / 180.0f);
            glVertex2f(x, y);
        }
        glEnd();

        float circumferenceX = circleXCenter + circleRadius * cos(time * PI / 180.0f);
        float circumferenceY = circleYCenter + circleRadius * sin(time * PI / 180.0f);

        // Draw moving radius of the circle
        glBegin(GL_LINES);
            glVertex2f(circleXCenter, circleYCenter);
            glVertex2f(circumferenceX, circumferenceY);
        glEnd();

        float prevCircumferenceX = circumferenceX;
        float prevCircumferenceY = circumferenceY;
        for (int i = 1; i <= NUM_CIRCLES; i++)
        {
            int n = (i * 2) + 1;
            float x;
            float y;

            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 360; i++)
            {
                x = prevCircumferenceX + (circleRadius / n) * cos(i * n * PI / 180.0f);
                y = prevCircumferenceY + (circleRadius / n) * sin(i * n * PI / 180.0f);

                glVertex2f(x, y);
            }
            glEnd();

            // Draw moving radius of the circle
            glBegin(GL_LINES);
                glVertex2f(prevCircumferenceX, prevCircumferenceY);
                glVertex2f(prevCircumferenceX + (circleRadius / n) * cos(time * n * PI / 180.0f), prevCircumferenceY + (circleRadius / n) * sin(time * n * PI / 180.0f));
            glEnd();

            
            if (i == NUM_CIRCLES)
            {
                wave.push_front(prevCircumferenceY + (circleRadius / n) * sin(time * n * PI / 180.0f));
                if (wave.size() > 15000) wave.pop_back();

                glBegin(GL_LINES);
                    glVertex2f(0, wave[0]);
                    glVertex2f(prevCircumferenceX + (circleRadius / n) * cos(time * n * PI / 180.0f), prevCircumferenceY + (circleRadius / n) * sin(time * n * PI / 180.0f));
                glEnd();
            }

            prevCircumferenceX = prevCircumferenceX + (circleRadius / n) * cos(time * n * PI / 180.0f);
            prevCircumferenceY = prevCircumferenceY + (circleRadius / n) * sin(time * n * PI / 180.0f);
        }

        // Draw Sine
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < wave.size(); i++)
        {
            float x = 0.000075 * i;
            float y = wave[i];
            glVertex2f(x, y);
        }
        glEnd();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        time += 0.075;
    }

    glfwTerminate();
    return 0;
}