#include <cmath>

#include "glm/gtc/type_ptr.hpp"

#include "graph.hpp"

namespace Music_Visualizer {

    static const std::filesystem::path shaderPath{"shaders/graph"};

    GraphView::GraphView(Keyboard* keyboard, glm::mat4 model, double duration, std::size_t points) : View{shaderPath, keyboard, model}, duration{duration} {
        positionAttrib = glGetAttribLocation(program, "position");
        glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(positionAttrib);

        colorLocation = glGetUniformLocation(program, "color");
        modelLocation = glGetUniformLocation(program, "model");

        for (size_t n = 0; n <= points; n++) {
            this->points.push_back(0);
            this->points.push_back(0);
        }

        glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(GL_FLOAT), this->points.data(), GL_STATIC_DRAW);
    }

    void GraphView::receiveEvent(KeyEvent event) {
        //
    }

    void GraphView::update(double dt) {
        start += dt;

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        for (size_t n = 1; n < points.size() / 2; n++) {
            points[2 * (n - 1)] = points[2 * n] + dt / duration;
            points[2 * (n - 1) + 1] = points[2 * n + 1];
        }

        points[points.size() - 2] = 0;
        points[points.size() - 1] = keyboard->amplitude();

        glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(GL_FLOAT), points.data());
    }

    void GraphView::render() {
        glBindVertexArray(vao);
        glUseProgram(program);

        glUniform3f(colorLocation, 0, 1, 0);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_LINE_STRIP, 0, points.size() / 2);
    }

}
