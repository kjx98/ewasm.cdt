cmake_minimum_required(VERSION 3.5)

#set( CMAKE_C_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang )
#set( CMAKE_CXX_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang++ )
#set( CMAKE_AR  llvm-ar )
#set( CMAKE_RANLIB  llvm-ranlib )

enable_language(CXX)
enable_language(C)

project(libc)
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# Defines libc library
#set(C_DEFINES, "--target=wasm32")
set( CMAKE_C_FLAGS " -Wall -O3 ${CMAKE_C_FLAGS} ${C_DEFINES}" )
set( CMAKE_CXX_FLAGS " -Wall -O3 ${CMAKE_CXX_FLAGS} ${C_DEFINES}" )
set ( libc_sources
   malloc.cpp
)

add_definitions( "--target=wasm32" )

file ( GLOB libc_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h" )
list( APPEND libc_sources ${libc_headers} )

add_library ( c STATIC ${libc_sources} )

target_include_directories( c PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/../include" )
install(TARGETS c
	ARCHIVE DESTINATION ewasm/lib)

#add_custom_command( TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib )
