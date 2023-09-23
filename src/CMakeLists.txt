cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wdouble-promotion
    -Wformat=2
    -Wformat-security
    -Wformat-signedness
    -Wnull-dereference
    -Wtrivial-auto-var-init
    -Wunused-parameter
    -Wunused-const-variable=2
    -Wuninitialized
    -Wmaybe-uninitialized
    -Wstringop-overflow=4
    -Wconversion
    -Wfloat-conversion
    -Wsign-conversion
    -Warith-conversion
    -Wbool-compare
    -Wduplicated-branches
    -Wduplicated-cond
    -Wfloat-equal
    -Wshadow
    -Wundef
    -Wunused-macros
    -Wcast-qual
    -Wcast-align=strict
    -Wlogical-op
    -Wmissing-declarations
    -Wredundant-decls
    # -Winline
    -Wlong-long
    -Woverloaded-virtual
    -Wimplicit-fallthrough=5
    -Wmissing-include-dirs
    -Wsuggest-override
    -Wnon-virtual-dtor
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

set(
    SRC_FILES
    main.cpp
    window.cpp
    renderer.cpp
    texture.cpp
    app_data.cpp
    app.cpp
)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

add_executable(metaballs ${SRC_FILES})

if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET metaballs PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

target_link_libraries(metaballs ${SDL2_LIBRARIES})
