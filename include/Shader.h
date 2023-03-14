//
// Created by mateuszp on 11/3/22.
//

#ifndef OPENGLTEMPLATE_SHADER_H
#define OPENGLTEMPLATE_SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

class Shader {
public:
    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    void use();

    void setBool(const std::string &name, bool val);
    void setInt(const std::string &name, int val);
    void setFloat(const std::string &name, float val);
private:
    unsigned int ID;
};


#endif //OPENGLTEMPLATE_SHADER_H
