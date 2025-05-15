cmake_minimum_required(VERSION 3.10)
project(Molecular_Dynamics_Playtest)
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
add_executable(Molecular_Dynamics_Playtest
    src/main.cpp

    src/ui/atom_management_widgets/AtomVTKPreview.cpp 
    src/ui/atom_management_widgets/AtomManager.cpp 
    src/ui/atom_management_widgets/StructureListViewer.cpp
    src/ui/atom_management_widgets/NewStructureDialog.cpp

    src/ui/data_loaders/BasicDataLoader.cpp

    src/ui/main_visualiser_widgets/MDVisualiser.cpp
    src/ui/main_visualiser_widgets/AtomVTKWidget.cpp

    src/ui/simulation_management_widgets/SimulationSettingsDialogue.cpp

    src/ui/SharedData.cpp
    src/ui/visualisation_functions.cpp
    src/ui/settings_functions.cpp

    src/simulation/StructureList.cpp
    src/simulation/Timestep.cpp

    src/atoms/Structure.cpp
    src/atoms/atom_generation_functions.cpp
    src/atoms/Atom.cpp
    src/atoms/AtomPairData.cpp
    src/atoms/AtomPairLibrary.cpp

    src/geometry/geometry_functions.cpp

    src/graphs/EnergyGraphWidget.cpp
    src/graphs/KineticEnergyHistogramWidget.cpp

    src/debug/logging.cpp
)

target_include_directories(Molecular_Dynamics_Playtest
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/physics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphs
        ${CMAKE_CURRENT_SOURCE_DIR}/src/debug
)


target_link_libraries(Molecular_Dynamics_Playtest
    PRIVATE
        ${VTK_LIBRARIES}
        Qt6::Charts
)

# --- Install the executable -----------------------------------------------
install(TARGETS Molecular_Dynamics_Playtest
        RUNTIME DESTINATION bin          # Windows
)
# --- tell CMake where your pre-built runtimes live ------------------------
set(QT_RUNTIME_DIR  "${CMAKE_SOURCE_DIR}/include/QtRuntime")   # adjust
set(VTK_RUNTIME_DIR "${CMAKE_SOURCE_DIR}/include/VtkRuntime")  # adjust

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
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\Molecular_Dynamics_Playtest.exe")
set(CPACK_NSIS_MENU_LINKS
    "https://github.com/prodmodfour/molecular_dynamics_playtest"  "Project home page")



# --- Debian / RPM specifics ----------------------------------------------
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ashraf.hussain@stfc.ac.uk")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE   "https://github.com/prodmodfour/molecular_dynamics_playtest")

include(CPack)                           # <- must be the last line