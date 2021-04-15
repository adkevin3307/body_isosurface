#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "constant.h"

class Camera {
private:
    glm::vec3 _position, _center, _up, _right;
    GLfloat _radius, _yaw, _pitch, _sensitivity;

    void update();

public:
    Camera();
    ~Camera();

    Camera(Camera const& rhs) = delete;
    Camera(Camera&& rhs) = delete;
    Camera& operator=(Camera const& rhs) = delete;
    Camera& operator=(Camera&& rhs) = delete;

    void reset();
    void process_mouse(CONSTANT::BUTTON button, double xoffset, double yoffset);

    glm::mat4 const view_matrix() const;
    glm::vec3 const center() const;
    glm::vec3 const up() const;
    glm::vec3 const position() const;
};
