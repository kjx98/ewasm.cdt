cmake_minimum_required(VERSION 3.5)

set( CMAKE_C_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang )
set( CMAKE_CXX_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang++ )
set( CMAKE_AR  llvm-ar )
set( CMAKE_RANLIB  llvm-ranlib )

project(cdt)

install(DIRECTORY include 
		DESTINATION ewasm)
install(FILES ewasm.syms
		DESTINATION ewasm)
install(PROGRAMS bldewasm.sh
		DESTINATION $ENV{HOME}/bin)

add_subdirectory( src )
