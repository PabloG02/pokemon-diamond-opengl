#pragma once

#include "Direction.h"
#include "Object.h"

class Player {
  public:
    Player() = default;

    void loadFromFile(const std::string &filename);
    void setCollisionMap(const std::vector<std::vector<std::string>> &map);
    void queueMovement(Direction direction);
    void startMovement(Direction direction);
    bool isTileBlocked(int x, int z) const;
    void update(double deltaTime);
    void render();
    // Getters return current position for camera following
    double getX() const;
    double getY() const;
    double getZ() const;
    bool getIsMoving() const;

    void interact() const;

  private:
    Object model;                               // The player's 3D model
    double scale{1.0};                          // The player's model scale to fit the 1x1 grid
    Direction orientation{Direction::DOWN};     // The player's orientation
    bool hasQueuedMovement = false;             // Whether we have a queued movement
    Direction queuedDirection{Direction::DOWN}; // Store next movement
    std::vector<std::vector<std::string>> collisionMap; // The map of collision tiles

    double x{15}, y{0}, z{15};    // The player's position (can be fractional during movement)
    int startX{15}, startZ{15};   // The player's start position for interpolation
    int targetX{15}, targetZ{15}; // The player's target position for interpolation
    bool isMoving{false};         // Whether the player is currently moving
    double moveProgress{0.0};     // The progress of the current movement (0.0 to 1.0)
    double moveSpeed{10.0};       // The speed at which the player moves
    // Set movement speed based on how many seconds you want per tile
    // const float SECONDS_PER_TILE = 0.1f;       // Takes 0.1 seconds to move one tile
    // float moveSpeed = 1.0f / SECONDS_PER_TILE; // Converts to moves per second

    // How close to the end of movement (0.0 to 1.0) before we allow queueing next move
    const double QUEUE_THRESHOLD = 0.8;
};
