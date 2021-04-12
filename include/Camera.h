#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "constant.h"

class Camera {
private:
    glm::vec3 _position, _center, _up, _right;
    GLfloat _radius, _fov, _yaw, _pitch, _sensitivity;

    void update();

public:
    Camera();
    ~Camera();

    void reset();

    glm::mat4 view_matrix();
    glm::vec3 center();
    glm::vec3 up();
    glm::vec3 position();

    void process_mouse(CONSTANT::BUTTON button, double xoffset, double yoffset);
};
