set(TEST_SRC
    "src/t_main.cpp"
    "src/t_attack_table.cpp"
)

add_executable(engine_test
    ${TEST_SRC}
)

target_compile_features(engine_test PRIVATE cxx_std_20)
set_target_properties(
    engine_test PROPERTIES
    OUTPUT_NAME engine_test
    EXPORT_NAME exe
    CXX_EXTENSIONS OFF
)

target_include_directories(engine_test PRIVATE include/doctest)
target_link_libraries(engine_test PRIVATE chess_engine)

doctest_discover_tests(engine_test)
