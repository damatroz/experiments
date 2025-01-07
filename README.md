# Experiments

## Build

To run this project Conan is required to import the dependencies:

    pip install conan
    conan profile detect --force

In the main folder install the dependencies

    conan install . --output-folder=build --build=missing

Then open the folder with VS Code. It should compile using the CMake plugin

## References

- [Boost Preprocessor Recursion](https://stackoverflow.com/questions/19579018/boost-preprocessor-recursion)
- [How to use boost preprocessor to generate accessors?](https://stackoverflow.com/questions/24309309/how-to-use-boost-preprocessor-to-generate-accessors)
- [How to single out the first parameter sent to a macro taking only a variadic parameter](https://stackoverflow.com/questions/4750688/how-to-single-out-the-first-parameter-sent-to-a-macro-taking-only-a-variadic-par)
- [Macro returning the number of arguments it is given in C?](https://stackoverflow.com/questions/2308243/macro-returning-the-number-of-arguments-it-is-given-in-c)
- [C Preprocessor tricks, tips, and idioms](https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms)
