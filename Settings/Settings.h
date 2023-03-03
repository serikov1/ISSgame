#ifndef ISS_GAME_SETTINGS_H
#define ISS_GAME_SETTINGS_H

// window
#define SCALE 40// how many pixels in unit of distance
#define SCALE_WINDOW 500
#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280

// texture
#define WALL_TEXTURE "../Textures/Walls/wall.jpg"
#define SKY_TEXTURE "../Textures/Sky/sky.jpg"
#define FLOOR_TEXTURE "../Textures/Floor/floor.jpg"
#define COLUMN_TEXTURE "../Textures/Column/column.jpg"

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
//#define RADIUS_CAMERA SCALE/10
#define CONVEX_NUMBER 100
#define FILED_OF_VEW_COLOR {255, 67, 67}
#define SKIN "../Textures/Player/skin.jpg"

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

#define TEXTURING_UNSELECT "../Texture/buttons/texturingUnselected.jpg"
#define SMOOTHING_UNSELECT "../Texture/Buttons/smoothingUnselected.jpg"
#define COLLISION_UNSELECT "../Texture/Buttons/collisionUnselected.jpg"

#define ABOUT_INFO "../Texture/Buttons/aboutInfo.jpg"


#endif //ISS_GAME_SETTINGS_H
