#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
#include <thread>



#define EV_TO_J_PER_MOLE 96400.0
#define J_PER_MOLE_TO_EV 1.037e-5
#define SCALING 0.01

/* Global variables */
double cu_mass = 63.546;                        /* atomic weight in g/mol */
double timestep_size = 0.001;                         /* in ps */
double epsilon = 0.4802 * EV_TO_J_PER_MOLE;  /* J/mol */
double sigma = 2.285;                    /* Angstrom */
double r_cutoff = 2.5 * sigma;                            /* 2.5 sigma */
double r_cutoff_squared = r_cutoff * r_cutoff;
int number_atoms;
int number_timesteps = 1000;
double velocity_scale = SCALING*timestep_size/cu_mass;

double lowest_x = 0;
double highest_x = 0;
double lowest_y = 0;
double highest_y = 0; 
double highest_z = 0;

 typedef struct {
   std::string element;
   double x, y, z;
   double vx, vy, vz;
   double fx, fy, fz;
} Type_atoms;

 std::ifstream xyz_file("case11.xyz");
 std::string line;

 Type_atoms *Atoms;

std::vector<std::string> split_sentence(std::string sen);
void print_atom_position(int atom_index, Type_atoms Atom);
void read_xyz();
void add_impact_atom(double z_offset, double applied_energy);\
void zero_forces();
double evaluate_forces();
double calculate_kinetic_energy(double sum_v_squared);


int main()
{
    auto start = std::chrono::high_resolution_clock::now();
 
    read_xyz();
    add_impact_atom(1, 1000);


    double kinetic_energy, potential_energy, sum_v_squared, total_energy;
    double vxi, vyi, vzi;
    double vxi2, vyi2, vzi2;
    double vxi3, vyi3, vzi3;
    double fxi, fyi, fzi;
    double delta_vxi, delta_vyi, delta_vzi;




    // Leapfrog Verlet Algorithm
    for (int timestep = 0; timestep < number_timesteps; timestep++)
    {
    // Reset variables
    zero_forces();
    sum_v_squared = 0.0;

    potential_energy = evaluate_forces();

    // Simulate Atom Trajectory
    for (int i = 0; i < number_atoms; i++)
    {
        // Calculate velocity V(t + 0.5dt)
        vxi = Atoms[i].vx;
        vyi = Atoms[i].vy;
        vzi = Atoms[i].vz;

        fxi = Atoms[i].fx;
        fyi = Atoms[i].fy;
        fzi = Atoms[i].fz;

        delta_vxi = fxi * velocity_scale;
        delta_vyi = fyi * velocity_scale;
        delta_vzi = fzi * velocity_scale;

        vxi2 = vxi + delta_vxi;
        vyi2 = vyi + delta_vyi;
        vzi2 = vzi + delta_vzi;

        // Update positions
        Atoms[i].x += vxi2 * timestep_size;
        Atoms[i].y += vyi2 * timestep_size;
        Atoms[i].z += vzi2 * timestep_size;

        // Calculate actual velocity at time t
        // For kinetic energy calculations only
        vxi3 = (vxi + vxi2) / 2;
        vyi3 = (vyi + vyi2) / 2;
        vzi3 = (vzi + vzi2) / 2;
        sum_v_squared += vxi3*vxi3 + vyi3*vyi3 + vzi3*vzi3;

        // Update velocities
        Atoms[i].vx = vxi2;
        Atoms[i].vy = vyi2;
        Atoms[i].vz = vzi2;
    }

    kinetic_energy = calculate_kinetic_energy(sum_v_squared);
    printf("Timestep %d Potential Energy: %f Kinetic Energy: %f Total Energy %f\n", timestep, potential_energy, kinetic_energy, potential_energy + kinetic_energy);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << duration.count() << "s " << std::endl;


 

}

std::vector<std::string> split_sentence(std::string sen) {
  
    // Create a stringstream object
    std::stringstream ss(sen);
    
    // Variable to hold each word
    std::string word;
    
    // Vector to store the words
    std::vector<std::string> words;
    
    // Extract words from the sentence
    while (ss >> word) {
      
        // Add the word to the vector
        words.push_back(word);
    }
    
    return words;
}

void print_atom_position(int atom_index, Type_atoms Atom)
{
    printf("Atom # %d element %s x %f y %f z %f\n", atom_index, Atom.element.c_str(), Atom.x, Atom.y, Atom.z);
}

void read_xyz()
{
int line_count = 0;
 while (std::getline(xyz_file, line)) 
 {
    if (line_count == 0)
    {
        number_atoms = std::stoi(line);
        number_atoms++;
        printf("Number of Atoms: %d\n", number_atoms);
        Atoms = new Type_atoms[number_atoms];
        line_count++;
        continue;
    }

    if (line_count == 1)
    {
        line_count++;
        continue;
    }

    std::vector<std::string> words = split_sentence(line);
    Atoms[line_count - 2].element = words[0];
    Atoms[line_count - 2].x = std::stod(words[1]);
    Atoms[line_count - 2].y = std::stod(words[2]);
    Atoms[line_count - 2].z = std::stod(words[3]);

    Atoms[line_count - 2].fx = 0;
    Atoms[line_count - 2].fy = 0;
    Atoms[line_count - 2].fz = 0;

    Atoms[line_count - 2].vx = 0;
    Atoms[line_count - 2].vy = 0;
    Atoms[line_count - 2].vz = 0;

    if (Atoms[line_count - 2].x < lowest_x)
    {
        lowest_x = Atoms[line_count - 2].x;
    }    

    if (Atoms[line_count - 2].x > highest_x)
    {
        highest_x = Atoms[line_count - 2].x;
    }

    if (Atoms[line_count - 2].y < lowest_y)
    {
        lowest_y = Atoms[line_count - 2].y;
    }    

    if (Atoms[line_count - 2].y > highest_y)
    {
        highest_y = Atoms[line_count - 2].y;
    }

    if (Atoms[line_count - 2].z > highest_z)
    {
        highest_z = Atoms[line_count - 2].z;
    }
    line_count++;
 }
}

void add_impact_atom(double z_offset, double applied_energy)
{
    Atoms[number_atoms - 1].element = "Cu";
    double x = (highest_x - lowest_x) / 2;
    Atoms[number_atoms - 1].x = x;
    double y = (highest_y - lowest_y) / 2;
    Atoms[number_atoms - 1].y = y;
    Atoms[number_atoms - 1].z = highest_z + z_offset;

    Atoms[number_atoms - 1].fx = 0;
    Atoms[number_atoms - 1].fy = 0;
    Atoms[number_atoms - 1].fz = 0;

    Atoms[number_atoms - 1].vx = 0;
    Atoms[number_atoms - 1].vy = 0;
    applied_energy *= EV_TO_J_PER_MOLE;
    Atoms[number_atoms - 1].vz = -std::sqrt((2.0*applied_energy)/cu_mass);
}

void zero_forces()
{
    for (int i = 0; i < number_atoms; i++)
    {
        Atoms[i].fx = 0;
        Atoms[i].fy = 0;
        Atoms[i].fz = 0;
    }
}

double evaluate_forces()
{
 double xi, yi, zi;
 double xj, yj, zj;
 double dx, dy, dz;
 double fxij, fyij, fzij, fij;
 double squared_distance, distance, sigma_squared;
 double sr2, sr6, sr12, epsilon4, epsilon24;
 double potential_ij, potential_energy, unit_vector_x, unit_vector_y, unit_vector_z;
 double reciprocal_distance;

 potential_energy = 0;

 // We will only ever have to deal with Cu-Cu interactions
 epsilon4 = 4 * epsilon;
 epsilon24 = 24 * epsilon;
 sigma_squared = sigma * sigma;

 // Find forces from every unique pair interaction
 for (int i = 0; i < number_atoms; i++)
 {

    xi = Atoms[i].x;
    yi = Atoms[i].y;
    zi = Atoms[i].z;

    for (int j = i + 1; j < number_atoms; j++)
    {
        xj = Atoms[j].x;
        yj = Atoms[j].y;
        zj = Atoms[j].z;

        dx = xi - xj;
        if (dx > r_cutoff)
        {
            continue;
        }
        dy = yi - yj;
        if (dy > r_cutoff)
        {
            continue;
        }
        dz = zi - zj;
        if (dz > r_cutoff)
        {
            continue;
        }

        squared_distance = dx*dx + dy*dy + dz*dz;
        if (squared_distance > r_cutoff_squared)
        {
            continue;
        }

        distance = std::sqrt(squared_distance);

        // Calculate force and potential between i and j
        sr2 = sigma_squared / squared_distance;
        sr6 = sr2 * sr2 * sr2;
        sr12 = sr6 * sr6;

        reciprocal_distance = 1 / distance;
        unit_vector_x = dx * reciprocal_distance;
        unit_vector_y = dy * reciprocal_distance;
        unit_vector_z = dz * reciprocal_distance;

        potential_ij = sr12 - sr6;
        potential_energy += potential_ij;

        fij = (2 * sr12 - sr6) * reciprocal_distance;
        fxij = fij * unit_vector_x;
        fyij = fij * unit_vector_y;
        fzij = fij * unit_vector_z; 

        /* Sum the forces on i due to all j */
        Atoms[i].fx += fxij;
        Atoms[i].fy += fyij;
        Atoms[i].fz += fzij;

        /* Sum the forces on j due to i in ij direction */
        Atoms[j].fx -= fxij;
        Atoms[j].fy -= fyij;
        Atoms[j].fz -= fzij;          
    }


    
    

 }
    for (int i = 0; i < number_atoms; i++)
    {
        Atoms[i].fx *= epsilon24;
        Atoms[i].fy *= epsilon24;
        Atoms[i].fz *= epsilon24;
    }
     // Multiply by energy factor
    potential_energy *= epsilon4;

    // Convert to eV
    potential_energy *= J_PER_MOLE_TO_EV;

    return potential_energy;

}

double calculate_kinetic_energy(double sum_v_squared)
{
    double kinetic_energy;
    kinetic_energy = 0.5*cu_mass*sum_v_squared;
    
    // Convert to eV
    kinetic_energy *= J_PER_MOLE_TO_EV;
    return kinetic_energy;
}