/*
 * sdks - Sudoku Solver
 * Copyright (C) 2021 Marcel Engelke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * idx.c
 * This file contains procedures for indexing the fields of sudoku cells
 */

#include <math.h>
#include <stdint.h>

#include "sdks.h"

static uint16_t idx_index(struct Cell **cells);
static uint16_t idx_index_adv(struct Cell **cells, uint16_t numAvail);

/* Initialize the index for unfilled cells. */
void idx_index_init(struct Sudoku *sdk)
{
	#ifdef DEBUG
		unsigned char *mod = "idx_index_init";
	#endif
	uint16_t i;
	LOG("%s: Initializing index\n", mod);
	for (i = 0; i < SDK_CELLS; i++) {
		if (sdk->cells[i].num) {
			continue;
		}
		sdk->cells[i].avail = SDK_AVAIL_DEF;
		sdk->freeCells++;
	}
	LOG("%s: Done\n", mod);
}

/* Iterate through sudoku segments and update each cell's index using elimination rules.
 * Return 0 upon detecting an unsolvable segment, non-zero otherwise.
 */
uint16_t idx_index_sdk(struct Sudoku *sdk)
{
	#ifdef DEBUG
		char *mod = "idx_index_sdk";
	#endif
	uint16_t i, numAvail;
	for (i = 0; i < SDK_WIDTH; i++) {
		LOG("%s: Indexing row %d\n", mod, i);
		numAvail = idx_index(sdk->rows[i]);
		if (!idx_index_adv(sdk->rows[i], numAvail)) {
			goto failure;
		}
		LOG("%s: Indexing column %d\n", mod, i);
		numAvail = idx_index(sdk->cols[i]);
		if (!idx_index_adv(sdk->cols[i], numAvail)) {
			goto failure;
		}
		LOG("%s: Indexing column %d\n", mod, i);
		numAvail = idx_index(sdk->groups[i]);
		if (!idx_index_adv(sdk->groups[i], numAvail)) {
			goto failure;
		}
	}
	LOG("%s: Done\n", mod);
	return 1;
	failure:
		LOG("%s: Error: Encountered invalid segment!\n", mod);
		return 0;
}

/* Find available numbers in sudoku segment and update each cell's index.
 * Return existing numbers as bitflags in an integer.
 */
static uint16_t idx_index(struct Cell **cells)
{
	#ifdef DEBUG
		unsigned char *mod = "idx_index";
	#endif
	uint16_t i;
	int numAvail = SDK_AVAIL_DEF;
	LOG("%s: Found: ", mod);
	for (i = 0; i < SDK_WIDTH; i++) {
		if (cells[i]->num) {
			numAvail &= ~(1 << cells[i]->num);
			LOG("%d ", cells[i]->num);
		}
	}
	LOG("\n%s: Available numbers: %x\n", mod, numAvail);
	for (i = 0; i < SDK_WIDTH; i++) {
		if (cells[i]->num) {
			continue;
		}
		cells[i]->avail &= numAvail;
	}
	LOG("%s: Done\n", mod);
	return numAvail;
}

/* Find available cells for missing numbers in sudoku segment.
 * Update a cell's index where applicable using sophisticated elimination rules.
 * Return 0 upon deeming the segment unsolvable, non-zero otherwise.
 * TODO: More complex rules necessary?
 */
static uint16_t idx_index_adv(struct Cell **cells, uint16_t numAvail)
{
	#ifdef DEBUG
		unsigned char *mod = "idx_index_adv";
	#endif
	uint16_t i, num, pos, posAvail;
	for (num = 1; num <= SDK_WIDTH; num++) {
		if (!(numAvail & (1 << num))) {
			continue;
		}
		for (i = 0, posAvail = 0; i < SDK_WIDTH; i++) {
			if (cells[i]->num) {
				continue;
			}
			if (cells[i]->avail & (1 << num)) {
				posAvail++;
				pos = i;
			}
		}
		LOG("%s: Found %d available cells for number %d\n", mod, posAvail, num);
		switch(posAvail) {
		case 0:
			LOG("%s: Error: No available cell found!\n", mod);
			return 0;
		case 1:
			cells[pos]->avail = 1 << num;
			break;
		}
	}
	LOG("%s: Done\n", mod);
	return 1;
}
