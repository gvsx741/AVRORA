cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(PROJECT_NAME AVRORA)
project(AVRORA)

add_executable(AVRORA
	src/main.cpp
	src/Renderer/Shaders.h
	src/Renderer/Shaders.cpp
	src/Renderer/ResourceManager.h
	src/Renderer/ResourceManager.cpp
	src/Renderer/Camera.cpp
	src/Renderer/Camera.h
	src/Renderer/Mesh.h 
	src/Renderer/Draw.h
	src/Renderer/Mesh.cpp
	src/Resources/stb_image.h 
	src/Resources/ModelLoader.h
	src/Resources/ModelLoader.cpp
	)
	
target_compile_features(AVRORA PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(AVRORA glfw)

add_subdirectory(external/glad)
target_link_libraries(AVRORA glad)

add_subdirectory(external/glm)
target_link_libraries(AVRORA glm)

add_subdirectory(external/assimp)
target_link_libraries(AVRORA assimp)


set_target_properties(AVRORA PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)