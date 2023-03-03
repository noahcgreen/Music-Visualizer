#ifndef MUSIC_VISUALIZER_KEYBOARD_HPP
#define MUSIC_VISUALIZER_KEYBOARD_HPP

#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <vector>

#include "glm/mat4x4.hpp"

namespace Music_Visualizer {

    using Rectangle = std::array<glm::vec2, 4>;

    enum class KeyColor { White, Black };

    struct KeyPress {
        size_t key;
        double velocity;
        double time;
    };

    struct KeyEvent {

        enum class Type {
            KeyDown,
            KeyUp
        } type;

        size_t key;
        double velocity;

    };

    class Keyboard {

        using Listener = std::function<void(KeyEvent)>;

    private:

        std::array<bool, 88> pressed{false};
        std::vector<Listener> listeners;

        std::array<std::optional<KeyPress>, 88> lastPress;

        void emit(KeyEvent event);

        double amplitude(std::size_t key);

    public:

        Keyboard();

        KeyColor getColor(std::size_t key);
        // Get the bounding coordinates (cc from bottom left) of the key (0-87)
        // Coordinates are normalized to [0, 1]^2
        Rectangle getBounds(std::size_t key);
        void listen(Listener listener);
        void press(std::size_t key, double velocity);
        void release(std::size_t key);
        bool isPressed(std::size_t key);

        void update(double dt);
        double amplitude();

        template <class T> void listen(T *listener) {
            listen([=](KeyEvent event) { listener->receiveEvent(event); });
        }

    };

}

#endif //MUSIC_VISUALIZER_KEYBOARD_HPP
