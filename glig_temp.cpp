//// 3D parametric shape generation with superquadrics
//
//#pragma once
//
//#include "freeglut.h"
//#include <cmath>
//#include <numbers>
//
//// Sign function (maths)
//// https://en.wikipedia.org/wiki/Sign_function
//float sign(float x) {
//    if (x > 0)
//        return 1.0f; // Positive case
//    else if (x < 0)
//        return -1.0f; // Negative case
//    else
//        return 0.0f; // Zero case
//}
//
//float f(float w, float m) {
//    return sign(std::sin(w)) * std::pow(std::abs(std::sin(w)), m);
//}
//
//float g(float w, float m) {
//    return sign(std::cos(w)) * std::pow(std::abs(std::cos(w)), m);
//}
//
//// Global scaling factors for superquadric dimensions
//float a1 = 1.0f, a2 = 1.0f, a3 = 1.0f;
//float r = 1.0f, s = 1.0f, t = 1.0f;
//
//float xSuperquadric(float u, float v) {
//    return a1 * g(v, 2.0f / r) * g(u, 2.0f / r);
//}
//
//float ySuperquadric(float u, float v) {
//    return a2 * g(v, 2.0f / s) * f(u, 2.0f / s);
//}
//
//float zSuperquadric(float u, float v) {
//    return a3 * f(v, 2.0f / t);
//}
//
//// https://en.wikipedia.org/wiki/Superquadrics
//void generateSuperquadricMesh(int uResolution, int vResolution) {
//    // u is in [-pi/2, pi/2]
//    float uStep = std::numbers::pi_v<float> / uResolution;
//    // v is in [-pi, pi]
//    float vStep = std::numbers::pi_v<float> / vResolution;
//    glBegin(GL_QUADS);
//    for (int i = 0; i <= uResolution; i++) {
//        float u = -std::numbers::pi_v<float> / 2 + i * uStep;
//
//        for (int j = 0; j <= vResolution; j++) {
//            float v = -std::numbers::pi_v<float> + j * vStep;
//
//            float x = xSuperquadric(u, v);
//            float y = ySuperquadric(u, v);
//            float z = zSuperquadric(u, v);
//        }
//
//    }
//
//    glEnd();
//}
