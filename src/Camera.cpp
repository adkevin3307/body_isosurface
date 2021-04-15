#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Camera::Camera()
    : _position(glm::vec3(0.0)), _center(glm::vec3(0.0)), _up(glm::vec3(0.0)), _right(glm::vec3(0.0)), _radius(300.0), _yaw(-90.0), _pitch(0.0), _sensitivity(0.1)
{
    this->update();
}

Camera::~Camera()
{
}

void Camera::update()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
    front.y = sin(glm::radians(this->_pitch));
    front.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));

    if (glm::length(front) > numeric_limits<float>::epsilon()) front = glm::normalize(front);

    this->_position = this->_center - this->_radius * front;
    this->_right = glm::normalize(glm::cross(front, glm::vec3(0.0, 1.0, 0.0)));
    this->_up = glm::normalize(glm::cross(this->_right, front));
}

void Camera::reset()
{
    this->_position = glm::vec3(0.0);
    this->_center = glm::vec3(0.0);
    this->_up = glm::vec3(0.0);
    this->_right = glm::vec3(0.0);

    this->_radius = 300.0;
    this->_yaw = -90.0;
    this->_pitch = 0.0;

    this->update();
}

void Camera::process_mouse(CONSTANT::BUTTON button, double xoffset, double yoffset)
{
    switch (button) {
        case CONSTANT::BUTTON::LEFT:
            this->_yaw += xoffset * this->_sensitivity;
            this->_pitch += yoffset * this->_sensitivity;

            if (this->_pitch > 89.0f) this->_pitch = 89.0f;
            if (this->_pitch < -89.0f) this->_pitch = -89.0f;

            break;
        case CONSTANT::BUTTON::RIGHT:
            this->_center += (float)(xoffset * this->_sensitivity) * this->_right;
            this->_center += (float)(yoffset * this->_sensitivity) * this->_up;

            break;
        default:
            break;
    }

    this->update();
}

glm::mat4 const Camera::view_matrix() const
{
    return glm::lookAt(this->_position, this->_center, this->_up);
}

glm::vec3 const Camera::center() const
{
    return this->_center;
}

glm::vec3 const Camera::up() const
{
    return this->_up;
}

glm::vec3 const Camera::position() const
{
    return this->_position;
}
