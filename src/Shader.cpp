#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Shader::Shader()
{
}

Shader::Shader(string vertex_path, string fragment_path)
{
    string vertex_code, fragment_code;
    fstream vertex_file, fragment_file;
    stringstream vertex_stream, fragment_stream;

    vertex_file.open(vertex_path, ios::in);
    fragment_file.open(fragment_path, ios::in);

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    vertex_code = vertex_stream.str();
    fragment_code = fragment_stream.str();

    vertex_file.close();
    fragment_file.close();

    const char* vertex_code_temp = vertex_code.c_str();
    const char* fragment_code_temp = fragment_code.c_str();

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code_temp, NULL);
    glCompileShader(vertex);

    GLint compile_vertex_success;
    GLchar compile_vertex_log[1024];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compile_vertex_success);
    if (compile_vertex_success) {
        cout << vertex_path << ": SHADER::VERTEX::COMPILATION_SUCCESS" << '\n';
    }
    else {
        glGetShaderInfoLog(vertex, 1024, NULL, compile_vertex_log);
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << compile_vertex_log << '\n';
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code_temp, NULL);
    glCompileShader(fragment);

    GLint compile_fragment_success;
    GLchar compile_fragment_log[1024];
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compile_fragment_success);
    if (compile_fragment_success) {
        cout << fragment_path << ": SHADER::FRAGMENT::COMPILATION_SUCCESS" << '\n';
    }
    else {
        glGetShaderInfoLog(fragment, 1024, NULL, compile_fragment_log);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << compile_fragment_log << '\n';
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(ID);

    GLint link_success;
    GLchar link_log[1024];
    glGetProgramiv(ID, GL_LINK_STATUS, &link_success);
    if (link_success) {
        cout << "SHADER::PROGRAM::LINKING_SUCCESS" << '\n';
    }
    else {
        glGetProgramInfoLog(ID, 1024, NULL, link_log);
        cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << link_log << '\n';
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::set_uniform(string const& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::set_uniform(string const& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_uniform(string const& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_uniform(string const& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void Shader::set_uniform(string const& name, glm::vec4 value) const
{
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void Shader::set_uniform(string const& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
