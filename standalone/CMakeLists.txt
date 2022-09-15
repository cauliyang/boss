cmake_minimum_required(VERSION 3.14...3.22)

project(BOSS_Standalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
        GITHUB_REPOSITORY ylab-hi/BINARY
        VERSION 1.1.0
        OPTIONS "BINARY_BUILD_STANDALONE NO"  "BINARY_BUILD_TESTS NO"
)


CPMAddPackage(
        GITHUB_REPOSITORY jarro2783/cxxopts
        VERSION 3.0.0
        OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)


CPMAddPackage(NAME BOSS SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)

# ---- Create standalone executable ----


function(add_standalone targetName)
    message(STATUS "Adding standalone ${targetName}")
    file(GLOB ${targetName}_source CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_LIST_DIR}/${targetName}/source/*.cpp
            )
    add_executable(${targetName} ${${targetName}_source})
    target_compile_features(${targetName} PRIVATE cxx_std_20)
    target_link_libraries(${targetName} PRIVATE boss::boss binary cxxopts stdc++fs)
    target_include_directories(
            ${targetName} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
            ${CMAKE_CURRENT_LIST_DIR}/${targetName}/include
    )
    target_compile_options(${targetName} PRIVATE -Wall -Wextra -Wnon-virtual-dtor -pedantic -Werror)
    set_target_properties(${targetName} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()

#file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_standalone(fqsp)