set -e

export X=~/work/llvm-project/riscv32-l64-install/
export CC="$X/bin/clang;-march=rv32gc;-mabi=ilp32d;-I$X/riscv32/include;-L$X/riscv32/lib/rv32imafdc/ilp32d/except;-L$X/riscv32/lib/rv32imafdc;-I$X/riscv32/include;-L$X/riscv32/lib/rv32imafdc/ilp32d/except"
mkdir -p build
cmake -B build -DPORT_DIR=ports/riscv -GNinja -DCMAKE_C_COMPILER=$CC -DCMAKE_ASM_COMPILER=$CC
ninja -C build
spike --isa=rv32gc build/audiomark
