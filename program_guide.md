# Program Guide

## Overview

### What does this program do?
This program gives you the ability to simulate, visualise and interact with a collection of atoms that have been pre configured by the user.

### Who is this guide for?
Developer who wish to extend the functionality of the program.

### Who is the program for?
Anyone who wants to play around with molecular dynamics simulations.


## Features

- Atom Generation
    - Single atoms
    - FCC Lattice

- Geometry
    - Determine the dimensions of the box that contains the atoms
    - Determine unit vectors between two points

- Physics
    - Apply kinetic energy to an atom
    - Evaluate the potentials and forces between atoms using Lennard-Jones potential
    - Update the velocity and position of an atom using Leapfrog integration

- Simulation
    - Run the simulation for a given number of timesteps
    - Vary these settings per timestep
        - R cutoff
        - Timestep size

- UI
    - Visualise the atoms in 3D
    - Control camera
    - Restart the simulation with different parameters
    
    
    

## Example Usage

### Basic atom simulation
    In this tutorial we will create a single atom with no initial velocity and watch it not move for 100 timesteps.

    In this tutorial we will learn how to:
        - Initialize the Qt application (QApplication app).
        - Create a single atom of type Cu with mass 63.546 and radius 1.28.
        - Configure the simulation (cutoff radius, timestep size).
        - Run the simulation for 100 timesteps, calling simulate_timestep each time.
        - Store each timestep’s data in simulation_data.
        - Print the position for each timestep (it should remain unchanged).
        - Visualize the results using MDVisualiser.
        -   // Test 1: Create a single atom with no initial velocity. Watch it not move for 100 timesteps.

            // Initialise Qt. This is required for the visualiser.
            QApplication app(argc, argv);
            
            ...
            
            return 0;
        }
        ```

        The `main()` function is the standard C++ entry point, taking command-line arguments via `argc` and `argv`. We initialize the Qt framework with `QApplication app(argc, argv)` to enable GUI features for visualization.

        2. Creating a Single Atom
        ```cpp
        std::string atom_type = "Cu";
        double mass = 63.546;
        double radius = 1.28;
        atoms::Atom atom(atom_type, mass, radius);
        std::vector<atoms::Atom> atoms;
        atoms.push_back(atom);
        ```

        We create a copper atom by:
        - Defining the type as "Cu"
        - Setting mass to 63.546 and radius to 1.28
        - Creating an `atoms::Atom` instance
        - Storing it in a vector for future extensibility
        
        With no initial velocity, the atom will remain at its starting position (typically (0,0,0)).

        3. Setting up the Simulation Configuration
        ```cpp
        double r_cutoff = 0; // It's just one atom, so no cutoff is needed
        double timestep_size = 0.001;
        simulation::Config config(r_cutoff, timestep_size);
        ```

        The configuration includes:
        - `r_cutoff`: The cutoff radius for atomic interactions (0 since we have one atom)
        - `timestep_size`: Time increment per simulation step (0.001 units)
        - A `Config` object that holds these parameters for the simulation

        4. Preparing the Atom Pair Library
        ```cpp
        atoms::AtomPairLibrary atom_pair_library;
        ```

        The `AtomPairLibrary` stores interaction parameters between different atom types. While not needed for a single atom, it's included as part of the framework.
        
        5. Running the Simulation
        ```cpp
        std::vector<simulation::Timestep> simulation_data;
        int total_timesteps = 100;
        double time = 0;

        for (int i = 0; i < total_timesteps; i++) 
        {
            simulation::Timestep timestep = simulation::simulate_timestep(
                config, 
                atoms, 
                time, 
                atom_pair_library
            );
            simulation_data.push_back(timestep);
        }
        ```

        The simulation:
        - Stores results in `simulation_data`
        - Runs for 100 timesteps
        - Calls `simulate_timestep()` each iteration to update the system
        - Since there's no velocity or forces, the atom's position remains unchanged

        6. Printing the Results
        ```cpp
        for (simulation::Timestep timestep : simulation_data)
        {
            std::cout << "Timestep: " << timestep.time 
                    << " Atom position: " 
                    << timestep.atoms[0].x << ", " 
                    << timestep.atoms[0].y << ", " 
                    << timestep.atoms[0].z << std::endl;
        }
        ```

        For each timestep, we print:
        - The simulation time
        - The atom's x, y, z coordinates
        - These values should remain constant throughout the simulation

    7. Visualizing the Simulation

    ```cpp
    ui::MDVisualiser visualiser(simulation_data);
    visualiser.show();
    app.exec();
    ```

    - `ui::MDVisualiser visualiser(simulation_data);`: A custom visualizer that takes all the timestep data.
    - `visualiser.show();`: Displays the visualization window (in a Qt environment).
    - `app.exec();`: Starts the Qt event loop, making the window responsive and interactive. The program continues running until the window is closed.


### Atom impacting a surface



# Introduction

In this tutorial, we will:

- Initialize the Qt application (`QApplication app`)
- Create an FCC lattice of copper atoms (8×8×4)
- Add a projectile atom (with 10 eV kinetic energy) above the lattice 
- Configure the simulation (cutoff radius, timestep size)
- Run the simulation for 100 timesteps, calling `simulate_timestep` each time
- Store each timestep's data in `simulation_data`
- Print the atom positions for each timestep
- Visualize the results using `MDVisualiser`




2. Setting Up the Main Function and Qt

```cpp
int main(int argc, char *argv[])
{
    // Initialize Qt. Required for the visualizer.
    QApplication app(argc, argv);

    // ... the rest of our simulation code will go here ...

    return 0;
}
```

By creating a QApplication instance at the start, we enable Qt’s event loop for later use in MDVisualiser.

3. Creating the FCC Lattice
We want to create a block of copper atoms arranged in an FCC pattern. Let’s define:

```cpp
cubes_in_x = 8
cubes_in_y = 8
cubes_in_z = 4
atom_spacing = 1.28 (generic value that fits copper’s atomic spacing in your model)
```

// Test 6: Create a FCC crystal lattice and fire an electron with 10 eV kinetic energy at it.

// 1 Dimensions of the crystal
```cpp
int cubes_in_x = 8;
int cubes_in_y = 8;
int cubes_in_z = 4;
double atom_spacing = 1.28;
```

// 2  Create a FCC crystal lattice of Cu atoms
```cpp
std::string atom_type = "Cu";
double mass   = 63.546;  // Mass of copper
double radius = 1.28;    // Approximate radius

std::vector<atoms::Atom> atoms = geometry::generate_atoms_in_fcc_pattern(
    cubes_in_x, 
    cubes_in_y, 
    cubes_in_z, 
    atom_spacing, 
    atom_type, 
    mass, 
    radius
);
```

Here, geometry::generate_atoms_in_fcc_pattern(...) returns a std::vector<atoms::Atom> containing the positions of all atoms in the lattice.

4. Determining the Box and Placing the Projectile Atom
Next, we define a geometry::Box that can encapsulate the lattice. The Box helps us determine where the “top” of the lattice is, so we can place our moving atom a bit above it:


// 3 Get the bounding box of the atom positions
```cpp
geometry::Box box(atoms);
```

// 4 Create a "moving atom" that starts a small offset above the top plane of the box
```cpp
atoms::Atom moving_atom(atom_type, mass, radius);
double z_offset = 3.0;  // Place it 3 units above the top plane
moving_atom.x = box.top_plane.center.x;
moving_atom.y = box.top_plane.center.y;
moving_atom.z = box.top_plane.center.z + z_offset;
```

The code:   

Calculates the box dimensions and planes (including the top plane).
Creates another copper “atom” (or any particle type in your system), set at the top’s center plus a small offset. You might conceptually treat this as an electron if your model allows.
5. Applying Kinetic Energy and Unit Vector
We want to give this moving atom a kinetic energy of 10 eV and direct it toward the crystal’s interior. We first calculate the unit vector from the moving atom to the center of the top plane. Then we apply the kinetic energy in that direction:


// 5 Calculate the direction vector toward the box interior
```cpp
Vector unit_vector = calculate_unit_vector_between_two_points(
    moving_atom.get_position(),
    box.top_plane.center
);
```

// 6) Apply 10 eV kinetic energy to the moving atom
```cpp
double kinetic_energy = 10.0;  // eV
apply_kinetic_energy(moving_atom, kinetic_energy, unit_vector);
```

// 7 Add the moving atom to the list of atoms
```cpp
atoms.push_back(moving_atom);
```

If your apply_kinetic_energy function sets velocity based on the given energy, this will ensure the moving atom is “fired” downwards into the lattice.

6. Configuring the Simulation
We specify the cutoff radius and timestep size for the molecular dynamics (MD) simulation. For instance, let’s use:

```cpp
r_cutoff = 4.0
timestep_size = 0.001
```
These values may differ depending on your MD model specifics:


double r_cutoff = 4.0;
double timestep_size = 0.001;
simulation::Config config(r_cutoff, timestep_size);
```

7. Running the Simulation
We will run the simulation for total_timesteps = 100, storing each Timestep in a vector:


```cpp
std::vector<simulation::Timestep> simulation_data;
int total_timesteps = 100;
double time = 0.0;

for (int i = 0; i < total_timesteps; i++) 
{
    simulation::Timestep timestep = simulation::simulate_timestep(config, atoms, time);

    // Store the timestep data
    simulation_data.push_back(timestep);

    // Update 'time' if needed (e.g., time += timestep_size) 
    // depending on how your simulate_timestep function handles time.
}
```

Each call to simulate_timestep should:

Compute forces on each atom (if multiple atoms are near the cutoff).
Update atom velocities and positions according to the MD integrator.
Return a Timestep object containing the updated positions, velocities, and time.
8. Printing the Results
We can verify the simulation by printing positions of the projectile atom (we’ll print the first atom in timestep.atoms just for demonstration, though in practice you might want a more selective print):

```cpp
for (const simulation::Timestep& timestep : simulation_data)
{
    // For brevity, just print the first atom's position:
    std::cout << "Timestep: " << timestep.time
              << "  Atom position: "
              << timestep.atoms[0].x << ", "
              << timestep.atoms[0].y << ", "
              << timestep.atoms[0].z << std::endl;
}
You could, of course, iterate over all atoms to see how the lattice is affected by the impact.

9. Visualizing the Simulation
Finally, we create and show an MDVisualiser window to see the motion of our projectile atom colliding with the crystal lattice:

```cpp
ui::MDVisualiser visualiser(simulation_data);
visualiser.show();

// Start Qt’s event loop
app.exec();
Once the visualization window closes, the program terminates.
```

Full Example Code
Putting it all together:


```cpp 
#include <QApplication>
#include <iostream>
#include <vector>
#include <string>

// Include your project-specific headers here...

int main(int argc, char *argv[])
{
    // Test 6: Create a FCC crystal lattice and fire an electron with 10 eV kinetic energy at it.

    // 1 Initialize Qt application
    QApplication app(argc, argv);

    // 2 Define crystal parameters
    int cubes_in_x = 8;
    int cubes_in_y = 8;
    int cubes_in_z = 4;
    double atom_spacing = 1.28;

    // 3 Create a FCC crystal lattice
    std::string atom_type = "Cu";
    double mass   = 63.546;
    double radius = 1.28;
    std::vector<atoms::Atom> atoms = geometry::generate_atoms_in_fcc_pattern(
        cubes_in_x,
        cubes_in_y,
        cubes_in_z,
        atom_spacing,
        atom_type,
        mass,
        radius
    );

    // 4 Determine bounding box and place a moving atom above it
    geometry::Box box(atoms);
    atoms::Atom moving_atom(atom_type, mass, radius);
    double z_offset = 3.0;
    moving_atom.x = box.top_plane.center.x;
    moving_atom.y = box.top_plane.center.y;
    moving_atom.z = box.top_plane.center.z + z_offset;

    // 5 Calculate direction and apply kinetic energy (10 eV)
    Vector unit_vector = calculate_unit_vector_between_two_points(
        moving_atom.get_position(),
        box.top_plane.center
    );
    double kinetic_energy = 10.0;
    apply_kinetic_energy(moving_atom, kinetic_energy, unit_vector);

    // 6 Add the moving atom to our system
    atoms.push_back(moving_atom);

    // 7 Create simulation config
    double r_cutoff = 4.0; 
    double timestep_size = 0.001;
    simulation::Config config(r_cutoff, timestep_size);

    // 8 Run the simulation for 100 timesteps
    std::vector<simulation::Timestep> simulation_data;
    int total_timesteps = 100;
    double time = 0.0;

    for (int i = 0; i < total_timesteps; i++)
    {
        simulation::Timestep timestep = simulation::simulate_timestep(config, atoms, time);
        simulation_data.push_back(timestep);

        // If time is not incremented by simulate_timestep, do so here:
        // time += timestep_size;
    }

    // 9 Print results for each timestep
    for (const auto& timestep : simulation_data)
    {
        std::cout << "Timestep: " << timestep.time 
                  << "  Atom position: "
                  << timestep.atoms[0].x << ", "
                  << timestep.atoms[0].y << ", "
                  << timestep.atoms[0].z << std::endl;
    }

    // 10 Visualize the simulation
    ui::MDVisualiser visualiser(simulation_data);
    visualiser.show();
    
    // Execute Qt event loop
    return app.exec();
}
```

Conclusion
By following these steps, you have:

Initialized Qt for visualization.
Generated an FCC lattice of copper atoms.
Placed a projectile atom above the lattice.
Applied a specific kinetic energy to fire it into the lattice.
Simulated 100 timesteps of MD.
Printed the projectile atom’s position at each timestep.
Visualized the entire process in MDVisualiser.
This general workflow can be adapted to any configuration (different lattice sizes, different projectile energies, etc.), making it a flexible starting point for more advanced MD simulations. Enjoy exploring what happens when your 10 eV “electron” collides with the copper lattice!

### Initialiser dialogue

### Restart simulation



