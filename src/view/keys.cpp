#include <array>

#include "glm/gtc/type_ptr.hpp"

#include "keys.hpp"

namespace Music_Visualizer {

    static const std::filesystem::path shaderPath{"shaders/keyboard"};

    KeysView::KeysView(Keyboard *keyboard, glm::mat4 model) : View{shaderPath, keyboard, model} {
        positionAttrib = glGetAttribLocation(program, "position");
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)0);
        glEnableVertexAttribArray(positionAttrib);

        colorAttrib = glGetAttribLocation(program, "color");
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(colorAttrib);

        modelLocation = glGetUniformLocation(program, "model");

        // 6 vertices per key, each with 3 coordinates and 3 colors
        std::array<GLfloat, 88 * 6 * (3 + 3)> keys{};

        for (int i = 0; i < 88; i++) {
            float z = keyboard->getColor(i) == KeyColor::White ? 0.0f : -1.0f;
            float color = keyboard->getColor(i) == KeyColor::White ? 1.0f : 0.0f;
            Rectangle bounds = keyboard->getBounds(i);

            keys[i * 36 + 6 * 0 + 0] = bounds[0].x;
            keys[i * 36 + 6 * 0 + 1] = bounds[0].y;
            keys[i * 36 + 6 * 0 + 2] = z;

            keys[i * 36 + 6 * 0 + 3] = color;
            keys[i * 36 + 6 * 0 + 4] = color;
            keys[i * 36 + 6 * 0 + 5] = color;

            keys[i * 36 + 6 * 1 + 0] = bounds[1].x;
            keys[i * 36 + 6 * 1 + 1] = bounds[1].y;
            keys[i * 36 + 6 * 1 + 2] = z;

            keys[i * 36 + 6 * 1 + 3] = color;
            keys[i * 36 + 6 * 1 + 4] = color;
            keys[i * 36 + 6 * 1 + 5] = color;

            keys[i * 36 + 6 * 2 + 0] = bounds[2].x;
            keys[i * 36 + 6 * 2 + 1] = bounds[2].y;
            keys[i * 36 + 6 * 2 + 2] = z;

            keys[i * 36 + 6 * 2 + 3] = color;
            keys[i * 36 + 6 * 2 + 4] = color;
            keys[i * 36 + 6 * 2 + 5] = color;

            keys[i * 36 + 6 * 3 + 0] = bounds[2].x;
            keys[i * 36 + 6 * 3 + 1] = bounds[2].y;
            keys[i * 36 + 6 * 3 + 2] = z;

            keys[i * 36 + 6 * 3 + 3] = color;
            keys[i * 36 + 6 * 3 + 4] = color;
            keys[i * 36 + 6 * 3 + 5] = color;

            keys[i * 36 + 6 * 4 + 0] = bounds[3].x;
            keys[i * 36 + 6 * 4 + 1] = bounds[3].y;
            keys[i * 36 + 6 * 4 + 2] = z;

            keys[i * 36 + 6 * 4 + 3] = color;
            keys[i * 36 + 6 * 4 + 4] = color;
            keys[i * 36 + 6 * 4 + 5] = color;

            keys[i * 36 + 6 * 5 + 0] = bounds[0].x;
            keys[i * 36 + 6 * 5 + 1] = bounds[0].y;
            keys[i * 36 + 6 * 5 + 2] = z;

            keys[i * 36 + 6 * 5 + 3] = color;
            keys[i * 36 + 6 * 5 + 4] = color;
            keys[i * 36 + 6 * 5 + 5] = color;
        }
        glBufferData(GL_ARRAY_BUFFER, keys.size() * sizeof(GLfloat), keys.data(), GL_STATIC_DRAW);
    }

    void KeysView::update(double dt) {
        //
    }

    void KeysView::render() {
        glBindVertexArray(vao);
        glUseProgram(program);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 88 * 6);
    }

    void KeysView::setKey(size_t key, bool on) {
        GLfloat color[3];
        if (on) {
            if (keyboard->getColor(key) == KeyColor::White) {
                color[0] = 0.f;
                color[1] = 1.f;
                color[2] = 0.f;
            } else {
                color[0] = 0.f;
                color[1] = 0.f;
                color[2] = 1.f;
            }
        } else {
            if (keyboard->getColor(key) == KeyColor::White) {
                color[0] = 1.f;
                color[1] = 1.f;
                color[2] = 1.f;
            } else {
                color[0] = 0.f;
                color[1] = 0.f;
                color[2] = 0.f;
            }
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        for (size_t i = 0; i < 6; i++) {
            glBufferSubData(GL_ARRAY_BUFFER, (key * 36 + 6 * i + 3) * sizeof(GL_FLOAT), 3 * sizeof(GL_FLOAT), color);
        }
    }

    void KeysView::receiveEvent(KeyEvent event) {
        switch (event.type) {
            case KeyEvent::Type::KeyDown:
                setKey(event.key, true);
                break;
            case KeyEvent::Type::KeyUp:
                setKey(event.key, false);
                break;
        }
    }

}
