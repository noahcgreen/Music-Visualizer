#ifndef MUSIC_VISUALIZER_DP_HPP
#define MUSIC_VISUALIZER_DP_HPP

#include "portmidi/portmidi.h"

#include "../model/keyboard.hpp"

namespace Music_Visualizer {

    class DigitalPiano {

        Keyboard *keyboard;
        PmDeviceID device{pmNoDevice};
        PortMidiStream *stream{nullptr};

    public:
        DigitalPiano(Keyboard *keyboard);
        ~DigitalPiano();

        void poll();

    };

}

#endif //MUSIC_VISUALIZER_DP_HPP
