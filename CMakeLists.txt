
# Название проекта, минимально требуемая версия CMake, стандарт C++ (включить если понадобится)
cmake_minimum_required(VERSION 3.1)
project(libmaycloud-example-webserver)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(LibMayCloud REQUIRED)
add_executable(testweb src/main.cpp)
target_link_libraries(testweb ${LibMayCloud_LIBRARIES})

# Реализация кастомной цели distclean
ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
set(DISTCLEANED CMakeCache.txt CMakeFiles cmake_install.cmake Makefile testweb)
ADD_CUSTOM_COMMAND (
	DEPENDS clean
	COMMENT "distribution clean"
	COMMAND rm
	ARGS -Rf ${DISTCLEANED}
	TARGET  distclean
)
