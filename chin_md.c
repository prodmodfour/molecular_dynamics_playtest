/* A simple MD program */
/* Chin Yong, Jul 2024 - STFC Daresbury Laboratory */

/* Temperature - kelvin (K) */
/* Length - angstrom (A) */
/* Energy - eV, change to J/mol */
/* Time - ps */
/* mass - atomic weight - g/mol */
/* Velocity scaling is 0.01, if to use all the units suggest here */
#define EV_TO_J_PER_MOLE 96400.0
#define J_PER_MOLE_TO_EV 1.037e-5
#define SCALING 0.01      

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Functions */
static void initialise_system(void);
static void calc_force(double *);
static double calc_ke(double);
static void calc_total_energy(double, double, int);
static void print_trajectory(int);
static void print_status(void);
static void deallocate_memory(void);

/* Global variables */
double Cu_mass = 63.546;                        /* atomic weight in g/mol */
double Timestep = 0.001;                         /* in ps */
double Cu_vdw_eps = 0.4802 * EV_TO_J_PER_MOLE;  /* J/mol */
double Cu_vdw_sigma = 2.285;                    /* Angstrom */
double Vdw_cutoff;                              /* 2.5 sigma */
int Total_atom;
int Total_MD_steps;
double Velocity_scale;
FILE *Fp;

typedef struct {
   char element[5];
   double x, y, z;
   double vx, vy, vz;
   double fx, fy, fz;
} Type_atoms;

Type_atoms *Atom;
 

void main()
{
 int loop, n;
 double ke, pe, sum_v_sq;
 double total_energy;
 double fvxi, fvyi, fvzi;
 double vxi, vyi, vzi;

 ke = pe = 0.0;
 initialise_system();

 /* Main MD loop - use Leap Frog algorithm */
 for(loop=1; loop <= Total_MD_steps; loop++) {

    /* (1) Calculate forces exert on every atoms */
    calc_force(&pe);

    /* Initialise KE and PE */
    sum_v_sq = 0.0;

    /* (2) MD trajectory */
    for(n=1; n <= Total_atom; n++) {

       /* Calculate velocity V(t + 0.5dt) */
       vxi = Atom[n].vx + Atom[n].fx * Velocity_scale; 
       vyi = Atom[n].vy + Atom[n].fy * Velocity_scale;
       vzi = Atom[n].vz + Atom[n].fz * Velocity_scale;

       /* Update positions r(t + dt) */
       Atom[n].x += vxi*Timestep;
       Atom[n].y += vyi*Timestep;
       Atom[n].z += vzi*Timestep;

       /* Calculate actual velocity at time t */
       /* (For kinetic energy calculations only) */
       fvxi = (vxi+Atom[n].vx)/2.0;
       fvyi = (vyi+Atom[n].vy)/2.0;
       fvzi = (vzi+Atom[n].vz)/2.0;
       sum_v_sq += fvxi*fvxi + fvyi*fvyi + fvzi*fvzi;
       
       /* Update initial velocities */ 
       Atom[n].vx = vxi;
       Atom[n].vy = vyi;
       Atom[n].vz = vzi;
    }

    ke = calc_ke(sum_v_sq);
    calc_total_energy(pe, ke, loop);
    print_trajectory(loop);
 }  /* end for MD steps */

 printf("Simulation finish.\n");
 deallocate_memory();
}


/* ------------------------------- */
/* This is where you setup your system */
static void initialise_system()
{
 int n;
 double applied_energy;
 FILE *fp;

 Total_MD_steps = 5000;
 Velocity_scale = SCALING*Timestep/Cu_mass;
 Vdw_cutoff = 2.5 * Cu_vdw_sigma;

 /* Read in coordinates */
 printf("Reading file config...\n");
 if((fp = fopen("config", "r")) == NULL) {
    printf("Fails to read input file. Looking for filename: config\n");
    exit(0);
 }
 fscanf(fp, "%d\n", &Total_atom);
 printf("Total atom = %d\n", Total_atom);

 /* Declare memory for the system */
 Atom = calloc((Total_atom+2), sizeof(Type_atoms));
 if(Atom == NULL) { 
    printf("Cannot allocate memory for Atom struct.\n");
    exit(0);
 }

 /* Assign particles */
 for(n=1; n <= Total_atom; n++) {
    fscanf(fp, "%s  %lf  %lf  %lf\n", Atom[n].element, &Atom[n].x, &Atom[n].y, &Atom[n].z);
 }

 fclose(fp);
 fp = NULL;

 /* Assign new impact atom with 10 eV on atom 912 (which is at the middle) */
//  applied_energy = 0.0;        /* eV */
//  applied_energy *= EV_TO_J_PER_MOLE;   /* To J/mol */
//  Atom[912].vz = -sqrt((2.0*applied_energy)/Cu_mass);

 /* Print status */
 print_status();

 /* Open file */
 Fp = fopen("simulation_cu", "w");
 fprintf(Fp, " %d  Time = 0.0 ps\n\n", Total_atom);
 for(n=1; n <= Total_atom; n++) 
    fprintf(Fp, "%s   %f  %f  %f\n", Atom[n].element, Atom[n].x, Atom[n].y, Atom[n].z);
 fflush(Fp);

}


/* ------------------------------------ */
static void print_status()
{
 printf("\n");
 printf("Simple MD driver, to model Cu atoms\n");
 printf("Cu...Cu interactions model by LJ 12-6.\n");
 printf("Epsilon = %f J/mol  sigma = %f Angstrom\n", Cu_vdw_eps, Cu_vdw_sigma);
 printf("Vdw cutoff = %f A (2.5 sigma)\n", Vdw_cutoff);
 printf("---------------------------\n\n");
} 


/* ------------------------------------ */
static double calc_ke(double sum_v_sq)
{
 double ke;

 ke = 0.5*Cu_mass*sum_v_sq;

 /* Converts to eV */
 ke *= J_PER_MOLE_TO_EV;

 return(ke);
}


/* ------------------------------------ */
static void calc_total_energy(double pe, double ke, int md_step)
{
 double total_energy;

 total_energy = pe+ke;
 printf("Time = %f ps   PE= %f  KE= %f  Total=%f eV\n", (double) md_step * Timestep, pe, ke, total_energy); 
}
  

/* ------------------------------------ */
/* LJ12-6 */
/* f(ij) = 24eps/r [2(sigma/r)^12 - (sigma/r)^6] * unit_vector (rij/r) */ 
/* Force, f is in unit Jmol^-1 A^-1 */
/* V(ij) = 4eps[(sigma/rij)^12 - (sigma/rij)^6] */
/* Potential, V is in unit J mol^-1 */
static void calc_force(double *v)
{
 int n, m;
 double xi, yi, zi;
 double xj, yj, zj;
 double dx, dy, dz;
 double fxij, fyij, fzij, fij;
 double sq_distance, distance, sigma_sq;
 double sr2, sr6, sr12, eps4, eps24;
 double potential_ij, total_potential, unit_vector_x, unit_vector_y, unit_vector_z; 
 char ai[7], aj[7];

 /* Assume is Cu-Cu only */
 eps4 = 4.0 * Cu_vdw_eps; 
 eps24 = 24.0 * Cu_vdw_eps;
 sigma_sq = Cu_vdw_sigma * Cu_vdw_sigma;

 /* Initialise forces for every timestep */
 for(n=1; n <= Total_atom; n++) 
    Atom[n].fx = Atom[n].fy = Atom[n].fz = 0.0;
 total_potential = 0.0;

 /* Loop over all unique pairs of atom i-j */
 for(n=1; n < Total_atom; n++) {
    xi = Atom[n].x;
    yi = Atom[n].y;
    zi = Atom[n].z;

    for(m=n+1; m <= Total_atom; m++) {
       xj = Atom[m].x;
       yj = Atom[m].y;
       zj = Atom[m].z;

       dx = xi - xj;
       dy = yi - yj;
       dz = zi - zj;
      
       sq_distance = dx*dx + dy*dy + dz*dz;
       distance = sqrt(sq_distance); 
       if(distance > Vdw_cutoff)
	  continue;

       /* Calculate force and potential between i-j */
       sr2 = sigma_sq/sq_distance;
       sr6 = sr2 * sr2 * sr2;
       sr12 = sr6 * sr6;
       unit_vector_x = dx/distance;
       unit_vector_y = dy/distance;
       unit_vector_z = dz/distance;

       potential_ij = sr12 - sr6;    
       total_potential += potential_ij;

       fij = (2.0*sr12 - sr6)/distance;
       fxij = fij * unit_vector_x;
       fyij = fij * unit_vector_y;
       fzij = fij * unit_vector_z; 
        
       /* Sum the forces on i due to all j */
       Atom[n].fx += fxij;
       Atom[n].fy += fyij;
       Atom[n].fz += fzij;

       /* Sum the forces on j due to i in ij direction */
       Atom[m].fx -= fxij;
       Atom[m].fy -= fyij;
       Atom[m].fz -= fzij;
    }
 }

 /* Multiply all forces with the energy factor 24epsilon */
 for(n=1; n <= Total_atom; n++) { 
    Atom[n].fx *= eps24;
    Atom[n].fy *= eps24;
    Atom[n].fz *= eps24;
 }

 /* Multiply by energy factor */
 *v = eps4 * total_potential;

 /* Convert to eV */
 *v = *v * J_PER_MOLE_TO_EV;
}


/* ------------------------------------ */
static void print_trajectory(int md_step)
{
 int n;

 fprintf(Fp, " %d    time = %f ps\n\n", Total_atom, (double) Timestep*md_step); 
 for(n=1; n <= Total_atom; n++)
    fprintf(Fp, "%s   %f   %f   %f\n", Atom[n].element, Atom[n].x, Atom[n].y, Atom[n].z);
}
    

/* ------------------------------------ */
static void deallocate_memory()
{
 free(Atom);
 Atom = NULL;

 fclose(Fp);
 Fp = NULL;
}
       

