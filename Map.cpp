#include "Map.h"
#include "Tile.h"
#include "glig.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // Load models in constructor
    house.loadFromFile("./assets/Twinleaf Town - Small house.obj");
    tree.loadFromFile("./assets/art/models/pine-tree/pine-tree.obj");
    flower.loadFromFile("./assets/flower.obj");
    grass.loadFromFile("./assets/art/models/grass/grass.obj");
    woodenSign.loadFromFile("./assets/art/models/wooden-sign/wooden-sign.obj");
    mailbox.loadFromFile("./assets/art/models/mailbox/mailbox.obj");
}

void Map::loadTerrain(const std::string &mapPath) {
    std::ifstream inputFile(mapPath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening terrain file: " << mapPath << std::endl;
        return;
    }

    terrain.clear();
    std::string row;
    while (std::getline(inputFile, row)) {
        if (!row.empty()) {
            std::istringstream tilesStream(row);
            std::vector<std::string> currentRow;
            std::string tile;

            while (tilesStream >> tile) {
                currentRow.push_back(tile);
            }

            terrain.push_back(currentRow);
        }
    }

    inputFile.close();
}

void Map::loadMapObjects(const std::string &mapPath) {
    std::ifstream inputFile(mapPath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening objects file: " << mapPath << std::endl;
        return;
    }

    objects.clear();
    std::string row;
    while (std::getline(inputFile, row)) {
        if (!row.empty()) {
            std::istringstream tilesStream(row);
            std::vector<std::string> currentRow;
            std::string tile;
            // TODO: Change to enum
            while (tilesStream >> tile) {
                currentRow.push_back(tile);
            }

            objects.push_back(currentRow);
        }
    }

    inputFile.close();
}

void Map::render() {
    renderTerrain();
    renderObjects();
}

void Map::renderTerrain() {
    glPushMatrix();
    for (int i = 0; i < terrain.size(); i++) {
        glPushMatrix();
        for (int j = 0; j < terrain[i].size(); j++) {
            Tile::render(std::stoi(terrain[i][j]));
            glTranslated(1.0, 0.0, 0.0);
        }
        glPopMatrix();
        glTranslated(0.0, 0.0, 1.0);
    }
    glPopMatrix();
}

void Map::renderObjects() {
    glPushMatrix();
    std::vector<std::vector<std::string>> objects_copy = objects; // Deep copy

    for (int i = 0; i < objects.size(); i++) {
        for (int j = 0; j < objects[i].size(); j++) {
            int objectId = std::stoi(objects_copy[i][j]);
            switch (objectId) {
            // No collision
            // Tall grass -> 1x1
            case 1:
                renderMapObject(grass, 1.0, 1.0 * j, 0.0, 1.0 * i, 1, 1, objects_copy, i, j);
                break;
            // Flowers -> 1x1
            case 30:
                renderMapObject(flower, 1.0, 1.0 * j, 0.0, 1.0 * i, 1, 1, objects_copy, i, j);
                break;
            // Edges (jumps) -> 1x1
            case 50: // Edges
                // renderMapObject(edge, 1.0, 1.0 * j, 0.0, 1.0 * i, 1, 1, objects_copy, i, j);
                break;

            // With collision
            // Trees -> 2x2
            case 100:
                renderMapObject(tree, 2.0, 1.0 * j + 0.5, 0.0, 1.0 * i + 0.5, 2, 2, objects_copy, i,
                                j);
                break;
            // Fences -> 1x1
            case 122:
                renderFence(ModelType::FenceH, j, 0.0, i);
                break;
            case 124:
                renderFence(ModelType::FenceV, j, 0.0, i);
                break;
            case 127:
                renderFence(ModelType::FenceTL, j, 0.0, i);
                break;
            case 129:
                renderFence(ModelType::FenceTR, j, 0.0, i);
                break;
            case 121:
                renderFence(ModelType::FenceBL, j, 0.0, i);
                break;
            case 123:
                renderFence(ModelType::FenceBR, j, 0.0, i);
                break;
            // Houses
            case 130: // 4x3
                renderMapObject(house, NULL, 1.0 * j + 1.5, 0.0, 1.0 * i + 1, 4, 3, objects_copy, i, j);
                break;
            // Sign -> 1x1
            case 200:
                renderMapObject(woodenSign, 1.0, 1.0 * j, 0.0, 1.0 * i, 1, 1, objects_copy, i, j);
                break;
            // Mailbox -> 1x1
            case 210:
                renderMapObject(mailbox, 1.0, 1.0 * j, 0.0, 1.0 * i, 1, 1, objects_copy, i, j);
                break;
            }
        }
    }
    glPopMatrix();
}

void Map::renderMapObject(const Object &object, double targetSize, double x, double y, double z,
                          int footprintWidth, int footprintHeight,
                          std::vector<std::vector<std::string>> &objects_copy, int i, int j) {
    glPushMatrix();

    double scale = 1.0;
    // Calculate scale factor
    if (targetSize != NULL) {
        scale = object.calculateScaleFactor(targetSize);
    }

    // Translate to grid position
    glTranslated(x, y, z);

    // Apply scaling
    glScaled(scale, scale, scale);

    // Render the model
    object.render();

    // Mark the grid footprint as used
    for (int dx = 0; dx < footprintHeight; ++dx) {
        for (int dy = 0; dy < footprintWidth; ++dy) {
            objects_copy[i + dx][j + dy] = "0";
        }
    }

    glPopMatrix();
}

void Map::renderFence(ModelType fenceType, double x, double y, double z) {
    glPushMatrix();
    glTranslated(0.0, 0.375, 0.0);
    glTranslated(x, y, z);

    using enum ModelType;
    switch (fenceType) {
    case FenceV:
    case FenceBL:
        glRotated(90, 0.0, 1.0, 0.0);
        break;
    case FenceBR:
        glRotated(180, 0.0, 1.0, 0.0);
        break;
    case FenceTR:
        glRotated(270, 0.0, 1.0, 0.0);
        break;
    }

    switch (fenceType) {
    case FenceH: // Default
    case FenceV:
        glColor3d(1.0, 1.0, 1.0);
        igSolidCube(0.33, 0.75, 0.33); // Vertical post
        glColor3d(0.8, 0.8, 0.8);
        igSolidCube(1.0, 0.25, 0.20); // Horizontal plank
        break;

    case FenceTL: // Default
    case FenceTR:
    case FenceBL:
    case FenceBR:
        glColor3d(1.0, 1.0, 1.0);
        igSolidCube(0.33, 0.75, 0.33); // Vertical post
        glColor3d(0.8, 0.8, 0.8);
        glTranslated(0.25, 0.0, 0.0);
        igSolidCube(0.5, 0.25, 0.20); // Horizontal plank
        glTranslated(-0.25, 0.0, 0.25);
        glRotated(90, 0.0, 1.0, 0.0); // Rotate for corner
        igSolidCube(0.5, 0.25, 0.20); // Vertical plank
        break;
    }

    glPopMatrix();
}
