#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

Shader::Shader()
{
}

Shader::Shader(string vertex_path, string fragment_path)
{
    string vertex_code = this->code(vertex_path);
    string fragment_code = this->code(fragment_path);

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    this->compile(vertex, vertex_path, vertex_code, "VERTEX");
    this->compile(fragment, fragment_path, fragment_code, "FRAGMENT");

    this->link(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(Shader const& rhs)
    : ID(rhs.ID)
{
}

Shader::Shader(Shader&& rhs)
    : ID(rhs.ID)
{
}

Shader::~Shader()
{
}

Shader& Shader::operator=(Shader const& rhs)
{
    if (this == &rhs) return *this;

    this->ID = rhs.ID;

    return *this;
}

Shader& Shader::operator=(Shader&& rhs)
{
    if (this == &rhs) return *this;

    this->ID = rhs.ID;

    return *this;
}

string Shader::code(string path)
{
    string code;

    fstream file;
    file.open(path, ios::in);

    stringstream ss;
    ss << file.rdbuf();
    code = ss.str();

    file.close();

    return code;
}

void Shader::compile(GLuint& shader, string path, string code, string type)
{
    const char* code_c_str = code.c_str();

    glShaderSource(shader, 1, &code_c_str, NULL);
    glCompileShader(shader);

    GLint compile_success;
    GLchar compile_log[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);

    if (compile_success) {
        cout << path << ": SHADER::" << type << "::COMPILATION_SUCCESS" << '\n';
    }
    else {
        glGetShaderInfoLog(shader, 1024, NULL, compile_log);

        throw runtime_error("ERROR::SHADER::" + type + "::COMPILATION_FAILED" + compile_log);
    }
}

void Shader::link(GLuint vertex, GLuint fragment)
{
    this->ID = glCreateProgram();

    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);

    glLinkProgram(ID);

    GLint link_success;
    GLchar link_log[1024];

    glGetProgramiv(this->ID, GL_LINK_STATUS, &link_success);

    if (link_success) {
        cout << "SHADER::PROGRAM::LINKING_SUCCESS" << '\n';
    }
    else {
        glGetProgramInfoLog(ID, 1024, NULL, link_log);

        throw runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED: " + string(link_log));
    }
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
