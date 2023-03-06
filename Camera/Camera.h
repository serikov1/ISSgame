#ifndef ISS_GAME_CAMERA_H
#define ISS_GAME_CAMERA_H

#include <memory>
#include "../Player/Player.h"
#include "../World/World.h"
#include "../Settings/Settings.h"
#include "../Weapon/Gun.h"

class Client;

struct RayCastStructure {
    double distance; // How far is this texture
    double progress; // progress defines the point of texture we should load
    Object* object; // hitted object. We need this to get its texture.
    double height; // objects has different height
};

struct CollisionInformation {
    double distance;
    Point collisionPoint;
    std::pair<Point, Point> edge;
};

class Camera : public Player {
public:
    explicit Camera(World& world, Point position, double verticalPosition = 0, double height = 0.6,
                    double direction = 0, double health = 100, const std::string& texture = SKIN,
                    double fieldOfView = PI / 2, double eyesHeight = 0.5, double depth = 25,
                    double walkSpeed = 1.7, double jumpSpeed = 2.75, double viewSpeed = .005);

    void addPlayer(const std::string& name, const std::shared_ptr<Player>& camera);
    void removePlayer(const std::string& name);

    bool isSmooth() const;
    void setSmooth(bool active);
    bool isCollision() const;
    void setCollision(bool active);
    bool isTextures() const;
    void setTextures(bool active);

    bool keyboardControl(double elapsedTime, sf::RenderWindow& window);
    void updateDistances(const World& world);
    void drawCameraView(sf::RenderTarget& window);
    void draw(sf::RenderTarget& window) override;

    void shiftPrecise(Point vector, double vertical = 0);

    Client *client = nullptr;
private:
    double directionSin = 0;
    double directionCos = 0;
    double horizontalCos[DISTANCES_SEGMENTS]{};
    double horizontalSin[DISTANCES_SEGMENTS]{};
    double verticalTan[SCREEN_HEIGHT]{};

    std::vector<std::vector<RayCastStructure>> distances_;
    std::vector<CollisionInformation> allCollisions_;
    std::map<std::string, std::shared_ptr<Player>> playersOnTheScreen_;

    double direction_;
    double fieldOfView_;
    double eyesHeight_;
    double depth_;
    double verticalSpeed_ = 0;

    double jumpSpeed_{};
    double walkSpeed_;
    double viewSpeed_{};

    bool collision_ = true;
    bool hadFocus_ = false;

    World& world_;
    std::vector<Weapon> weapons_;

    sf::Vector2i localMousePosition_;

    bool textures_ = true;
    bool smooth_ = false;

    //    sf::Sound walkSound;

    // find all objects hitted by ray, except object with given name
    void objectsRayCrossed(const std::pair<Point, Point>& ray, std::vector<RayCastStructure>& rayCastStruct,
                           const std::string& name, int reflections = 0);

    // find objects crossed by ray near enough for collisions.
    // must be use only on invisible segments.
    void hiddenObjectsRayCrossed(const std::pair<Point, Point>& ray, const std::string& name);

    // draw 1 pixel column of screen
    void drawVerticalStrip(sf::RenderTarget& window, const RayCastStructure& obj, int shift, int f);

    void recursiveDrawing(sf::RenderTarget& window, const std::vector<RayCastStructure>& RayCastStructure,
                          int shift, int rec = 1);

    std::pair<double, double> heightInPixels(double distance, double height, double vertical);

    static void drawHealth(sf::RenderTarget& window, int x, int y, int width, int health);

    void fire();

    std::pair<Object*, double> cameraRayCheck(RayCastStructure& structure);
    static double scalarWithNormal(Point edge, Point vector);
};

#endif //ISS_GAME_CAMERA_H
