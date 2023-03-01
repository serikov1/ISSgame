cmake_minimum_required(VERSION 3.22)
project(ISS_game)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Camera/Camera.h Camera/Camera.cpp 2DPoint/2DPoint.cpp 2DPoint/2DPoint.h Drawable/Drawable.h 2DObject/2DObject.h 2DObject/2DObject.cpp Settings/Settings.h GraphicsLoader/GraphicsLoader.h GraphicsLoader/GraphicsLoader.cpp World/World.cpp World/World.h)

if(${APPLE})
    include_directories(/usr/local/include)
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})

else()

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
    else ()
        add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    endif ()

    set(SFML_STATIC_LIBRARIES TRUE)

endif()

set(SFML_DIR "C:/PHYSTECH/infa/code clion/lib/sfml/GCC 7.3.0 MinGW (SEH) - 64-bit/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 REQUIRED graphics network audio window system)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} sfml-audio sfml-network sfml-graphics sfml-window sfml-system)
    set(PROJECT_NAME FPSOnline)
endif()
