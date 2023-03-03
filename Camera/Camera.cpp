#include "Camera.h"
#include <cmath>

Camera::Camera(World &world, Point position, double verticalPosition, double height, double direction, double health,
               const std::string& texture, double fieldOfView, double eyesHeight, double depth, double walkSpeed,
               double jumpSpeed, double viewSpeed)
               : world_(world), Player(position, verticalPosition, height, health, texture), direction_(direction),
               fieldOfView_(fieldOfView), eyesHeight_(eyesHeight), depth_(depth), walkSpeed_(walkSpeed) {
    for(int i = 0; i < DISTANCES_SEGMENTS; i++) {
        double halfWidth = std::tan(fieldOfView_ / 2) * ((double)SCREEN_WIDTH / SCREEN_HEIGHT);
        double offset = ((i * 2.0 / (DISTANCES_SEGMENTS - 1.0)) - 1.0) * halfWidth;
        Point dir = { 1, 1 * offset };
        dir = dir.normalize();
        double angle = atan2(dir.y, dir.x);
        horizontalCos[i] = cos(angle);
        horizontalSin[i] = sin(angle);
    }
    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        double halfWidth = tan(fieldOfView_ / 2);
        double offset = ((i * 2.0 / (SCREEN_HEIGHT - 1.0)) - 1.0) * halfWidth;
        Point dir = { 1, 1 * offset };
        dir = dir.normalize();
        verticalTan[i] = tan(atan2(dir.y, dir.x));
    }
}

void Camera::addPlayer(const std::string& name, const std::shared_ptr<Player>& camera) {
    playersOnTheScreen_.insert({name, camera});
}

void Camera::removePlayer(const std::string &name) {
    playersOnTheScreen_.erase(name);
}

bool Camera::isSmooth() {
    return smooth_;
}

void Camera::setSmooth(bool active) {
    smooth_ = active;
}

bool Camera::isCollision() {
    return collision_;
}

void Camera::setCollision(bool active) {
    collision_ = active;
}

bool Camera::isTextures() {
    return textures_;
}

void Camera::setTextures(bool active) {
    textures_ = active;
}

bool Camera::keyboardControl(double elapsedTime, sf::RenderWindow &window) {
    // Prevents control from another window
    if(!window.hasFocus()) {
        hadFocus_ = false;
//        walkSound.pause();
        return true;
    }

    // Exit to menu
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        hadFocus_ = false;
        return false;
    }

    double dx = 0;
    double dy = 0;
    double sin_ = sin(direction_);
    double cos_ = cos(direction_);

    // Left and right
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        dx += sin_;
        dy -= cos_;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dx -= sin_;
        dy += cos_;
    }
    // Forward and backward
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        dx += cos_;
        dy += sin_;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        dx -= cos_;
        dy -= sin_;
    }
    if(getVerticalPosition() == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        verticalSpeed_ = jumpSpeed_ * (getHealth() / 1000 * 7 + 0.3); // health modificator from 0.3 to 1 => 0.3 + (health / 100) * 0.7
    }

    // Fire
//    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
//        if (weapons_[selectedWeapon_].fire())
//            fire();
//    }

    // Mouse movement
    if(sf::Mouse::getPosition(window).x != localMousePosition_.x) {
        int difference = sf::Mouse::getPosition(window).x - localMousePosition_.x;
        //sf::Mouse::setPosition({ (int)window.getSize().x / 2, (int)window.getSize().y / 2 }, window);
        localMousePosition_ = sf::Mouse::getPosition(window);
        // Ignoring first frame after window focus or game start
        if(hadFocus_) {
            direction_ += viewSpeed_ * difference;
        }
    }

    // Start/stop walk sound
//    if ((dx * dx + dy * dy) > walkSpeed_ * elapsedTime * walkSpeed_ * elapsedTime / 10 && getVerticalPosition() <= 0) {
//        if (walkSound_.getStatus() != sf::Sound::Status::Playing)
//            walkSound_.play();
//    }
//    else {
//        walkSound_.pause();
//    }

    // Move player
    shiftPrecise({ dx * walkSpeed_ * elapsedTime * (getHealth() / 100), dy * walkSpeed_ * elapsedTime * (getHealth() / 100) }, verticalSpeed_ * elapsedTime);
    verticalSpeed_ -= 9.8 * elapsedTime;

    // Remember that we had focused and weren't in menu
    hadFocus_ = true;
    return true;
}

void Camera::updateDistances(const World &world) {
    distances_.clear();
    allCollisions_.clear();

    Point forward = {std::cos(direction_), std::sin(direction_)};
    Point left = {-forward.y, forward.x};
    double halfWidth = std::tan(fieldOfView_/2) * ((double)SCREEN_WIDTH / SCREEN_HEIGHT);

    int i;
    int segments = static_cast<int>(2 * PI / fieldOfView_ * DISTANCES_SEGMENTS);

    // Visible for player segments
    for(i = 0; i < DISTANCES_SEGMENTS; i++) { // Need top-down map? Set limit to segments. No need? DISTANCE_SEGMENTS.
        std::pair<Point, Point> extraSegment;

        double offset = ((i * 2.0 / (DISTANCES_SEGMENTS - 1.0)) - 1.0) * halfWidth;
        extraSegment = {{getX(), getY()}, {getX() + depth_ * (forward.x + offset * left.x), getY() + depth_ * (forward.y + offset * left.y)}};

        std::vector<RayCastStructure> rayCastStructure;

        objectsRayCrossed(extraSegment, rayCastStructure, getName());

        if(!rayCastStructure.empty())
            distances_.push_back(rayCastStructure);
        else
            distances_.push_back({{depth_, 0, nullptr, 1}});
    }

    // Invisible for player segments
    if(!collision_) return;

    for(; i < segments; i++) {
        std::pair<Point, Point> extraSegment;
        double direction = direction_ + ((double)i / DISTANCES_SEGMENTS - 0.5) * fieldOfView_;
        extraSegment = {{getX(), getY()}, {getX() + COLLISION_AREA * cos(direction), getY() + COLLISION_AREA * std::sin(direction)} };

        hiddenObjectsRayCrossed(extraSegment, getName());
    }
}

void Camera::drawCameraView(sf::RenderTarget &window) {
    if(distances_.empty()) return;

    directionSin = std::sin(direction_);
    directionCos = std::cos(direction_);

    // Draw sky and rotate floor
    if(textures_) {
        sf::Sprite spriteSky;
        spriteSky.setTexture(world_.getSkyTexture());
        spriteSky.setTextureRect(sf::IntRect((int)(direction_ * SCREEN_WIDTH / 2), spriteSky.getTextureRect().height / 2 - SCREEN_HEIGHT / 2, SCREEN_WIDTH, 1080));
        spriteSky.setPosition(sf::Vector2f(0, 0)); // absolute position
        window.draw(spriteSky);
        world_.getFloorSprite().setRotation(static_cast<float>(-direction_ / PI * 180 - 90));

        if(smooth_) {
            sf::Image floorPrerendered;
            floorPrerendered.create(SCREEN_WIDTH, SCREEN_HEIGHT, sf::Color(0, 0, 0, 0));
            sf::Image floorTexture = world_.getFloorTexture().copyToImage();
            const int scale = 400;
            for(int i = 0; i < SCREEN_WIDTH; i++) {
                int horIndex = i / (SCREEN_WIDTH / DISTANCES_SEGMENTS);
                double baseOffset = (eyesHeight_ + getVerticalPosition()) / horizontalCos[horIndex];
                double horMod = horizontalCos[horIndex] * directionCos - horizontalSin[horIndex] * directionSin;
                double verMod = horizontalSin[horIndex] * directionCos + horizontalCos[horIndex] * directionSin;
                for(int j = SCREEN_HEIGHT / 2; j < SCREEN_HEIGHT; j++) {
                    double offset = baseOffset / verticalTan[j];
                    int left = (int)(scale * (getPosition().x + offset * horMod)) % 960;
                    int top = (int)(scale * (getPosition().y + offset * verMod)) % 960;
                    left += (left < 0) * 960;
                    top += (top < 0) * 960;
                    int alpha2 = 255 * (j * 2 - SCREEN_HEIGHT) / SCREEN_HEIGHT;
                    sf::Color col = floorTexture.getPixel(left, top);
                    col.a = alpha2;
                    floorPrerendered.setPixel(i, j, col);
                }
            }
            sf::Texture floorText;
            floorText.loadFromImage(floorPrerendered);
            sf::Sprite floorSprite(floorText);
            window.draw(floorSprite);
        }
    }

    for(int i = 0; i < DISTANCES_SEGMENTS; i++) {
        recursiveDrawing(window, distances_[i], i);
    }

    double halfWidth = std::tan(fieldOfView_ / 2) * ((double)SCREEN_WIDTH / SCREEN_HEIGHT);

    for(const auto& player : playersOnTheScreen_) {
        Point enemyDirection = (player.second->getPosition() - getPosition()).normalize();
        enemyDirection = {enemyDirection.x * std::cos(-direction_) - enemyDirection.y * std::sin(-direction_), enemyDirection.x * std::sin(-direction_) + enemyDirection.y * std::cos(-direction_)};
        double offset = enemyDirection.y / enemyDirection.x;
        if(std::abs(offset) < halfWidth && enemyDirection.x > 0) {
            int xPos = (int)((enemyDirection.y / enemyDirection.x / halfWidth + 1.0) / 2.0 * (DISTANCES_SEGMENTS - 1.0));
            int yPos = (int)(heightInPixels((player.second->getPosition() - getPosition()).abs(), player.second->getHeight() + 0.2, player.second->getVerticalPosition()).first);
            auto healthProgress = (double)player.second->getHealth();
            drawHealth(window, xPos - 50, yPos, 100, (int)healthProgress);
        }
    }
    drawHealth(window, 50, SCREEN_HEIGHT - 50, 400, (int)getHealth());
//    weapons_[i_selectedWeapon].draw(window);
}

void Camera::draw(sf::RenderTarget &window) {
    if(distances_.empty() || getHealth() <= 0) return;

    sf::CircleShape circle;
    circle.setRadius(COLLISION_DISTANCE * SCALE);
    circle.setOutlineColor(OUTLINE_CAMERA_COLOR);
    circle.setFillColor(FILL_CAMERA_COLOR);
    circle.setOutlineThickness(OUTLINE_CAMERA_THICKNESS);
    circle.setPosition((float)getX() * SCALE - COLLISION_DISTANCE * SCALE, (float)getY() * SCALE - COLLISION_DISTANCE * SCALE);

    double left = direction_ - fieldOfView_/2;
    double right = direction_ + fieldOfView_/2;

    sf::ConvexShape triangle;
    triangle.setPointCount(CONVEX_NUMBER + 2);
    triangle.setPoint(0, sf::Vector2f(0, 0));
    for(int i = 0; i <= CONVEX_NUMBER; i++) {
        int index = distances_[(int)i * DISTANCES_SEGMENTS / CONVEX_NUMBER].size() - 1;
        triangle.setPoint(i + 1, sf::Vector2f(static_cast<float>(distances_[i * DISTANCES_SEGMENTS / CONVEX_NUMBER][index].distance * cos(left + (right - left) * i / CONVEX_NUMBER) * SCALE),
                                              static_cast<float>(distances_[i * DISTANCES_SEGMENTS / CONVEX_NUMBER][index].distance * sin(left + (right - left) * i / CONVEX_NUMBER) * SCALE)));
    }
    triangle.setOutlineColor(OUTLINE_CAMERA_COLOR);
    triangle.setFillColor(FILED_OF_VEW_COLOR);
    triangle.setOutlineThickness(OUTLINE_CAMERA_THICKNESS);
    triangle.setPosition((float)getX() * SCALE, (float)getY() * SCALE);

    window.draw(triangle);
    window.draw(circle);}

void Camera::shiftPrecise(Point vector, double vertical) {
    setVerticalPosition(getVerticalPosition() + vertical);
    if(getVerticalPosition() < 0) {
        verticalSpeed_ = 0;
        setVerticalPosition(0);
    }
    if(!collision_) {
        shift(vector);
        return;
    }

    for(auto c : allCollisions_) {
        Point edgeVector = c.edge.second - c.edge.first;
        Point normal = { edgeVector.y, -edgeVector.x };
        normal = normal.normalize();
        double scalar = vector.x * normal.x + vector.y * normal.y;
        if(scalar < 0 && c.distance - std::abs(scalar) < COLLISION_DISTANCE) {
            vector.x -= normal.x * scalar;
            vector.y -= normal.y * scalar;
        }
    }

    shift(vector);
}

void Camera::objectsRayCrossed(const std::pair<Point, Point> &ray, std::vector<RayCastStructure> &rayCastStruct,
                               const std::string &name, int reflections) {
    std::pair<Point, Point> edge;
    Point nearCross;
    double len = 0;
    double closest = depth_;
    for(auto object : world_.getObjects()) {
        if(object.first == name || object.second.nodes().size() < 2) continue;

        // check crossing
        Point crossPoint = ray.second;

        std::pair<Point, Point> wall;
        // if ray crossed with object
        if(object.second.isCross(ray, wall, crossPoint, len)) {
            rayCastStruct.push_back({(ray.first - crossPoint).abs(), len, &object.second, object.second.getHeight()});
            // for collision
            double dist = (crossPoint - getPosition()).abs();
            if(dist < closest) {
                edge = std::move(wall);
                closest = dist;
                nearCross = crossPoint;
            }
        }
    }
    // collision
    if(collision_ && name == getName() && COLLISION_AREA >= closest) {
        CollisionInformation newCollision;
        newCollision.distance = (nearCross - getPosition()).abs();
        newCollision.edge = std::move(edge);
        newCollision.collisionPoint = nearCross;
        allCollisions_.push_back(newCollision);
    }
    // sort hits by descending of distance
    std::sort(rayCastStruct.begin(), rayCastStruct.end(), [](const RayCastStructure& lh, const RayCastStructure& rh) { return lh.distance > rh.distance; });
}

void Camera::hiddenObjectsRayCrossed(const std::pair<Point, Point> &ray, const std::string &name) {
    Object* obj = nullptr;
    std::pair<Point, Point> edge;
    double len = 0;
    Point nearCross = ray.second;
    for(auto object : world_.getObjects()) {
        if(object.first == name || object.second.nodes().size() < 2) continue;

        // check collision
        Point crossPoint = ray.second;

        // if object hitted and near closer than already finded - rember it
        std::pair<Point, Point> wall;
        if(object.second.isCross(ray, wall, crossPoint, len) && (nearCross - ray.first).abs() > (crossPoint - ray.first).abs()) {
            nearCross = crossPoint;
            obj = &object.second;
            edge = std::move(wall);
        }
    }
    // if something was hitted close enough - write it
    if(obj) {
        CollisionInformation newCollision;
        newCollision.distance = (nearCross - getPosition()).abs();
        newCollision.edge = std::move(edge);
        newCollision.collisionPoint = nearCross;
        allCollisions_.push_back(newCollision);
    }
}

void Camera::drawVerticalStrip(sf::RenderTarget &window, const RayCastStructure &obj, int shift, int f) {
    int horIndex = shift / (SCREEN_WIDTH / DISTANCES_SEGMENTS);
    sf::ConvexShape polygon;
    polygon.setPointCount(4);

    double vertical = 0;
    if(obj.object->type() == ObjectType::Player) vertical = dynamic_cast<Player*>(obj.object)->getVerticalPosition();

    std::pair<double, double> height_now = heightInPixels(obj.distance * horizontalCos[horIndex], obj.height, vertical);

    int alpha = (int)(255 * (1 - obj.distance / depth_));
    if (alpha > 255) alpha = 255;
    if (alpha < 0) alpha = 0;
    alpha = 255 - alpha;

    if (!textures_) polygon.setFillColor({ 255, 174, 174, static_cast<sf::Uint8>(alpha)});
    else polygon.setFillColor({ 255, 255, 255, static_cast<sf::Uint8>(alpha)});

    //polygon.setOutlineThickness(0); // we can make non zero thickness for debug
    polygon.setPosition((float)(shift * SCREEN_WIDTH / DISTANCES_SEGMENTS), 0);

    sf::Sprite sprite;
    if (obj.object && textures_) {
        sprite.setPosition(sf::Vector2f((float)shift * SCREEN_WIDTH / DISTANCES_SEGMENTS, (float)SCREEN_HEIGHT/2)); // absolute position
        int left, top, bot;
        sprite.setTexture(obj.object->loadTexture());
        left = obj.progress * SCREEN_WIDTH;
        top = 0;
        bot = SCREEN_HEIGHT;
        sprite.scale(1, (float)(height_now.second - height_now.first) / SCREEN_HEIGHT);


        sprite.setTextureRect(sf::IntRect(left, top, SCREEN_WIDTH / DISTANCES_SEGMENTS, bot - top));
        window.draw(sprite);
    }

    if (std::abs(obj.distance - depth_) > 0.001)
        window.draw(polygon);

    // Floor drawing
    int x = shift * SCREEN_WIDTH / DISTANCES_SEGMENTS;

    if (smooth_ || (f == 0) || (x % FLOOR_SEGMENT_SIZE != 0) || !textures_) return;

    const int scale = 400;
    double baseOffset = (eyesHeight_ + getVerticalPosition()) / horizontalCos[horIndex];
    double horMod = horizontalCos[horIndex] * directionCos - horizontalSin[horIndex] * directionSin;// cos (horizontalAngles[shift] + d_direction) = cos a * cos b - sin a * sin b
    double verMod = horizontalSin[horIndex] * directionCos + horizontalCos[horIndex] * directionSin;// sin (horizontalAngles[shift] + d_direction) = sin a * cos b + cos a * sin b

    for (int z = SCREEN_HEIGHT/2; z < SCREEN_HEIGHT; z += FLOOR_SEGMENT_SIZE) {
        double offset = baseOffset / verticalTan[z];
        int left = (int)(scale * (getPosition().x + offset * horMod));
        int top = (int)(scale * (getPosition().y + offset * verMod));
        int alpha2 = 255 * 2 * (z - SCREEN_HEIGHT / 2) / SCREEN_HEIGHT;

        sf::Sprite& floor = world_.getFloorSprite();

        floor.setTextureRect(sf::IntRect(left, top, FLOOR_SEGMENT_SIZE, FLOOR_SEGMENT_SIZE));
        floor.setPosition(sf::Vector2f((float)x, (float)z)); // absolute position
        floor.setColor({255, 255, 255, static_cast<sf::Uint8>(alpha2)});

        window.draw(floor);
    }
}

void
Camera::recursiveDrawing(sf::RenderTarget &window, const std::vector<RayCastStructure> &RayCastStructure, int shift,
                         int rec) {
    int i = 0;
    for (const auto& k : RayCastStructure) {
        if (i + 1 != RayCastStructure.size() || (rec != 1)) drawVerticalStrip(window, k, shift, 0);
        else drawVerticalStrip(window, k, shift, 1);
        i++;
    }
}

std::pair<double, double> Camera::heightInPixels(double distance, double height, double vertical) {
    std::pair<double, double> h = {0, 0};
    double mod = std::tan(fieldOfView_/2) * distance;
    h.first = (1 - (height - eyesHeight_ - getVerticalPosition() + vertical)/mod) * SCREEN_HEIGHT/2;
    h.second = (1 + (eyesHeight_ + getVerticalPosition() - vertical)/mod) * SCREEN_HEIGHT/2;
    return h;
}

std::pair<Object *, double> Camera::cameraRayCheck(RayCastStructure &structure) {
    std::pair<Object*, double> result = {nullptr, 1};

    if (structure.object->type() == ObjectType::Player) {
        auto heights = heightInPixels(structure.distance, structure.object->getHeight(), dynamic_cast<Player*>(structure.object)->getVerticalPosition());
        if (heights.first < SCREEN_HEIGHT/2 && heights.second > SCREEN_HEIGHT/2) {
            result.first = structure.object;
            result.second = structure.distance;
        }
    }
    return result;
}

double Camera::scalarWithNormal(Point edge, Point vector) {
    Point normal = {edge.y, -edge.x};
    normal = normal.normalize();
    double scalar = vector.x * normal.x + vector.y * normal.y;
    return scalar;
}

void Camera::drawHealth(sf::RenderTarget &window, int x, int y, int width, int health) {
    sf::ConvexShape polygon1;
    polygon1.setPointCount(4);
    sf::ConvexShape polygon2;
    polygon2.setPointCount(4);

    polygon1.setPoint(0, sf::Vector2f((float)x, (float)y));
    polygon1.setPoint(1, sf::Vector2f((float)x + width, (float)y));
    polygon1.setPoint(2, sf::Vector2f((float)x + width, (float)y + 20));
    polygon1.setPoint(3, sf::Vector2f((float)x, (float)y + 20));

    polygon2.setPoint(0, sf::Vector2f((float)x, (float)y));
    polygon2.setPoint(1, sf::Vector2f((float)x + width * health / 100, (float)y));
    polygon2.setPoint(2, sf::Vector2f((float)x + width * health / 100, (float)y + 20));
    polygon2.setPoint(3, sf::Vector2f((float)x, (float)y + 20));

    polygon1.setFillColor({255, 174, 174});
    polygon2.setFillColor({static_cast<sf::Uint8>((100 - health) * 255), static_cast<sf::Uint8>(health * 255 / 100), 0});

    polygon1.setOutlineThickness(3);
    window.draw(polygon1);
    window.draw(polygon2);
}


