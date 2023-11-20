# RISC-V benchmark automation

Collection of benchmarks to run for the RISC-V architecture, with a TUI (terminal UI) made with curses.

## Prereqs

+ `lief` and `gitpython` Python packages
+ your clang toolchain path in `./env`
+ spike (riscv-isa-sim)
+ CMake
+ GNU Make
+ bash

## Usage

### Run

Run `python3 tui.py`. Press `r` to run all benchmarks. The contents of `./env` are reloaded every time before all benchmarks are run. This means that you can change the compiler path or optimization level in CFLAGS as you need.
You can run multiple runs, creating multiple columns. If you run out of terminal columns, the program dumps the CSV and terminates.

### Analyze

Press `m` to cycle between visualization modes.

### Dump CSV

Press `q` to close the program. This creates a file named `output.csv`.
