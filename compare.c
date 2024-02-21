#include <stdio.h>
#include <stdlib.h>
#include <math.h>


float compare_frames(const int width, const int height, float u_ref[], float u_out[]) {
	const int size = width * height;
	float max = 0.0;

	for (int i = 0; i < size; ++i) {
		const float diff = fabs(u_ref[i] - u_out[i]);

		if (diff > max) {
			max = diff;
		}
	}

	return max;
}


int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Invalid command line arguments\n");
		fprintf(stderr, "Usage: compare <reference binary> <output binary>\n");

		return -1;
	}


	const char *reference_filename = argv[1];
	const char *output_filename    = argv[2];

	FILE *ref = fopen(reference_filename, "r");
	FILE *out = fopen(output_filename,    "r");

	if (ref == NULL) {
		fprintf(stderr, "Could not open file %s\n", reference_filename);
		return 1;
	}

	if (out == NULL) {
		fprintf(stderr, "Could not open file %s\n", output_filename);
		return 2;
	}

	int ref_width, ref_height, ref_it;
	int out_width, out_height, out_it;

	fscanf(ref, "%d %d\n", &ref_width, &ref_height);
	fscanf(out, "%d %d\n", &out_width, &out_height);

	if (ref_width != out_width || ref_height != out_height) {
		fprintf(stderr, "The two files differ in dimension\n");
		return 3;
	}

	float *u_ref = (float *) malloc(sizeof(float) * ref_width * ref_height);
	float *u_out = (float *) malloc(sizeof(float) * ref_width * ref_height);

	while (fscanf(ref, "%d\n", &ref_it) == 1 && fscanf(out, "%d\n", &out_it) == 1) {
		fread(u_ref, sizeof(float), ref_width * ref_height, ref);
		fread(u_out, sizeof(float), ref_width * ref_height, out);

		printf("frame %d, err %g\n", ref_it, compare_frames(ref_width, ref_height, u_ref, u_out));
	}

	fclose(ref);
	fclose(out);

	return 0;
}
