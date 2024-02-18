#ifndef __LBM_H__
#define __LBM_H__


void lbm_setup(
	  float f[]
	, float rho[]
	, float ux[]
	, float uy
	, const bool obstacles[]
	, const int width
	, const int height
);


void lbm_calc_boundary(
	  int boundary[]
	, const bool obstacles[]
	, const int width
	, const int height
);


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
);


void lbm_step2(
	  float f[]
	, const float new_f[]
	, const bool obstacles[]
	, const int width
	, const int height
);


#endif
