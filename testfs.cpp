#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "minunit.h"

#include "scratchfs.h"




using namespace cv;

const int t0_count = 3;
const int t1_count = 3;
	
const int t0_rows = 3;
const int t0_cols = 4;
const int t0_rXc = t0_rows * t0_cols;
	
const int t0_scale_0 = 1;
const int t0_scale_1 = 2;
const int t0_scale_2 = 100;

const bgrPxl t0_pxl_0[3] = {t0_scale_0, t0_scale_0, t0_scale_0};

const bgrPxl e0_gt_pxl = {t0_scale_2, t0_scale_2, t0_scale_2};
const bgrPxl e0_lt_pxl = {t0_scale_0, t0_scale_0, t0_scale_0};
	
const float t0_scalef_0 = 2.0;
const float t0_scalef_1 = 50.0;

const int t1_rows = 1;
const int t1_cols = 5;
const int t1_rXc = t1_rows * t1_cols;

const int l_int = 1;
const int g_int = l_int + 1;

	

const int t1_imgInt_0[t1_rXc] =  {  1,  2,  3,  4,  5 };
const int t1_imgInt_1[t1_rXc] =  {  4,  0,  1,  4,  3 };
const int t1_imgInt_2[t1_rXc] =  {  2,  1,  4,  3,  0 };

const int r1_gt_imgInt[t1_rXc] = {  1,  0,  2,  1,  0 };
const int r1_lt_imgInt[t1_rXc] = {  0,  1,  1,  2,  2 };

const int *t1_imgInts_0[t1_count] = {t1_imgInt_0, t1_imgInt_1, t1_imgInt_2};
	

const Vec3b vec3b0(1,1,1);
const Vec3b vec3b1(2,2,2);
const Vec3b vec3b2(100,100,100);

/*
const Vec3s vec3s0((short)2,(short)2,(short)2);
const Vec3i vec3i0(100L,100L,100L);

const Mat mat_8uc3  = Mat(t0_rows, t0_cols, CV_8UC3 , vec3b0);
const Mat mat_16uc3 = Mat(t0_rows, t0_cols, CV_16UC3, vec3s0);//Scalar(t0_scale_1,t0_scale_1,t0_scale_1) );
const Mat mat_32sc3 = Mat(t0_rows, t0_cols, CV_32SC3, vec3i0);//Scalar(t0_scale_2,t0_scale_2,t0_scale_2) );
const Mat mats[3] = {mat_8uc3, mat_16uc3, mat_32sc3};
*/
const Mat mat_0  = Mat(t0_rows, t0_cols, CV_8UC3 , vec3b0);
const Mat mat_1  = Mat(t0_rows, t0_cols, CV_8UC3 , vec3b1);
const Mat mat_2  = Mat(t0_rows, t0_cols, CV_8UC3 , vec3b2);

const Mat mats[3] = {mat_0, mat_1, mat_2};


static bgrPxl t0_imgPxl_0[ t0_rXc ];
static bgrPxl t0_imgPxl_1[ t0_rXc ];
static bgrPxl t0_imgPxl_2[ t0_rXc ];
static bgrPxl *t0_imgPxls_0[t0_count];


static bgrPxlF t0_imgPxlf_0[ t0_rXc ];
static bgrPxlF t0_imgPxlf_1[ t0_rXc ];


const unsigned long int e0_sum_0 = t0_rXc * t0_scale_0 * 3;
const unsigned long int e0_sum_1 = t0_rXc * t0_scale_1 * 3;
const unsigned long int e0_sum_2 = t0_rXc * t0_scale_2 * 3;
const unsigned long int e0_sums_0[t0_count] = {e0_sum_0, e0_sum_1, e0_sum_2};

static int e0_imgSum_0[ t0_rXc ];
static int e0_imgSum_1[ t0_rXc ];
static int e0_imgSum_2[ t0_rXc ];
static int *e0_imgSums_0[t0_count];


static bgrPxl e0_imgDiff_mats01[t0_rXc];
static bgrPxl e0_imgDiff_mats12[t0_rXc];
	
static bgrPxl e0_imgDiff_pxls01[t0_rXc];
static bgrPxl e0_imgDiff_pxls12[t0_rXc];
	
static bgrPxlF e0_imgDivPxlf_01[ t0_rXc ];
static bgrPxlF e0_imgDivPxlf_12[ t0_rXc ];

static bgrPxl e0_imgMulPxl_00[ t0_rXc ];
static bgrPxl e0_imgMulPxl_11[ t0_rXc ];
	
	
static bgrPxl *e0_imgDiffs_mats[t0_count-1];
static bgrPxl *e0_imgDiffs_pxls[t0_count-1];



char *
test_cmp_gt()
{
	mu_assert( cmp_gt(&g_int, &l_int), "should be a > b, but not");
	mu_assert( !cmp_gt(&l_int, &g_int), "should be a < b, but not"); 
	
	return NULL;
}


char *
test_cmp_lt()
{
	mu_assert( cmp_lt(&l_int, &g_int), "should be a < b, but not");
	mu_assert( !cmp_lt(&g_int, &l_int), "should be a > b, but not");
	
	return NULL;
}


char *
test_idx_cmp()
{
	int c = (int)sizeof(t1_imgInt_0)/(int)sizeof(t1_imgInt_0[0]) -1 ;
	int x   =  t1_imgInt_0[0];	//  {  1,  2,  3,  4,  5 };
	const int *xs = &t1_imgInt_0[1];
	int r = idx_cmp(x, 0, 1, c, xs, cmp_gt);
	int e = 4;
	std::string r_str = std::to_string(r);
	std::string e_str = std::to_string(e);
	mu_assert_re( r == e, "idx_cmp, cmp_gt", r_str.c_str(), e_str.c_str() );
	
	r = idx_cmp(x, 0, 1, c, xs, cmp_lt);
	e = 0;
	r_str = std::to_string(r);
	e_str = std::to_string(e);
	mu_assert_re( r == e, "idx_cmp, cmp_lt", r_str.c_str(), e_str.c_str());
	
	r = idx_cmp(x, 0, 3, c, xs, cmp_gt);
	e = 6;
	r_str = std::to_string(r);
	e_str = std::to_string(e);
	mu_assert_re( r == e, "idx_cmp, cmp_gt, idx_next different, already ordered", r_str.c_str(), e_str.c_str());
	
	r = idx_cmp(x, 0, 3, c, xs, cmp_lt);
	e = 0;
	r_str = std::to_string(r);
	e_str = std::to_string(e);
	mu_assert_re( r == e, "cmp_lt, idx_next, already ordered", r_str.c_str(), e_str.c_str());
	
	x = t1_imgInt_1[0];
	xs = &t1_imgInt_1[1];
	r = idx_cmp(x, 0, 1, c, xs, cmp_gt);
	e = 3;
	r_str = std::to_string(r);
	e_str = std::to_string(e);
	mu_assert_re( r == e, "cmp_gt, x high, same as val at idx 3", r_str.c_str(), e_str.c_str());
	
	r = idx_cmp(x, 0, 1, c, xs, cmp_lt);
	e = 1;
	r_str = std::to_string(r);
	e_str = std::to_string(e);
	mu_assert_re( r == e, "cmp_lt, val at idx 1 low", r_str.c_str(), e_str.c_str());
	
	return NULL;
}


char *
test_bgr_sv_idx()
{
	mu_assert( bgr_sv_idx(100,  0,  1) == 0, "should be blue (0)");
	mu_assert( bgr_sv_idx(  2,100,  3) == 1, "should be green (1)");
	mu_assert( bgr_sv_idx(  4,  5,100) == 2, "should be red (2)");
	mu_assert( bgr_sv_idx(100,100,  0) == 3, "should be blue-green (3)");
	mu_assert( bgr_sv_idx(100,  7,100) == 4, "should be blue-red (4)");
	mu_assert( bgr_sv_idx(  6,100,100) == 5, "should be green-red (5)");
	mu_assert( bgr_sv_idx(100,100,100) == 5, "expected be green-red (5)");
	
	return NULL;
}


char *
test_mats_to_sums()
{
	unsigned long int *r_sums = mats_to_sums(t0_count, mats);
	
	mu_assert( !memcmp(r_sums, e0_sums_0, sizeof(unsigned long int)*t0_count), 
		"sums mismatch");
	
	free(r_sums);
	
	return NULL;
}


char *
test_mats_into_sums()
{
	unsigned long int *r_sums = (unsigned long *) calloc( t0_count, sizeof(unsigned long) );
	mats_into_sums(t1_count, mats, r_sums);
	
	mu_assert( !memcmp(r_sums, e0_sums_0, sizeof(unsigned long int)*t0_count), 
		"sums mismatch");
	
	free(r_sums);
	
	return NULL;
}

char *
test_mat_to_imgSum()
{
	int *r_imgSum = mat_to_imgSum(mat_0);
	
	mu_assert( !memcmp(r_imgSum, e0_imgSum_0, sizeof(int)*t0_rXc), "imgSum mismatch");
	
	free(r_imgSum);
	
	return NULL;
}


char *
test_mat_into_imgSum()
{
	int *r_imgSum_0 = (int *) calloc( t0_rXc, sizeof(int) );
	mat_into_imgSum(mat_1, r_imgSum_0);
	
	mu_assert( !memcmp(r_imgSum_0, e0_imgSum_1, sizeof(int)*t0_rXc), "r_imgSum_0 mismatch");
	
	int *r_imgSum_1 = mat_to_imgSum(mat_1);
	mu_assert( !memcmp(r_imgSum_0, r_imgSum_1, sizeof(int)*t0_rXc), "r_imgSum_0 <> r_imgSum_1");
	
	
	free(r_imgSum_0);
	free(r_imgSum_1);
	
	return NULL;
}


char *
test_imgPxl_to_imgSum()
{
	int *r_imgSum = imgPxl_to_imgSum(t0_rows, t0_cols, t0_imgPxl_0);
	
	mu_assert( !memcmp(r_imgSum, e0_imgSum_0, sizeof(int)*t0_rXc), "r_imgSum mismatch");
	
	free(r_imgSum);
	
	return NULL;
}


char *
test_imgPxl_into_imgSum()
{
	int *r_imgSum_0 = (int *) calloc( t0_rXc, sizeof(int) );
	imgPxl_into_imgSum(t0_rows, t0_cols, t0_imgPxl_2, r_imgSum_0);
	
	mu_assert( !memcmp(r_imgSum_0, e0_imgSum_2, sizeof(int)*t0_rXc), 
		"r_imgSum_0 mismatch");
	
	int *r_imgSum_1 = imgPxl_to_imgSum(t0_rows, t0_cols, t0_imgPxl_2);
	mu_assert( !memcmp(r_imgSum_0, r_imgSum_1, sizeof(int)*t0_rXc), 
		"r_imgSum_0 <> r_imgSum_1");
	
	
	free(r_imgSum_0);
	free(r_imgSum_1);
	
	return NULL;
}


char *
test_mats_to_imgSums()
{
	int **r_imgSums = mats_to_imgSums(t0_count, mats);
	
	for (int i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp(*(r_imgSums+i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
			"an r_imgSums' imgSum not correct");
	}
	
	for (int i=0; i < t0_count; i++)
	{
		free(r_imgSums[i]);
	}
	free(r_imgSums);
	
	return NULL;
}


char *
test_mats_into_imgSums()
{
	int **r_imgSums = (int **) calloc( t0_count, sizeof(int *) );
	for (int i=0; i < t0_count; i++)
	{
		*(r_imgSums+i) = (int *) calloc( t0_rXc, sizeof(int) );
	}
	mats_into_imgSums(t0_count, mats, r_imgSums);
	
	for (int i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp(*(r_imgSums+i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
			"an r_imgSums' imgSum not correct");
	}
	
	for (int i=0; i < t0_count; i++)
	{
		free(r_imgSums[i]);
	}
	free(r_imgSums);
	
	return NULL;
}


char *
test_imgPxls_to_imgSums()
{
	int **r_imgSums = imgPxls_to_imgSums(t0_count, t0_rows, t0_cols, t0_imgPxls_0);
	
	for (int i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp(*(r_imgSums+i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
			"an r_imgSums' imgSum not correct");
	}
	
	for (int i=0; i < t0_count; i++)
	{
		free(r_imgSums[i]);
	}
	free(r_imgSums);
	
	return NULL;
}

char *
test_imgPxls_into_imgSums()
{
	int **r_imgSums = (int **) calloc( t0_count, sizeof(int *) );
	for (int i=0; i < t0_count; i++)
	{
		*(r_imgSums+i) = (int *) calloc( t0_rXc, sizeof(int) );
	}
	imgPxls_into_imgSums(t0_count, t0_rows, t0_cols, t0_imgPxls_0, r_imgSums);
	
	for (int i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp( *(r_imgSums+i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
			"an r_imgSums' imgSum not correct");
	}
	
	for (int i=0; i < t0_count; i++)
	{
		free(r_imgSums[i]);
	}
	free(r_imgSums);
	
	return NULL;
}


char *
test_imgInts_imgIdx()
{
	int *r_imgIdx_0 = imgInts_imgIdx(t1_count, t1_rows, t1_cols, cmp_gt, t1_imgInts_0 );
	mu_assert( !memcmp(r_imgIdx_0, r1_gt_imgInt, sizeof(int)*t1_rXc), "problem with gt" );
	
	int *r_imgIdx_1 = imgInts_imgIdx(t1_count, t1_rows, t1_cols, cmp_lt, t1_imgInts_0 );
	mu_assert( !memcmp(r_imgIdx_1, r1_lt_imgInt, sizeof(int)*t1_rXc), "problem with lt" );
	
	free(r_imgIdx_0);
	free(r_imgIdx_1);
	
	return NULL;
}


char *
test_matsBGR_to_imgPxlDiffs()
{
	bgrPxl **r_imgPxlDiffs = matsBGR_to_imgPxlDiffs( t0_count, mats);

	for (int i=0; i < t0_count-1 ; i++)
	{
		mu_assert( !memcmp(r_imgPxlDiffs[i], e0_imgDiffs_mats[i], sizeof(bgrPxl)*t0_rXc),
			"r_imgPxlDiffs' imgPxlDiff error");
	}
	
	for (int i=0; i < t0_count-1; i++)
	{
		free(r_imgPxlDiffs[i]);
	}
	free(r_imgPxlDiffs);
	
	return NULL;
}

char *
test_matsBGR_into_imgPxlDiffs()
{
	bgrPxl **r_imgPxlDiffs = (bgrPxl **) calloc( t0_count-1, sizeof(bgrPxl*) );
	
	for (int i=0; i < (t0_count-1); i++)
	{
		*(r_imgPxlDiffs+i) = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	}
	
	matsBGR_into_imgPxlDiffs( t0_count, mats, r_imgPxlDiffs);
	
	for (int i=0; i < t0_count-1 ; i++)
	{
		mu_assert( !memcmp(r_imgPxlDiffs[i], e0_imgDiffs_mats[i], sizeof(bgrPxl)*t0_rXc),
			"r_imgPxlDiffs' imgPxlDiff error");
	}
	
	for (int i=0; i < t0_count-1; i++)
	{
		free(r_imgPxlDiffs[i]);
	}
	free(r_imgPxlDiffs);
	
	return NULL;
}

char *
test_imgPxls_to_imgPxlDiffs()
{
	bgrPxl **r_imgPxlDiffs = imgPxls_to_imgPxlDiffs( t0_count, t0_rows, t0_cols, t0_imgPxls_0);
	
	for (int i=0; i < t0_count-1 ; i++)
	{
		mu_assert( !memcmp(r_imgPxlDiffs[i], e0_imgDiffs_pxls[i], sizeof(bgrPxl)*t0_rXc),
			"r_imgPxlDiffs' imgPxlDiff error");
	}
	
	for (int i=0; i < t0_count-1; i++)
	{
		free(r_imgPxlDiffs[i]);
	}
	free(r_imgPxlDiffs);
	
	return NULL;
}

char *
test_imgPxls_into_imgPxlDiffs()
{
	bgrPxl **r_imgPxlDiffs = (bgrPxl **) calloc( t0_count-1, sizeof(bgrPxl *) );
	
	for (int i=0; i < t0_count-1; i++)
	{
		*(r_imgPxlDiffs+i) = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	} 
	
	imgPxls_into_imgPxlDiffs( t0_count, t0_rows, t0_cols, t0_imgPxls_0, r_imgPxlDiffs);

	for (int i=0; i < t0_count-1 ; i++)
	{
		mu_assert( !memcmp(r_imgPxlDiffs[i], e0_imgDiffs_mats[i], sizeof(bgrPxl)*t0_rXc),
			"r_imgPxlDiffs' imgPxlDiff error");
	}
	
	for (int i=0; i < t0_count-1; i++)
	{
		free(r_imgPxlDiffs[i]);
	}
	free(r_imgPxlDiffs);
	
	return NULL;
}


char *
test_imgPxls_cmp_bgrPxl()
{
	bgrPxl r_pxl = imgPxls_cmp_bgrPxl(t0_count, t0_rows, t0_cols, cmp_gt, t0_imgPxls_0);
	
	String r_str = std::to_string(r_pxl.blue);
	String e_str = std::to_string(e0_gt_pxl.blue);
	mu_assert_re(r_pxl.blue  == e0_gt_pxl.blue,  "gt pixel wrong blue", 
					r_str.c_str(), e_str.c_str() );
	
	r_str = std::to_string(r_pxl.green);
	e_str = std::to_string(e0_gt_pxl.green);
	mu_assert_re(r_pxl.green == e0_gt_pxl.green, "gt pixel wrong green", 
					r_str.c_str(), e_str.c_str() );
	
	r_str = std::to_string(r_pxl.red);
	e_str = std::to_string(e0_gt_pxl.red);
	mu_assert_re(r_pxl.red   == e0_gt_pxl.red,   "gt pixel wrong red",
					r_str.c_str(), e_str.c_str() );	
	
	
	r_pxl = imgPxls_cmp_bgrPxl(t0_count, t0_rows, t0_cols, cmp_lt, t0_imgPxls_0);
		
	r_str = std::to_string(r_pxl.blue);
	e_str = std::to_string(e0_lt_pxl.blue);
	mu_assert_re(r_pxl.blue  == e0_lt_pxl.blue,  "lt pixel wrong blue",
					r_str.c_str(), e_str.c_str() );
					
	r_str = std::to_string(r_pxl.green);
	e_str = std::to_string(e0_lt_pxl.green);
	mu_assert_re(r_pxl.green == e0_lt_pxl.green, "lt pixel wrong green",
					r_str.c_str(), e_str.c_str() );
	
	r_str = std::to_string(r_pxl.red);
	e_str = std::to_string(e0_lt_pxl.red);
	mu_assert_re(r_pxl.red   == e0_lt_pxl.red,   "lt pixel wrong red",
					r_str.c_str(), e_str.c_str() );
	
	return NULL;
}


char *
test_acc_imgPxl_imgPxl()
{
	bgrPxl *r_imgPxl = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	acc_imgPxl_imgPxl(t0_rows, t0_cols, r_imgPxl, t0_imgPxl_0);
	
	mu_assert( !memcmp(r_imgPxl, t0_imgPxl_0, sizeof(bgrPxl)*t0_rXc), "acc has wrong value" );
	
	free(r_imgPxl);
	
	return NULL;
}


char *
test_acc_imgPxl_mat()
{
	bgrPxl *r_imgPxl = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );

	acc_imgPxl_mat(r_imgPxl, mats[0]);

	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=(r_imgPxl[y*t0_cols + x].blue  != (int)mats[0].at<Vec3b>(y,x)[0]);
			eq+=(r_imgPxl[y*t0_cols + x].green != (int)mats[0].at<Vec3b>(y,x)[1]);
			eq+=(r_imgPxl[y*t0_cols + x].red   != (int)mats[0].at<Vec3b>(y,x)[2]);
		}
	}
	
	std::string r_str = std::to_string(eq);	
	mu_assert_re( eq==0, "acc has wrong value", r_str.c_str(), "0" );
	
	free(r_imgPxl);
	
	return NULL;
}


char *
test_divf_imgPxl_by_imgPxl_to_imgPxlf()
{
	bgrPxlF *r_imgPxlf = divf_imgPxl_by_imgPxl_to_imgPxlf(t0_rows, t0_cols, 
							t0_imgPxl_1, t0_imgPxl_0);
		
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=((r_imgPxlf[y*t0_cols + x].blue - e0_imgDivPxlf_01[y*t0_cols + x].blue)*
				(r_imgPxlf[y*t0_cols + x].blue - e0_imgDivPxlf_01[y*t0_cols + x].blue) > 1E-7F);
			eq+=((r_imgPxlf[y*t0_cols + x].green - e0_imgDivPxlf_01[y*t0_cols + x].green)*
				(r_imgPxlf[y*t0_cols + x].green - e0_imgDivPxlf_01[y*t0_cols + x].green) > 1E-7F);
			eq+=((r_imgPxlf[y*t0_cols + x].red - e0_imgDivPxlf_01[y*t0_cols + x].red)*
				(r_imgPxlf[y*t0_cols + x].red - e0_imgDivPxlf_01[y*t0_cols + x].red) > 1E-7F);
		}
	}
	
	std::string r_str = std::to_string(eq);
	mu_assert_re( eq==0, "r_imgPxlf has wrong value", r_str.c_str(), "0" );
	
	free(r_imgPxlf);
	
	return NULL;
}

char *
test_divf_imgPxl_by_imgPxl_into_imgPxlf()
{
	bgrPxlF *r_imgPxlf = (bgrPxlF *) calloc(t0_rXc, sizeof(bgrPxlF) );
	divf_imgPxl_by_imgPxl_into_imgPxlf(t0_rows, t0_cols, 
							t0_imgPxl_2, t0_imgPxl_1, r_imgPxlf);
	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=((r_imgPxlf[y*t0_cols + x].blue - e0_imgDivPxlf_12[y*t0_cols + x].blue)*
				(r_imgPxlf[y*t0_cols + x].blue - e0_imgDivPxlf_12[y*t0_cols + x].blue) > 1E-7F);
			eq+=((r_imgPxlf[y*t0_cols + x].green - e0_imgDivPxlf_12[y*t0_cols + x].green)*
				(r_imgPxlf[y*t0_cols + x].green - e0_imgDivPxlf_12[y*t0_cols + x].green) > 1E-7F);
			eq+=((r_imgPxlf[y*t0_cols + x].red - e0_imgDivPxlf_12[y*t0_cols + x].red)*
				(r_imgPxlf[y*t0_cols + x].red - e0_imgDivPxlf_12[y*t0_cols + x].red) > 1E-7F);
		}
	}
	
	std::string r_str = std::to_string(eq);
	mu_assert_re( eq==0, "r_imgPxl has wrong value", r_str.c_str(), "0" );
	
	free(r_imgPxlf);
	
	return NULL;
}


char *
test_mul_imgPxl_by_imgPxlf()
{
	bgrPxl *r_imgPxl = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			r_imgPxl[y*t0_cols + x].blue  = t0_imgPxls_0[0][y*t0_cols + x].blue;
			r_imgPxl[y*t0_cols + x].green = t0_imgPxls_0[0][y*t0_cols + x].green;
			r_imgPxl[y*t0_cols + x].red   = t0_imgPxls_0[0][y*t0_cols + x].red;
		}
	}
	
	mul_imgPxl_by_imgPxlf(t0_rows, t0_cols, r_imgPxl, t0_imgPxlf_0);
	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=(r_imgPxl[y*t0_cols + x].blue  != e0_imgMulPxl_00[y*t0_cols + x].blue);
			eq+=(r_imgPxl[y*t0_cols + x].green != e0_imgMulPxl_00[y*t0_cols + x].green);
			eq+=(r_imgPxl[y*t0_cols + x].red   != e0_imgMulPxl_00[y*t0_cols + x].red);
		}
	}
	
	std::string r_str = std::to_string(eq);
	mu_assert_re( eq==0, "r_imgPxl has wrong value", r_str.c_str(), "0" );
	
	free(r_imgPxl);
	
	return NULL;
}


char *
test_mul_imgPxl_by_imgPxlf_to_imgPxl()
{
	bgrPxl *r_imgPxl = mul_imgPxl_by_imgPxlf_to_imgPxl(t0_rows, t0_cols, 
											t0_imgPxls_0[0], t0_imgPxlf_0);
	
	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=(r_imgPxl[y*t0_cols + x].blue  != e0_imgMulPxl_00[y*t0_cols + x].blue);
			eq+=(r_imgPxl[y*t0_cols + x].green != e0_imgMulPxl_00[y*t0_cols + x].green);
			eq+=(r_imgPxl[y*t0_cols + x].red   != e0_imgMulPxl_00[y*t0_cols + x].red);
		}
	}

	std::string r_str = std::to_string(eq);
	mu_assert_re( eq==0, "r_imgPxl has wrong value", r_str.c_str(), "0" );
	
	free(r_imgPxl);
	
	return NULL;
}

char *
test_mul_imgPxl_by_imgPxlf_into_imgPxl()
{
	bgrPxl *r_imgPxl = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	mul_imgPxl_by_imgPxlf_into_imgPxl(t0_rows, t0_cols, 
								t0_imgPxls_0[0], t0_imgPxlf_0, r_imgPxl);
	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=(r_imgPxl[y*t0_cols + x].blue  != e0_imgMulPxl_00[y*t0_cols + x].blue);
			eq+=(r_imgPxl[y*t0_cols + x].green != e0_imgMulPxl_00[y*t0_cols + x].green);
			eq+=(r_imgPxl[y*t0_cols + x].red   != e0_imgMulPxl_00[y*t0_cols + x].red);
		}
	}

	std::string r_str = std::to_string(eq);
	mu_assert_re( eq==0, "r_imgPxl has wrong value", r_str.c_str(), "0" );
	
	free(r_imgPxl);
	
	return NULL;
}

char *
test_imgPxl_into_mat()
{
	Mat r_mat = Mat(t0_rows, t0_cols, CV_8UC3, Scalar(1,1,1) );
	
	imgPxl_into_mat (t0_rows, t0_cols, t0_imgPxl_1, r_mat);
	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=(r_mat.at<Vec3b>(y,x)[0] != mats[1].at<Vec3b>(y,x)[0]);
			eq+=(r_mat.at<Vec3b>(y,x)[1] != mats[1].at<Vec3b>(y,x)[1]);
			eq+=(r_mat.at<Vec3b>(y,x)[2] != mats[1].at<Vec3b>(y,x)[2]);
		}
	}

	std::string r_str = std::to_string(eq);
	mu_assert_re( (eq==0), "r_mat not as expected", r_str.c_str(), "0" );
	
	r_mat.release();
	
	return NULL;
}


char *
all_tests()
{
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			t0_imgPxlf_0[y*t0_cols + x].blue  = t0_scalef_0;
			t0_imgPxlf_0[y*t0_cols + x].green = t0_scalef_0;
			t0_imgPxlf_0[y*t0_cols + x].red   = t0_scalef_0;
			
			t0_imgPxlf_1[y*t0_cols + x].blue  = t0_scalef_1;
			t0_imgPxlf_1[y*t0_cols + x].green = t0_scalef_1;
			t0_imgPxlf_1[y*t0_cols + x].red   = t0_scalef_1;
			
			t0_imgPxl_0[y*t0_cols + x].blue = vec3b0[0];
			t0_imgPxl_0[y*t0_cols + x].green = vec3b0[1];
			t0_imgPxl_0[y*t0_cols + x].red = vec3b0[2];
			
			t0_imgPxl_1[y*t0_cols + x].blue = vec3b1[0];
			t0_imgPxl_1[y*t0_cols + x].green = vec3b1[1];
			t0_imgPxl_1[y*t0_cols + x].red = vec3b1[2];
			
			t0_imgPxl_2[y*t0_cols + x].blue = vec3b2[0];
			t0_imgPxl_2[y*t0_cols + x].green = vec3b2[1];
			t0_imgPxl_2[y*t0_cols + x].red = vec3b2[2];
			
			
			e0_imgSum_0[y*t0_cols + x] = t0_imgPxl_0[y*t0_cols + x].blue +
											t0_imgPxl_0[y*t0_cols + x].green +
											t0_imgPxl_0[y*t0_cols + x].red;
			e0_imgSum_1[y*t0_cols + x] = t0_imgPxl_1[y*t0_cols + x].blue +
											t0_imgPxl_1[y*t0_cols + x].green +
											t0_imgPxl_1[y*t0_cols + x].red;
			e0_imgSum_2[y*t0_cols + x] = t0_imgPxl_2[y*t0_cols + x].blue +
											t0_imgPxl_2[y*t0_cols + x].green +
											t0_imgPxl_2[y*t0_cols + x].red;
			
			e0_imgDiff_mats01[y*t0_cols + x].blue = 
				mats[1].at<Vec3b>(y,x).val[0] - mats[0].at<Vec3b>(y,x).val[0];
			e0_imgDiff_mats01[y*t0_cols + x].green = 
				mats[1].at<Vec3b>(y,x).val[0] - mats[0].at<Vec3b>(y,x).val[0];
			e0_imgDiff_mats01[y*t0_cols + x].red = 
				mats[1].at<Vec3b>(y,x).val[0] - mats[0].at<Vec3b>(y,x).val[0];
					
			e0_imgDiff_mats12[y*t0_cols + x].blue = 
				mats[2].at<Vec3b>(y,x).val[0] - mats[1].at<Vec3b>(y,x).val[0];
			e0_imgDiff_mats12[y*t0_cols + x].green = 
				mats[2].at<Vec3b>(y,x).val[0] - mats[1].at<Vec3b>(y,x).val[0];
			e0_imgDiff_mats12[y*t0_cols + x].red = 
				mats[2].at<Vec3b>(y,x).val[0] - mats[1].at<Vec3b>(y,x).val[0];
		}
	}
	
	t0_imgPxls_0[0] = t0_imgPxl_0;
	t0_imgPxls_0[1] = t0_imgPxl_1; 
	t0_imgPxls_0[2] = t0_imgPxl_2;
	
	e0_imgDiffs_mats[0] = e0_imgDiff_mats01;
	e0_imgDiffs_mats[1] = e0_imgDiff_mats12;
	
	e0_imgSums_0[0] = e0_imgSum_0; 
	e0_imgSums_0[1] = e0_imgSum_1; 
	e0_imgSums_0[2] = e0_imgSum_2;
	
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			e0_imgDiff_pxls01[y*t0_cols + x].blue = 
				t0_imgPxls_0[1][y*t0_cols+x].blue - t0_imgPxls_0[0][y*t0_cols+x].blue;
			e0_imgDiff_pxls01[y*t0_cols + x].green = 
				t0_imgPxls_0[1][y*t0_cols+x].green - t0_imgPxls_0[0][y*t0_cols+x].green;
			e0_imgDiff_pxls01[y*t0_cols + x].red = 
				t0_imgPxls_0[1][y*t0_cols+x].red - t0_imgPxls_0[0][y*t0_cols+x].red;
					
			e0_imgDiff_pxls12[y*t0_cols + x].blue = 
				t0_imgPxls_0[2][y*t0_cols+x].blue - t0_imgPxls_0[1][y*t0_cols+x].blue;
			e0_imgDiff_pxls12[y*t0_cols + x].green = 
				t0_imgPxls_0[2][y*t0_cols+x].green - t0_imgPxls_0[1][y*t0_cols+x].green;
			e0_imgDiff_pxls12[y*t0_cols + x].red = 
				t0_imgPxls_0[2][y*t0_cols+x].red - t0_imgPxls_0[1][y*t0_cols+x].red;
				
				
			e0_imgDivPxlf_01[y*t0_cols + x].blue = 
				(float)t0_imgPxls_0[1][y*t0_cols+x].blue / 
				(float)t0_imgPxls_0[0][y*t0_cols+x].blue;
			e0_imgDivPxlf_01[y*t0_cols + x].green = 
				(float)t0_imgPxls_0[1][y*t0_cols+x].blue / 
				(float)t0_imgPxls_0[0][y*t0_cols+x].blue;
			e0_imgDivPxlf_01[y*t0_cols + x].red = 
				(float)t0_imgPxls_0[1][y*t0_cols+x].blue / 
				(float)t0_imgPxls_0[0][y*t0_cols+x].blue;
					
			e0_imgDivPxlf_12[y*t0_cols + x].blue = 
				(float)t0_imgPxls_0[2][y*t0_cols+x].blue / 
				(float)t0_imgPxls_0[1][y*t0_cols+x].blue;
			e0_imgDivPxlf_12[y*t0_cols + x].green = 
				(float)t0_imgPxls_0[2][y*t0_cols+x].blue / 
				(float)t0_imgPxls_0[1][y*t0_cols+x].blue;
			e0_imgDivPxlf_12[y*t0_cols + x].red = 
				(float)t0_imgPxls_0[2][y*t0_cols+x].blue / 
				(float)t0_imgPxls_0[1][y*t0_cols+x].blue;
				
			e0_imgMulPxl_00[y*t0_cols + x].blue = 
				t0_imgPxlf_0[y*t0_cols+x].blue *
				t0_imgPxls_0[0][y*t0_cols+x].blue;
			e0_imgMulPxl_00[y*t0_cols + x].green = 
				t0_imgPxlf_0[y*t0_cols+x].green *
				t0_imgPxls_0[0][y*t0_cols+x].green;
			e0_imgMulPxl_00[y*t0_cols + x].red = 
				t0_imgPxlf_0[y*t0_cols+x].red *
				t0_imgPxls_0[0][y*t0_cols+x].red;
					
			e0_imgMulPxl_11[y*t0_cols + x].blue = 
				t0_imgPxlf_1[y*t0_cols+x].blue *
				t0_imgPxls_0[1][y*t0_cols+x].blue;
			e0_imgMulPxl_11[y*t0_cols + x].green = 
				t0_imgPxlf_1[y*t0_cols+x].green *
				t0_imgPxls_0[1][y*t0_cols+x].green;
			e0_imgMulPxl_11[y*t0_cols + x].red = 
				t0_imgPxlf_1[y*t0_cols+x].red *
				t0_imgPxls_0[1][y*t0_cols+x].red;
		}
	}


	e0_imgDiffs_pxls[0] = e0_imgDiff_pxls01;
	e0_imgDiffs_pxls[1] = e0_imgDiff_pxls12;


	mu_suite_start();
	
	mu_run_test(test_cmp_gt);
	mu_run_test(test_cmp_lt);
	mu_run_test(test_idx_cmp);
	mu_run_test(test_bgr_sv_idx);
	mu_run_test(test_mats_to_sums);
	mu_run_test(test_mats_into_sums);
	mu_run_test(test_mat_to_imgSum);
	mu_run_test(test_mat_into_imgSum);
	mu_run_test(test_imgPxl_to_imgSum);
	mu_run_test(test_imgPxl_into_imgSum);
	mu_run_test(test_mats_to_imgSums);
	mu_run_test(test_mats_into_imgSums);
	mu_run_test(test_imgPxls_to_imgSums);
	mu_run_test(test_imgPxls_into_imgSums);
	mu_run_test(test_imgInts_imgIdx);
	mu_run_test(test_matsBGR_to_imgPxlDiffs);
	mu_run_test(test_matsBGR_into_imgPxlDiffs);
	mu_run_test(test_imgPxls_to_imgPxlDiffs);
	mu_run_test(test_imgPxls_into_imgPxlDiffs);
	mu_run_test(test_imgPxls_cmp_bgrPxl);
	mu_run_test(test_acc_imgPxl_imgPxl);
	mu_run_test(test_acc_imgPxl_mat);
	mu_run_test(test_divf_imgPxl_by_imgPxl_to_imgPxlf);
	mu_run_test(test_divf_imgPxl_by_imgPxl_into_imgPxlf);
	mu_run_test(test_mul_imgPxl_by_imgPxlf);
	mu_run_test(test_mul_imgPxl_by_imgPxlf_to_imgPxl);
	mu_run_test(test_mul_imgPxl_by_imgPxlf_into_imgPxl);
	mu_run_test(test_imgPxl_into_mat);
	
	return NULL;
}


RUN_TESTS(all_tests);














