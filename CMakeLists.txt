cmake_minimum_required(VERSION 3.5)

set( CMAKE_C_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang$ENV{SUF} )
set( CMAKE_CXX_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang++$ENV{SUF} )
set( CMAKE_AR  llvm-ar$ENV{SUF} )
set( CMAKE_RANLIB  llvm-ranlib$ENV{SUF} )

#message(STATUS "use ${CMAKE_AR} for ar")
#message(STATUS "use ${CMAKE_RANLIB} for ranlib")

# CMAKE_INSTALL_PREFIX set install prefix

#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

project(cdt)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
		add_definitions( "-D_DEBUG")
else()
		add_definitions( "-DNDEBUG")
endif()

add_definitions( "--no-standard-libraries -fno-exceptions -Wpedantic")
add_definitions( "--target=wasm32" )
#add_definitions( "-DUSE_MALLOC_ABI")

install(DIRECTORY include 
		DESTINATION ewasm)
install(FILES ewasm.syms
		DESTINATION ewasm)
install(PROGRAMS bldewasm.sh
		DESTINATION $ENV{HOME}/bin)

add_subdirectory( rt )
add_subdirectory( libc )
