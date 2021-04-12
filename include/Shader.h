#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Shader {
private:
    GLuint ID;

public:
    Shader();
    Shader(std::string vertex_path, std::string fragment_path);
    ~Shader();

    void use();
    void set_uniform(std::string const& name, bool value) const;
    void set_uniform(std::string const& name, int value) const;
    void set_uniform(std::string const& name, float value) const;
    void set_uniform(std::string const& name, glm::vec3 value) const;
    void set_uniform(std::string const& name, glm::vec4 value) const;
    void set_uniform(std::string const& name, glm::mat4 value) const;
};
