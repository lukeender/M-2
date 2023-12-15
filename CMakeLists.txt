cmake_minimum_required(VERSION 3.4...3.18)
project(ImageProcModule)
set(CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
find_package(Ceres REQUIRED )
find_package(pybind11 REQUIRED)

pybind11_add_module(ImageProcModule Modules/ImageProcModule/ImageProcModule_pybinding.cpp)

target_link_libraries( ImageProcModule PRIVATE ${OpenCV_LIBS} )
target_link_libraries( ImageProcModule PRIVATE Ceres::ceres)
