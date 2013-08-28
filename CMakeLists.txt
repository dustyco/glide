


#########################################################
# This is a small C++11 test project
#########################################################

cmake_minimum_required(VERSION 2.8)
project(mingwtest CXX C)



#########################################################
# GNU profiling
#########################################################

# Other compiler id's: Clang, GNU, Intel, MSVC
if(PROFILE)
#	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
#		message("Building in C GNU profiling")
#		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#	else()
#		message("Requested C profiling but we don't have it for this compiler: " ${CMAKE_C_COMPILER_ID})
#	endif()
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message("Building in CXX GNU profiling")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
	else()
		message("Requested CXX profiling but we don't have it for this compiler: " ${CMAKE_CXX_COMPILER_ID})
	endif()
endif()



#########################################################
# Find OpenGL
#########################################################

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
message(" OPENGL_INCLUDE_DIR: ${OPENGL_INCLUDE_DIR}")
message(" OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message("OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY}")



#########################################################
# Find SFML
#########################################################

if (NOT "$ENV{SFML_ROOT}" STREQUAL "")
	set(CMAKE_MODULE_PATH $ENV{SFML_ROOT}/cmake/Modules ${CMAKE_MODULE_PATH})
endif ()
if (MSVC)
	set(SFML_STATIC_LIBRARIES TRUE)
endif ()
find_package(SFML REQUIRED COMPONENTS system window graphics)
include_directories(${SFML_INCLUDE_DIR})
message("   SFML_LIBRARIES: ${SFML_LIBRARIES}")
set(SFML_LIBRARIES ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${SFML_LIBRARIES})



#########################################################
# Find Boost
#########################################################

if(MSVC)
	set(Boost_USE_STATIC_LIBS TRUE)
endif()
message("Boost_USE_STATIC_LIBS: ${Boost_USE_STATIC_LIBS}")
set(BOOST_COMPONENTS
	system
	filesystem
)
find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL_NO_LIB)



#########################################################
# Find JPEG
#########################################################

if (NOT "$ENV{JPEG_ROOT}" STREQUAL "")
	set (CMAKE_INCLUDE_PATH "$ENV{JPEG_ROOT}/include;${CMAKE_INCLUDE_PATH}")
	set (CMAKE_LIBRARY_PATH "$ENV{JPEG_ROOT}/lib;${CMAKE_LIBRARY_PATH}")
endif ()
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
message("JPEG_LIBRARY: ${JPEG_LIBRARY}")



#########################################################
# Subfolders
#########################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(src)



