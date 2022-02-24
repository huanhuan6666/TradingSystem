cmake_minimum_required(VERSION 3.21)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project1 main.cpp Users.cpp Users.h Administrator.cpp Administrator.h MainWindow.cpp MainWindow.h
        Buyers.cpp Buyers.h Sellers.cpp Sellers.h InfoManager.cpp InfoManager.h common.h SqlHelper.cpp SqlHelper.h Calculator.cpp Calculator.h)
