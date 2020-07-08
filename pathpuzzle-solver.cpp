#include "vendor/ordered_set.h"
#include <vector>
#include <array>
#include <algorithm>
#include <cstdio>
#include <string_view>

using namespace std::literals::string_view_literals;

struct Cell {
	unsigned int n, r, c;
	std::array<int, 4> neighbors;
	std::array<unsigned int, 4> counters;
};

std::vector<Cell> cells;
std::vector<unsigned int> terminals; //indices into cells
std::vector<int> counters;


void parse(std::FILE* input) {
	unsigned int cell_size, counter_size, terminal_size;
	Cell cell;
	unsigned int counter_index;
	int counter_value;
	unsigned int terminal_cell;

	while (!std::feof(input)) {
		if (std::fscanf(input, " s %u %u %u", &cell_size, &counter_size, &terminal_size) == 3) {
			cells.resize(cell_size);
			counters.resize(counter_size);
			//We don't resize terminals because we use push_back.
		} else if (std::fscanf(input, " a %u %u %u %d %d %d %d %u %u %u %u",
				&cell.n, &cell.r, &cell.c,
				&cell.neighbors[0], &cell.neighbors[1], &cell.neighbors[2], &cell.neighbors[3],
				&cell.counters[0], &cell.counters[1], &cell.counters[2], &cell.counters[3]) == 11)
			cells[cell.n] = cell;
		else if (std::fscanf(input, " c %u %d", &counter_index, &counter_value) == 2)
			counters[counter_index] = counter_value;
		else if (std::fscanf(input, " t %u", &terminal_cell) == 1)
			terminals.push_back(terminal_cell);
	}
	std::sort(terminals.begin(), terminals.end());
}


using PathSet = tsl::ordered_set<unsigned int>;
const auto le_zero = [](int x){return x <= 0;};

void solve_recurse(PathSet& path, std::vector<std::vector<unsigned int>>& results) {
	//We check path.back() > path.front() so we don't find each path twice
	//(once reversed).
	if (path.back() > path.front() && 
			std::find(terminals.begin(), terminals.end(), path.back()) != terminals.end() &&
			std::all_of(counters.begin(), counters.end(), le_zero))
		results.push_back(std::vector<unsigned int>(path.begin(), path.end()));

	for (int n : cells[path.back()].neighbors) {
		if (n < 0) continue;
		bool counters_continue = false;
		for (int ci : cells[n].counters)
			if (counters[ci] == 0)
				counters_continue = true;
		if (counters_continue) continue;
		if (!path.insert(n).second) continue;

		for (int ci : cells[n].counters)
			--counters[ci];
		solve_recurse(path, results);
		for (int ci : cells[n].counters)
			++counters[ci];
		path.pop_back();
	}
}

std::vector<std::vector<unsigned int>> solve() {
	std::vector<std::vector<unsigned int>> results;
	PathSet path;
	for (unsigned int ti = 0; ti + 1 < terminals.size(); ++ti) {
		path.clear();
		path.insert(terminals[ti]);
		for (int ci : cells[path.back()].counters)
			--counters[ci];
		solve_recurse(path, results);
		for (int ci : cells[path.back()].counters)
			++counters[ci];
	}
	return results;
}

int main(int argc, char* argv[]) {
	FILE* input = stdin, *output = stdout;
	if (argc >= 2 && argv[1] != "-"sv)
		input = std::fopen(argv[1], "r");
	if (argc >= 3 && argv[2] != "-"sv)
		output = std::fopen(argv[1], "w");
	parse(input);

	std::vector<std::vector<unsigned int>> solutions = solve();

	for (const std::vector<unsigned int>& soln : solutions) {
		std::fprintf(output, "(%d, %d)", cells[soln[0]].r, cells[soln[0]].c);
		for (unsigned int i = 1; i < soln.size(); ++i)
			std::fprintf(output, " (%d, %d)", cells[soln[i]].r, cells[soln[i]].c);
		std::fprintf(output, "\n");
	}
	return 0;
}
