#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "constant.h"

class Slice {
private:
    glm::vec3 m_center, m_normal, m_position;
    GLfloat m_radius, m_yaw, m_pitch, m_sensitivity;

    void update();

public:
    Slice();
    ~Slice();

    Slice(Slice const& rhs) = delete;
    Slice(Slice&& rhs) = delete;
    Slice& operator=(Slice const& rhs) = delete;
    Slice& operator=(Slice&& rhs) = delete;

    void reset();
    void process_mouse(CONSTANT::BUTTON button, double xoffset, double yoffset);

    glm::vec4 const normal() const;
};
