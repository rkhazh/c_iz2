project(tests_getsum)
file(GLOB SOURCES *.cpp)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${DIAG_SUM_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DIAG_SUM_LIB_LIBRARIES} GTest::GTest GTest::Main)