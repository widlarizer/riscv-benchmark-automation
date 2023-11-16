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


def dump_size(bin, cwd):
    r(f'{SIZE} {bin} > size.log', cwd=cwd, shell=True)


def rmdir(x):
    shutil.rmtree(x, ignore_errors=True)


def extract_size_score(cwd, keyword):
    # 3 = "dec" column in size output
    return int(extract_score(cwd / 'size.log', keyword)[3])


def run_audiomark():
    cwd = pathlib.Path("audiomark")
    rmdir(cwd / 'build')
    r('./build.sh', cwd=cwd, shell=True)
    # print(f'AudioMark speed,{extract_score(cwd / "run.log", "AudioMarks")[0]}')
    dump_size('build/audiomark', cwd)
    return ('AudioMark', (f'{extract_score(cwd / "run.log", "AudioMarks")[0]}', extract_size_score(cwd, 'build/audiomark')))


def run_coremark():
    cwd = pathlib.Path("Coremark")
    r('./coremark-run.sh > run.log', cwd=cwd, shell=True)
    # print(f'CoreMark speed,{extract_score(cwd / "run.log", "CoreMark 1.0")[1]}')
    # dump_size('coremark.riscv', cwd)
    return ('CoreMark', (f'{extract_score(cwd / "run.log", "CoreMark 1.0")[1]}', extract_size_score(cwd, 'coremark.riscv')))


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

    def gather(data, name, type_):
        individual = data[f"detailed {name} results"]
        # gathered = [["Benchmark", name]]
        gathered = []
        gathered.append(("geometric mean", type_(data[f"{name} geometric mean"])))
        for benchmark, datum in individual.items():
            gathered.append([benchmark, type_(datum)])

        return gathered
        # for row in gathered: # CSV print
        #     print(','.join(map(str, row)))

    speeds = gather(speed_json.popitem()[1], "speed", float)
    sizes = gather(size_json.popitem()[1], "size", int)
    return ("EmBench", (speeds, sizes))


def get_versions():
    cc_ver = (r(f"{TOOLS}/bin/clang --version", shell=True)).splitlines()[0].split()
    cc_hash = f"{cc_ver[5][:8]}({cc_ver[3]})"
    repo = git.Repo(search_parent_directories=False)
    repo_hash = repo.head.commit.hexsha[:8]
    is_dirty = repo.is_dirty(untracked_files=False)
    with_dirty = repo_hash + ("-dirty" if is_dirty else "")
    return (with_dirty, cc_hash)


def report_versions():
    versions = get_versions()
    print(f"RISC-V benchmarks {versions[0]}, clang {versions[1]}")


def run_bench(b):
    if b == "AudioMark":
        return run_audiomark()
    elif b == "CoreMark":
        return run_coremark()
    elif b == "EmBench":
        return run_embench()
    else:
        assert False, "Unreachable?"


def main():
    print("This main function is for testing only. Please use tui.py for interactive benchmarking or CSV output.")
    parser = argparse.ArgumentParser(description='Example script with --header option')
    parser.add_argument('--header', action="store_true", help='Just print CSV row headers')
    parser.add_argument('bench', nargs='?', choices=speeds, help='Optional: select benchmark')
    args = parser.parse_args()
    report_versions()
    if args.header:
        print("TODO")
    else:
        benches = [args.bench] if args.bench else speeds
        for b in benches:
            print(run_bench(b))



if __name__ == "__main__":
    main()

