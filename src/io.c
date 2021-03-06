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

/* io.c
 * This file contains procedures to read/write from/to stdin/stdout.
 */

#include <stdio.h>
#include <stdint.h>

#include "sdks.h"

/* Parse stdin and populate sudoku structure. */
void io_parse(struct Sudoku *sdk)
{
	uint16_t n, i;
	for (i = 0; i < SDK_CELLS; i++) {
		n = getchar() - '0';
		sdk->cells[i].num = n;
		/* Next char is a delimiter. */
		getchar();
	}
}


/* Print sudoku cells in machine readable form. */
void io_print(struct Sudoku *sdk)
{
	unsigned char c;
	uint16_t i;
	for (i = 0; i < SDK_CELLS; i++) {
		c = ((i + 1) % 9) ? ',' : '\n';
		printf("%d%c", sdk->cells[i].num, c);
	}
}
