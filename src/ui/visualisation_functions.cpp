#include "visualisation_functions.h"

Color determine_colour_based_on_kinetic_energy(double kinetic_energy, double reference_ke)
{

    if (reference_ke == 0) {
        reference_ke = 1.0; 
    }

    double ratio = kinetic_energy / reference_ke;
    if (ratio > 1) {
        ratio = 1;
    }
    else if (ratio < 0) {
        ratio = 0;
    }


    double red = ratio * 255.0;
    double blue = (1.0 - ratio) * 255.0;

    // Calculate opacity based on redness
    // Hotter atoms (redder) are more opaque
    // Cooler atoms (bluer) are less opaque but have a minimum opacity
    double min_opacity = 0.1;  
    double opacity_ratio = min_opacity + ratio * (1.0 - min_opacity);
    unsigned char alpha = static_cast<unsigned char>(opacity_ratio * 255.0);  

    Color color(static_cast<unsigned char>(red), 0, static_cast<unsigned char>(blue), alpha);

    return color;
}
