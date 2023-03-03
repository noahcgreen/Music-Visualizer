#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "src/input/dp.hpp"
#include "src/view/keys.hpp"
#include "src/view/notes.hpp"
#include "src/view/graph.hpp"

int main() {
    // OpenGL setup and window initialization
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Music Visualizer", nullptr, nullptr); // Windowed

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Model/view setup
    Music_Visualizer::Keyboard keyboard;
    Music_Visualizer::DigitalPiano dp{&keyboard};

    // Create and position each view

    float keysHeight = 2.0f / 779.0f * 96.0f;
    float graphHeight = 0.3f;
    // Keys
    glm::mat4 keysModel{1.0f};
    keysModel = glm::translate(keysModel, glm::vec3{-1.f, -1.f + graphHeight, 0.f});
    keysModel = glm::scale(keysModel, glm::vec3{2.0f, keysHeight, 1.f});
    Music_Visualizer::KeysView keys{&keyboard, keysModel};

    // Notes
    glm::mat4 notesModel{1.0f};
    notesModel = glm::translate(notesModel, glm::vec3{-1, -1 + graphHeight + keysHeight, 0});
    notesModel = glm::scale(notesModel, glm::vec3{2, 2 - (graphHeight + keysHeight), 1});
    Music_Visualizer::NotesView notes{&keyboard, notesModel, 3};

    // Frequency graph
    glm::mat4 graphModel{1.0f};
    graphModel = glm::translate(graphModel, glm::vec3{-1, -1 + graphHeight / 2., 0.});
    graphModel = glm::scale(graphModel, glm::vec3{2., graphHeight / 2., 1.});
    Music_Visualizer::GraphView graph{&keyboard, graphModel, 3., 500};

    // Main loop
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double dt = now - lastTime;
        lastTime = now;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update components
        keyboard.update(dt);
        keys.update(dt);
        notes.update(dt);
        graph.update(dt);

        // Render views
        keys.render();
        notes.render();
        graph.render();

        // Draw
        glfwSwapBuffers(window);

        // Handle events (effects are seen next frame)
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        for (int i = 0; i < 9; i++) {
            if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS) {
                keyboard.press(i, 1);
            } else {
                keyboard.release(i);
            }
        }

        dp.poll();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
