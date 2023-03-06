#ifndef ISS_GAME_SETTINGS_H
#define ISS_GAME_SETTINGS_H

// window
#define SCALE 40// how many pixels in unit of distance
#define SCALE_WINDOW 500
#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280

// texture
#define WALL_TEXTURE "../Texture/Walls/wall.jpg"
#define SKY_TEXTURE "../Texture/Sky/sky.jpg"
#define FLOOR_TEXTURE "../Texture/Floor/floor.jpg"
#define COLUMN_TEXTURE "../Texture/Walls/wall1.jpg"

// object
#define OUTLINE_COLOR {0, 0, 0}
#define OUTLINE_THICKNESS 3

// background
#define OUTLINE_BACK_COLOR {255, 255, 255}
#define FILL_BACK_COLOR {255, 255, 255}
#define OUTLINE_BACK_THICKNESS 5

// circle
#define CIRCLE_CONVEX_NUMBER 10
#define PI 3.141529

// player
#define COLLISION_DISTANCE 0.1
#define PLAYER_CONVEX_NUMBER 4

// camera
#define DISTANCES_SEGMENTS 1280 // how many vertical sectors in ray cast
#define FLOOR_SEGMENT_SIZE 8
#define COLLISION_AREA 1
#define OUTLINE_CAMERA_THICKNESS 1
#define OUTLINE_CAMERA_COLOR {0, 0, 0}
#define FILL_CAMERA_COLOR {255, 67, 67}
#define CONVEX_NUMBER 100
#define FILED_OF_VEW_COLOR {255, 67, 67}
#define SKIN "../Texture/Player/skin.png"
#define AIM_TEXTURE "../Texture/Weapon/aim.png"
#define FIRE_TEXTURE "../Texture/Weapon/fire.png"

// menu
#define PLAYGAME_PRESSED_TEXTURE "../Texture/Buttons/playGameSelected.jpg"
#define SETTINGS_PRESSED_TEXTURE "../Texture/Buttons/settingsSelected.jpg"
#define ABOUT_PRESSED_TEXTURE "../Texture/Buttons/aboutSelected.jpg"
#define QUIT_PRESSED_TEXTURE "../Texture/Buttons/quitSelected.jpg"

#define PLAYGAME_TEXTURE "../Texture/Buttons/playGame.jpg"
#define SETTINGS_TEXTURE "../Texture/Buttons/settings.jpg"
#define ABOUT_TEXTURE "../Texture/Buttons/about.jpg"
#define QUIT_TEXTURE "../Texture/Buttons/quit.jpg"

#define TEXTURING_SELECT "../Texture/Buttons/texturing.jpg"
#define SMOOTHING_SELECT "../Texture/Buttons/smoothing.jpg"
#define COLLISION_SELECT "../Texture/Buttons/collisionSelected.jpg"

#define TEXTURING_UNSELECT "../Texture/buttons/texturing.jpg"
#define SMOOTHING_UNSELECT "../Texture/Buttons/smoothing.jpg"
#define COLLISION_UNSELECT "../Texture/Buttons/collision.jpg"

#define ABOUT_INFO "../Texture/Buttons/aboutInfo.jpg"

// weapon
#define GUN_ARM_TEXTURE "../Texture/Weapon/Gun/arm.png"
#define GUN_HANDLE_TEXTURE "../Texture/Weapon/Gun/handle.png"
#define GUN_TRUNK_TEXTURE "../Texture/Weapon/Gun/trunk.png"

// network
#define TIMEOUT_SECONDS 5
#define RELIABLE_RETRY_TIME (1.0/20)
#define NETWORK_VERSION 2U
#define WORLD_UPDATE_RATE 30


#endif //ISS_GAME_SETTINGS_H
