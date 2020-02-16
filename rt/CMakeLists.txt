cmake_minimum_required(VERSION 3.5)

#set( CMAKE_C_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang )
#set( CMAKE_CXX_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang++ )
#set( CMAKE_AR  llvm-ar )
#set( CMAKE_RANLIB  llvm-ranlib )

enable_language(CXX)
enable_language(C)

project(librt)
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# Defines builtins library
#set(C_DEFINES, "--target=wasm32")
set( CMAKE_C_FLAGS " -Wall -O3 ${CMAKE_C_FLAGS} ${C_DEFINES}" )
set( CMAKE_CXX_FLAGS " -Wall -O3 ${CMAKE_CXX_FLAGS} ${C_DEFINES}" )
set ( builtins_sources
   compiler_builtins.cpp
   memops.cpp
   ewasm_main.c
   memory.c
)

add_definitions( "--target=wasm32" )

file ( GLOB builtins_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h" )
list( APPEND builtins_sources ${builtins_headers} )

add_library ( rt STATIC ${builtins_sources} )

target_include_directories( rt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/../include" )
install(TARGETS rt
	ARCHIVE DESTINATION ewasm/lib)

#add_custom_command( TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib )
