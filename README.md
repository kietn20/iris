# Iris: A High-Performance C++/Python Image Processing Library

Iris is a demonstration project showcasing the power of combining Python and C++ to build high-performance applications. The core, computationally-intensive image processing algorithms are written in C++, while a simple and user-friendly API is exposed to Python using Pybind11.

This project was built to explore and demonstrate a common architectural pattern used in fields like AI/ML, robotics, and scientific computing where performance is critical.

## Features

- **Grayscale Conversion:** A fast, in-place grayscale filter.
- **Box Blur:** A highly-optimized box blur filter that demonstrates a significant performance increase over a pure Python implementation.

## Benchmark Results

The primary goal of this project was to showcase the performance gains of using C++ for heavy computation. A benchmark was run on a 3500x2500 pixel image, applying a box blur with a radius of 5 pixels.

**The C++ engine outperformed the pure Python implementation by nearly 70x.**

![Benchmark Results](https://github.com/kietn20/iris/blob/main/benchmark.png)

## How to Build and Run

This project is built using CMake.

### Prerequisites

- A C++ compiler (g++)
- CMake (version 3.15+)
- Python 3 (with development headers)
- Pip

### Setup

1.  **Clone the repository and set up a virtual environment:**
    ```bash
    git clone <your-repo-url>
    cd <repo-name>
    python3 -m venv venv
    source venv/bin/activate
    ```

2.  **Install Python dependencies:**
    ```bash
    pip install pybind11 numpy pillow
    ```

3.  **Build the C++ module:**
    ```bash
    # Create a build directory
    mkdir build
    cd build

    # Configure with CMake and build with make
    # This command explicitly tells CMake where to find the pybind11 installed in the venv
    cmake -Dpybind11_DIR=$(python3 -c "import pybind11; print(pybind11.get_cmake_dir())") ..
    make

    # Move the compiled module to the project root
    cd ..
    mv build/*.so .
    ```

4.  **Run the demo and benchmark:**
    ```bash
    python3 run_demo.py
    ```

## Key Learnings

- Seamlessly binding C++ functions to Python using **Pybind11**.
- Building a C++ project with **CMake**.
- Manipulating **NumPy** data buffers directly in C++ for maximum performance.
- Understanding and preventing low-level bugs like **integer overflows**.