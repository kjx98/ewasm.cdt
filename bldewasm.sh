#! /bin/bash
#
# Requires llvm 8.0.0 or higher
# llvm 7 does not support import_module/import_name attributes
# llvm 8 may export __heap_base and __data_end
#

SUF=""
which wasm-ld-9 >& /dev/null && SUF="-9"
EXPO="--export=main"
#EXPO="--export=main --export=__heap_base"
if [ -d ../include/ewasm ]; then
	INCD=../include
	LIBPATH=../build/rt
	OPTS="-O2"
else
	INCD=${HOME}/opt/ewasm/include
	LIBPATH=${HOME}/opt/ewasm/lib
	OPTS="-O3"
fi

if [ "${DEBUG}" != "" ]; then
	OPTS+=" -D_DEBUG"
	EXPO+=" --export-all"
else
	OPTS+=" -DNDEBUG"
	EXPO+=" --strip-all"
fi
if [ "${RAW}" = "" ]; then
	LIBS="-L${LIBPATH} -lrt"
else
	LIBS=
	EXPO=" --export-all"
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
	clang++${SUF} -c ${OPTS} -Wall -Wpedantic -I${INCD} --target=wasm32 ${SRC}
else
	echo "compile c ${SFIL}"
	clang${SUF} -c ${OPTS} -Wall -Wpedantic -I${INCD} --target=wasm32 ${SFIL}.c
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
wasm-ld${SUF} --no-entry --allow-undefined-file=${HOME}/opt/ewasm/ewasm.syms ${EXPO} ${OBJS} ${LIBS} -o ${FILE}.wasm
#clang${SUF} --target=wasm32 --no-standard-libraries -Wl,--no-entry -Wl,--allow-undefined-file=${HOME}/opt/ewasm/ewasm.syms -Wl,${EXPO} -Wl,--strip-all -L${LIBPATH} -lrt -o ${FILE}.wasm ${OBJS}
rm -f ${OBJS}
