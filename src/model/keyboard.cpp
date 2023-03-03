#include <cmath>

#include "glm/vec2.hpp"

#include "keyboard.hpp"

namespace Music_Visualizer {

    static constexpr float MODEL_WIDTH = 779.0f;
    static constexpr float MODEL_HEIGHT = 96.0f;

    static constexpr float WHITE_WIDTH = 14.0f;
    static constexpr float WHITE_HEIGHT = 96.0f;

    static constexpr float BLACK_WIDTH = 7.0f;
    static constexpr float BLACK_HEIGHT = 63.0f;

    static constexpr float BLACK_OFFSET_X = 11.0f;
    static constexpr float BLACK_OFFSET_Y = 33.0f;

    static constexpr float KEY_GAP = 1.0f;

    static constexpr Rectangle WHITE_TEMPLATE{
        glm::vec2{0.0f, 0.0f},
        glm::vec2{WHITE_WIDTH, 0.0f},
        glm::vec2{WHITE_WIDTH, WHITE_HEIGHT},
        glm::vec2{0.0f, WHITE_HEIGHT}
    };

    static constexpr Rectangle BLACK_TEMPLATE{
        glm::vec2{BLACK_OFFSET_X, BLACK_OFFSET_Y} + glm::vec2{0.0f, 0.0f},
        glm::vec2{BLACK_OFFSET_X, BLACK_OFFSET_Y} + glm::vec2{BLACK_WIDTH, 0.0f},
        glm::vec2{BLACK_OFFSET_X, BLACK_OFFSET_Y} + glm::vec2{BLACK_WIDTH, BLACK_HEIGHT},
        glm::vec2{BLACK_OFFSET_X, BLACK_OFFSET_Y} + glm::vec2{0.0f, BLACK_HEIGHT}
    };

    Keyboard::Keyboard() {
        lastPress.fill(std::nullopt);
    }

    KeyColor Keyboard::getColor(std::size_t key) {
        switch (key % 12) {
            case 1:
            case 4:
            case 6:
            case 9:
            case 11:
                return KeyColor::Black;
            default:
                return KeyColor::White;
        }
    }

    Rectangle Keyboard::getBounds(std::size_t key) {
        std::size_t previousWhites[12] = {0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6};
        std::size_t prevWhites = 7 * (key / 12) + previousWhites[key % 12];
        glm::vec2 offset{0.0f, 0.0f};
        offset.x = (WHITE_WIDTH + KEY_GAP) * (float)prevWhites;
        glm::vec2 scale{1.0f / MODEL_WIDTH, 1.0f / MODEL_HEIGHT};
        Rectangle result;
        switch (getColor(key)) {
            case KeyColor::White:
                for (int i = 0; i < 4; i++) {
                    result[i] = (WHITE_TEMPLATE[i] + offset) * scale;
                }
                break;
            case KeyColor::Black:
                for (int i = 0; i < 4; i++) {
                    result[i] = (BLACK_TEMPLATE[i] + offset) * scale;
                }
                break;
        }
        return result;
    }

    void Keyboard::listen(Listener listener) {
        listeners.push_back(listener);
    }

    void Keyboard::emit(KeyEvent event) {
        for (auto& listener : listeners) {
            listener(event);
        }
    }

    void Keyboard::press(std::size_t key, double velocity) {
        if (pressed[key]) {
            return;
        }
        pressed[key] = true;
        lastPress[key] = KeyPress{.key=key, .velocity=velocity, .time=0};
        emit(KeyEvent{KeyEvent::Type::KeyDown, key, velocity});
    }

    void Keyboard::release(std::size_t key) {
        if (!pressed[key]) {
            return;
        }
        pressed[key] = false;
        lastPress[key] = std::nullopt;
        emit(KeyEvent{KeyEvent::Type::KeyUp, key, 0});
    }

    bool Keyboard::isPressed(std::size_t key) {
        return pressed[key];
    }

    double Keyboard::amplitude(std::size_t key) {
        if (auto press = lastPress[key]) {
            double a = 0;
            // Base waveform
            double f = 27.5 * std::exp2((double)key / 12);
            double t = press->time;
            a += std::sin(2 * 3.1415926585 * f * t);
            double v = press->velocity - 0.03 * t;
            if (v < 0) {
                return 0;
            } else {
                return v * a;
            }
        } else {
            return 0;
        }
    }

    double Keyboard::amplitude() {
        double a = 0;
        double k = 0;
        for (size_t key = 0; key < 88; key++) {
            double ak = amplitude(key);
            if (ak != 0) {
                k++;
                a += ak;
            }
        }
        return a / k;
    }

    void Keyboard::update(double dt) {
        for (auto& press : lastPress) {
            if (press) {
                press->time += dt;
            }
        }
    }

}
