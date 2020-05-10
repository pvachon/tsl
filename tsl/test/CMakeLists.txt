add_executable(test_tsl
    test_alloc.c
    test_cpumask.c
    test_main.c
    test_printf_helpers.c
    test_rbtree.c
    test_refcnt.c)

target_link_libraries(test_tsl tsl)
target_include_directories(test_tsl PRIVATE "${TSL_BASE_DIR}")
