cmake_minimum_required(VERSION 3.10)
project(Test_Simulation_Runs)
set(PROJECT_VERSION 0.7)  

# Tell CMake to use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic Qt processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

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
    ../../../ui/atom_management_widgets/AtomVTKPreview.cpp 
    ../../../ui/atom_management_widgets/AtomManager.cpp 
    ../../../simulation/StructureList.cpp
    ../../../simulation/Timestep.cpp
    ../../../ui/SharedData.cpp
    ../../../atoms/Structure.cpp
    ../../../atoms/atom_generation_functions.cpp
    ../../../geometry/geometry_functions.cpp
    ../../../ui/SimulationSettingsDialogue.cpp
    ../../../ui/atom_management_widgets/StructureListViewer.cpp
    ../../../ui/atom_management_widgets/NewStructureDialog.cpp
    ../../../graphs/EnergyGraphWidget.cpp
    ../../../graphs/KineticEnergyHistogramWidget.cpp
)

# If you have headers in non-standard locations:
target_include_directories(Test_Simulation_Runs
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../physics
        ${CMAKE_CURRENT_SOURCE_DIR}/../../simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/../../graphs
)


target_link_libraries(Test_Simulation_Runs
    PRIVATE
        ${VTK_LIBRARIES}
        Qt6::Charts
)

# --- Install the executable -----------------------------------------------
install(TARGETS Test_Simulation_Runs
        RUNTIME DESTINATION bin          # Windows & UNIX
)
# --- tell CMake where your pre-built runtimes live ------------------------
set(QT_RUNTIME_DIR  "${CMAKE_SOURCE_DIR}/QtRuntime")   # adjust
set(VTK_RUNTIME_DIR "${CMAKE_SOURCE_DIR}/VtkRuntime")  # adjust

# -- Qt --------------------------------------------------------------------
# Core DLLs + plugins + translations
install(DIRECTORY ${QT_RUNTIME_DIR}/ DESTINATION bin
        FILES_MATCHING PATTERN "*.dll")



# -- VTK -------------------------------------------------------------------
install(DIRECTORY ${VTK_RUNTIME_DIR}/ DESTINATION bin
        FILES_MATCHING PATTERN "*.dll")



# --------------------------------------------------------------------------
# Packaging
# --------------------------------------------------------------------------
include(InstallRequiredSystemLibraries)  # copies MSVC runtime on Windows

set(CPACK_PACKAGE_NAME            "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR          "STFC")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VTK/Qt visualiser for simulation runs")  
set(CPACK_PACKAGE_VERSION         "${PROJECT_VERSION}")

# Choose the generators you want available
#   Windows:   NSIS, WIX, ZIP
#   macOS:     DragNDrop, ZIP
#   Linux:     DEB, RPM, TGZ
set(CPACK_GENERATOR "ZIP")

# --- Windows specifics ----------------------------------------------------
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\Test_Simulation_Runs.exe")
set(CPACK_NSIS_MENU_LINKS
    "https://github.com/prodmodfour/molecular_dynamics_playtest"  "Project home page")



# --- Debian / RPM specifics ----------------------------------------------
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ashraf.hussain@stfc.ac.uk")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE   "https://github.com/prodmodfour/molecular_dynamics_playtest")

include(CPack)                           # <- must be the last line