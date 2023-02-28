#include "2DPoint.h"
#include <cmath>

Point Point::operator+(const Point &point) const {
    return Point{x + point.x, y + point.y};
}

Point Point::operator-(const Point &point) const {
    return Point{x - point.x, y - point.y};
}

Point Point::operator*(double number) const {
    return Point{x * number, y * number};
}

Point Point::operator/(double number) const {
    return Point{x/number, y/number};
}

Point &Point::operator=(const Point &point) = default;

Point &Point::operator+=(const Point &point) {
    x += point.x;
    y += point.y;
    return *this;
}

Point &Point::operator-=(const Point &point) {
    x -= point.x;
    y -= point.y;
    return *this;
}

Point &Point::operator*=(double number) {
    x *= number;
    y *= number;
    return *this;
}

Point &Point::operator/=(double number) {
    x /= number;
    y /= number;
    return *this;
}

bool Point::operator==(const Point &point) const {
    return x == point.x && y == point.y;
}

bool Point::operator!=(const Point &point) const {
    return x != point.x && y != point.y;
}

double Point::operator*(const Point &point) const {
    return x * point.x + y * point.y;
}

double Point::cross(const Point &point) const {
    return x * point.y - y * point.x;
}

double Point::dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

double Point::cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

Point Point::normalize() const {
    double length = abs();
    return Point{x/length, y/length};
}

double Point::sqrAbs() const {
    return std::pow(x, 2) + std::pow(y, 2);
}

double Point::abs() const {
    return std::sqrt(sqrAbs());
}


