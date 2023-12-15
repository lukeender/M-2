#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "ImageProcModule.cpp" 

namespace py = pybind11;

PYBIND11_MODULE(ImageProcModule, m) {
    py::class_<ImageProcessor>(m, "ImageProcessor")
        .def(py::init<std::string, std::string>())
        .def("run", &ImageProcessor::run)
        .def("getresx", &ImageProcessor::getresx)
        .def("getresy", &ImageProcessor::getresy);
}