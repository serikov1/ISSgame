#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Settings/Settings.h"
#include "2DPolygon/2DPolygon.h"
#include "2DCircle/2DCircle.h"
#include "World/World.h"
#include "Camera/Camera.h"
#include "Menu/Menu.h"
#include "2DObject/2DObject.h"
#include "GraphicsLoader/GraphicsLoader.h"
#include "Time/Time.h"
#include "Connection/Client/Client.h"
#include "Connection/Server/Server.h"

void InitNetwork(Server& server, Client& client) {
    std::string clientIp;
    sf::Uint16 clientPort;
    sf::Uint16 serverPort;
    std::ifstream connectfile("connect.txt", std::ifstream::in);

    // If failed to read client settings
    if (!connectfile.is_open() || !(connectfile >> clientIp >> clientPort) || sf::IpAddress(clientIp) == sf::IpAddress::None) {
        connectfile.close();
        // Create file and write default settings
        clientIp = "127.0.0.1";
        clientPort = 54000;
        std::ofstream temp("connect.txt", std::ofstream::out);
        temp << clientIp << std::endl << clientPort;
        temp.close();
    }
    connectfile.close();

    // If failed to read server settings
    connectfile.open("server.txt", std::ifstream::in);
    if (!connectfile.is_open() || !(connectfile >> serverPort)) {
        connectfile.close();
        // Create file and write default settings
        serverPort = 54000;
        std::ofstream temp("server.txt", std::ofstream::out);
        temp << serverPort;
        temp.close();
    }
    connectfile.close();

    if (clientIp == sf::IpAddress::LocalHost) server.start(serverPort);
    client.connect(clientIp, clientPort);
}

int main() {
    

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pseudo3DEngine");
    window.setFramerateLimit(60);

    std::vector<Point> columnPositions = {{0,0}, {3,0}, {3,3}, {6,3}, {6,8}, {3,8}, {3,11}, {0,11}, {0,8}, {-3,8}, {-3,3}, {0,3}};
    std::vector<Circle> columns(12);
    std::vector<Polygon> walls(12);

    World world(100, 100);
    Camera* camera = nullptr;
    for (size_t i = 0; i < columns.size(); i++) {
        columns[i].setPosition(columnPositions[i]);
        if (i != columns.size() - 1) walls[i].setPoints({columnPositions[i], columnPositions[i + 1]});
        else walls[i].setPoints({columnPositions[i], columnPositions[0]});

        world.addObject(walls[i], "wall" + std::to_string(i + 1));
        world.addObject(columns[i], "col" + std::to_string(i + 1));
    }

    Polygon object3({ {1, 1}, {2, 2}, {1, 2}, {1, 2} }, { -0.79, 3.95 });
    Polygon object4({ {0, 0}, {.1, 0}, {.1, .1}, {0, .1} }, { 1.92, 6.18 });
    Polygon object5({ {0, 0}, {.3, 0}, {.3, .3}, {0, .3} }, { 1.84, 9 });
    Polygon object6({ {0, 0}, {.3, 0}, {.3, .3}, {0, .3} }, { 1.11, 7 }, 0.5);
    Polygon object7({ {0, 0}, {.1, 0}, {.1, .1}, {0, .1} }, { 1, 2 }, 0.5);

    Polygon object8({ {0, 0}, {.3, 0}, {.3, .3}, {0, .3} }, { 3.6, 4.6 }, 0.9);
    Polygon object9({ {0, 0}, {.3, 0}, {.3, .3}, {0, .3} }, { 1.5, 4.2 }, 0.9);
    Polygon object10({ {0, 0}, {.1, 0}, {.1, .1}, {0, .1} }, { 2.5, 5.2 }, 0.9);

    world.addObject(object3, "object3");
    world.addObject(object4, "object4");
    world.addObject(object5, "object5");
    world.addObject(object6, "object6");
    world.addObject(object7, "object7");

    world.addObject(object8, "object8");
    world.addObject(object9, "object9");
    world.addObject(object10, "object10");

    Menu menu;

    Server server(world);
    Client client(world);
    server.addSpawn({1.5, 1.5});
    server.addSpawn({1.5, 9});

    while (window.isOpen()) {
        // Time update
        Time::update();
        double elapsedTime = Time::deltaTime();

        // Title update
        std::string title = "Pseudo3DEngine " + std::to_string((double) 1 / elapsedTime) + "fps.";
        if (camera != nullptr) title += " x:" + std::to_string(camera->getX()) + ", y:" + std::to_string(camera->getY());
        window.setTitle(title);

        // Close event search
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // Actually game
        window.clear();
        if (!menu.isPaused() && camera != nullptr) {
            window.setMouseCursorVisible(false);
            camera->updateDistances(world);
            camera->drawCameraView(window);
            //world.draw(window); // top-down debug map. To fix exception - look into "Camera::updateDistances"

//             if escape was pressed
            if (!camera->keyboardControl(elapsedTime, window)) {
                client.disconnect();
                server.stop();
                menu.setPause();
            }
            server.update();
            client.update();

            // if client timeout or disconnected
            if (!client.isWorking()) {
                menu.setPause();
                camera = nullptr;
            }
        }
            // Menu
        else {
            window.setMouseCursorVisible(true);
            menu.drawMenu(window, elapsedTime);
            // if "play game" pressed
            if (!menu.isPaused()) {
                window.clear({255, 255, 255});
                window.display();
                InitNetwork(server, client);
                while (client.isWorking() && !client.isConnected()) {
                    client.update();
                    server.update();
                    Time::update();
                }
                // If connect fail - return to menu
                if (!client.isWorking()) {
                    menu.setPause();
                    server.stop();
                }
                    // If connect success - init camera and start game
                else {
                    camera = client.localPlayer();
                    camera->client = &client;
                    camera->setTextures(menu.isTextures());
                    camera->setSmooth(menu.isSmooth());
                    camera->setCollision(menu.isCollision());
                }
            }
        }
        window.display();
    }
    GraphicsLoader::unloadTextures();
    return 0;
}
