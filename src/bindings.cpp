#include <pybind11/pybind11.h>

int add (int i, int j) {
    return i + j;
}

PYBIND11_MODULE(iris, m) {
    m.doc() = "pybind11 example plugin for the Iris project";
    m.def("add", &add, "A function which adds two numbers");

}