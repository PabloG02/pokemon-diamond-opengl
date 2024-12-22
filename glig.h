#pragma once

#include "freeglut.h"
#include <cmath>
#include <numbers>

/**
 * @brief Sign function.
 * @see https://en.wikipedia.org/wiki/Sign_function
 *
 * @param x Input value.
 * @return Returns 1.0 if x > 0, -1.0 if x < 0, and 0.0 if x == 0.
 */
float sign(float x);

/**
 * @brief Calculates the x-coordinate of a point in R3 based on parameters in R2.
 *
 * This function uses the superquadric equation to calculate the x-coordinate.
 *
 * @param u First coordinate of a point in R2.
 * @param v Second coordinate of a point in R2.
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction.
 * @param s2 Curvature exponent along the v direction.
 * @return The x-coordinate of a point in R3.
 */
float xSuperquadric(float u, float v, float R, float s1, float s2);

/**
 * @brief Calculates the y-coordinate of a point in R3 based on parameters in R2.
 *
 * This function uses the superquadric equation to calculate the y-coordinate.
 *
 * @param u First coordinate of a point in R2.
 * @param v Second coordinate of a point in R2.
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction.
 * @param s2 Curvature exponent along the v direction.
 * @return The y-coordinate of a point in R3.
 */
float ySuperquadric(float u, float v, float R, float s1, float s2);

/**
 * @brief Calculates the z-coordinate of a point in R3 based on parameters in R2.
 *
 * This function uses the superquadric equation to calculate the z-coordinate.
 *
 * @param u First coordinate of a point in R2.
 * @param v Second coordinate of a point in R2.
 * @param R Radius of the enveloping sphere.
 * @param s1 Curvature exponent along the u direction.
 * @param s2 Curvature exponent along the v direction.
 * @return The z-coordinate of a point in R3.
 */
float zSuperquadric(float u, float v, float R, float s1, float s2);

/**
 * @brief Creates a quadric object for drawing in 3D space.
 *
 * This function generates a wireframe representation of a superquadric based on the specified
 * parameters.
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
void igCreateQuadricObject(int pu, int pv, float uMax, float vMax, float R, float s1, float s2);

/**
 * @brief Draws a wireframe sphere.
 *
 * Creates a wireframe representation of a sphere with the given parameters.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireSphere(int pu, int pv);

/**
 * @brief Draws a wireframe cylinder (rulo).
 *
 * Creates a wireframe representation of a cylinder.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireRulo(int pu, int pv);

/**
 * @brief Draws a wireframe cube (dado).
 *
 * Creates a wireframe representation of a cube.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireDado(int pu, int pv);

/**
 * @brief Draws a wireframe semi-sphere.
 *
 * Creates a wireframe representation of a semi-sphere.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireSemiSphere(int pu, int pv);

/**
 * @brief Draws a wireframe cone.
 *
 * Creates a wireframe representation of a cone.
 *
 * @param pu Number of divisions in the u direction.
 * @param pv Number of divisions in the v direction.
 */
void igWireCone(int pu, int pv);

/**
 * @brief Draws a solid cube.
 *
 * Creates a wireframe representation of a cube.
 */
void igSolidCube(float width, float height, float depth);