cmake_minimum_required(VERSION 3.10)
project(cg_with_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake to find the Legacy stack
cmake_policy(SET CMP0072 OLD)
set(OpenGL_GL_PREFERENCE LEGACY)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)


# source files
set(SOURCES
    # src/main.cpp
    # src/assignment-2210573.cpp
    tests/camera.cpp
    tests/cone.cpp
    src/main.cpp
)

# For header files, -I
# include_directories(${PROJECT_SOURCE_DIR}/include)
set(HEADERS
)

add_executable(cg_with_opengl 
    ${SOURCES} 
    ${HEADERS}
)

target_include_directories(cg_with_opengl
    PRIVATE  
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/private
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
