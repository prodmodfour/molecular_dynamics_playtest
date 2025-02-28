cmake_minimum_required(VERSION 3.10)
project(Tutorial_1)

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
add_executable(Tutorial_1
    tutorial_1.cpp
    ../../../ui/MDVisualiser.cpp
    ../../../ui/AtomVTKWidget.cpp
)

# If you have headers in non-standard locations:
target_include_directories(Tutorial_1
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../ui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../geometry
)


target_link_libraries(Tutorial_1
    PRIVATE
        ${VTK_LIBRARIES}
)

# ----------------------------------------------------------------------------
# POST-BUILD STEP: Copy needed DLLs into the same folder as Tutorial_1.exe
# ----------------------------------------------------------------------------
foreach(vtk_lib IN LISTS VTK_LIBRARIES)
    get_target_property(_type "${vtk_lib}" TYPE)
    if(_type STREQUAL "SHARED_LIBRARY" OR _type STREQUAL "MODULE_LIBRARY")
        add_custom_command(
            TARGET Tutorial_1
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${vtk_lib}>"
                "$<TARGET_FILE_DIR:Tutorial_1>"
        )
    endif()
endforeach()


