#ifndef ISS_GAME_2DOBJECT_H
#define ISS_GAME_2DOBJECT_H

#include "../2DPoint/2DPoint.h"
#include "../Drawable/Drawable.h"
#include "vector"
#include "../Settings/Settings.h"


class Object: virtual public Drawable {
public:
    explicit Object(Point position = {0, 0}, std::vector<Point> points = {}, double height = 1,
           const std::string& texture = WALL_TEXTURE, Point velocity = {0, 0});

    Object(const Object& object);

    // Check the intersection of the ray with this object.
    // Returns true if ray intersects with this object, false otherwise.
    // wall - hitted wall.
    // point - cross point.
    // distance - distance from the beginning of wall to the cross point.
    virtual bool isCross(const std::pair<Point, Point>& ray, std::pair<Point, Point>& wall, Point& point, double& distance);

    // Check segments intersection.
    // Returns true if segments crossing, false otherwise.
    // point - cross point.
    static bool segmentsCrossing(const std::pair<Point, Point>& segmentFirst, const std::pair<Point, Point>& segmentSecond, Point& point);

    void draw(sf::RenderTarget& window) override;
    sf::Texture& loadTexture();

    double getHeight() const;
    double getX() const;
    double getY() const;
    Point getPosition() const;
    std::string getName() const;

    void setPosition(Point position);
    void setNameLink(std::string nameLink);

    void shift(Point vector);
    std::vector<Point>& nodes();
    void setPoints(std::vector<Point> points);

//    virtual ObjectType type();

private:
    std::vector<Point> points_;
    Point position_;
    Point velocity_;
    std::string nameLink_;
    double height_;
    sf::Texture &texture_;
};

#endif //ISS_GAME_2DOBJECT_H
