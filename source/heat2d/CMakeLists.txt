cmake_minimum_required(VERSION 3.14)
project(heat2d CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Build options -------------------------------------------------
option(HEAT2D_ENABLE_ASAN  "Enable AddressSanitizer"  OFF)
option(HEAT2D_ENABLE_TSAN  "Enable ThreadSanitizer"   OFF)

# ---- Compiler flags ------------------------------------------------
add_compile_options(-O2 -Wall -Wextra)

if(HEAT2D_ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

if(HEAT2D_ENABLE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

# ---- Threads -------------------------------------------------------
find_package(Threads REQUIRED)

# ---- Source library ------------------------------------------------
set(HEAT2D_SOURCES
    src/grid_utils.cpp
    src/heat_serial.cpp
    src/heat_parallel.cpp
    src/runner.cpp
)

add_library(heat2d_lib STATIC ${HEAT2D_SOURCES})
target_include_directories(heat2d_lib PUBLIC include)
target_link_libraries(heat2d_lib PUBLIC Threads::Threads)

# ---- Main runner ---------------------------------------------------
add_executable(heat2d_runner main.cpp)
target_link_libraries(heat2d_runner PRIVATE heat2d_lib)

# ---- Testbench -----------------------------------------------------
add_executable(heat2d_tb testbench.cpp)
target_link_libraries(heat2d_tb PRIVATE heat2d_lib)

# ---- CTest integration ---------------------------------------------
enable_testing()
add_test(NAME testbench COMMAND heat2d_tb)

# ---- Install -------------------------------------------------------
install(TARGETS heat2d_runner heat2d_tb DESTINATION bin)
