#!/usr/bin/env python

import os
import subprocess
import re
import pathlib
import shutil
import argparse
import json
import git

TOOLS = os.environ.get('TOOLS')
SIZE = os.path.join(os.environ.get('SIZE', default=f'{TOOLS}/bin/llvm-size'))
CFLAGS = os.environ.get('CFLAGS')
LDFLAGS = os.environ.get('LDFLAGS')

speeds = ['CoreMark', 'AudioMark', 'EmBench']

def extract_nums(input):
    numbers = re.findall(r'-?\d+\.\d+|\d+', input)
    return list(map(float, numbers))


def extract_score(filename, keyword):
    with open(filename, 'r') as f:
        return extract_nums([line.strip() for line in f.readlines() if keyword in line][0])


def r(args, **kwargs):
    result = subprocess.check_output(args, **kwargs, text=True)
    return result


def log_size(bin, cwd):
    r(f'{SIZE} {bin} > size.log', cwd=cwd, shell=True)


def rmdir(x):
    shutil.rmtree(x, ignore_errors=True)


def run_audiomark():
    cwd = pathlib.Path("audiomark")
    rmdir(cwd / 'build')
    r('./build.sh', cwd=cwd, shell=True)
    print(f'AudioMark speed,{extract_score(cwd / "run.log", "AudioMarks")[0]}')
    log_size('build/audiomark', cwd)


def run_coremark():
    cwd = pathlib.Path("Coremark")
    r('./coremark-run.sh > run.log', cwd=cwd, shell=True)
    print(f'CoreMark speed,{extract_score(cwd / "run.log", "CoreMark 1.0")[1]}')
    log_size('coremark.riscv', cwd)


def run_embench():
    cwd = pathlib.Path("embench")
    for dir in 'bd results logs'.split():
        rmdir(cwd / dir)

    build_args = [
        './build_all.py',
        '--clean',
        '--verbose',
        '--arch', 'riscv32',
        '--chip=generic',
        '--board=spike',
        f'--cc={TOOLS}/bin/clang',
        f'--cflags=-O3 -g {CFLAGS}',
        f'--ldflags={LDFLAGS}',
    ]
    speed_args = [
        'python3',
        './benchmark_speed.py',
        '--target-module=run_spike',
        '--json-output',
    ]
    size_args = [
        'python3',
        './benchmark_size.py',
        '--json-output',
    ]
    
    _ = r(build_args, cwd=cwd)
    speed_json = json.loads(r(speed_args, cwd=cwd))
    size_json = json.loads(r(size_args, cwd=cwd))

    def csvify(data, name):
        individual = data[f"detailed {name} results"]
        csv_rows = [["Benchmark", name]]
        csv_rows.append(["geometric mean", data[f"{name} geometric mean"]])
        for benchmark, datum in individual.items():
            csv_rows.append([benchmark, datum])

        for row in csv_rows:
            print(','.join(map(str, row)))

    csvify(speed_json.popitem()[1], "speed")
    csvify(size_json.popitem()[1], "size")

def report_versions():
    print(r(f"{TOOLS}/bin/clang --version", shell=True))
    try:
        repo = git.Repo(search_parent_directories=False)
        commit_hash = repo.head.commit.hexsha
        is_dirty = repo.is_dirty(untracked_files=False)
        print(f"RISC-V benchmarks {commit_hash}" + ("-dirty" if is_dirty else ""))
    except git.InvalidGitRepositoryError:
        print("Is this a stupid copy? Please use git repos.")


def main():
    parser = argparse.ArgumentParser(description='Example script with --header option')
    parser.add_argument('--header', action="store_true", help='Just print CSV row headers')
    args = parser.parse_args()
    report_versions()
    if args.header:
        print("TODO")
    else:
        run_audiomark()
        run_coremark()
        run_embench()


if __name__ == "__main__":
    main()

