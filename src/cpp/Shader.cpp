//
// Created by mateuszp on 11/3/22.
//

#include "../../include/Shader.h"

Shader::Shader(const char *vertex_shader_path, const char *fragment_shader_path) {
    std::string vertex_shader_code_str;
    std::string fragment_shader_code_str;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        fragment_shader_code_str = fragment_shader_stream.str();
        vertex_shader_code_str = vertex_shader_stream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "Could not read file" << std::endl;
    }

    const char* vertex_shader_code = vertex_shader_code_str.c_str();
    const char* fragment_shader_code = fragment_shader_code_str.c_str();

    unsigned int vertex_shader;
    unsigned int fragment_shader;
    int success;
    char log[512];

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, log);
        std::cout << "Vertex shader failed to compile:\n" << log << std::endl;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, log);
        std::cout << "Fragment shader failed to compile:\n" << log << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glCompileShader(ID);

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, log);
        std::cout << "Failed to link program:\n" << log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool val) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

void Shader::setInt(const std::string &name, int val) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setFloat(const std::string &name, float val) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}
