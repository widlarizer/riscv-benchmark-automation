# Copyright HighTec EDV-Systeme GmbH 2023
# SPDX-License-Identifier: BSD-1-Clause

import curses
import concurrent.futures
from run_all import Runner
from enum import Enum, auto
from itertools import cycle
from pathlib import Path
import csv
import logging
import sys

class Modes(Enum):
    Speed = 0
    Size = 1
    RelSpeed = 2
    RelSize = 3


class Benches(Enum):
    AudioMark = 0
    CoreMark = 1
    EmBench = 2


SUBS = {
    "EmBench": [
        "aha-mont64",
        "crc32",
        "cubic",
        "edn",
        "huffbench",
        "matmult-int",
        "md5sum",
        "minver",
        "nbody",
        "nettle-aes",
        "nettle-sha256",
        "nsichneu",
        "picojpeg",
        "primecount",
        "qrduino",
        "sglib-combined",
        "slre",
        "st",
        "statemate",
        "tarfind",
        "ud",
        "wikisort",
    ]
}

# Think of this like a constexpr even though it really isn't (Python sucks)
def iter_subs():
    for suite, benches in SUBS.items():
        for bench in benches:
            yield suite, bench


def len_subs():
    return sum(1 for _ in iter_subs())

class Tui():
    WIDTH = 12
    # Suites with multiple executables
    DETAILED = [Benches.EmBench]

    def __init__(self, stdscr):
        # Curses setup
        curses.curs_set(0)
        self.stdscr = stdscr
        self.stdscr.clear()
        # No data yet
        self.col = 0
        self.baseline_col = 0
        self.labels = self.init_labels()
        self.data = self.init_array()
        self.modes_cycle = cycle(Modes)
        self.detail_cycle = cycle(self.DETAILED)
        self.mode = next(self.modes_cycle)
        self.detail = next(self.detail_cycle)
        assert self.mode == Modes.Speed
        self.repo_version = Runner().get_versions()[0]
        self.cc_ids = []
        # Initial UI setup
        self.status = ""
        self.help = True
        self.render()
        self.stdscr.refresh()

    def cycle_mode(self):
        self.mode = next(self.modes_cycle)
    
    def cycle_detail(self):
        self.detail = next(self.detail_cycle)

    def adjust_baseline(self, incr):
        self.baseline_col = self.baseline_col + incr
        self.baseline_col = max(0, min(self.baseline_col, self.col - 1))
        self.update_relative()

    def render(self):
        l = self.WIDTH-1
        self.stdscr.clear()
        self.draw_status()
        # Print mode in the corner
        self.stdscr.addstr(0, 0, f"{self.mode.name}")
        # Print CC commit hash and options as column labels
        for j in range(self.col):
            self.stdscr.addstr(0, (j + 1) * self.WIDTH, self.cc_ids[j][0][:l]) 
            self.stdscr.addstr(1, (j + 1) * self.WIDTH, self.cc_ids[j][1][:l])

        for i, row in enumerate(self.data[self.mode.value]):
            # Print benchmark names as row labels
            self.stdscr.addstr(i + 2, 0, self.labels[i][:l])
            for j, value in enumerate(row):
                if value is not None:
                    try:
                        # Print benchmark result value
                        # f-strings really, really wouldn't work here
                        str_val = ("{:" + str(l) + ".6}").format(value) if type(value) == float else str(value).rjust(l)
                        # We have to write the full width every time to clear the old value
                        # TODO flush the screen instead?
                        logging.debug(str_val)
                        assert len(str_val) == l, "Formatting error"
                        self.stdscr.addstr(i + 2, (j + 1) * self.WIDTH, str_val)
                    except curses.error as e:
                        curses.endwin()
                        print("curses error, terminal probably too narrow")
                        filename = self.dump_csv()
                        if filename:
                            print(f"Data exported to {filename}")
                        exit(1)


    def clear_line(self, line):
        _, width = self.stdscr.getmaxyx()
        # Overwrite the line with spaces
        self.stdscr.addstr(line, 0, " " * width)

    # Change bottom help/status line
    def draw_status(self):
        height, _ = self.stdscr.getmaxyx()
        end = height - 1
        
        self.clear_line(end - 1)
        self.stdscr.addstr(end - 1, 0, self.status)
        if self.help:
            self.stdscr.addstr(end, 0, "[q]uit [r]un [m]ode [c]sv [d]elete [←|→] change baseline")
        self.stdscr.refresh()


    def init_labels(self):
        labels = []
        for b in Benches.__members__:
            labels.append(b)
        for b in iter_subs():
            labels.append(f"{b[0][:2]}_{b[1]}")
        return labels


    def init_array(self):
        return [[[] for _ in range(len(Benches) + len_subs())] for _ in Modes]


    def add_col(self):
        for mode_array in self.data:
            for i in range(len(Benches) + len_subs()):
                mode_array[i].append('...')
        self.col += 1


    def del_col(self):
        for mode_array in self.data:
            for i in range(len(Benches) + len_subs()):
                mode_array[i].pop()
        self.cc_ids.pop()
        self.col -= 1
        self.adjust_baseline(0)


    def set_done(self, num_done):
        self.status = f"Running benchmarks... {num_done}/{len(Benches)}"
        self.help = False


    def update_relative(self):
        def percent(x):
            return f"{x:.1%}"

        for c in range(self.col):
            for i in range(len(Benches) + len_subs()):
                speed_data = self.data[Modes.Speed.value][i]
                size_data = self.data[Modes.Size.value][i]
                rel_speed_data = self.data[Modes.RelSpeed.value][i]
                rel_size_data = self.data[Modes.RelSize.value][i]
                rel_speed_data[c] = percent(float(speed_data[c]) / float(speed_data[self.baseline_col]))
                rel_size_data[c] = percent(float(size_data[c]) / float(size_data[self.baseline_col]))

    def run_all(self):
        # This also reloads the environment file
        runner = Runner()
        # Include paths are boring, filter them out
        perf_opts = ' '.join(list(filter(lambda x: '-I' not in x, runner.CFLAGS.split(' '))))
        self.cc_ids.append((runner.get_versions()[1], perf_opts))
        # New column for this run
        self.add_col()
        done = 0
        # Initialize progress counter
        self.set_done(done)

        # Run the benchmark simulations in parallel
        with concurrent.futures.ThreadPoolExecutor() as executor:
            futures = [executor.submit(runner.run_bench, b) for b in Benches.__members__]
            # Iterate over results as we get them
            for future in concurrent.futures.as_completed(futures):
                res = future.result()
                if res:
                    b, (speeds, sizes) = future.result()
                    # Get a "mutable reference" to a row (Python sucks)
                    speed_data = self.data[Modes.Speed.value][Benches[b].value]
                    size_data = self.data[Modes.Size.value][Benches[b].value]
                    # Fill in the corresponding fields in the new column
                    if Benches[b] in self.DETAILED:
                        # TODO use self.detail cycle
                        speed_data[self.col - 1] = speeds[0][1]
                        size_data[self.col - 1] = sizes[0][1]
                        for ((sub_name, speed), (_, size)) in zip(speeds, sizes):
                            if sub_name == 'geometric mean':
                                continue
                            size_data[self.col - 1] = sizes[0][1]
                            name = f"{Benches[b]}_{sub_name}"
                            # logging.debug(f"{name}:{speed}:{size}")
                            idx = len(Benches) + list(iter_subs()).index((Benches[b].name, sub_name))
                            self.data[Modes.Speed.value][idx][self.col - 1] = speed
                            self.data[Modes.Size.value][idx][self.col - 1] = size
                    else:
                        speed_data[self.col - 1] = speeds
                        size_data[self.col - 1] = sizes
                    done += 1
                    self.set_done(done)
                    self.render()
                    self.stdscr.refresh()

        self.update_relative()
        self.status = "Done!"
        self.help = True
        self.render()


    def dump_csv(self):
        # No data, no dump
        if self.col == 0:
            return
        # First commit hash
        filename = self.cc_ids[0][0].split('(')[0]
        # logging.debug(filename)
        existing = sorted(list(Path('.').glob(f"{filename}*.csv")))
        if existing:
            logging.debug(existing[-1].name)
            parts = existing[-1].name.split('.')[0].split('_')
            idx = 0
            assert(len(parts) <= 2)
            if len(parts) == 2:
                idx = int(parts[-1])
            filename += f"_{str(idx + 1)}"
        filename += ".csv"
        logging.debug(f"Writing {filename}")

        with open(filename, "w", newline="") as csvfile:
            csv_writer = csv.writer(csvfile)
            csv_writer.writerow([f"RISC-V benchmark suite {self.repo_version}"] + self.cc_ids)
            for mode in [Modes.Speed, Modes.Size]:
                csv_writer.writerow([mode])
                mode_array = self.data[mode.value]
                for row in mode_array:
                    csv_writer.writerow(row)
        return filename


def main(stdscr):
    tui = Tui(stdscr)

    while True:
        stdscr.refresh()
        key = stdscr.getch()
        if key == ord('q') or key == ord('Q'):
            filename = tui.dump_csv()
            curses.endwin()
            sys.stdout.flush()
            if filename:
                print(f"Data exported to {filename}")
            break

        if key == ord('r') or key == ord('R'):
            tui.run_all()

        if key == ord('m') or key == ord('M'):
            tui.cycle_mode()
            tui.render()
        
        # if key == ord('d') or key == ord('D'):
        #     # TODO
        #     tui.cycle_detail()
        #     tui.render()

        if key == ord('d') or key == ord('D'):
            tui.del_col()
            tui.render()

        if key == ord('c') or key == ord('C'):
            filename = tui.dump_csv()
            if filename:
                tui.status = f"Data exported to {filename}"
            tui.render()

        if key == curses.KEY_RIGHT:
            tui.adjust_baseline(+1)
            tui.render()

        if key == curses.KEY_LEFT:
            tui.adjust_baseline(-1)
            tui.render()

if __name__ == "__main__":
    logging.basicConfig(filename='debug.log', level=logging.WARNING)
    curses.wrapper(main)
