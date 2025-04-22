cmake_minimum_required(VERSION 3.10)
project(Test_Simulation_Runs)

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
add_executable(Test_Simulation_Runs
    test_simulation_runs.cpp
    ../../../ui/MDVisualiser.cpp
    ../../../ui/AtomVTKWidget.cpp
    ../../../ui/visualisation_functions.cpp
    ../../../ui/data_loaders/BasicDataLoader.cpp
    ../../../ui/atom_management_widgets/AtomStructureInserter.cpp
    ../../../ui/atom_management_widgets/AtomStructureVTKPreview.cpp 

)

# If you have headers in non-standard locations:
target_include_directories(Test_Simulation_Runs
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../geometry
)


target_link_libraries(Test_Simulation_Runs
    PRIVATE
        ${VTK_LIBRARIES}
)

