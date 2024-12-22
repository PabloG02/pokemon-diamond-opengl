#pragma once
#include <string>

struct Color {
    double r, g, b;
};

struct Material {
    std::string name;

    // Ambient color
    Color Ka;

    // Diffuse color
    Color Kd;

    // Specular color
    Color Ks;

    // Emission color
    Color Ke;

    // Transmission filter
    Color Tf;

    // Specular exponent (shininess)
    double Ns;

    // Optical density (index of refraction)
    double Ni{1.0};

    // Dissolution (1.0 is fully opaque, 0.0 is fully dissolved/transparent)
    double d{1.0};

    // Illumination model
    int illum{};

    // Texture maps
    std::string map_Ka;   // ambient texture
    std::string map_Kd;   // diffuse texture
    std::string map_Ks;   // specular texture
    std::string map_Ns;   // specular exponent texture
    std::string map_d;    // alpha texture
    std::string map_bump; // bump map
};