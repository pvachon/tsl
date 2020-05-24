add_executable(test_app test_app.c)

target_link_libraries(test_app PRIVATE
    tsltestframework
    tslapp
    tslconfig
    tsl
    jansson
    ck)

target_include_directories(test_app PRIVATE ${PROJECT_SOURCE_DIR})
