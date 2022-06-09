cmake_minimum_required(VERSION 3.0.0)

project(SDL2CV VERSION 0.1.0)

add_executable(SDL2CV src/main.cpp)

target_link_libraries(SDL2CV SDL2main SDL2 GL)
