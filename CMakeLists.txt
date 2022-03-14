cmake_minimum_required(VERSION 3.16.3)
project(RBTree VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDART 20)

file(GLOB_RECURSE RBT_FILES *.cpp *.hpp)

add_compile_options (
    -Wall -Werror -Wextra -Wpedantic -Wcast-qual -Wcast-align -Wconversion
    -Wsign-promo -Wfloat-equal -Wenum-compare -Wold-style-cast -Wredundant-decls
    -Wnon-virtual-dtor -Woverloaded-virtual 
)

add_custom_target(
    clang_format
    COMMAND clang-format -i ${RBT_FILES}
)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    --format-style=file;
    --header-filter=*.hpp;
)

add_subdirectory("RBTree")
add_subdirectory("tests")