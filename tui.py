import curses
import concurrent.futures
from run_all import Runner
from enum import Enum, auto
from itertools import cycle
import csv
import logging

class Modes(Enum):
    Speed = auto(0)
    Size = auto()
    RelSpeed = auto()
    RelSize = auto()


class Benches(Enum):
    AudioMark = auto(0)
    CoreMark = auto()
    EmBench = auto()


class Tui():
    WIDTH = 12
    # Suites with multiple executables
    DETAILED = [Benches.EmBench]

    def __init__(self, stdscr):
        curses.curs_set(0)
        self.stdscr = stdscr
        self.stdscr.clear()
        self.col = 0
        self.baseline_col = 0
        self.labels = self.init_labels()
        self.data = self.init_array(0)
        self.modes_cycle = cycle(Modes)
        self.mode = next(self.modes_cycle)
        self.repo_version = Runner().get_versions()[0]
        self.cc_versions = []
        assert self.mode == Modes.Speed
        self.draw_array()
        self.stdscr.refresh()

    def cycle_mode(self):
        self.mode = next(self.modes_cycle)

    def cycle_baseline(self, right):
        self.baseline_col = self.baseline_col + 1 if right else self.baseline_col - 1
        self.baseline_col = max(0, min(self.baseline_col, self.col))

    def draw_array(self):
        logging.debug("draw")
        logging.debug(self.data[self.mode.value])
        self.clear_line(0)
        self.stdscr.addstr(0, 0, f"{self.mode.name}")
        for i, row in enumerate(self.data[self.mode.value]):
            self.stdscr.addstr(i + 1, 0, f"{self.labels[i]:^{self.WIDTH-1}}")
            for j, value in enumerate(row):
                if value is not None:
                    try:
                        logging.debug(value)
                        self.stdscr.addstr(i + 1, (j + 1) * self.WIDTH, f"{value:^{self.WIDTH-1}}")
                    except curses.error as e:
                        curses.endwin()
                        print("curses error, terminal probably too narrow")
                        exit(1)


    def clear_line(self, line):
        _, width = self.stdscr.getmaxyx()
        self.stdscr.addstr(line, 0, " " * width)  # Overwrite the line with spaces


    def set_status(self, status):

        height, _ = self.stdscr.getmaxyx()
        end = height - 1
        
        self.clear_line(end - 1)
        self.stdscr.addstr(end - 1, 0, status)
        # clear_line(end)
        self.stdscr.addstr(end, 0, "Press q to terminate")

        self.stdscr.refresh()


    def init_labels(self):
        labels = []
        for b in Benches.__members__:
            labels.append(b)
        return labels


    def init_array(self, cols):
        data = [[[None]*cols for _ in Benches] for _ in Modes]
        for mode_array in data:
            for b in Benches.__members__:
                for c in range(cols):
                    mode_array[Benches[b].value][c] = '...'

        return data


    def add_col(self):
        for mode_array in self.data:
            for b in Benches.__members__:
                mode_array[Benches[b].value].append('...')
        self.col += 1


    def set_done(self, num_done):
        self.set_status(f"Running benchmarks... {num_done}/{len(Benches)}")


    def update_relative(self):
        def percent(x):
            return f"{x:.1%}"

        for b in Benches.__members__:
            speed_data = self.data[Modes.Speed.value][Benches[b].value]
            size_data = self.data[Modes.Size.value][Benches[b].value]
            rel_speed_data = self.data[Modes.RelSpeed.value][Benches[b].value]
            rel_size_data = self.data[Modes.RelSize.value][Benches[b].value]
            rel_speed_data[self.col - 1] = percent(float(speed_data[self.col - 1]) / float(speed_data[0]))
            rel_size_data[self.col - 1] = percent(float(size_data[self.col - 1]) / float(size_data[0]))

    def run_all(self):
        runner = Runner()
        self.cc_versions.append(runner.get_versions()[1])
        self.add_col()
        done = 0
        self.set_done(done)

        with concurrent.futures.ThreadPoolExecutor() as executor:
            futures = [executor.submit(runner.run_bench, b) for b in Benches.__members__]
            for future in concurrent.futures.as_completed(futures):
                res = future.result()
                if res:
                    logging.debug(f"res {res}")
                    b, (speeds, sizes) = future.result()
                    logging.debug(f"bench, result {Benches[b].value},{(speeds, sizes)}")
                    speed_data = self.data[Modes.Speed.value][Benches[b].value]
                    size_data = self.data[Modes.Size.value][Benches[b].value]
                    if Benches[b] in self.DETAILED:
                        speed_data[self.col - 1] = speeds[0][1]
                        size_data[self.col - 1] = sizes[0][1]
                    else:
                        speed_data[self.col - 1] = speeds
                        size_data[self.col - 1] = sizes
                    logging.debug(f"speeeed {speed_data[self.col - 1]},{speed_data[0]}")
                    logging.debug(f"siiiize {size_data[self.col - 1]},{size_data[0]}")
                    self.draw_array()
                    done += 1
                    self.set_done(done)
                    self.stdscr.refresh()
        self.update_relative()
        self.set_status("Done!")


    def dump_csv(self, filename):
        with open(filename, "w", newline="") as csvfile:
            csv_writer = csv.writer(csvfile)
            csv_writer.writerow([f"RISC-V benchmark suite {self.repo_version}"] + self.cc_versions)
            for mode in [Modes.Speed, Modes.Size]:
                csv_writer.writerow([self.mode])
                mode_array = self.data[mode.value]
                for row in mode_array:
                    csv_writer.writerow(row)


def main(stdscr):
    tui = Tui(stdscr)

    while True:
        stdscr.refresh()
        key = stdscr.getch()
        if key == ord('q') or key == ord('Q'):
            filename = 'output.csv' # TODO
            tui.dump_csv(filename)
            curses.endwin()
            print(f"Data exported to {filename}")
            break

        if key == ord('r') or key == ord('R'):
            tui.run_all()

        if key == ord('m') or key == ord('M'):
            tui.cycle_mode()
            tui.draw_array()

        if key == curses.KEY_RIGHT:
            tui.cycle_baseline(right=True)
            tui.draw_array()

        if key == curses.KEY_LEFT:
            tui.cycle_baseline(right=False)
            tui.draw_array()

if __name__ == "__main__":
    logging.basicConfig(filename='debug.log', level=logging.DEBUG)
    curses.wrapper(main)
