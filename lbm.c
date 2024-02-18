#include "lbm.h"


void lbm_setup(
	  float f[]
	, float rho[]
	, float ux[]
	, float uy
	, const bool obstacles[]
	, const int width
	, const int height
) {

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
	  const int it
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
	, const int width
	, const int height
) {

}


void lbm_step2(
	  float f[]
	, const float new_f[]
	, const bool obstacles[]
	, const int width
	, const int height
) {

}
