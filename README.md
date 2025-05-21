# Molecular Dynamics Playtest

**Version:** 0.7

## Overview

VTK/Qt visualiser for simulation runs. This project provides a platform for visualizing molecular dynamics simulations.

## Dependencies

*   C++17 compatible compiler
*   CMake (version 3.10 or later)
*   Qt6 (core and charts modules, configured for VTK)
*   VTK (version 9.3.1 or later)

## Build Instructions

1.  **Clone the repository (if you haven't already):**
    ```bash
    git clone <repository-url>
    cd molecular_dynamics_playtest
    ```

2.  **Configure CMake:**
    Create a build directory and run CMake from there:
    ```bash
    mkdir build
    cd build
    cmake .. 
    ```
    *   For a **Release** build (optimized, no debug logging):
        ```bash
        cmake .. -DCMAKE_BUILD_TYPE=Release 
        ```
    *   For a **Debug** build (with debug logging):
        ```bash
        cmake .. -DCMAKE_BUILD_TYPE=Debug
        ```

3.  **Build the project:**
    ```bash
    cmake --build .
    ```
    *   Alternatively, on a multi-core system, you can speed up the build:
        ```bash
        cmake --build . --parallel <number-of-cores>
        ```

4.  **Post-build (Windows):**
    After building, you need to ensure the necessary DLLs are available to the executable. You have two options:
    *   Use `windeployqt` (typically found in your Qt installation's `bin` directory) to automatically copy Qt-related DLLs. You may need to point it to your executable.
    *   Manually copy the required DLLs from the `include/QtRuntime` and `include/VtkRuntime` directories (as specified in `CMakeLists.txt`) into your build directory (e.g., `build/bin/Release` or `build/bin/Debug`). The `install` commands in CMake should handle this if you run the install step.

## Installation

After building, you can install the project using:
```bash
cmake --install . --prefix <your_desired_install_location>
```
This will copy the executable and necessary runtime DLLs to the specified location (or a default system location if `--prefix` is not given).

The project is also configured to be packaged into a ZIP file using CPack. To generate the package:
```bash
cd build
cpack
```
This will create a `Molecular_Dynamics_Playtest-0.7-win64.zip` (or similar, depending on your system) file.

## Project Structure Highlights

*   `src/`: Contains the C++ source code.
    *   `atoms/`: Code related to atom definitions, structures, and interactions.
    *   `debug/`: Logging utilities.
    *   `geometry/`: Geometric primitives (Box, Plane, Vector) and functions.
    *   `graphs/`: Qt chart widgets for displaying energy data.
    *   `physics/`: Physics-related functions.
    *   `simulation/`: Core simulation logic, configuration, and data structures.
    *   `ui/`: User interface elements, including Qt widgets and VTK integration.
*   `include/`: Contains pre-built runtime libraries for Qt and VTK.
*   `CMakeLists.txt`: The main CMake build script.
*   `README.md`: This file.

## Usage

Run the executable generated in the build directory (e.g., `build/bin/Release/Molecular_Dynamics_Playtest.exe` or `build/bin/Debug/Molecular_Dynamics_Playtest.exe`).