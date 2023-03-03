#ifndef MUSIC_VISUALIZER_VIEW_HPP
#define MUSIC_VISUALIZER_VIEW_HPP

#include <filesystem>

#include "GL/glew.h"
#include "glm/mat4x4.hpp"

#include "../model/keyboard.hpp"

namespace Music_Visualizer {

    class View {

    protected:

        Keyboard *keyboard;

        glm::mat4 model;

        GLuint vao;
        GLuint vbo;
        GLuint program;

    public:

        View(std::filesystem::path shaders, Keyboard *keyboard, glm::mat4 model);
        ~View();

        virtual void receiveEvent(KeyEvent event) = 0;
        virtual void update(double dt) = 0;
        virtual void render() = 0;

    };

}

#endif //MUSIC_VISUALIZER_VIEW_HPP
