#include "Slice.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

using namespace std;

Slice::Slice()
    : m_center(glm::vec3(0.0)), m_normal(glm::vec3(0.0)), m_position(glm::vec3(0.0)), m_radius(0.0), m_yaw(0.0), m_pitch(0.0), m_sensitivity(0.1)
{
    this->update();
}

Slice::~Slice()
{
}

void Slice::update(bool keep)
{
    glm::vec3 front;

    front.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
    front.y = sin(glm::radians(this->m_pitch));
    front.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));

    if (glm::length(front) > numeric_limits<float>::epsilon()) front = glm::normalize(front);

    glm::vec3 normal_origin_sign;
    normal_origin_sign.x = this->m_normal.x < 0.0 ? -1.0 : 1.0;
    normal_origin_sign.y = this->m_normal.y < 0.0 ? -1.0 : 1.0;
    normal_origin_sign.z = this->m_normal.z < 0.0 ? -1.0 : 1.0;

    this->m_position = this->m_center - this->m_radius * front;
    if (glm::length(this->m_position) > numeric_limits<float>::epsilon()) this->m_normal = glm::normalize(this->m_position);

    if (keep) {
        glm::vec3 check = normal_origin_sign * this->m_normal;

        if (check.x < 0) this->m_normal.x *= -1.0;
        if (check.y < 0) this->m_normal.y *= -1.0;
        if (check.z < 0) this->m_normal.z *= -1.0;
    }
}

void Slice::reset()
{
    this->m_center = glm::vec3(0.0);
    this->m_normal = glm::vec3(0.0);
    this->m_position = glm::vec3(0.0);

    this->m_radius = 0.0;
    this->m_yaw = 0.0;
    this->m_pitch = 0.0;

    this->update();
}

void Slice::process_mouse(CONSTANT::BUTTON button, double xoffset, double yoffset)
{
    bool keep = false;

    switch (button) {
        case CONSTANT::BUTTON::LEFT:
            this->m_yaw += xoffset * this->m_sensitivity;
            this->m_pitch += yoffset * this->m_sensitivity;

            break;
        case CONSTANT::BUTTON::RIGHT:
            break;
        case CONSTANT::BUTTON::MIDDLE:
            keep = true;

            this->m_radius += yoffset;

            break;
        default:
            break;
    }

    this->update(keep);
}

glm::vec4 const Slice::normal() const
{
    float distance = -1.0 * glm::dot(this->m_normal, this->m_position);

    cout << glm::to_string(glm::vec4(this->m_normal, distance)) << '\n';

    return glm::vec4(this->m_normal, distance);
}
