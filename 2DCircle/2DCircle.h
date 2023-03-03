#ifndef ISS_GAME_2DCIRCLE_H
#define ISS_GAME_2DCIRCLE_H

#include "../2DObject/2DObject.h"
#include "../Settings/Settings.h"

class Circle : public Object {
public:
    explicit Circle(double radius = 0.5, Point position = {0, 0}, double height = 1,
           const std::string &textureLink = COLUMN_TEXTURE, int convexNumber = CIRCLE_CONVEX_NUMBER);

    // Check the intersection of the ray with circle.
    // Returns true if ray intersects with circle, false otherwise.
    // wall - hitted wall.
    // point - hit point.
    // distance - distance from the beginning of wall to the hit point.
    bool isCross(const std::pair<Point, Point>& ray, std::pair<Point, Point>& wall, Point& point, double& distance) override;

private:
    double radius_;
};

#endif //ISS_GAME_2DCIRCLE_H
