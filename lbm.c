#include "lbm.h"
#include <math.h>


void lbm_init(
	  float f[]
	, float rho[]
	, float ux[]
	, float uy[]
	, const int width
	, const int height
	, const bool obstacles[]
) {
	const int size = width * height;
	const float weights[9] = {
		  4.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 36.0
		, 1.0 / 36.0
		, 1.0 / 36.0
		, 1.0 / 36.0
	};


	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			const int index = col + row * width;

			if (obstacles[index]) {
				ux[index] = NAN;
				uy[index] = NAN;
			}
			else {
				for (int i = 0; i < 9; ++i) {
					f[index + size * i] = weights[i];
				}

				rho[index] = 1;
				ux[index]  = 0;
				uy[index]  = 0;
			}
		}
	}
}


void lbm_calc_boundary(
	  int boundary[]
	, const bool obstacles[]
	, const int width
	, const int height
) {
	const int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};
	const int size = width * height;


	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			const int index = col + row * width;

			for (int d = 0; d < 4; d++) {
				// get the offsets for the current direction
				const int dx = dirs[d][0];
				const int dy = dirs[d][1];

				// check the adjacent cells in the current direction
				if (col - dx >= 0 && row - dy >= 0 && obstacles[col - dx + (row - dy) * width]) {
					boundary[size * d + index] = -1;
				}
				else if (col + dx < width && row + dy < height && obstacles[col + dx + (row + dy) * width]) {
					boundary[size * d + index] = 1;
				}
				else {
					boundary[size * d + index] = 0;
				}
			}
		}
	}
}


void lbm_step1(
	  const int width
	, const int height
	, const int it
	, const float u_in_now
	, const float om_p
        , const float sum_param
	, const float sub_param
	, float f[]
	, float new_f[]
	, float rho[]
	, float ux[]
        , float uy[]
	, float u_out[]
	, const int boundary[]
	, const bool obstacles[]
) {
	const int size = width * height;
	const int velocitiesX[9] = {0, 1, 0, -1, 0, 1, -1, -1, 1};
	const int velocitiesY[9] = {0, 0, -1, 0, 1, -1, -1, 1, 1};
	const int opposite[9]    = {0, 3, 4, 1, 2, 7, 8, 5, 6};
	const float weights[9] = {
		  4.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 9.0
		, 1.0 / 36.0
		, 1.0 / 36.0
		, 1.0 / 36.0
		, 1.0 / 36.0
	};


	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			const int index = row * width + col;

			if(!obstacles[index]) {
				
			}
		}
	}
}


void lbm_step2(
	  const int width
	, const int height
	, float f[]
	, const float new_f[]
	, const bool obstacles[]
) {

	#define F(x) f[size * x + index]
	#define NEW_F(x) new_f[size * x + index]


	const int size = width * height;
	const int velocitiesX[9] = {0, 1, 0, -1, 0, 1, -1, -1, 1};
	const int velocitiesY[9] = {0, 0, -1, 0, 1, -1, -1, 1, 1};


	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			const int index = row * width + col;

			if (!obstacles[index]) {
				// stream for index 0
				F(0) = NEW_F(0);

				// stream for other indices
				for (int i = 1; i < 9; i++) {
					// obtain new indices
					const int new_row = row + velocitiesY[i];
					const int new_col = col + velocitiesX[i];
					const int new_index = new_row * width + new_col;

					// stream if new index is not out of bounds or obstacle
					if (new_row >= 0 && new_row < height && new_col >= 0 && new_col < width && !obstacles[new_index]) {
						f[size * i + new_index] = NEW_F(i);
					}
				}
			}
		}
	}

	#undef F
	#undef NEW_F
}
