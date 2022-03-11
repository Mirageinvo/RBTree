add_executable(main sources/main.cpp)
target_include_directories(main PUBLIC includes/)

add_custom_target(
    run_main
    COMMENT "Running main for RBTree"
    COMMAND ./main
)
add_dependencies(run_main main)

if (ADD_SANITIZERS)
    target_compile_options(main PUBLIC -fsanitize=address -g)
    set_target_properties(main PROPERTIES LINK_FLAGS "-fsanitize=address")
endif()