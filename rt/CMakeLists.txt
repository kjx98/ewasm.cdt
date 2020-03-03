cmake_minimum_required(VERSION 3.5)

enable_language(CXX)
enable_language(C)

set( CMAKE_AR  llvm-ar$ENV{SUF} )
set( CMAKE_RANLIB  llvm-ranlib$ENV{SUF} )
message(STATUS "rt use ${CMAKE_AR} for ar")
message(STATUS "rt use ${CMAKE_RANLIB} for ranlib")

project(librt)
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# Defines builtins library
#set(C_DEFINES, "--target=wasm32 -Wall -Wpedantic")
set( CMAKE_C_FLAGS "-std=gnu11 -O3 ${CMAKE_C_FLAGS} ${C_DEFINES}" )
#set( CMAKE_CXX_FLAGS "-std=c++17 -O3 ${CMAKE_CXX_FLAGS} ${C_DEFINES}" )
set( CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS} ${C_DEFINES}" )
#   compiler_builtins.cpp
set ( builtins_sources
	ashti3.cpp
	divti3.cpp
	lshti3.cpp
	modti3.cpp
	multi3.cpp
	memops.c
	memcmp.c
#	memrcpy.c
	strlen.c
	ewasm_main.c
	abi.cpp
	abi_ret.cpp
	abi.h
	memory.c
)

file ( GLOB builtins_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h" )
list( APPEND builtins_sources ${builtins_headers} )

add_library ( rt STATIC ${builtins_sources} )

target_include_directories( rt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/../include" )
install(TARGETS rt
	ARCHIVE DESTINATION ewasm/lib)

#add_custom_command( TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib )
