#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <string>
#include <sstream>
#include <tuple>
#include <stdexcept>

#include "Volume.h"

using namespace std;

PYBIND11_MODULE(_volume, m)
{
    pybind11::class_<VOXEL>(m, "Voxel")
        .def_readwrite("value", &VOXEL::value)
        .def_property("normal",
            [](VOXEL& voxel) {
                return make_tuple(voxel.normal.x, voxel.normal.y, voxel.normal.z);
            },
            [](VOXEL& voxel, pybind11::tuple value) {
                if (value.size() != 3) {
                    throw runtime_error("Invalid argument size, need 3 dimension.");
                }

                voxel.normal.x = value[0].cast<float>();
                voxel.normal.y = value[1].cast<float>();
                voxel.normal.z = value[2].cast<float>();
            }
        );

    pybind11::class_<Volume>(m, "Volume")
        .def(pybind11::init<>())
        .def(pybind11::init<string, string>())
        .def("__getitem__", [](Volume& m, pybind11::tuple index) {
            if (index.size() != 3) {
                throw runtime_error("Invalid argument size, need 3 dimension.");
            }

            return m(index[0].cast<int>(), index[1].cast<int>(), index[2].cast<int>());
        })
        .def("__setitem__", [](Volume& m, pybind11::tuple index, VOXEL voxel) {
            if (index.size() != 3) {
                throw runtime_error("Invalid argument size, need 3 dimension.");
            }

            m(index[0].cast<int>(), index[1].cast<int>(), index[2].cast<int>()) = voxel;
        })
        .def("__str__", [](Volume& m) {
            stringstream ss;
            ss << m;

            return ss.str();
        })
        .def_property_readonly("shape", [](Volume& volume) {
            return make_tuple(volume.shape().x, volume.shape().y, volume.shape().z);
        })
        .def_property_readonly("voxel_size", [](Volume& volume) {
            return make_tuple(volume.voxel_size().x, volume.voxel_size().y, volume.voxel_size().z);
        })
        .def_property_readonly("average", &Volume::average)
        .def_property_readonly("min_value", &Volume::min_value)
        .def_property_readonly("max_value", &Volume::max_value);
}
