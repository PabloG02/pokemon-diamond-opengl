#pragma once

#include <map>
#include <string>

struct MapInfo {
    std::string name;
    std::string soundtrack;
};

namespace MapData {

inline const std::map<std::string, MapInfo> maps = {
    {"tp-twin", {"Twinleaf Town", "./assets/audio/music/twinleaf-town.mp3"}},
    {"tp-sand", {"Sandgem Town", "./assets/audio/music/sandgem-town.mp3"}},
    {"tp-r201", {"Route 201", "./assets/audio/music/route-201.mp3"}}
};

}