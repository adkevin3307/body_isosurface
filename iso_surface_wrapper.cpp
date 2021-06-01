#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>

#include "Volume.h"
#include "IsoSurface.h"

using namespace std;

PYBIND11_MODULE(_iso_surface, m)
{
    pybind11::class_<IsoSurface>(m, "IsoSurface")
        .def(pybind11::init<>())
        .def(pybind11::init<Volume>())
        .def("__str__", [](IsoSurface& m) {
            stringstream ss;
            ss << m;

            return ss.str();
        })
        .def("run", &IsoSurface::run)
        .def_property("iso_value", [](IsoSurface& iso_surface) {
            return iso_surface.iso_value();
        }, [](IsoSurface& iso_surface, float value) {
            iso_surface.iso_value() = value;
        })
        .def_property_readonly("vertices", &IsoSurface::vertices)
        .def_property_readonly("normals", &IsoSurface::normals);
}
