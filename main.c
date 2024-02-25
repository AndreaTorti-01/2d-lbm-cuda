#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lbm.h"


int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Invalid command line arguments\n");
		fprintf(stderr, "Usage: serial <input filename> <output filename>\n");

		return -1;
	}


	const char *input_filename = argv[1];
	const char *output_filename = argv[2];


	FILE *in  = fopen(input_filename,  "r");
	FILE *out = fopen(output_filename, "w");


	if (in == NULL) {
		fprintf(stderr, "Could not open input file %s\n", input_filename);
		return 1;
	}

	if (out == NULL) {
		fprintf(stderr, "Could not open output file %s\n", output_filename);
		return 1;
	}


	lbm_setup(in);
	fprintf(out, "%d %d\n", width, height);


	for (int it = 0; it <= max_it; ++it) {
		lbm_step(it);

		if (it % (max_it / 100) == 0) {
			lbm_dump_solution(out, it);
		}

		printf("it %d\n", it);
	}


	return 0;
}
