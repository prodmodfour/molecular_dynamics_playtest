cmake_minimum_required(VERSION 3.10)
project(Tutorial_1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn on automatic Qt processing (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt5 (Widgets is necessary for QApplication, QMainWindow, etc.)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Find VTK; you may specify a minimum version if needed, e.g. find_package(VTK 9.0 REQUIRED)
find_package(VTK 9 REQUIRED COMPONENTS
  CommonCore
  CommonDataModel
  RenderingCore
  RenderingOpenGL2
)
include(${VTK_USE_FILE})

# Add your source files here. If you have other .cpp files, list them as well:
add_executable(MDVisualizer
    tutorial_1.cpp
    ../../../ui/MainWindow.cpp
    ../../../ui/AtomVTKWidget.cpp
)

# If your headers are in non-standard locations, add them to the include path:
target_include_directories(MDVisualizer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../ui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../geometry

)

# Link against Qt5 and VTK libraries
target_link_libraries(MDVisualizer
    PRIVATE
        Qt5::Widgets
        ${VTK_LIBRARIES}
)
