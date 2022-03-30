set(INCL_PATH "../RBTree/includes/")

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests.cpp)
target_link_libraries(tests gtest_main)
target_include_directories(tests PUBLIC ${INCL_PATH})

include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(
    run_tests
    COMMENT "Running tests for RBTree"
    COMMAND ./tests
)
add_dependencies(run_tests tests)

if (ADD_SANITIZERS)
    target_compile_options(tests PUBLIC -fsanitize=address -g)
    set_target_properties(tests PROPERTIES LINK_FLAGS "-fsanitize=address")
endif()

add_custom_target(
    run_all
    COMMENT "Running tests and main for RBTree"
)
add_dependencies(run_all run_tests run_main)