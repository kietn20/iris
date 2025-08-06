#include <pybind11/pybind11.h>
#include <pybind11/numpy.h> // Required for NumPy array support
#include <vector>

namespace py = pybind11;

/*
 * Converts an image represented by a NumPy array to grayscale.
 * The conversion is done in-place, meaning the original array is modified.
 *
 * Args:
 *   image: A 3D NumPy array of dtype=uint8 and shape (height, width, channels).
 *          Channels can be 3 (RGB) or 4 (RGBA).
 */
void convert_to_grayscale(py::array_t<unsigned char> image) {
    // 1. Request buffer information from the NumPy array
    py::buffer_info buf = image.request();

    // 2. Validate the input array 
    // it must be a 3D array (height, width, channels)
    if (buf.ndim != 3) { 
        throw std::runtime_error("NumPy array must be 3-dimensional"); 
    }

    // It must have 3 or 4 channels (RGB or RGBA)
    ssize_t channels = buf.shape[2];
    if (channels != 3 && channels != 4) { 
        throw std::runtime_error("Image must have 3 or 4 channels"); 
    }

    // 3. Get a raw C++ pointer to the beginning of the array data
    auto *ptr = static_cast<unsigned char *>(buf.ptr);

    ssize_t height = buf.shape[0];
    ssize_t width = buf.shape[1];


    // 4. Iterate over each pixel of the image
    // The outer loop goes over each row (height) and the inner loop goes over each column in that row (width)
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            
            // Read the Red, Green, and Blue values
            // unsigned char red = pixel_ptr[0];
            // unsigned char green = pixel_ptr[1];
            // unsigned char blue = pixel_ptr[2];
            
            // calculate the memory offset to the first channel (R) of the current pixel
            unsigned char* pixel_ptr = ptr + (r * width * channels) + (c * channels);
            unsigned char gray = static_cast<unsigned char>(0.21 * pixel_ptr[0] + 0.72 * pixel_ptr[1] + 0.07 * pixel_ptr[2]); // 5. Calculate the grayscale value using the luminosity method.
            pixel_ptr[0] = pixel_ptr[1] = pixel_ptr[2] = gray; // write the new grayscale value back to all three channels
        }
    }
}

/*
 * Applies a box blur filter to an image.
 *
 * Args:
 *   image: A 3D NumPy array of dtype=uint8 and shape (height, width, channels).
 *   radius: The radius of the blur box. A radius of 1 creates a 3x3 box.
 *
 * Returns:
 *   A new NumPy array of the same shape and type containing the blurred image.
 */
py::array_t<unsigned char> apply_box_blur(py::array_t<unsigned char> image, int radius) {
    py::buffer_info input_buf = image.request();
    auto *input_ptr = static_cast<unsigned char *>(input_buf.ptr);

    if (input_buf.ndim != 3) { throw std::runtime_error("NumPy array must be 3-dimensional"); }
    if (radius < 1) { throw std::runtime_error("Radius must be 1 or greater"); }
    
    ssize_t height = input_buf.shape[0];
    ssize_t width = input_buf.shape[1];
    ssize_t channels = input_buf.shape[2];

    auto output_arr = py::array_t<unsigned char>(input_buf.size);
    output_arr.resize({height, width, channels});
    py::buffer_info output_buf = output_arr.request();
    auto *output_ptr = static_cast<unsigned char *>(output_buf.ptr);

    // iterate over each pixel of the image (as the center of the blur box)
    for (ssize_t r = 0; r < height; ++r) {
        for (ssize_t c = 0; c < width; ++c) {
            
            // for each channel (R, G, B, and maybe A)
            for (ssize_t ch = 0; ch < channels; ++ch) {
                unsigned int total = 0;
                int count = 0;

                // iterate over the box of pixels around the center pixel (r, c)
                for (ssize_t y = -radius; y <= radius; ++y) {
                    for (ssize_t x = -radius; x <= radius; ++x) {
                        ssize_t current_r = r + y;
                        ssize_t current_c = c + x;

                        // check bounds to make sure we're not reading outside the image
                        if (current_r >= 0 && current_r < height && current_c >= 0 && current_c < width) {
                            // read the original pixel value from the input buffer
                            total += input_ptr[(current_r * width + current_c) * channels + ch];
                            count++;
                        }
                    }
                }
                // calculate the average and write it to the output buffer
                output_ptr[(r * width + c) * channels + ch] = static_cast<unsigned char>(total / count);
            }
        }
    }
    return output_arr;
}


// ---  Module Definition ---
PYBIND11_MODULE(iris, m) {
    m.doc() = "Iris: A high-performance C++/Python image processing library";

    m.def("convert_to_grayscale", &convert_to_grayscale, 
          "Converts an image to grayscale in-place.",
          py::arg("image"));

    m.def("apply_box_blur", &apply_box_blur,
          "Applies a box blur to an image and returns a new blurred image.",
          py::arg("image"), py::arg("radius") = 1); // default to 1
}