#!/bin/bash

set -e

source ./env

python3 run_all.py

# SIZE="${SIZE:-$TOOLS/bin/llvm-size}"

# pushd audiomark > /dev/null
#     rm -rf build
#     ./build.sh | tee run.log | grep "AudioMarks"
#     $SIZE build/audiomark
# popd > /dev/null

# pushd Coremark > /dev/null
#     ./coremark-run.sh | tee run.log | grep "CoreMark 1.0"
#     $SIZE coremark.riscv
# popd > /dev/null

# pushd embench > /dev/null
#     python3 run_all.py --hightec > run.log
#     echo EmBench IoT: $(grep "Geometric mean" results/htc-clang-speed.json)
#     echo EmBench IoT size: $(grep "Geometric mean" results/htc-clang-size.json)
# popd > /dev/null
