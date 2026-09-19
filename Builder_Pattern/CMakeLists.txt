cmake_minimum_required(VERSION 4.2)
project(Builder_Pattern)

set(CMAKE_CXX_STANDARD 17)

add_executable(Builder_Pattern
        main.cpp
        House.cpp
        WoodenHouseBuilder.cpp
        GlassHouseBuilder.cpp
        Engineer.cpp
        ClassicBuilderDemo.cpp
        Pizza.cpp
        FluentBuilderDemo.cpp
)
