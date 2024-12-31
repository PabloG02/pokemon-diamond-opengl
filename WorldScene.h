#pragma once

#include "Menu.h"
#include "Object.h"
#include "Player.h"
#include "Scene.h"
#include "Tile.h"
#include "ModelType.h"
#include <string>
#include <vector>
#include "Map.h"
#include "MapData.h"

class WorldScene : public Scene {
  public:
    static WorldScene &getInstance() {
        static WorldScene instance;
        return instance;
    }

    void initialize() override;
    void registerInputCallbacks() override;
    void render() override;
    void update(double deltaTime) override;

    void keyboardCallback(unsigned char key, int x, int y);
    void specialKeyboardCallbackMovement(int key, int x, int y);
    void specialKeyboardCallbackMenu(int key, int x, int y);

    std::string getCurrentMapId() const;
    void changeMap(const std::string &mapId);

  private:
    WorldScene() = default; // Private constructor for singleton

    void renderPlayer();

    Player player;
    Map map;
    std::string currentMapId{"tp-twin"};
    Menu menu;

    double alpha{0.0};
    double beta{35.0};
    double scale{0.15};
};