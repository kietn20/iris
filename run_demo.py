import sys
import time
import numpy as np
from PIL import Image

try:
    import iris
except ImportError:
    print("Error: The 'iris' module could not be found.")
    print("Please ensure it has been built and the .so file is in the project root.")
    sys.exit(1)

def python_box_blur(image_np, radius):
    """A pure python implementation of the box blur algorithm."""
    height, width, channels = image_np.shape
    # Create a new array for the output, filled with zeros
    output_np = np.zeros_like(image_np)

    for r in range(height):
        for c in range(width):
            for ch in range(channels):
                total = 0
                count = 0
                for y in range(-radius, radius + 1):
                    for x in range(-radius, radius + 1):
                        current_r, current_c = r + y, c + x
                        if 0 <= current_r < height and 0 <= current_c < width:
                            total += image_np[current_r, current_c, ch]
                            count += 1
                output_np[r, c, ch] = total // count
    return output_np


def main():
    """Loads an image, benchmarks C++ vs Python, and saves the results."""
    input_filename = "test_image.png"
    output_cpp_filename = "blur_output_cpp.png"
    output_py_filename = "blur_output_python.png"
    blur_radius = 5  # a larger radius will show a more dramatic speed difference

    print("--- Phase 3: C++ vs. Python Performance Benchmark ---")
    print(f"Loading image '{input_filename}' and preparing for blur (radius={blur_radius})...")
    
    try:
        image = Image.open(input_filename)
        image = image.convert("RGBA")
        image_np_original = np.array(image)
    except FileNotFoundError:
        print(f"Error: Input file not found: '{input_filename}'")
        sys.exit(1)

    print(f"Image details: Shape={image_np_original.shape}, Dtype={image_np_original.dtype}\n")

    # --- C++ Benchmark ---
    print("Benchmarking C++ implementation...")
    # making a fresh copy to avoid any caching effects
    image_to_process_cpp = np.copy(image_np_original)
    start_time_cpp = time.perf_counter()
    blurred_image_cpp = iris.apply_box_blur(image_to_process_cpp, radius=blur_radius)
    end_time_cpp = time.perf_counter()
    duration_cpp = end_time_cpp - start_time_cpp
    print(f"C++ implementation took: {duration_cpp:.4f} seconds")

    Image.fromarray(blurred_image_cpp).save(output_cpp_filename)
    print(f"Saved C++ result to '{output_cpp_filename}'\n")

    # --- Python Benchmark ---
    print("Benchmarking pure Python implementation...")
    # making a fresh copy
    image_to_process_py = np.copy(image_np_original)
    start_time_py = time.perf_counter()
    blurred_image_py = python_box_blur(image_to_process_py, radius=blur_radius)
    end_time_py = time.perf_counter()
    duration_py = end_time_py - start_time_py
    print(f"Python implementation took: {duration_py:.4f} seconds")
    
    Image.fromarray(blurred_image_py).save(output_py_filename)
    print(f"Saved Python result to '{output_py_filename}'\n")

    # --- Results Summary ---
    print("--- Benchmark Summary ---")
    if duration_cpp > 0:
        speedup = duration_py / duration_cpp
        print(f"Speed-up: {speedup:.2f}x")
    else:
        print("C++ implementation was too fast to measure a speed-up!")


if __name__ == "__main__":
    main()