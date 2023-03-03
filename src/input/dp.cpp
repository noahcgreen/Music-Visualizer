#include <iostream>
#include <string>

#include "portmidi/portmidi.h"

#include "dp.hpp"

static const int PM_BUFFER_SIZE = 128;

namespace Music_Visualizer {

    static void showPMError(std::string_view message, PmError error) {
        const char *description = Pm_GetErrorText(error);
        std::cerr << message << ": " << description << std::endl;
    }

    static PmDeviceID getDPInputDevice() {
        int deviceCount = Pm_CountDevices();
        if (deviceCount == 0) return pmNoDevice;

        for (PmDeviceID device = 0; device < deviceCount; device++) {
            const PmDeviceInfo *info = Pm_GetDeviceInfo(device);
            if (!info) continue;

            std::string name = info->name;
            if (name.find("Digital Piano") == 0 && info->input) {
                return device;
            }
        }

        return pmNoDevice;
    }

    DigitalPiano::DigitalPiano(Keyboard* keyboard) : keyboard{keyboard} {
        PmError error;

        error = Pm_Initialize();
        if (error != pmNoError) {
            showPMError("Failed to initialize PortMidi", error);
            return;
        }

        device = getDPInputDevice();
        if (device == pmNoDevice) {
            std::cerr << "Digital piano not found" << std::endl;
            return;
        }

        error = Pm_OpenInput(&stream, device, nullptr, PM_BUFFER_SIZE, nullptr, nullptr);
        if (error != pmNoError) {
            showPMError("Failed to open DP stream", error);
            return;
        }
    }

    DigitalPiano::~DigitalPiano() {
        PmError error;

        if (stream) {
            error = Pm_Close(stream);
            if (error != pmNoError) {
                showPMError("Failed to close DP stream", error);
            }
        }

        error = Pm_Terminate();
        if (error != pmNoError) {
            showPMError("Failed to terminated PortMidi", error);
        }
    }

    void DigitalPiano::poll() {
        if (device == pmNoDevice) return;

        PmEvent events[PM_BUFFER_SIZE];
        int read = Pm_Read(stream, events, PM_BUFFER_SIZE);
        if (read < 0) {
            showPMError("Failed to read MIDI events", static_cast<PmError>(read));
            return;
        }

        for (int i = 0; i < read; i++) {
            PmEvent &event = events[i];
            if (Pm_MessageStatus(event.message) == 0x90) {
                std::size_t note = Pm_MessageData1(event.message) - 21;
                int velocity = Pm_MessageData2(event.message);
                if (velocity == 0) {
                    // Note off
                    keyboard->release(note);
                } else {
                    // Note on
                    keyboard->press(note, (double)velocity / 128.);
                }
            }
        }
    }

}
