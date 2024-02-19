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
	#define F(x) f[size * x + index]
	#define NEW_F(x) new_f[size * x + index]


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
				// if i'm a any boundary set u to 0
				if (row == 0 || row == height - 1 || col == 0 || col == width - 1) {
					ux[index] = 0;
					uy[index] = 0;
				}

				// set parabolic profile inlet
				if (col == 0) {
					const float halfDim = static_cast<float>(height - 1) / 2.0;
					const float temp = static_cast<float>(row / halfDim) - 1.0;
					const float mul = 1.0 - temp * temp;

					ux[index] = u_in_now * mul;
				}

				// zou he

				// top wall
				if (row == 0 && col != 0 && col != width - 1) {
					rho[index] = (F(0) + F(1) + F(3) + 2.0 * (F(2) + F(5) + F(6))) / (1.0 + uy[index]);
					F(4) = F(2) - 2.0 / 3.0 * rho[index] * uy[index];
					F(7) = F(5) + 0.5 * (F(1) - F(3)) - 0.5 * rho[index] * ux[index] - 1.0 / 6.0 * rho[index] * uy[index];
					F(8) = F(6) - 0.5 * (F(1) - F(3)) + 0.5 * rho[index] * ux[index] - 1.0 / 6.0 * rho[index] * uy[index];
				}
				// right wall
				else if (col == width - 1 && row != 0 && row != height - 1) {
					rho[index] = 1;
					ux[index] = F(0) + F(2) + F(4) + 2.0 * (F(1) + F(5) + F(8)) - 1.0;
					F(3) = F(1) - 2.0 / 3.0 * ux[index];
					F(6) = F(8) - 0.5 * (F(2) - F(4)) - 1.0 / 6.0 * ux[index];
					F(7) = F(5) + 0.5 * (F(2) - F(4)) - 1.0 / 6.0 * ux[index];
				}
				// bottom wall
				else if (row == height - 1 && col != 0 && col != width - 1) {
					rho[index] = (F(0) + F(1) + F(3) + 2.0 * (F(4) + F(7) + F(8))) / (1.0 - uy[index]);
					F(2) = F(4) + 2.0 / 3.0 * rho[index] * uy[index];
					F(5) = F(7) - 0.5 * (F(1) - F(3)) + 0.5 * rho[index] * ux[index] + 1.0 / 6.0 * rho[index] * uy[index];
					F(6) = F(8) + 0.5 * (F(1) - F(3)) - 0.5 * rho[index] * ux[index] + 1.0 / 6.0 * rho[index] * uy[index];
				}
				// left wall
				else if (col == 0 && row != 0 && row != height - 1) {
					rho[index] = (F(0) + F(2) + F(4) + 2.0 * (F(3) + F(7) + F(6))) / (1.0 - ux[index]);
					F(1) = F(3) + 2.0 / 3.0 * rho[index] * ux[index];
					F(5) = F(7) - 0.5 * (F(2) - F(4)) + 1.0 / 6.0 * rho[index] * ux[index] + 0.5 * rho[index] * uy[index];
					F(8) = F(6) + 0.5 * (F(2) - F(4)) + 1.0 / 6.0 * rho[index] * ux[index] - 0.5 * rho[index] * uy[index];
				}
				// top right corner
				else if (row == 0 && col == width - 1) {
					rho[index] = rho[index - 1];
					F(3) = F(1) - 2.0 / 3.0 * rho[index] * ux[index];
					F(4) = F(2) - 2.0 / 3.0 * rho[index] * uy[index];
					F(7) = F(5) - 1.0 / 6.0 * rho[index] * ux[index] - 1.0 / 6.0 * rho[index] * uy[index];
					F(8) = 0;
					F(6) = 0;
					F(0) = rho[index] - F(1) - F(2) - F(3) - F(4) - F(5) - F(7);
				}
				// bottom right corner
				else if (row == height - 1 && col == width - 1) {
					rho[index] = rho[index - 1];
					F(3) = F(1) - 2.0 / 3.0 * rho[index] * ux[index];
					F(2) = F(4) + 2.0 / 3.0 * rho[index] * uy[index];
					F(6) = F(8) + 1.0 / 6.0 * rho[index] * uy[index] - 1.0 / 6.0 * rho[index] * ux[index];
					F(7) = 0;
					F(5) = 0;
					F(0) = rho[index] - F(1) - F(2) - F(3) - F(4) - F(6) - F(8);
				}
				// bottom left corner
				else if (row == height - 1 && col == 0) {
					rho[index] = rho[index + 1];
					F(1) = F(3) + 2.0 / 3.0 * rho[index] * ux[index];
					F(2) = F(4) + 2.0 / 3.0 * rho[index] * uy[index];
					F(5) = F(7) + 1.0 / 6.0 * rho[index] * ux[index] + 1.0 / 6.0 * rho[index] * uy[index];
					F(6) = 0;
					F(8) = 0;
					F(0) = rho[index] - F(1) - F(2) - F(3) - F(4) - F(5) - F(7);
				}
				// top left corner
				else if (row == 0 && col == 0) {
					rho[index] = rho[index + 1];
					F(1) = F(3) + 2.0 / 3.0 * rho[index] * ux[index];
					F(4) = F(2) - 2.0 / 3.0 * rho[index] * uy[index];
					F(8) = F(6) + 1.0 / 6.0 * rho[index] * ux[index] - 1.0 / 6.0 * rho[index] * uy[index];
					F(7) = 0;
					F(5) = 0;
					F(0) = rho[index] - F(1) - F(2) - F(3) - F(4) - F(6) - F(8);
				}

				// update macro

				rho[index] = 0;
				ux[index] = 0;
				uy[index] = 0;
				for (int i = 0; i < 9; i++) {
					rho[index] += F(i);
					ux[index] += F(i) * velocitiesX[i];
					uy[index] += F(i) * velocitiesY[i];
				}
				ux[index] /= rho[index];
				uy[index] /= rho[index];
				u_out[index] = sqrtf(ux[index] * ux[index] + uy[index] * uy[index]);

				// equilibrium
				float feq[9];
				const float temp1 = 1.5 * (ux[index] * ux[index] + uy[index] * uy[index]);
				for (int i = 0; i < 9; i++) {
					const float temp2 = 3.0 * (velocitiesX[i] * ux[index] + velocitiesY[i] * uy[index]);
					feq[i] = weights[i] * rho[index] * (1.0 + temp2 + 0.5 * temp2 * temp2 - temp1);
				}

				// collision for index 0
				NEW_F(0) = (1.0 - om_p) * F(0) + om_p * feq[0];

				// collision for other indices
				for (int i = 1; i < 9; i++) {
					NEW_F(i) = (1.0 - sum_param) * F(i) - sub_param * F(opposite[i]) + sum_param * feq[i] + sub_param * feq[opposite[i]];
				}

				// regular bounce back

				if (boundary[index] == 1) {
					F(3) = NEW_F(1);
				}
				else if (boundary[index] == -1) {
					F(1) = NEW_F(3);
				}
				if (boundary[size + index] == 1) {
					F(2) = NEW_F(4);
				}
				else if (boundary[size + index] == -1) {
					F(4) = NEW_F(2);
				}
				if (boundary[size * 2 + index] == 1) {
					F(6) = NEW_F(8);
				}
				else if (boundary[size * 2 + index] == -1) {
					F(8) = NEW_F(6);
				}
				if (boundary[size * 3 + index] == 1) {
					F(5) = NEW_F(7);
				}
				else if (boundary[size * 3 + index] == -1) {
					F(7) = NEW_F(5);
				}
			}
		}
	}

	#undef F
	#undef NEW_F
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
