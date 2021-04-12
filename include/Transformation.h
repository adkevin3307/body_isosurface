#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "constant.h"
#include "Shader.h"

class Transformation {
private:
    glm::mat4 _model, _view, _projection;
    Shader shader;

public:
    Transformation(Shader shader);
    ~Transformation();

    void init(CONSTANT::TRANSFORMATION type);
    void set_model(CONSTANT::TRANSFORMATION type, glm::vec3 v, float angle = 0.0);
    void set_view(glm::mat4 view);
    void set_projection(GLfloat width, GLfloat height, GLfloat rate, GLfloat near, GLfloat far);
    void set(bool set_model = false);
};
