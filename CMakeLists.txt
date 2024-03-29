cmake_minimum_required(VERSION 3.0)
project(LearnOpenGL)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if (NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(includes/
                    libs/glad/include/
                    libs/glfw/include/
)

set(EXERCISES
    learn_opengl
    two_shaders
    two_triangles
)

function(create_project_from_exercise exercise)
    file(GLOB PROJECT_SOURCES 
        "src/${exercise}/*.c"
        "src/${exercise}/*.frag"
        "src/${exercise}/*.vert"
    )
    file(GLOB PROJECT_HEADERS "src/${exercises}/*.h")
    file (GLOB GLAD_SOURCE "libs/glad/src/gl.c")

    add_executable(${exercise} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${GLAD_SOURCE})
    target_link_libraries(${exercise} glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

    set_target_properties(${exercise} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${exercise}
    )
    # copy shader files to build directory
    file(GLOB SHADERS
        "src/${exercise}/*.frag"
        "src/${exercise}/*.vert"
    )
    foreach(SHADER ${SHADERS})
        if(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_BINARY_DIR}/bin/${exercise})
        endif(UNIX AND NOT APPLE)
    endforeach(SHADER ${SHADERS})

    
endfunction()

foreach(EXERCISE ${EXERCISES})
    create_project_from_exercise(${EXERCISE})
endforeach(EXERCISE ${EXERCISES})

# file (GLOB GLAD_SOURCE libs/glad/src/gl.c)
# file (GLOB PROJECT_HEADERS includes/*.h)
# file (GLOB PROJECT_SOURCES src/*.c)


# add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${GLAD_SOURCE})
# target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

# set_target_properties(${PROJECT_NAME} PROPERTIES 
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

