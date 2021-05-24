#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <string>
#include <sstream>

#include "Volume.h"

using namespace std;

PYBIND11_MODULE(_volume, m)
{
    pybind11::class_<VOXEL>(m, "Voxel")
        .def_readwrite("value", &VOXEL::value)
        .def_readwrite("normal", &VOXEL::normal);

    pybind11::class_<Volume>(m, "Volume")
        .def(pybind11::init<>())
        .def(pybind11::init<string, string>())
        .def("__getitem__", [](Volume& m, pybind11::tuple index) {
            if (index.size() != 3) {
                throw std::runtime_error("Invalid state!");
            }

            return m(index[0].cast<int>(), index[1].cast<int>(), index[2].cast<int>());
        })
        .def("__setitem__", [](Volume& m, pybind11::tuple index, VOXEL voxel) {
            if (index.size() != 3) {
                throw std::runtime_error("Invalid state!");
            }

            m(index[0].cast<int>(), index[1].cast<int>(), index[2].cast<int>()) = voxel;
        })
        .def("__str__", [](Volume& m) {
            stringstream ss;
            ss << m;

            return ss.str();
        })
        .def_property_readonly("shape", &Volume::shape)
        .def_property_readonly("voxel_size", &Volume::voxel_size)
        .def_property_readonly("average", &Volume::average)
        .def_property_readonly("min_value", &Volume::min_value)
        .def_property_readonly("max_value", &Volume::max_value);
}
