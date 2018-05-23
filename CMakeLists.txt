cmake_minimum_required(VERSION 2.8.11)

project(remesher)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(TARGET ${CMAKE_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(QT5_LIBRARIES 
        Qt5::Widgets
        Qt5::OpenGL 
        Qt5::Core
)

set(SOURCES
        main.cpp
        Window.cpp
        Model.cpp 
        ObjHandler.cpp
        Camera.cpp
        MainWindow.cpp
)
set (CMAKE_CXX_STANDARD 17)
set(UI_SOURCES
    mainWindow.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_executable(${TARGET} ${SOURCES} ${UI_GENERATED_HEADERS})


target_link_libraries(${TARGET} ${QT5_LIBRARIES})
