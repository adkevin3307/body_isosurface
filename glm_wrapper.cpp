#include <pybind11/pybind11.h>

#include <glm/glm.hpp>

PYBIND11_MODULE(_glm, m)
{
    pybind11::class_<glm::vec3>(m, "vec3")
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z);

    pybind11::class_<glm::ivec3>(m, "ivec3")
        .def_readwrite("x", &glm::ivec3::x)
        .def_readwrite("y", &glm::ivec3::y)
        .def_readwrite("z", &glm::ivec3::z);
}
