#!/usr/bin/env python3

import argparse


class Cell(object):
    def __init__(self, n, r, c):
        self.n = n
        self.r = r
        self.c = c
        self.neighbors = []
        self.counters = []


def main(args):
    lines = [x.strip('\n') for x in args.input.readlines()]
    lines = [x.split('\t') for x in lines]

    things = {}
    cell_number = 0
    cells = []
    terminals = []
    for r in range(len(lines)):
        for c in range(len(lines[r])):
            if lines[r][c] == 'x':
                things[r, c] = Cell(cell_number, r, c)
                cell_number += 1
                cells.append(things[r, c])
            elif lines[r][c] == 'T':
                things[r, c] = Cell(cell_number, r, c)
                cell_number += 1
                cells.append(things[r, c])
                terminals.append(things[r, c])
            else:
                try:
                    things[r,c] = int(lines[r][c])
                except ValueError:
                    pass

    counter_number = 1
    counters = {0: -1} # maps to initial value; 0 is the dummy counter
    for (r, c), thing in things.items():
        if isinstance(thing, int):
            counters[counter_number] = thing
            # Scan blocks of cells in each direction until encountering a non-cell.
            x, y = r, c
            while isinstance(things.get((x - 1, y)), Cell):
                things[x - 1, y].counters.append(counter_number)
                x -= 1
            x, y = r, c
            while isinstance(things.get((x + 1, y)), Cell):
                things[x + 1, y].counters.append(counter_number)
                x += 1
            x, y = r, c
            while isinstance(things.get((x, y-1)), Cell):
                things[x, y - 1].counters.append(counter_number)
                y -= 1
            x, y = r, c
            while isinstance(things.get((x, y + 1)), Cell):
                things[x, y + 1].counters.append(counter_number)
                y += 1
            counter_number += 1
        elif isinstance(thing, Cell):
            if isinstance(things.get((r - 1, c)), Cell):
                thing.neighbors.append(things[r - 1, c].n)
            if isinstance(things.get((r + 1, c)), Cell):
                thing.neighbors.append(things[r + 1, c].n)
            if isinstance(things.get((r, c - 1)), Cell):
                thing.neighbors.append(things[r, c - 1].n)
            if isinstance(things.get((r, c + 1)), Cell):
                thing.neighbors.append(things[r, c + 1].n)
            while len(thing.neighbors) < 4:
                thing.neighbors.append(-1)

    # A cell can be in range of at most four clues (one in each direction).
    # To avoid branching, pad with the dummy counter.
    for cell in cells:
        while len(cell.counters) < 4:
            cell.counters.append(0)

    items = ['s', str(len(cells)), str(len(counters)), str(len(terminals))]
    args.output.write(' '.join(items)+'\n')
    for cell in cells:
        items = ['a', str(cell.n), str(cell.r), str(cell.c)] + list(map(str, cell.neighbors)) + list(map(str, cell.counters))
        args.output.write(' '.join(items)+'\n')
    for counter, value in counters.items():
        items = ['c', str(counter), str(value)]
        args.output.write(' '.join(items) + '\n')
    for cell in terminals:
        items = ['t', str(cell.n)]
        args.output.write(' '.join(items) + '\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=argparse.FileType('r'))
    parser.add_argument('output', type=argparse.FileType('w'))
    main(parser.parse_args())
