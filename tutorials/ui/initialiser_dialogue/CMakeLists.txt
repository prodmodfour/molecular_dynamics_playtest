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
    InteractionStyle
)

# ------------------------------------------------------------------------------
#  Add your executable and source files
# ------------------------------------------------------------------------------
add_executable(Initialiser_Dialogue
    initialiser_dialogue.cpp
    ../../../ui/MDVisualiser.cpp
    ../../../ui/AtomVTKWidget.cpp
    ../../../ui/InitialiserDialog.cpp
    ../../../ui/data_loaders/BasicDataLoader.cpp
)

# If you have headers in non-standard locations:
target_include_directories(Initialiser_Dialogue
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../geometry
)


target_link_libraries(Initialiser_Dialogue
    PRIVATE
        ${VTK_LIBRARIES}
)

