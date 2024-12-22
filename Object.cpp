#include "Object.h"
#include "freeglut.h"
#include "stb_image.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <array>
#include <algorithm>
#include "TextureLoader.h"

void Object::loadFromFile(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string currentGroup;

        std::string line;
        while (std::getline(inputFile, line)) {
            if (!line.empty() && line[0] != '#') { // Skip empty lines and comments
                std::istringstream stream(line);
                std::string keyword;
                stream >> keyword;

                if (keyword == "v") {
                    Vertex vertex;
                    stream >> vertex.x >> vertex.y >> vertex.z;
                    vertices.push_back(vertex);
                } else if (keyword == "vn") {
                    Normal normal;
                    stream >> normal.nx >> normal.ny >> normal.nz;
                    normals.push_back(normal);
                } else if (keyword == "vt") {
                    TextureCoord textureCoord;
                    stream >> textureCoord.u >> textureCoord.v;
                    textureCoords.push_back(textureCoord);
                } else if (keyword == "f") {
                    Face face;
                    std::string token;
                    int i = 0;
                    while (stream >> token) {
                        std::istringstream tokenStream(token);
                        std::string index;
                        std::vector<int> indices;

                        // Retrieve the vertex, texture, and normal indices
                        while (std::getline(tokenStream, index, '/')) {
                            indices.push_back(std::stoi(index) - 1); // Adjust for 0-based index
                        }

                        face.vertex_indices.emplace_back(
                            std::make_tuple(indices.at(0), indices.at(1), indices.at(2)));
                    }
                    groups[currentGroup].faces.emplace_back(face);
                } else if (keyword == "g") {
                    stream >> currentGroup;
                } else if (keyword == "mtllib") {
                    std::string materialLibrary;
                    stream >> materialLibrary;
                    materialLibraries.push_back(materialLibrary);
                } else if (keyword == "usemtl") {
                    stream >> groups[currentGroup].material;
                } else {
                    std::cerr << "OBJ unknown keyword: " << keyword << std::endl;
                }
            }
        }
        inputFile.close();

        // Now calculate the min and max coordinates of the vertices
        if (!vertices.empty()) {
            double minX = std::numeric_limits<double>::max();
            double maxX = std::numeric_limits<double>::lowest();
            double minY = std::numeric_limits<double>::max();
            double maxY = std::numeric_limits<double>::lowest();
            double minZ = std::numeric_limits<double>::max();
            double maxZ = std::numeric_limits<double>::lowest();

            for (const auto &vertex : vertices) {
                minX = std::min(minX, vertex.x);
                maxX = std::max(maxX, vertex.x);
                minY = std::min(minY, vertex.y);
                maxY = std::max(maxY, vertex.y);
                minZ = std::min(minZ, vertex.z);
                maxZ = std::max(maxZ, vertex.z);
            }

            std::cout << "Min Coordinates: (" << minX << ", " << minY << ", " << minZ << ")\n";
            std::cout << "Max Coordinates: (" << maxX << ", " << maxY << ", " << maxZ << ")\n";

            boundingBox = {Vertex{minX, minY, minZ}, Vertex{maxX, maxY, maxZ}};
        }

        for (const auto &materialLibrary : materialLibraries) {
            loadMaterialLibrary(materialLibrary,
                                filename.substr(0, filename.find_last_of('/') + 1));
        }
    }
}

void Object::loadMaterialLibrary(const std::string &mtlPath, const std::string &texturePath) {
    std::ifstream inputFile(texturePath + mtlPath);
    if (!inputFile) {
        std::cerr << "Failed to open material file: " << mtlPath << std::endl;
    }

    parseMaterialFile(inputFile);

    for (const auto &[name, material] : materials) {
        if (!material.map_Kd.empty()) {
            textures[name] = TextureLoader::loadTexture(texturePath + material.map_Kd);
            std::cout << "Loaded texture: " << material.map_Kd << " for material: " << name
                      << std::endl;
        }
    }

    inputFile.close();
}

void Object::parseMaterialFile(std::ifstream &inputFile) {
    std::string line;
    std::string currentMaterial;

    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '#') // Skip empty lines and comments
            continue;

        std::istringstream stream(line);
        std::string keyword;
        stream >> keyword;

        if (keyword == "newmtl") {
            stream >> currentMaterial;
        } else if (keyword == "Ka") {
            materials[currentMaterial].Ka = parseColor(stream);
        } else if (keyword == "Kd") {
            materials[currentMaterial].Kd = parseColor(stream);
        } else if (keyword == "Ks") {
            materials[currentMaterial].Ks = parseColor(stream);
        } else if (keyword == "Ke") {
            materials[currentMaterial].Ke = parseColor(stream);
        } else if (keyword == "Tf") {
            materials[currentMaterial].Tf = parseColor(stream);
        } else if (keyword == "Ns") {
            stream >> materials[currentMaterial].Ns;
        } else if (keyword == "Ni") {
            stream >> materials[currentMaterial].Ni;
        } else if (keyword == "d") {
            stream >> materials[currentMaterial].d;
        } else if (keyword == "illum") {
            stream >> materials[currentMaterial].illum;
        } else if (keyword == "map_Ka") {
            stream >> materials[currentMaterial].map_Ka;
        } else if (keyword == "map_Kd") {
            stream >> materials[currentMaterial].map_Kd;
        } else if (keyword == "map_Ks") {
            stream >> materials[currentMaterial].map_Ks;
        } else if (keyword == "map_Ns") {
            stream >> materials[currentMaterial].map_Ns;
        } else if (keyword == "map_d") {
            stream >> materials[currentMaterial].map_d;
        } else if (keyword == "map_bump" || keyword == "bump") {
            stream >> materials[currentMaterial].map_bump;
        } else {
            std::cerr << "MTL unknown keyword: " << keyword << std::endl;
        }
    }
}

Color Object::parseColor(std::istringstream &stream) {
    Color color;
    stream >> color.r >> color.g >> color.b;
    return color;
}

void Object::setGroupWithScrollingTexture(const std::string &groupName, double velocityX,
                                          double velocityY) {
    if (groups.contains(groupName)) {
        scrollingTextures[groupName] = {std::make_pair(velocityX, velocityY),
                                        std::make_pair(0.0f, 0.0f)};
    } else {
        std::cerr << "Group name not found: " << groupName << std::endl;
    }
}

void Object::update(const double deltaTime) {
    // Scrolling textures
    for (auto &[name, scrollingTexture] : scrollingTextures) {
        const auto &velocity = scrollingTexture.velocity;
        auto &offset = scrollingTexture.offset;
        // Calculate the new offset
        offset.first += velocity.first * deltaTime;
        offset.second += velocity.second * deltaTime;
    }
}

void Object::render() const {
    glColor3ub(255, 255, 255);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    for (const auto &[name, group] : groups) {
        // Set the material properties
        if (materials.contains(group.material)) {
            const auto &material = materials.at(group.material);
            GLfloat ambient[] = {static_cast<GLfloat>(material.Ka.r),
                                static_cast<GLfloat>(material.Ka.g),
                                static_cast<GLfloat>(material.Ka.b), 1.0f};
            GLfloat diffuse[] = {static_cast<GLfloat>(material.Kd.r),
                                static_cast<GLfloat>(material.Kd.g),
                                static_cast<GLfloat>(material.Kd.b), 1.0f};
            GLfloat specular[] = {static_cast<GLfloat>(material.Ks.r),
                                static_cast<GLfloat>(material.Ks.g),
                                static_cast<GLfloat>(material.Ks.b), 1.0f};

            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialf(GL_FRONT, GL_SHININESS, static_cast<GLfloat>(material.Ns));

            // Bind texture if the material has one
            if (!material.map_Kd.empty() && textures.contains(group.material)) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textures.at(group.material));

                // Apply texture transformation if it's a scrolling texture
                // TODO: name is set, but group.name is empty
                if (scrollingTextures.contains(name)) {
                    auto &scrollingTexture = scrollingTextures.at(name);
                    auto &offset = scrollingTexture.offset;

                    // Apply the offset
                    glMatrixMode(GL_TEXTURE);
                    glLoadIdentity();
                    glTranslated(offset.first, offset.second, 0.0);
                    glMatrixMode(GL_MODELVIEW);
                }
            } else {
                glDisable(GL_TEXTURE_2D);
            }
        } else {
            glDisable(GL_TEXTURE_2D);
        }

        glBegin(GL_TRIANGLES);
        // Render the faces
        for (const auto &face : group.faces) {
            //// Generate random color
            //float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            //float g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            //float b = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

            //// Set the color for the current triangle
            //glColor3f(r, g, b);

            for (const auto &vertex_index : face.vertex_indices) {
                int vertexIdx = std::get<0>(vertex_index);
                int texCoordIdx = std::get<1>(vertex_index);
                int normalIdx = std::get<2>(vertex_index);

                if (normalIdx >= 0) {
                    const auto &n = normals[normalIdx];
                    glNormal3d(n.nx, n.ny, n.nz);
                }
                if (texCoordIdx >= 0) {
                    const auto &tc = textureCoords[texCoordIdx];
                    glTexCoord2d(tc.u, tc.v);
                }

                const auto &v = vertices[vertexIdx];
                glVertex3d(v.x, v.y, v.z);
            }
        }
        glEnd();

        // Reset the texture matrix
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
    }

    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);
}

BoundingBox Object::getBoundingBox() const {
    return boundingBox;
}

double Object::calculateScaleFactor(double targetSize) const {
    const auto maxDimension =
        std::max({boundingBox.max.x - boundingBox.min.x, boundingBox.max.y - boundingBox.min.y,
                  boundingBox.max.z - boundingBox.min.z});

    return targetSize / maxDimension;
}
