import sys

try:
    import iris
except ImportError:
    print("Error: The 'iris' module has not been built.")
    print("Please run the following commands from the project root:")
    print("  mkdir build")
    print("  cd build")
    print("  cmake ..")
    print("  make")
    sys.exit(1)

def main():
    """
    Calls the C++ add function and prints the result.
    """
    print("Attempting to call C++ function from Python...")

    a = 10
    b = 25
    result = iris.add(a, b)

    print(f"Successfully called C++ function: {a} + {b} = {result}")

if __name__ == "__main__":
    main()