#!/bin/bash

set -e

BASEDIR=$PWD

TC_FOLDER=$1
FLAGS="-O3 -march=rv32imafdc -mabi=ilp32d -I$TC_FOLDER/riscv32/include"
LFLAGS_EXTRA="-march=rv32imafdc -mabi=ilp32d -L$TC_FOLDER/riscv32/lib/rv32imafdc/ilp32d/except -L$TC_FOLDER/riscv32/lib/rv32imafdc"

CM_FOLDER=coremark
PORT_FOLDER=riscv32-spike

rm -f $BASEDIR/$CM_FOLDER/*.o
rm -f $BASEDIR/$PORT_FOLDER/*.o

cd $BASEDIR/$CM_FOLDER

make clean

# run the compile
echo "Start compilation"
make PORT_DIR="../"$PORT_FOLDER ITERATIONS=5000 LFLAGS_EXTRA="$LFLAGS_EXTRA" USER_FLAGS="$FLAGS" TC=$TC_FOLDER link

#make PORT_DIR=../riscv64-baremetal compile
mv coremark.riscv ../
cd ..
echo "Start simulation (takes time)"
spike --isa=RV32IMAFDC_Zicsr coremark.riscv
