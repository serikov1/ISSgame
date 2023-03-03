#ifndef ISS_GAME_2DPOLYGON_H
#define ISS_GAME_2DPOLYGON_H

#include "../2DObject/2DObject.h"

class Polygon : public Object {
public:
    explicit Polygon(std::vector<Point> points = {}, Point position = {0, 0}, double height = 1,
            const std::string& textureLink = WALL_TEXTURE);
};

#endif //ISS_GAME_2DPOLYGON_H
