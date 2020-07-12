# pathpuzzle-solver

This is a solver for [path puzzles](https://www.enigami.fun/the-path-puzzles-book).  For copyright reasons, only the font from our [path puzzles hardness paper](https://arxiv.org/abs/1803.01176) is included in this repository.

## Solver input format

You probably don't want to enter puzzles directly in this format.  Instead, use the Python preprocessing script described below.

The input is a series of lines that start with a single character followed by one or integers, space separated.
The first line should be of the form `s a b c` where `a` is the number of cells in the puzzle, `b` is the number of counters (clues, plus one dummy counter), and `c` is the number of terminals (doors).  Then, in any order:

- `a n r c n1 n2 n3 n4 c1 c2 c3 c4` specifies a cell.  `n` is the index of the cell (starting at 0), `r` and `c` are the row and column respectively, `n1` through `n4` are the indices of the cell's neighbors (use `-1` if there are fewer than four neighbors) and `c1` through `c4` are the counters covering the cell (use the dummy counter if there are fewer than four).
- `c n v` specifies a counter, which usually corresponds to a clue.  `n` is the index of the counter and `v` is the counter's initial value.  There should be one dummy counter, conventionally numbered 0, whose initial value is -1; this counter is used so that every cell is covered by four counters even if it isn't covered by four clues.
- `t i` specifies that the cell with index `i` is a terminal (door).

## More convenient input formats

`preprocess.py` takes as input a tab-separated file where `x` denotes a normal cell, `T` denotes a terminal, and numbers denote clues with that value.  This makes it easy to draw a puzzle in a spreadsheet program and export as TSV.

For rectangular puzzles (like the font), there is a simpler input format consisting of just the row and column clues (`-` for a missing clue) and terminals as row-column pairs.  See the font for examples.  Pass the `--font` argument to `preprocess.py` to use this format.
