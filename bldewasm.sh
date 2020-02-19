#! /bin/bash
#
# Requires llvm 8.0.0 or higher
# llvm 7 does not support import_module/import_name attributes
# llvm 8 may export __heap_base and __data_end
#

if [ -d ../include/ewasm ]; then
	INCD=../include
	LIBPATH=../build/rt
	OPTS="-O2"
	#EXPO="--export=main --export-all"
	#EXPO="--export=main --export=__heap_base"
	EXPO="--export=main"
else
	INCD=${HOME}/opt/ewasm/include
	LIBPATH=${HOME}/opt/ewasm/lib
	OPTS="-O3"
	#EXPO="--export=main --export=__heap_base"
	EXPO="--export=main"
fi

compile() {
SFIL=$1
SRC=""
if [ -f ${SFIL}.cc ]; then
	SRC=${SFIL}.cc
elif [ -f ${SFIL}.cpp ]; then
	SRC=${SFIL}.cpp
fi
if [ "${SRC}" != "" ]; then
	echo "compile c++ ${SRC}"
	clang++ -c ${OPTS} -Wall -Wpedantic -I${INCD} --target=wasm32 ${SRC}
else
	echo "compile c ${SFIL}"
	clang -c ${OPTS} -Wall -Wpedantic -I${INCD} --target=wasm32 ${SFIL}.c
fi
}

FILE=$1
if [ "$FILE" = "" ]; then
	echo "empty proj"
	exit 0
fi

echo "build ${FILE} with opt ${OPTS}"
compile $1
OBJS=${FILE}.o
if [ $# -gt 1 ]; then
	compile $2
	OBJS+=" $2.o"
fi

# moved to compile fuc
#clang -c -O3 -Wall -I${INCD}/opt/ewasm/include --target=wasm32 ${FILE}.c
#clang -c ${OPTS} -Wall -I${INCD} --target=wasm32 ${FILE}.c

#echo "ewasm lib dir is ${LIBPATH}"
echo "wasm-ld export is ${EXPO}"
#wasm-ld --no-entry --allow-undefined-file=${HOME}/opt/ewasm/ewasm.syms --export=main --strip-all ${OBJS} -L${LIBPATH} -lrt -o ${FILE}.wasm
wasm-ld --no-entry --allow-undefined-file=${HOME}/opt/ewasm/ewasm.syms ${EXPO} --strip-all ${OBJS} -L${LIBPATH} -lrt -o ${FILE}.wasm
rm -f ${OBJS}
#wasm-dis /tmp/${FILE}.wasm | sed -s 's/Main/main/' > /tmp/${FILE}.wat
#wasm-as /tmp/${FILE}.wat -o ${FILE}.wasm
