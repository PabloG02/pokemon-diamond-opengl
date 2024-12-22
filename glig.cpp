#include "glig.h"
#include "freeglut.h"
#include <array>

constexpr auto ALPHA(float v) {
    return (0.5 - v) * std::numbers::pi_v<float>;
}

constexpr auto BETA(float u) {
    return 2 * std::numbers::pi_v<float> * u;
}

/**
 * @brief Sign function.
 * @see https://en.wikipedia.org/wiki/Sign_function
 *
 * @param x Input value.
 * @return Returns 1.0 if x > 0, -1.0 if x < 0, and 0.0 if x == 0.
 */
float sign(float x) {
    if (x > 0)
        return 1.0f; // Positive case
    else if (x < 0)
        return -1.0f; // Negative case
    else
        return 0.0f; // Zero case
}

/**
 * @brief Calculates the x-coordinate of a point in R3 based on parameters in R2.
 *
 * @param u First coordinate of a point in R2.
 * @param v Second coordinate of a point in R2.
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction.
 * @param s2 Curvature exponent along the v direction.
 * @return The x-coordinate of a point in R3.
 */
float xSuperquadric(float u, float v, float R, float s1, float s2) {
    float cosAlpha = std::cos(ALPHA(v));
    float cosBeta = std::cos(BETA(u));

    float powCosAlpha = sign(cosAlpha) * std::pow(std::abs(cosAlpha), s1);
    float powCosBeta = sign(cosBeta) * std::pow(std::abs(cosBeta), s2);

    return R * powCosAlpha * powCosBeta;
}

/**
 * @brief Calculates the y-coordinate of a point in R3 based on parameters in R2.
 *
 * @param u First coordinate of a point in R2.
 * @param v Second coordinate of a point in R2.
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction.
 * @param s2 Curvature exponent along the v direction.
 * @return The y-coordinate of a point in R3.
 */
float ySuperquadric(float u, float v, float R, float s1, float s2) {
    float sinAlpha = std::sin(ALPHA(v));

    float powSinAlpha = sign(sinAlpha) * std::pow(std::abs(sinAlpha), s1);

    return R * powSinAlpha;
}

/**
 * @brief Calculates the z-coordinate of a point in R3 based on parameters in R2.
 *
 * @param u First coordinate of a point in R2.
 * @param v Second coordinate of a point in R2.
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction.
 * @param s2 Curvature exponent along the v direction.
 * @return The z-coordinate of a point in R3.
 */
float zSuperquadric(float u, float v, float R, float s1, float s2) {
    float cosAlpha = std::cos(ALPHA(v));
    float sinBeta = std::sin(BETA(u));

    float powCosAlpha = sign(cosAlpha) * std::pow(std::abs(cosAlpha), s1);
    float powSinBeta = sign(sinBeta) * std::pow(std::abs(sinBeta), s2);

    return R * powCosAlpha * powSinBeta;
}

/**
 * @brief Creates a quadric object based on parametric equations.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 * @param uMax Maximum bound for the u parameter, controlling the angular span in the u direction
 *        (1.0 for full rotation or less for partial sections).
 * @param vMax Maximum bound for the v parameter, controlling the angular span in the v direction
 *        (1.0 for full rotation or less for partial sections).
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction; controls the shape's bulge or flatness in
 *        this axis (values >1 for sharper edges, <1 for rounder, more gradual curvature).
 * @param s2 Curvature exponent along the v direction; controls the shape's bulge or flatness in
 *        this axis (values >1 for sharper edges, <1 for rounder, more gradual curvature).
 */
void igCreateQuadricObject(int pu, int pv, float uMax, float vMax, float R, float s1, float s2) {
    float u, v, inc_u, inc_v;
    float x, y, z;

    inc_u = uMax / pu;
    inc_v = vMax / pv;

    v = 0.0f;
    for (int j = 0; j <= pv; j++) {
        u = 0.0f;

        // Begin drawing a line loop
        glBegin(GL_LINE_STRIP);
        // Loop through each division in the u direction => Creates a circle
        for (int i = 0; i <= pu; i++) {
            // Calculate 3D coordinates using parametric equations
            x = xSuperquadric(u, v, R, s1, s2);
            y = ySuperquadric(u, v, R, s1, s2);
            z = zSuperquadric(u, v, R, s1, s2);
            // Specify a vertex at the calculated coordinates
            glVertex3f(x, y, z);
            // Increment u for the next iteration
            u += inc_u;
        }
        // End drawing the line loop
        glEnd();

        v += inc_v;
    }

    u = 0.0f;
    for (int i = 0; i <= pu; i++) {
        v = 0.0f;

        // Begin drawing a line loop
        glBegin(GL_LINE_STRIP);
        // Loop through each division in the v direction => Creates a circle
        for (int j = 0; j <= pv; j++) {
            // Calculate 3D coordinates using parametric equations
            x = xSuperquadric(u, v, R, s1, s2);
            y = ySuperquadric(u, v, R, s1, s2);
            z = zSuperquadric(u, v, R, s1, s2);
            // Specify a vertex at the calculated coordinates
            glVertex3f(x, y, z);
            // Increment v for the next iteration
            v += inc_v;
        }
        // End drawing the line loop
        glEnd();

        u += inc_u;
    }
}

/**
 * @brief Draws a wireframe sphere.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireSphere(int pu, int pv) {
    igCreateQuadricObject(pu, pv, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

/**
 * @brief Draws a wireframe ...
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireRulo(int pu, int pv) {
    igCreateQuadricObject(pu, pv, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f);
}

/**
 * @brief Draws a wireframe ...
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireDado(int pu, int pv) {
    igCreateQuadricObject(pu, pv, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f);
}

/**
 * @brief Draws a wireframe semi-sphere.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireSemiSphere(int pu, int pv) {
    igCreateQuadricObject(pu, pv, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f);
}

/**
 * @brief Draws a wireframe cone.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireCone(int pu, int pv) {
    igCreateQuadricObject(pu, pv, 1.0f, 0.5f, 1.0f, 2.0f, 1.0f);
}

/**
 * @brief Draws a wireframe cube.
 */
void igSolidCube(float width, float height, float depth) {
    float vertices[8][3] = {
        {-0.5f, 0.5f, 0.5f},   // Top front left
        {-0.5f, -0.5f, 0.5f},  // Bottom front left
        {0.5f, -0.5f, 0.5f},   // Bottom front right
        {0.5f, 0.5f, 0.5f},    // Top front right
        {-0.5f, 0.5f, -0.5f},  // Top back left
        {-0.5f, -0.5f, -0.5f}, // Bottom back left
        {0.5f, -0.5f, -0.5f},  // Bottom back right
        {0.5f, 0.5f, -0.5f}    // Top back right
    };

    // Define the indices for the cube faces (each face uses 4 unique vertices, arranged in a quad)
    std::array<GLuint, 24> indices{{
        0, 1, 2, 3, // Front face
        4, 5, 6, 7, // Back face
        0, 1, 5, 4, // Bottom face
        2, 3, 7, 6, // Top face
        0, 3, 7, 4, // Left face
        1, 2, 6, 5  // Right face
    }};

    glPushMatrix(); // Save current transformation state

    glScalef(width, height, depth); // Scale the cuboid

    // Enable indexed drawing
    glEnableClientState(GL_VERTEX_ARRAY);      // Enable vertex array for drawing
    glVertexPointer(3, GL_FLOAT, 0, vertices); // Point to the vertex data

    // Draw the cube using the indices array (each set of 4 indices corresponds to a quad)
    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}
