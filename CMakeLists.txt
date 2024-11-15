cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(molecular_dynamics_playtest VERSION 1.0)

# Basic build settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages via vcpkg
find_package(VTK CONFIG REQUIRED)
find_package(Qt6 COMPONENTS 
    Core
    Gui
    Widgets
    OpenGLWidgets
    REQUIRED
)

# Define executable
add_executable(visualise_md
    visualise_md.cpp
    MDVisualiser.cpp
    Launcher.cpp
)

# Link libraries
target_link_libraries(visualise_md PRIVATE 
    ${VTK_LIBRARIES}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# VTK module initialization
vtk_module_autoinit(
    TARGETS visualise_md
    MODULES ${VTK_LIBRARIES}
)