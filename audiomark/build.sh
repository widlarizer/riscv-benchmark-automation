set -e

rm -f *.log
mkdir -p build
C_ASM_FLAGS="-march=rv32imafdc -mabi=ilp32d -ffunction-sections -fdata-sections $CFLAGS"
cmake -B build -DPORT_DIR=ports/riscv -GNinja -DCMAKE_C_COMPILER="$CC" -DCMAKE_ASM_COMPILER="$CC" -DCMAKE_C_FLAGS="$C_ASM_FLAGS" -DCMAKE_ASM_FLAGS="$C_ASM_FLAGS" -DCMAKE_EXE_LINKER_FLAGS="-march=rv32imafdc -mabi=ilp32d -Wl,--gc-sections $LDFLAGS"
ninja -vC build
script '-c' 'spike --isa=rv32gc build/audiomark' '-e' > run.log
