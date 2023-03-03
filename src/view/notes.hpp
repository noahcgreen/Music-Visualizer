#ifndef MUSIC_VISUALIZER_NOTES_HPP
#define MUSIC_VISUALIZER_NOTES_HPP

#include <array>
#include <optional>
#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "../model/keyboard.hpp"

#include "view.hpp"

namespace Music_Visualizer {

    class NotesView : public View {

        struct Note {
            double start;
            std::optional<double> end;
            double velocity;
        };

    private:

        std::array<std::vector<Note>, 88> notes;
        double start{0};
        double duration;

        GLint positionAttrib;
        GLint colorLocation;
        GLint modelLocation;

    public:

        NotesView(Keyboard *keyboard, glm::mat4 model, double duration);

        void receiveEvent(KeyEvent event) override;
        void update(double dt) override;
        void render() override;

    };

}

#endif //MUSIC_VISUALIZER_NOTES_HPP
