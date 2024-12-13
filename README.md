To run this project Conan is required to import the dependencies:

    pip install conan
    conan profile detect --force

In the main folder install the dependencies

    conan install . --output-folder=build --build=missing

Then open the folder with VS Code. It should compile using the CMake plugin
