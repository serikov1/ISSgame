#ifndef ISS_GAME_2DPOINT_H
#define ISS_GAME_2DPOINT_H

struct Point {
    double x = 0;
    double y = 0;

    Point operator+(const Point& point) const;
    Point operator-(const Point& point) const;
    Point operator*(double number) const;
    Point operator/(double number) const;

    Point& operator=(const Point& point);

    Point& operator+=(const Point& point);
    Point& operator-=(const Point& point);
    Point& operator*=(double number);
    Point& operator/=(double number);

    bool operator==(const Point& point) const;
    bool operator!=(const Point& point) const;

    // scalar product
    double operator*(const Point& point) const;
    // vector product
    double cross(const Point& point) const;

    static double dot(const Point& a, const Point& b);

    static double cross(const Point& a, const Point& b);
    // Returns normalized vector
    Point normalize() const;
    // Returns squared vector length
    double sqrAbs() const;
    // Returns vector length
    double abs() const;
};
#endif //ISS_GAME_2DPOINT_H
