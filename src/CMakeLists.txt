


#########################################################
# Compiler specific flags
#########################################################

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")
endif()

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")



#########################################################
# Define the executable
#########################################################

set(target_NAME glide)

add_executable(${target_NAME}
	Edit.cpp
	ResourceManager.cpp
	Image.cpp
	Image_readFileInfo.cpp
	Image_loadJpeg.cpp
	Camera.cpp
	Frame.cpp
	Controls.cpp
	Layout.cpp
	Main.cpp
)

target_link_libraries(${target_NAME}
	${SFML_LIBRARIES}
	${Boost_SYSTEM_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	${JPEG_LIBRARY}
	pthread
	X11
)

# Copy data assets
#add_custom_command(
#	TARGET ${target_NAME} POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#	"${PROJECT_SOURCE_DIR}/data"
#	$<TARGET_FILE_DIR:${target_NAME}>
#)



