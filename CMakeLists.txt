cmake_minimum_required(VERSION 3.0.0)
project(iz2)

#add_subdirectory(roads)

#message("ROADS_LIB_INCLUDE_DIRS = ${ROADS_LIB_INCLUDE_DIRS}")
#message("ROADS_LIB_LIBRARIES = ${ROADS_LIB_LIBRARIES}")

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

#target_include_directories(${PROJECT_NAME} PUBLIC ${ROADS_LIB_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${ROADS_LIB_LIBRARIES})

#add_subdirectory(tests)
