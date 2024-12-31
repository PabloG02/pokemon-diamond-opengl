#pragma once

#include "freeglut.h"
#include <array>
#include <string>
#include <unordered_map>

class Tile {
  public:
    // Each tile type has a base value (multiplied by 10)
    enum class TileType {
        Unknown = -1,
        Grass = 1,
        Sand = 2,
        GrassSand = 3,
        GrassSandCorner = 4,
        Beach = 5,
        GrassBeach = 6,
        GrassBeachCorner = 7,
        LakeWater = 9
    };

    // Direction follows numpad layout:
    // 7 8 9
    // 4 5 6
    // 1 2 3
    enum class Region {
        BottomLeft = 1,
        BottomCenter = 2,
        BottomRight = 3,
        CenterLeft = 4,
        Center = 5,
        CenterRight = 6,
        TopLeft = 7,
        TopCenter = 8,
        TopRight = 9
    };

    struct TileProperties {
        std::string texturePath;
        bool coversEntireTile;
        GLuint textureID{0};
    };

    static void render(int tileNumber);

  private:
    static void renderLakeWaterTile(Region region);

    static GLuint getOrLoadTexture(TileType tileType);
    static std::array<std::array<float, 2>, 4> calculateTexCoords(bool coversEntireTile,
                                                                  TileType tileType, Region region);
    static std::unordered_map<TileType, TileProperties> tilePropertiesMap;
};
