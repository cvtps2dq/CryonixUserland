cmake_minimum_required(VERSION 3.29)
project(CryonixUserland)

set(CMAKE_CXX_STANDARD 23)

add_executable(CryonixUserland
        land-utils/dd/dd.cpp
        land-utils/fox/fox.cpp
        land-utils/say/say.cpp
		land-utils/modech/modech.cpp
	)
