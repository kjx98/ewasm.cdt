cmake_minimum_required(VERSION 3.5)

set( CMAKE_C_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang )
set( CMAKE_CXX_COMPILER  ${COMPILER_PATH}${CROSS_COMPILE}clang++ )
set( CMAKE_AR  llvm-ar )
set( CMAKE_RANLIB  llvm-ranlib )

# CMAKE_INSTALL_PREFIX set install prefix

project(cdt)

#add_definitions( "-DUSE_MALLOC_ABI")

install(DIRECTORY include 
		DESTINATION ewasm)
install(FILES ewasm.syms
		DESTINATION ewasm)
install(PROGRAMS bldewasm.sh
		DESTINATION $ENV{HOME}/bin)

add_subdirectory( rt )
add_subdirectory( libc )
