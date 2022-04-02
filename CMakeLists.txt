cmake_minimum_required(VERSION 3.0.0)
project(iz2)

if (${SYNC})
    add_subdirectory(single_sum)
elseif (NOT ${SYNC})
    add_subdirectory(multiproc_sum) 
endif()

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

target_include_directories(${PROJECT_NAME} PUBLIC ${DIAG_SUM_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DIAG_SUM_LIB_LIBRARIES})

add_subdirectory(tests)
