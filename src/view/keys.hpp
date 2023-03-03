#ifndef MUSIC_VISUALIZER_KEYS_HPP
#define MUSIC_VISUALIZER_KEYS_HPP

#include "view.hpp"

namespace Music_Visualizer {

    class KeysView : public View {

    private:

        GLint positionAttrib;
        GLint colorAttrib;
        GLint modelLocation;

    public:

        KeysView(Keyboard *keyboard, glm::mat4 model);

        void receiveEvent(KeyEvent event) override;
        void update(double dt) override;
        void render() override;

        void setKey(size_t key, bool on);

    };

}


#endif //MUSIC_VISUALIZER_KEYS_HPP
