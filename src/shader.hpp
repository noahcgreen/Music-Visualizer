#ifndef MUSIC_VISUALIZER_SHADER_HPP
#define MUSIC_VISUALIZER_SHADER_HPP

#include <filesystem>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include <GL/glew.h>

namespace Music_Visualizer {

    std::optional<GLuint> createShader(GLenum type, std::filesystem::path source);

    std::optional<GLuint> createProgram(std::filesystem::path vsSource, std::filesystem::path fsSource);

}

#endif //MUSIC_VISUALIZER_SHADER_HPP
