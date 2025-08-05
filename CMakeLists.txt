cmake_minimum_required(VERSION 3.15)

project(iris)

find_package(pybind11 REQUIRED)

pybind11_add_module(iris src/bindings.cpp)