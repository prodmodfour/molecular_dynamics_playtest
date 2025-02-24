cmake_minimum_required(VERSION 3.10)
project(Initialiser_Dialogue)

# Tell CMake to use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic Qt processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ------------------------------------------------------------------------------
#  Find Qt5 via Config mode. 
#  vcpkg installs Qt5 in a way that makes find_package(Qt5 CONFIG ...) possible.
# ------------------------------------------------------------------------------
find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)

# ------------------------------------------------------------------------------
#  Find VTK via Config mode. 
#  vcpkg installs VTK in a way that makes find_package(VTK CONFIG ...) possible.
# ------------------------------------------------------------------------------
find_package(VTK CONFIG REQUIRED COMPONENTS
    CommonCore
    CommonColor
    CommonDataModel
    RenderingCore
    RenderingOpenGL2
    GUISupportQt
)

# ------------------------------------------------------------------------------
#  Add your executable and source files
# ------------------------------------------------------------------------------
add_executable(Initialiser_Dialogue
    src/initialiser_dialogue.cpp
    src/MDVisualiser.cpp
    src/AtomVTKWidget.cpp
)

# If you have headers in non-standard locations:
target_include_directories(Initialiser_Dialogue
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../ui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../geometry
)

# ------------------------------------------------------------------------------
#  Link against Qt5 and VTK libraries
# ------------------------------------------------------------------------------
target_link_libraries(Initialiser_Dialogue
    PRIVATE
        Qt5::Widgets
        ${VTK_LIBRARIES}
)


