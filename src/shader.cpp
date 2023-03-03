#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.hpp"

namespace Music_Visualizer {

    std::optional<GLuint> createShader(GLenum type, std::filesystem::path source) {
        GLuint shader = glCreateShader(type);

        std::ifstream ifs{source};
        if (!ifs) {
            std::cerr << "Failed to open " << source.string() << std::endl;
            glDeleteShader(shader);
            return std::nullopt;
        }
        std::stringstream contentStream;
        contentStream << ifs.rdbuf();
        std::string content = contentStream.str();
        const char *c_source = content.c_str();

        glShaderSource(shader, 1, &c_source, nullptr);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint maxErrorLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxErrorLength);
            char *c_error = new char[maxErrorLength];
            glGetShaderInfoLog(shader, maxErrorLength, nullptr, c_error);
            std::string error{c_error};
            std::cerr << "Failed to compile shader: " << error << std::endl;
            delete[] c_error;
            glDeleteShader(shader);
            return std::nullopt;
        }

        return shader;
    }

    std::optional<GLuint> createProgram(std::filesystem::path vsSource, std::filesystem::path fsSource) {
        GLuint vertexShader, fragmentShader;

        if (auto vs = createShader(GL_VERTEX_SHADER, vsSource)) {
            vertexShader = *vs;
        } else {
            return std::nullopt;
        }

        if (auto fs = createShader(GL_FRAGMENT_SHADER, fsSource)) {
            fragmentShader = *fs;
        } else {
            glDeleteShader(vertexShader);
            return std::nullopt;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint maxErrorLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxErrorLength);
            char *c_error = new char[maxErrorLength];
            glGetProgramInfoLog(program, maxErrorLength, nullptr, c_error);
            std::string error{c_error};
            std::cerr << "Failed to link program: " << error << std::endl;
            delete[] c_error;
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(program);
            return std::nullopt;
        }

        glDetachShader(program, vertexShader);
        glDeleteShader(vertexShader);

        glDetachShader(program, fragmentShader);
        glDeleteShader(fragmentShader);

        return program;
    }

}
