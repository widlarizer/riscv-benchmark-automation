#!/bin/bash

set -e

BASEDIR=$PWD

TC_FOLDER=/home/acarminati/OtherVersions/riscv
FLAGS="-O2 -save-temps=obj"

CM_FOLDER=coremark
PORT_FOLDER=riscv32-spike

rm -f $BASEDIR/$CM_FOLDER/*.o
rm -f $BASEDIR/$PORT_FOLDER/*.o

cd $BASEDIR/$CM_FOLDER

make clean

# run the compile
echo "Start compilation"
make PORT_DIR="../"$PORT_FOLDER ITERATIONS=5000 USER_FLAGS="$FLAGS" TC=$TC_FOLDER link

#make PORT_DIR=../riscv64-baremetal compile
mv coremark.riscv ../
cd ..
echo "Start simulation (takes time)"
$TC_FOLDER/sim/spike --isa=RV32IMAFDC_Zicsr coremark.riscv
