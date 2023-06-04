#include "BOP.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> //change python list to C++ vector<T>
namespace py = pybind11;

PYBIND11_MODULE(_BOP, m) {
    py::class_<BoolOperaPolygon>(m, "BoolOperaPolygon")
        .def(py::init<std::vector<Polygon>, const char*>())
        .def("create_SweepEvent", &BoolOperaPolygon::create_SweepEvent)
        .def("subdividing_edge", &BoolOperaPolygon::subdividing_edge)
        .def("joining_edge", &BoolOperaPolygon::joining_edge)
        .def_readwrite("resultP", &BoolOperaPolygon::resultP)
        .def_readwrite("result_INOUT", &BoolOperaPolygon::result_INOUT);
    py::class_<Polygon>(m, "Polygon")
        .def(py::init<std::vector<std::vector<pdd>>>())
        .def_readwrite("pid", &Polygon::pid);
}