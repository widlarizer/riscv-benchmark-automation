#!/usr/bin/env python

import os
import subprocess
import re
import pathlib
import shutil
import argparse
import json
import git

def r(args, **kwargs):
    result = subprocess.check_output(args, **kwargs, text=True)
    return result

def extract_nums(input):
    numbers = re.findall(r'-?\d+\.\d+|\d+', input)
    return list(map(float, numbers))


def extract_score(filename, keyword):
    with open(filename, 'r') as f:
        return extract_nums([line.strip() for line in f.readlines() if keyword in line][0])

def extract_size_score(cwd, keyword):
    # 3 = "dec" column in size output
    return int(extract_score(cwd / 'size.log', keyword)[3])

def rmdir(x):
    shutil.rmtree(x, ignore_errors=True)

class Runner():
    def __init__(self):
        self.update_env()

    def update_env(self):
        bash = r("which bash", shell=True).rstrip()
        print(bash)
        command = f'env -i {bash} -c "source ./env && env"'
        print(command)
        for line in subprocess.getoutput(command).splitlines():
            print(line)
            key, value = line.split("=", maxsplit=1)
            print(key)
            print(value)
            os.environ[key] = value
        self.TOOLS = os.environ.get('TOOLS')
        self.SIZE = os.path.join(os.environ.get('SIZE', default=f'{self.TOOLS}/bin/llvm-size'))
        self.CFLAGS = os.environ.get('CFLAGS')
        self.LDFLAGS = os.environ.get('LDFLAGS')

    def dump_size(self, bin, cwd):
        r(f'{self.SIZE} {bin} > size.log', cwd=cwd, shell=True)


    def run_audiomark(self):
        cwd = pathlib.Path("audiomark")
        rmdir(cwd / 'build')
        r('./build.sh', cwd=cwd, shell=True)
        # print(f'AudioMark speed,{extract_score(cwd / "run.log", "AudioMarks")[0]}')
        self.dump_size('build/audiomark', cwd)
        return ('AudioMark', (f'{extract_score(cwd / "run.log", "AudioMarks")[0]}', extract_size_score(cwd, 'build/audiomark')))


    def run_coremark(self):
        cwd = pathlib.Path("Coremark")
        r('./coremark-run.sh > run.log', cwd=cwd, shell=True)
        # print(f'CoreMark speed,{extract_score(cwd / "run.log", "CoreMark 1.0")[1]}')
        # dump_size('coremark.riscv', cwd)
        return ('CoreMark', (f'{extract_score(cwd / "run.log", "CoreMark 1.0")[1]}', extract_size_score(cwd, 'coremark.riscv')))


    def run_embench(self):
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
            f'--cc={self.TOOLS}/bin/clang',
            f'--cflags=-O3 -g {self.CFLAGS}',
            f'--ldflags={self.LDFLAGS}',
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


    def get_versions(self):
        cc_ver = (r(f"{self.TOOLS}/bin/clang --version", shell=True)).splitlines()[0].split()
        cc_hash = f"{cc_ver[5][:8]}({cc_ver[3]})"
        repo = git.Repo(search_parent_directories=False)
        repo_hash = repo.head.commit.hexsha[:8]
        is_dirty = repo.is_dirty(untracked_files=False)
        with_dirty = repo_hash + ("-dirty" if is_dirty else "")
        return (with_dirty, cc_hash)


    def report_versions(self):
        versions = self.get_versions()
        print(f"RISC-V benchmarks {versions[0]}, clang {versions[1]}")


    def run_bench(self, b):
        # This is pretty stupid
        if b == "AudioMark":
            return self.run_audiomark()
        elif b == "CoreMark":
            return self.run_coremark()
        elif b == "EmBench":
            return self.run_embench()
        else:
            # This is super stupid
            assert False, "Unreachable?"


def main():
    print("This main function is for testing only. Please use tui.py for interactive benchmarking or CSV output.")
    runner = Runner()
    parser = argparse.ArgumentParser(description='Example script with --header option')
    parser.add_argument('--header', action="store_true", help='Just print CSV row headers')
    all_benches = ['CoreMark', 'AudioMark', 'EmBench']
    parser.add_argument('bench', nargs='?', choices=all_benches, help='Optional: select benchmark')
    args = parser.parse_args()
    runner.report_versions()
    if args.header:
        print("TODO")
    else:
        benches = [args.bench] if args.bench else all_benches
        for b in benches:
            print(runner.run_bench(b))



if __name__ == "__main__":
    main()

