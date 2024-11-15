cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Enable vcpkg integration if toolchain file is specified
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(molecular_dynamics_playtest VERSION 1.0)

# Basic build settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt settings (needed for VTK's Qt integration)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find VTK with Qt components
find_package(VTK CONFIG REQUIRED COMPONENTS 
    CommonCore
    GUISupportQt
    InteractionStyle
    RenderingOpenGL2
    RenderingQt
)

# Qt components are automatically found through VTK's Qt integration
find_package(Qt6 COMPONENTS 
    Core
    Gui
    Widgets
    OpenGLWidgets
    REQUIRED
)

# Define executable
add_executable(visualise_md
    main.cpp
    MDVisualiser.cpp
    Launcher.cpp
)

# Set Windows-specific properties
if(WIN32)
    set_target_properties(visualise_md PROPERTIES
        WIN32_EXECUTABLE TRUE
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug"
    )
endif()

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