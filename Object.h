#pragma once

#include "Material.h"
#include "freeglut.h"
#include <string>
#include <unordered_map>
#include <vector>

struct Vertex {
    double x, y, z;
};

struct TextureCoord {
    double u, v;
};

struct Normal {
    double nx, ny, nz;
};

struct Face {
    // (vertex_index, texture_index, normal_index)
    std::vector<std::tuple<int, int, int>> vertex_indices;
};

struct Group {
    std::string name;
    std::string material;
    std::vector<Face> faces;
};

struct BoundingBox {
    Vertex min, max;
};

struct ScrollingTexture {
    std::pair<double, double> velocity; // Direction and speed of scrolling (X and Y) in seconds
    std::pair<double, double> offset;   // Current offset
};

class Object {
  public:
    // Constructor
    Object() = default;

    void loadFromFile(const std::string &filename);
    void loadMaterialLibrary(const std::string &mtlPath, const std::string &texturePath);
    void parseMaterialFile(std::ifstream &inputFile);
    Color parseColor(std::istringstream &stream);
    void setGroupWithScrollingTexture(const std::string &groupName, double speedX, double speedY);
    BoundingBox getBoundingBox() const;
    double calculateScaleFactor(double targetSize) const;
    void render();
    void update(const double deltaTime);

  private:
    std::vector<Vertex> vertices;
    std::vector<TextureCoord> textureCoords;
    std::vector<Normal> normals;
    std::unordered_map<std::string, Group> groups;
    BoundingBox boundingBox{};

    std::vector<std::string> materialLibraries; // The material library files
    std::unordered_map<std::string, Material> materials;
    std::unordered_map<std::string, GLuint> textures;

    std::unordered_map<std::string, ScrollingTexture> scrollingTextures;

    GLuint displayListID = 0;

     // An object is static none of it's properties change (textures, geometry...)
    bool isStatic() const;
};
