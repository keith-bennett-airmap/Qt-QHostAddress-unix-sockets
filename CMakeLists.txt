CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(qt-qhostaddress-unix-sockets CXX)

IF (NOT CMAKE_BUILD_TYPE)
	SET (CMAKE_BUILD_TYPE "RelWithDebInfo")
ENDIF()
MESSAGE (STATUS "Current build type is ${CMAKE_BUILD_TYPE}")

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# Must find threads. We don't specifically use threading in our model, but
# do use some supporting features of it
FIND_PACKAGE(Threads REQUIRED)

FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Widgets Network)

# TODO: try_compile each option
ADD_COMPILE_OPTIONS(
	-Wall -Wextra -pedantic
	-Werror=return-type -Werror=shift-count-overflow
	-Werror=shift-negative-value -Werror=shift-overflow -Wno-long-long
	-Wno-unused-function -Wno-vla
	-march=native -mtune=native
)

# Enable cmake test features
enable_testing()

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.71.0 REQUIRED COMPONENTS program_options system filesystem iostreams)
INCLUDE_DIRECTORIES(BEFORE SYSTEM ${Boost_INCLUDE_DIRS})

OPTION(ASAN_BUILD "When on, we will build for and link against Address Sanitizer" off)
IF(ASAN_BUILD)
	MESSAGE(STATUS "Address Sanitizer libraries selected")
	ADD_COMPILE_OPTIONS(
		-fsanitize=address -fno-omit-frame-pointer
	)
	ADD_LINK_OPTIONS(
		-fsanitize=address
	)
	# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	# SET(LINK_FLAGS "${LINK_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	# SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

# ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(src)
