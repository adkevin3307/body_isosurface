#pragma once

#include <vector>

#include <GL/glew.h>

class Buffer {
private:
    GLuint vertex_array, vertex_buffer;

    friend class BufferManagement;
};

class BufferManagement {
public:
    static Buffer generate();
    static void bind(const Buffer& buffer);
    static void fill(std::vector<GLfloat>& vertex);
    static void set(GLuint index = 0, GLint size = 3, GLint stride = 6, int offset = 0);
    static void unbind();
    static void draw(const Buffer& buffer, int first, int count, GLenum render_mode, GLenum rasterize_mode);
};
