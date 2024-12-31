#pragma once

#include "ModelType.h"
#include "Object.h"
#include <string>
#include <vector>

class Map {
  public:
    Map();

    void loadMap(const std::string &mapName);
    void loadTerrain(const std::string &mapPath);
    void loadMapObjects(const std::string &objectsPath);
    void loadEvents(const std::string &eventsPath);
    void render();

    const std::vector<std::vector<std::string>> &getCollisionMap() const {
        return objects;
    }
    const std::vector<std::vector<std::string>> &getEvents() const {
        return events;
    }

  private:
    void renderTerrain();
    void renderObjects();
    void renderMapObject(Object &object, double targetSize, double x, double y, double z,
                         int footprintWidth, int footprintHeight,
                         std::vector<std::vector<std::string>> &objects_copy, int i, int j);
    void renderFence(ModelType fenceType, double x, double y, double z);

    std::vector<std::vector<std::string>> terrain;
    std::vector<std::vector<std::string>> objects;
    std::vector<std::vector<std::string>> events;
    Object house;
    Object tree;
    Object flower;
    Object grass;
    Object woodenSign;
    Object mailbox;
    Object pokemonCenter;
    Object pokeMart;
    Object pokemonResearchLab;
};
