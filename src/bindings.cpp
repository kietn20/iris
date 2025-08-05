#include <pybind11/pybind11.h>
#include <pybind11/numpy.h> // for NumPy array

namespace py = pybind11;

void convert_to_grayscale(py::array_t<unsigned char> image) {
    // 1. Request buffer information from the NumPy array
    py::buffer_info buf = image.request();

    // 2. Validate the input array
    // It must be a 3D array (height, width, channels)
    if (buf.ndim != 3) {
        throw std::runtime_error("NumPy array must be 3-demensional");
    }

    // It must have 3 or 4 channels (RGB or RGBA)
    ssize_t channels = buf.shape[2];
    if (channels != 3 && channels != 4) {
        throw std:runtime_error("Image must have 3 (RGB) or 4 (RGBA) channels");
    }

    // 3. Get a raw C++ pointer to the beginning of the array data
    auto *ptr = static_cast<unsigned char *>(buf.ptr);

    ssize_t height = buf.shape[0];
    ssize_t width = buf.shape[1];


    // 4. Iterate over each pixel of the image
    // The outer loop goes over each row (height)
    // The inner loop goes over each column in that row (width)
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            unsigned char* pixel_ptr = ptr + (r * width * channels) + (c * channels);

            unsigned char red = pixel_ptr[0];
            unsigned char green = pixel_ptr[1];
            unsigned char blue = pixel_ptr[2];

            unsigned char gray = static_cast<unsigned char>(0.21 * red + 0.72 * green + 0.07 * blue);

            pixel_ptr[0] = gray;
            pixel_ptr[1] = gray;
            pixel_ptr[2] = gray;
        }
    }
}

PYBIND11_MODULE(iris, m) {
    m.doc() = "Iris: A high-performance C++/Python image processing library";

    m.def("convert_to_grayscale", &convert_to_grayscale, "Converts an image to grayscale in-place.", py::arg("image"));
}