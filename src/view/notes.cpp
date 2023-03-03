#include <algorithm>
#include <filesystem>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "notes.hpp"

namespace Music_Visualizer {

    static const std::filesystem::path shaderPath{"shaders/notes"};

    NotesView::NotesView(Keyboard* keyboard, glm::mat4 model, double duration) : View{shaderPath, keyboard, model}, duration{duration} {
        positionAttrib = glGetAttribLocation(program, "position");
//        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)0);
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(positionAttrib);

        colorLocation = glGetUniformLocation(program, "color");

//        colorAttrib = glGetAttribLocation(program, "color");
//        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
//        glEnableVertexAttribArray(colorAttrib);

        modelLocation = glGetUniformLocation(program, "model");

        std::array<GLfloat, 6 * 6> vertices = {
                0, 0, 0,// 1, 0, 0,
                1, 0, 0,// 1, 0, 0,
                1, 1, 0,// 1, 0, 0,

                1, 1, 0,// 1, 0, 0,
                0, 1, 0,// 1, 0, 0,
                0, 0, 0,// 1, 0, 0
        };
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    }

    void NotesView::receiveEvent(KeyEvent event) {
        switch (event.type) {
            case KeyEvent::Type::KeyDown: {
                notes[event.key].push_back(Note{start, std::nullopt, event.velocity});
                break;
            }
            case KeyEvent::Type::KeyUp:
                for (auto& note : notes[event.key]) {
                    if (!note.end.has_value()) {
                        note.end = start;
                    }
                }
                break;
        }
    }

    void NotesView::update(double dt) {
        start += dt;
        for (size_t key = 0; key < 88; key++) {
            std::vector<Note> filtered;
            for (auto& note : notes[key]) {
                if (start - note.end.value_or(start) < duration) {
                    filtered.push_back(note);
                }
            }
            notes[key] = filtered;
        }
    }

    void NotesView::render() {
        glBindVertexArray(vao);
        glUseProgram(program);

        for (size_t key = 0; key < 88; key++) {
            for (auto& note : notes[key]) {
                double offset = (start - note.end.value_or(start)) / duration;
                double length = std::clamp((note.end.value_or(start) - note.start) / duration, 0., 1. - offset);

                Rectangle r = keyboard->getBounds(key);
                glm::mat4 noteModel = model;
                noteModel = glm::translate(noteModel, glm::vec3{r[0].x, offset, 0});
                noteModel = glm::scale(noteModel, glm::vec3{r[2].x - r[0].x, length, 1});

                double velocity = std::clamp(note.velocity, 0.2, 0.8);
                glUniform3f(colorLocation, velocity, 0, 0);
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(noteModel));
                glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
            }
        }
    }

}
