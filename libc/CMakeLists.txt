cmake_minimum_required(VERSION 3.5)

enable_language(CXX)
enable_language(C)

set( CMAKE_AR  llvm-ar$ENV{SUF} )
set( CMAKE_RANLIB  llvm-ranlib$ENV{SUF} )

message(STATUS "libc use ${CMAKE_AR} for ar")
message(STATUS "libc use ${CMAKE_RANLIB} for ranlib")

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

file ( GLOB libc_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h" )
list( APPEND libc_sources ${libc_headers} )

add_library ( c STATIC ${libc_sources} )

target_include_directories( c PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/../include" )
install(TARGETS c
	ARCHIVE DESTINATION ewasm/lib)

#add_custom_command( TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib )
