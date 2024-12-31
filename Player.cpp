#include "freeglut.h"
#include "Player.h"
#include "MapData.h"
#include "WorldScene.h"
#include <algorithm>

void Player::setIdleModel(const std::string &filename) {
    idleModel.loadFromFile(filename);
    BoundingBox box = idleModel.getBoundingBox();
    // Scale the model to fit the 1x1 grid
    scale = 1.0 / std::max(box.max.x - box.min.x, box.max.z - box.min.z);

    currentModel = std::make_shared<Object>(idleModel);
}

void Player::setWalkingModel(const std::vector<std::string> &filenames) {
    walkingModels.clear(); // Clear any previous models
    for (const auto &filename : filenames) {
        Object model;
        model.loadFromFile(filename);
        walkingModels.push_back(std::move(model));
    }
}

void Player::setCollisionMap(const std::vector<std::vector<std::string>> &map) {
    collisionMap = map;
}

void Player::setEventsMap(const std::vector<std::vector<std::string>> &map,
                          const std::string &mapId) {
    eventsMap = map;
}

void Player::queueMovement(Direction direction) {
    // If not moving, start movement immediately
    if (!isMoving) {
        startMovement(direction);
    }
    // If we're past the threshold of current movement, queue the next one
    else if (moveProgress >= QUEUE_THRESHOLD) {
        queuedDirection = direction;
        hasQueuedMovement = true;
    }
}

void Player::startMovement(Direction direction) {
    // Store start position for interpolation
    startX = x;
    startZ = z;

    // Direction deltas for movement
    int deltaX = 0, deltaZ = 0;

    // Set target position based on direction
    using enum Direction;
    switch (direction) {
    case UP:
        deltaZ = -1;
        orientation = UP;
        break;
    case DOWN:
        deltaZ = 1;
        orientation = DOWN;
        break;
    case RIGHT:
        deltaX = 1;
        orientation = RIGHT;
        break;
    case LEFT:
        deltaX = -1;
        orientation = LEFT;
        break;
    }

    // Check if the target tile is blocked (collision detection)
    if (isTileBlocked(static_cast<int>(x) + deltaX, static_cast<int>(z) + deltaZ)) {
        return; // If blocked, do not start movement
    }

    // Set target position based on deltas
    targetX = static_cast<int>(x) + deltaX;
    targetZ = static_cast<int>(z) + deltaZ;

    isMoving = true;
    moveProgress = 0.0;
}

bool Player::isTileBlocked(int x, int z) const {
    if (x < 0 || z < 0 || x >= collisionMap[0].size() || z >= collisionMap.size()) {
        return true; // Out of bounds
    }

    // Check if the tile is blocked
    return std::stoi(collisionMap[z][x]) >= 100;
}

void Player::update(double deltaTime) {
    if (!isMoving) {
        currentModel = std::make_shared<Object>(idleModel);
        return;
    }

    // Update model to use the walking animation
    currentModel = std::make_shared<Object>(walkingModels[currentWalkingModel]);

    moveProgress += moveSpeed * deltaTime;
    // Clamp progress to 1.0
    if (moveProgress >= 1.0) {
        moveProgress = 1.0;
        isMoving = false;

        // Snap to final position
        x = static_cast<double>(targetX);
        z = static_cast<double>(targetZ);

        // If we have a queued movement, start it immediately
        if (hasQueuedMovement) {
            // Cycle through walking models
            currentWalkingModel = (currentWalkingModel + 1) % walkingModels.size();

            startMovement(queuedDirection);
            hasQueuedMovement = false;
        }
    } else {
        // Interpolate position
        x = startX + (targetX - startX) * moveProgress;
        z = startZ + (targetZ - startZ) * moveProgress;
    }
}

void Player::render() {
    // Translate to the center of the map
    glTranslated(x, y, z);
    glRotated(90 * static_cast<int>(orientation), 0, 1, 0);
    glScaled(scale, scale, scale);
    currentModel->render();
}

double Player::getX() const {
    return x;
}

double Player::getY() const {
    return y;
}

double Player::getZ() const {
    return z;
}

bool Player::getIsMoving() const {
    return isMoving;
}

void Player::interact() {
    // Direction deltas for movement
    int deltaX = 0, deltaZ = 0;

    // Set target position based on direction
    using enum Direction;
    switch (orientation) {
    case UP:
        deltaZ = -1;
        break;
    case DOWN:
        deltaZ = 1;
        break;
    case RIGHT:
        deltaX = 1;
        break;
    case LEFT:
        deltaX = -1;
        break;
    }

    int targetEventX = static_cast<int>(x) + deltaX;
    int targetEventZ = static_cast<int>(z) + deltaZ;

    // Check if the tile the player is on is a teleporter
    if (eventsMap[static_cast<int>(z)][static_cast<int>(x)].starts_with("tp-")) {
        // Get the map ID of the event tile
        std::string currentMapId = WorldScene::getInstance().getCurrentMapId();
        // Set the new map
        WorldScene::getInstance().changeMap(eventsMap[static_cast<int>(z)][static_cast<int>(x)]);
        // EventsMap has been updated to the new one. Find the player's new position
        for (int i = 0; i < eventsMap.size(); i++) {
            for (int j = 0; j < eventsMap[i].size(); j++) {
                if (eventsMap[i][j] == currentMapId) {
                    x = static_cast<double>(j);
                    z = static_cast<double>(i);
                    return;
                }
            }
        }
    }
    // Check if the tile the player is facing is interactable
    else if (collisionMap[targetEventZ][targetEventX] == "100") { // TODO: Change
        printf("Interacted with event at (%d, %d)\n", targetEventX, targetEventZ);
    }
}
