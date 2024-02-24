#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lbm.h"


void dump_solution(FILE *out, const float solution[], const int it, const int width, const int height) {
	fprintf(out, "%d\n", it);
	fwrite(solution, sizeof(float), width * height, out);
}


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
		const float u_in_now = u_in * (1.0 - exp(-(it * it) / double_square_sigma));

		lbm_step1(width, height, it, u_in_now, omega_plus, sum_param, sub_param, f, new_f, rho, ux, uy, u_out, boundary, obstacles);
		lbm_step2(width, height, f, new_f, obstacles);

		if (it % (max_it / 100) == 0) {
			dump_solution(out, u_out, it, width, height);
		}

		printf("it %d\n", it);
	}


	return 0;
}
