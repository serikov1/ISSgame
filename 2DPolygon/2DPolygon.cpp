#include "2DPolygon.h"

#include <utility>

Polygon::Polygon(std::vector<Point> points, Point position, double height, const std::string& textureLink):
    Object(position, std::move(points), height, textureLink) {}

