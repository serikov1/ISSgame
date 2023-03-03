#include "2DCircle.h"
#include <cmath>

Circle::Circle(double radius, Point position, double height, const std::string &textureLink, int convexNumber)
    : Object(position, {}, height, textureLink), radius_(radius) {
    for(int i = 0u; i < convexNumber; i++) {
        double x = radius_ * std::cos((double)i / convexNumber * 2 * PI + PI/4);
        double y = radius_ * std::sin((double)i / convexNumber * 2 * PI + PI/4);

        nodes().push_back({x, y});
    }
}

bool Circle::isCross(const std::pair<Point, Point> &ray, std::pair<Point, Point> &wall, Point &point, double &distance) {
    bool success = false;
    Point segment = ray.second - ray.first;
    Point offset = ray.first - position_;

    // (segment.x + offset.x)^2 + (segment.y + offset.y)^2 = radius^2
    auto a = static_cast<float>(segment.x * segment.x + segment.y * segment.y);
    auto b = static_cast<float>(2 * (offset.x * segment.x + offset.y * segment.y));
    auto c = static_cast<float>((offset.x * offset.x + offset.y * offset.y) - radius_ * radius_);

    float discriminant = b * b - 4 * a * c;
    if (discriminant >= 0)
    {
        discriminant = std::sqrt(discriminant);
        float t1 = (-b - discriminant) / (2 * a);
        float t2 = (-b + discriminant) / (2 * a);

        if (t1 >= 0 && t1 <= 1)
        {
            point.x = ray.first.x + t1 * segment.x;
            point.y = ray.first.y + t1 * segment.y;
            success = true;
        }
        else if (t2 >= 0 && t2 <= 1)
        {
            point.x = ray.first.x + t2 * segment.x;
            point.y = ray.first.y + t2 * segment.y;
            success = true;
        }
    }
    if (success)
    {
        double cx = cos(PI / 4), cy = sin(PI / 4);
        Point diff = point - position_;
        double angle = std::atan2((cx * diff.y - cy * diff.x) / radius_, (cx * diff.x + cy * diff.y) / radius_);
        wall.first = { position_.x + diff.y, position_.y - diff.x };
        wall.second = { position_.x - diff.y, position_.y + diff.x };
        distance = radius_ * angle;
    }
    return success;
}
