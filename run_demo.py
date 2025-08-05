import sys
import numpy as np
from PIL import Image

try:
    import iris
except ImportError:
    print("Error: The 'iris' module could not be found.")
    print("Please ensure it has been built and the .so file is in the project root.")
    sys.exit(1)

def main():
    """
    Loads an image, calls the C++ grayscale function, and saves the result.
    """
    input_filename = "test_image.jpg"
    output_filename = "grayscale_output.png"

    print(f"Loading image '{input_filename}'...")

    try:
        image = Image.open(input_filename)
    except FileNotFoundError:
        print(f"Error: Input file not found at '{input_filename}'")
        print("Please upload test image into root directory.'")
        sys.exit(1)
    
    image_np = np.array(image)

    print("Image loaded into NumPy array. Shape:", image_np.shape, "Dtype:", image_np.dtype)
    print("Calling C++ function 'convert_to_grayscale'...")

    iris.convert_to_grayscale(image_np)

    print("C++ function executed successfully.")

    grayscale_image = Image.fromarray(image_np)
    grayscale_image.save(output_filename)

    print(f"Successfully created '{output_filename}'.")

if __name__ == "__main__":
    main()