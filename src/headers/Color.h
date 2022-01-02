#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h"

#include <iostream>

void write_color(std::ostream& out, Color pixel_color, int samples_per_pixel) 
{
    float r = pixel_color.r();
    float g = pixel_color.g();
    float b = pixel_color.b();

    //Divide the color by the number of samples and gamma-correct for gamma=2.0
    float scale = 1.f / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.f, 0.999f)) << ' '
        << static_cast<int>(256 * clamp(g, 0.f, 0.999f)) << ' '
        << static_cast<int>(256 * clamp(b, 0.f, 0.999f)) << '\n';
}

#endif