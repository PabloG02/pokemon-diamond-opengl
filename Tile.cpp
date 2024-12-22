#include "Tile.h"
#include "TextureLoader.h"

std::unordered_map<Tile::TileType, Tile::TileProperties> Tile::tilePropertiesMap = {
    {TileType::Grass, {"./assets/art/tileset/ngrass.png", true}},
    {TileType::Sand, {"./assets/art/tileset/nsand.png", true}},
    {TileType::GrassSand, {"./assets/art/tileset/nsandp.png", false}},
    {TileType::GrassSandCorner, {"./assets/art/tileset/nsandp.png", false}}};

GLuint Tile::getOrLoadTexture(TileType tileType) {
    if (!tilePropertiesMap.contains(tileType)) {
        glColor3ub(255, 0, 0); // Set error color.
        return 0;
    }

    auto &properties = tilePropertiesMap.at(tileType);
    if (properties.textureID == 0) {
        properties.textureID = TextureLoader::loadTexture(properties.texturePath.c_str(), false);
        if (properties.textureID == 0) {
            // Texture loading failed.
            glColor3ub(255, 0, 0); // Set error color.
        }
    }
    return properties.textureID;
}

std::array<std::array<float, 2>, 4> Tile::calculateTexCoords(bool coversEntireTile,
                                                             TileType tileType, Region region) {
    if (coversEntireTile) {
        return {{
            {0.0f, 0.0f}, // Bottom-left
            {1.0f, 0.0f}, // Bottom-right
            {1.0f, 1.0f}, // Top-right
            {0.0f, 1.0f}  // Top-left
        }};
    }

    // Handle corner tiles
    if (tileType == TileType::GrassSandCorner) {
        switch (region) {
        case Tile::Region::TopRight:
            return {{
                {0.5f, 0.5f}, // Bottom-left
                {0.5f, 0.0f}, // Bottom-right
                {1.0f, 0.0f}, // Top-right
                {1.0f, 0.5f}  // Top-left
            }};
        case Tile::Region::TopLeft:
            return {{
                {0.5f, 0.0f}, // Bottom-left
                {1.0f, 0.0f}, // Bottom-right
                {1.0f, 0.5f}, // Top-right
                {0.5f, 0.5f}  // Top-left
            }};
        case Tile::Region::BottomLeft:
            return {{
                {1.0f, 0.0f}, // Bottom-left
                {1.0f, 0.5f}, // Bottom-right
                {0.5f, 0.5f}, // Top-right
                {0.5f, 0.0f}  // Top-left
            }};
        case Tile::Region::BottomRight:
            return {{
                {1.0f, 0.5f}, // Bottom-left
                {0.5f, 0.5f}, // Bottom-right
                {0.5f, 0.0f}, // Top-right
                {1.0f, 0.0f}  // Top-left
            }};
        }
    }

    // Split texture into quadrants (0.5f is the midpoint)
    switch (region) {
    case Tile::Region::TopRight: // ok
        return {{
            {0.0f, 0.5f}, // Bottom-left
            {0.0f, 0.0f}, // Bottom-right
            {0.5f, 0.0f}, // Top-right
            {0.5f, 0.5f}  // Top-left
        }};
    case Tile::Region::TopLeft: // ok
        return {{
            {0.0f, 0.0f}, // Bottom-left
            {0.5f, 0.0f}, // Bottom-right
            {0.5f, 0.5f}, // Top-right
            {0.0f, 0.5f}  // Top-left
        }};
    case Tile::Region::BottomRight: // ok
        return {{
            {0.5f, 0.5f}, // Bottom-left
            {0.0f, 0.5f}, // Bottom-right
            {0.0f, 0.0f}, // Top-right
            {0.5f, 0.0f}  // Top-left
        }};
    case Tile::Region::BottomLeft: // ok
        return {{
            {0.5f, 0.0f}, // Bottom-left
            {0.5f, 0.5f}, // Bottom-right
            {0.0f, 0.5f}, // Top-right
            {0.0f, 0.0f}  // Top-left
        }};
    case Tile::Region::BottomCenter: // ok
        return {{
            {0.0f, 0.5f}, // Bottom-left
            {0.5f, 0.5f}, // Bottom-right
            {0.5f, 1.0f}, // Top-right
            {0.0f, 1.0f}  // Top-left
        }};
    case Tile::Region::CenterRight: // ok
        return {{
            {1.0f, 0.5f}, // Bottom-left
            {1.0f, 1.0f}, // Bottom-right
            {0.5f, 1.0f}, // Top-right
            {0.5f, 0.5f}  // Top-left
        }};
    case Tile::Region::CenterLeft: // ok
        return {{
            {0.5f, 1.0f}, // Bottom-left
            {0.5f, 0.5f}, // Bottom-right
            {1.0f, 0.5f}, // Top-right
            {1.0f, 1.0f}  // Top-left
        }};
    case Tile::Region::TopCenter: // CHECK
        return {{
            {0.5f, 1.0f}, // Bottom-left
            {0.0f, 1.0f}, // Bottom-right
            {0.0f, 0.5f}, // Top-right
            {0.5f, 0.5f}  // Top-left
        }};
    case Tile::Region::Center:
    default:
        return {{
            {0.0f, 0.0f}, // Bottom-left
            {1.0f, 0.0f}, // Bottom-right
            {1.0f, 1.0f}, // Top-right
            {0.0f, 1.0f}  // Top-left
        }};
    }
}

void Tile::render(int tileCode) {
    TileType tileType{static_cast<TileType>(tileCode / 10)};
    Region region{static_cast<Region>(tileCode % 10)};
    bool regionChangesGeometry{false};

    GLuint textureID{getOrLoadTexture(tileType)};

    if (tileType == TileType::LakeWater) {
        glColor3ub(99, 198, 255);
        regionChangesGeometry = true;
    }

    if (!regionChangesGeometry) {
        if (textureID != 0) {
            glColor3ub(255, 255, 255);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        auto &properties = tilePropertiesMap[tileType];
        auto texCoords = calculateTexCoords(properties.coversEntireTile, tileType, region);

        glBegin(GL_QUADS);
        glTexCoord2f(texCoords[0][0], texCoords[0][1]);
        glVertex3f(-0.5, 0.0, -0.5);
        glTexCoord2f(texCoords[1][0], texCoords[1][1]);
        glVertex3f(0.5, 0.0, -0.5);
        glTexCoord2f(texCoords[2][0], texCoords[2][1]);
        glVertex3f(0.5, 0.0, 0.5);
        glTexCoord2f(texCoords[3][0], texCoords[3][1]);
        glVertex3f(-0.5, 0.0, 0.5);
        glEnd();

        if (textureID != 0) {
            glDisable(GL_TEXTURE_2D);
        }
    } else {
        glPushMatrix();
        using enum TileType;
        switch (tileType) {
        case LakeWater:
            Tile::renderLakeWaterTile(region);
            break;
        }
        glPopMatrix();
    }
    /*glDisable(GL_TEXTURE_2D);*/
}

void Tile::renderLakeWaterTile(Region region) {
    using enum Region;
    switch (region) {
    case CenterLeft:
        glRotated(90.0, 0.0, 1.0, 0.0);
        break;
    case BottomCenter:
        glRotated(180.0, 0.0, 1.0, 0.0);
        break;
    case CenterRight:
    case TopRight:
        glRotated(270.0, 0.0, 1.0, 0.0);
        break;
    }

    switch (region) {
    case TopLeft:
    case TopRight:
    case BottomLeft:
    case BottomRight:
        // Default -> TopRight
        glBegin(GL_QUADS);
        // Top
        glVertex3f(-0.5, 0.0, -0.5);
        glVertex3f(-0.5, 0.0, 0.5);
        glVertex3f(-0.375, -0.125, 0.5);
        glVertex3f(-0.375, -0.125, -0.375);

        glVertex3f(-0.5, 0.0, -0.5);
        glVertex3f(-0.375, -0.125, -0.375);
        glVertex3f(0.5, -0.125, -0.375);
        glVertex3f(0.5, 0.0, -0.5);

        // Middle
        glVertex3f(-0.375, -0.125, -0.375);
        glVertex3f(-0.375, -0.125, 0.5);
        glVertex3f(-0.25, -0.5625, 0.5);
        glVertex3f(-0.25, -0.5625, -0.25);

        glVertex3f(-0.375, -0.125, -0.375);
        glVertex3f(-0.25, -0.5625, -0.25);
        glVertex3f(0.5, -0.5625, -0.25);
        glVertex3f(0.5, -0.125, -0.375);

        // Bottom
        glVertex3f(-0.25, -0.5625, -0.25);
        glVertex3f(-0.25, -0.5625, 0.5);
        glVertex3f(0.5, -0.5625, 0.5);
        glVertex3f(0.5, -0.5625, -0.25);
        glEnd();
        break;

    case TopCenter:
    case BottomCenter:
    case CenterLeft:
    case CenterRight:
        // Default -> TopCenter
        glBegin(GL_QUADS);
        glVertex3f(-0.5, 0.0, -0.5);
        glVertex3f(-0.5, -0.125, -0.375);
        glVertex3f(0.5, -0.125, -0.375);
        glVertex3f(0.5, 0.0, -0.5);

        glVertex3f(-0.5, -0.125, -0.375);
        glVertex3f(-0.5, -0.5625, -0.25);
        glVertex3f(0.5, -0.5625, -0.25);
        glVertex3f(0.5, -0.125, -0.375);

        glVertex3f(-0.5, -0.5625, -0.25);
        glVertex3f(-0.5, -0.5625, 0.5);
        glVertex3f(0.5, -0.5625, 0.5);
        glVertex3f(0.5, -0.5625, -0.25);
        glEnd();
        break;

    case Center:
        glBegin(GL_QUADS);
        glVertex3f(-0.5, -0.5625, -0.5);
        glVertex3f(-0.5, -0.5625, 0.5);
        glVertex3f(0.5, -0.5625, 0.5);
        glVertex3f(0.5, -0.5625, -0.5);
        glEnd();
        break;
    }
}