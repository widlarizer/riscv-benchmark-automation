set -e

export CC="$TOOLS/bin/clang;-march=rv32gc;-mabi=ilp32d;$(echo "$CFLAGS" | tr ' ' ';');$(echo "$LDFLAGS" | tr ' ' ';')"
mkdir -p build
cmake -B build -DPORT_DIR=ports/riscv -GNinja -DCMAKE_C_COMPILER=$CC -DCMAKE_ASM_COMPILER=$CC
ninja -C build
script '-c' 'spike --isa=rv32gc build/audiomark' '-e'
