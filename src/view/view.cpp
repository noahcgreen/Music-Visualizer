#include "../shader.hpp"
#include "view.hpp"

namespace Music_Visualizer {

    View::View(std::filesystem::path shaders, Keyboard* keyboard, glm::mat4 model) : keyboard{keyboard}, model{model} {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        program = *createProgram(shaders / "vertex.glsl", shaders / "fragment.glsl");

        keyboard->listen(this);
    }

    View::~View() {
        glDeleteProgram(program);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

}
