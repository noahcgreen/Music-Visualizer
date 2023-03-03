#ifndef MUSIC_VISUALIZER_GRAPH_HPP
#define MUSIC_VISUALIZER_GRAPH_HPP

#include <utility>
#include <vector>

#include "view.hpp"

namespace Music_Visualizer {

    class GraphView : public View {

    private:

        double start{0};
        double duration;
        std::vector<float> points;

        GLint positionAttrib;
        GLint modelLocation;
        GLint colorLocation;

    public:
        GraphView(Keyboard *keyboard, glm::mat4 model, double duration, std::size_t points);

        void receiveEvent(KeyEvent event) override;
        void update(double dt) override;
        void render() override;

    };

}

#endif //MUSIC_VISUALIZER_GRAPH_HPP
