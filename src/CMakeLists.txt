

# Suppress switch warnings
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(target_NAME glide)
add_executable(${target_NAME}
	Mutex.cpp
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
	${Boost_THREAD_LIBRARY}
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
