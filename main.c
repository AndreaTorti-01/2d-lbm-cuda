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


	int width, height, max_it;
	float reynolds, u_in;


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


	fscanf(in, "%d %d\n%f %d %f\n", &width, &height, &reynolds, &max_it, &u_in);
	fprintf(out, "%d %d\n",  width,  height);


	const float nu = u_in * (float) (height) / reynolds * 2.0 / 3.0;
	const float tau = 3.0 * nu + 0.5;
	const float sigma = ceil(10.0 * height);
	const float double_square_sigma = 2.0 * sigma * sigma;
	const float lambda_trt = 1.0 / 4.0;
	const float tau_minus = lambda_trt / (tau - 0.5) + 0.5;
	const float omega_plus = 1.0 / tau;
	const float omega_minus = 1.0 / tau_minus;
	const float sub_param = 0.5 * (omega_plus - omega_minus);
	const float sum_param = 0.5 * (omega_plus + omega_minus);


	float *ux, *uy, *f, *new_f, *rho, *u_out;
	int *boundary;
	bool *obstacles;


	obstacles = (bool  *) malloc(width * height * sizeof(bool));
	ux        = (float *) malloc(width * height * sizeof(float));
	uy        = (float *) malloc(width * height * sizeof(float));
	u_out     = (float *) malloc(width * height * sizeof(float));
	rho       = (float *) malloc(width * height * sizeof(float));
	f         = (float *) malloc(9 * width * height * sizeof(float));
	new_f     = (float *) malloc(9 * width * height * sizeof(float));
	boundary  = (int   *) malloc(4 * width * height * sizeof(int));


	memset(obstacles, 0, width * height * sizeof(bool));


	int x, y;
	while (fscanf(in, "%d %d\n", &x, &y) == 2) {
		obstacles[x + y * width] = true;
	}
	fclose(in);


	lbm_calc_boundary(boundary, obstacles, width, height);
	lbm_init(f, rho, ux, uy, width, height, obstacles);

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
