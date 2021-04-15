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

void Slice::update()
{
    glm::vec3 front;

    front.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
    front.y = sin(glm::radians(this->m_pitch));
    front.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));

    if (glm::length(front) > numeric_limits<float>::epsilon()) front = glm::normalize(front);

    this->m_position = this->m_center - this->m_radius * front;

    this->m_normal = this->m_center - fabs(this->m_radius) * front;
    if (glm::length(this->m_normal) > numeric_limits<float>::epsilon()) this->m_normal = glm::normalize(this->m_normal);
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
    switch (button) {
        case CONSTANT::BUTTON::LEFT:
            this->m_yaw += xoffset * this->m_sensitivity;
            this->m_pitch += yoffset * this->m_sensitivity;

            break;
        case CONSTANT::BUTTON::RIGHT:
            break;
        case CONSTANT::BUTTON::MIDDLE:
            this->m_radius += yoffset;

            break;
        default:
            break;
    }

    this->update();
}

glm::vec4 const Slice::normal() const
{
    float distance = -1.0 * glm::dot(this->m_normal, this->m_position);

    return glm::vec4(this->m_normal, distance);
}
