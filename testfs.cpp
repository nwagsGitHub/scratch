#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "dbg.h"
#include "minunit.h"

#include "scratchfs.h"

/*
bgrPxl *imgPxl_input1, *imgPxl_input2, *imgPxl_expected1;
bgrPxlF *imgPxlF_input1, *imgPxlF_expected1;
bgrPxl **imgPxls_input1, **imgPxls_expected1;
Mat mat_input1, mat_expected;
Mat *mats_input1;
int *img1_input1, *img1_input2;
*/




/*

idx_cmp
	cmp_gt
	cmp_lt
bgr_sv_idx
mats_to_sums
mats_into_sums
mat_to_imgSum
mat_into_imgSum
imgPxl_to_imgSum
imgPxl_into_imgSum
mats_to_imgSums
mats_into_imgSums
imgPxls_to_imgSums
imgPxls_into_imgSums
img1s_imgIdx
matsBGR_to_imgPxlDiffs
matsBGR_into_imgPxlDiffs
imgPxls_to_imgPxlDiffs
imgPxls_into_imgPxlDiffs
imgPxls_cmp_bgrPxl
acc_imgPxl_imgPxl
acc_imgPxl_mat
divf_imgPxl_by_imgPxl_to_imgPxlf
divf_imgPxl_by_imgPxl_into_imgPxlf
mul_imgPxl_by_imgPxlf
mul_imgPxl_by_imgPxlf_to_imgPxl
mul_imgPxl_by_imgPxlf_into_imgPxl
imgPxl_into_mat

*/


char *
test_cmp_gt()
{
	mu_assert( cmp_gt(g_int, l_int), "should be a > b, but not");
	mu_assert( !cmp_gt(l_int, g_int), "should be a < b, but not"); 
}


char *
test_cmp_lt()
{
	mu_assert( cmp_lt(l_int, g_int), "should be a < b, but not");
	mu_assert( !cmp_lt(g_int, l_int), "should be a > b, but not");
}


char *
test_idx_cmp()
{
	int c = t1_count-1;
	
	int x   =  t1_img1_0[0];
	int *xs = &t1_img1_0[1];
	int r = idx_cmp(x, 0, 1, c, xs, cmp_gt);
	int e = 4;
	mu_assert( r == e, "cmp_gt, already ordered");
	
	r = idx_cmp(x, 0, 1, c, xs, cmp_lt);
	e = 0;
	mu_assert( r == e, "cmp_lt, already ordered");
	
	r = idx_cmp(x, 0, 3, c, xs, cmp_gt);
	e = 6;
	mu_assert( r == e, "cmp_gt, idx_next, already ordered");
	
	r = idx_cmp(x, 0, 3, c, xs, cmp_lt);
	e = 0;
	mu_assert( r == e, "cmp_lt, idx_next, already ordered");
	
	x = t1_img1_1[0];
	xs = &t1_img1_1[1];
	r = idx_cmp(x, 0, 1, c, xs, cmp_gt);
	e = 3;
	mu_assert( r == e, "cmp_gt, x high, same as val at idx 3");
	
	r = idx_cmp(int_t, 0, 1, c, xs, cmp_lt);
	e = 1;
	mu_assert( r == e, "cmp_lt, val at idx 1 low");
	
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
	unsigned long int *r_sums = mats_to_sums(t0_count, mats_ones);
	mu_assert( !memcmp(r_sums, e0_sums_0, sizeof(unsigned long int)*t0_count), 
		"sums mismatch");
	
	free(r_sums);
	
	return NULL;
}


char *
test_mats_into_sums()
{
	unsigned long int *r_sums = (unsigned long *) calloc( t0_count, sizeof(unsigned long) );
	mats_to_sums(t1_count, mats_ones, r_sums);
	
	mu_assert( !memcmp(r_sums, e0_sums_0, sizeof(unsigned long int)*t0_count), 
		"sums mismatch");
	
	free(r_sums);
	
	return NULL;
}

char *
test_mat_to_imgSum()
{
	int *r_imgSum = mat_to_imgSum(mat_8u);
	
	mu_assert( !memcmp(r_imgSum, e0_imgSum_0, sizeof(int)*r0_rXc), "imgSum mismatch");
	
	free(r_imgSum);
	
	return NULL;
}


char *
test_mat_into_imgSum()
{
	int *r_imgSum_0 = (int *) calloc( t0_rXc, sizeof(int) );
	mat_into_imgSum(mat_16u, r_imgSum_0);
	
	mu_assert( !memcmp(r_imgSum_0, e0_imgSum_0, sizeof(int)*t0_rXc), "r_imgSum_0 mismatch");
	
	int *r_imgSum_1 = mat_to_imgSum(mat_16u);
	mu_assert( !memcmp(r_imgSum0, r_imgSum_1, sizeof(int)*t0_rXc), "r_imgSum_0 <> r_imgSum_1");
	
	
	free(r_imgSum_0);
	free(r_imgSum_1);
}


char *
test_imgPxl_to_imgSum()
{
	int *r_imgSum = imgPxl_to_imgSum(t0_imgPxl_0);
	
	mu_assert( !memcmp(r_imgSum, e0_imgSum_0, sizeof(int)*t0_rXc, "r_imgSum mismatch");
	
	free(r_imgSum);
	
	return NULL;
}


char *
test_imgPxl_into_imgSum()
{
	int *r_imgSum_0 = (int *) calloc( t0_rXc, sizeof(int) );
	imgPxl_into_imgSum(t0_imgPxl_2, r_imgSum_0);
	
	mu_assert( !memcmp(r_imgSum_0, e0_imgSum_2, sizeof(int)*t0_rXc), 
		"r_imgSum_0 mismatch");
	
	int *r_imgSum_1 = imgPxl_to_imgSum(t0_imgPxl_2);
	mu_assert( !memcmp(r_imgSum_0, r_imgSum_1, sizeof(int)*t0_rXc), 
		"r_imgSum_0 <> r_imgSum_1");
	
	
	free(r_imgSum_0);
	free(r_imgSum_1);
}


char *
test_mats_to_imgSums()
{
	int **r_imgSums = mats_to_imgSums(mats);
	
	for (img i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp(*(r_imgSums+i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
			"an r_imgSums' imgSum not correct");
	}
	
	for (img i=0; i < t0_count; i++)
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
	mats_into_imgSums(mats, r_imgSums);
	
	for (int i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp(*(imgSums*i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
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
	int **r_imgSums = imgPxls_to_imgSums(t0_imgPxls_0);
	
	for (img i=0; i < t0_count; i++)
	{
		mu_assert( !memcmp(*(r_imgSums+i), e0_imgSums_0[i], sizeof(int)*t0_rXc), 
			"an r_imgSums' imgSum not correct");
	}
	
	for (img i=0; i < t0_count; i++)
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
	imgPxls_into_imgSums(t0_imgPxls_0, r_imgSums);
	
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
}


char *
test_matsBGR_to_imgPxlDiffs()
{
	bgrPxl **r_imgPxlDiffs = matsBGR_into_imgPxlDiffs( t0_count, mats);

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
	bgrPxl **r_imgPxlDiffs;
	for (int i=0; i < t0_count-1; i++)
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
	bgrPxl **r_imgPxlDiffs = imgPxls_to_imgPxlDiffs( t0_count, t0_imgPxls_0);

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
	bgrPxl **r_imgPxlDiffs;
	for (int i=0; i < t0_count-1; i++)
	{
		*(r_imgPxlDiffs+i) = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl *) );
	} 
	
	imgPxls_into_imgPxlDiffs( t0_count, t0_imgPxls_0, r_imgPxlDiffs);

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
	
	mu_assert(r_pxl == e0_gt_pxl, "gt pixel wrong");
	
	r_pxl = imgPxls_cmp_bgrPxl(t0_count, t0_rows, t0_cols, cmp_lt, t0_imgPxls_0);
	
	mu_assert(r_pxl == e0_lt_pxl, "lt pixel wrong");
}


char *
test_acc_imgPxl_imgPxl()
{
	bgrPxl *r_imgPxl = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	acc_imgPxl_imgPxl(t0_rows, t0_cols, r_imgPxl, t0_imgPxl_0);
	
	mu_assert( !memcmp(r_imgPxl, t0_imgPxl_0, sizeof(bgrPxl)*t0_rXc) );
	
	free(r_imgPxl);
}


char *
test_acc_imgPxl_mat()
{
	bgrPxl *r_imgPxl = (bgrPxl *) calloc( t0_rXc, sizeof(bgrPxl) );
	acc_imgPxl_imgPxl(t0_rows, t0_cols, r_imgPxl, mats[0]);
	
	int eq=0;
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			eq+=!(r_imgPxl[y*t0_cols + x]->blue  == (int)mats[0].at<Vec3b>(y,x)[0]);
			eq+=!(r_imgPxl[y*t0_cols + x]->green == (int)mats[0].at<Vec3b>(y,x)[1]);
			eq+=!(r_imgPxl[y*t0_cols + x]->red   == (int)mats[0].at<Vec3b>(y,x)[2]);
		}
	}
	mu_assert( eq==0, "acc has wrong value" );
	
	free(r_imgPxl);
}


char *
test_divf_imgPxl_by_imgPxl_to_imgPxlf()
{
	
}

char *
test_divf_imgPxl_by_imgPxl_into_imgPxlf()
{

}


char *
test_mul_imgPxl_by_imgPxlf()
{

}


char *
test_mul_imgPxl_by_imgPxlf_to_imgPxl()
{

}

char *
test_mul_imgPxl_by_imgPxlf_into_imgPxl()
{

}

char *
test_imgPxl_into_mat()
{
	
}


char *
all_tests()
{
	int t_int;
	float t_float;

	int t0_count = 3;
	int t1_count = 3;
	
	int t0_rows = 3;
	int t0_cols = 4;
	int t0_rXc = t0_rows * t0_cols;
	
	int t0_scale_0 = 1;
	int t0_scale_1 = 2;
	int t0_scale_2 = 100;

	int t1_rows = 1;
	int t1_cols = 5;
	int t1_rXc = t1_rows * t1_cols;

	int l_int = 1;
	int g_int = l_int + 1;

	

	int t1_imgInt_0[t1_rXc] =  {  1,  2,  3,  4,  5 };
	int t1_imgInt_1[t1_rXc] =  {  4,  0,  1,  4,  3 };
	int t1_imgInt_2[t1_rXc] =  {  2,  1,  4,  3,  0 };

	int r1_gt_imgInt[t1_rXc] = {  1,  0,  2,  1,  0 };
	int r1_lt_imgInt[t1_rXc] = {  0,  1,  1,  2,  2 };

	int t1_imgInts_0[t1_count] = {t1_img1_0, t1_img1_1, t1_img1_2};
	



	Mat mat_8u  = Mat::ones(t0_rows, t0_cols, CV_8U )*t0_scale_0;
	Mat mat_16u = Mat::ones(t0_rows, t0_cols, CV_16U)*t0_scale_1;
	Mat mat_32u = Mat::ones(t0_rows, t0_cols, CV_32U)*t0_scale_2;
	Mat mats[3] = {mat_8u, mat_16u, mat_32u};



	bgrPxl t0_imgPxl_0[ t0_rXc ];
	bgrPxl t0_pxl_0 = {t0_scale_0, t0_scale_0, t0_scale_0};
	for (int i=0; i < t0_rXc; i++)
	{
		t0_imgPxl_0[i] = t0_pxl_0;
	}

	bgrPxl t0_imgPxl_1[ t0_rXc ];
	bgrPxl t0_pxl_1 = {t0_scale_1, t0_scale_1, t0_scale_1};
	for (int i=0; i < t0_rXc; i++)
	{
		t0_imgPxl_1[i] = t0_pxl_1;
	}

	bgrPxl t0_imgPxl_2[ t0_rXc ];
	bgrPxl t0_pxl_2 = {t0_scale_2, t0_scale_2, t0_scale_2};
	for (int i=0; i < t0_rXc; i++)
	{
		t0_imgPxl_2[i] = t0_pxl_2;
	}

	bgrPxl t0_imgPxls_0 = {t0_imgPxl_0, t0_imgPxl_1, t0_imgPxl_2};
	
	e0_gt_pxl = {t0_scale_2, t0_scale_2, t0_scale_2};
	e0_lt_pxl = {t0_scale_0, t0_scale_0, t0_scale_0};
	
	
	
	unsigned long int e0_sum_0 = t0_rXc * t0_scale_0;
	unsigned long int e0_sum_1 = t0_rXc * t0_scale_1;
	unsigned long int e0_sum_2 = t0_rXc * t0_scale_2;
	unsigned long int e0_sums_0[t0_count] = {e0_sum_0, e0_sum_1, e0_sum_2};

	int e0_imgSum_0[ t0_rXc ];
	for (int i=0; i < t0_rXc; i++)
	{
		e0_imgSum_0[i] = 3;
	}
	
	int e0_imgSum_1[ t0_rXc ];
	for (int i=0; i < t0_rXc; i++)
	{
		e0_imgSum_1[i] = 6;
	}

	int e0_imgSum_2[ t0_rXc ];
	for (int i=0; i < t0_rXc; i++)
	{
		e0_imgSum_2[i] = 300;
	}
	
	int *e0_imgSums_0[t0_count] = {e0_imgSum_0, e1_imgSum_1, e0_imgSum_2};

	bgrPxl e0_imgDiff_mats01[t0_rXc];
	bgrPxl e0_imgDiff_mats12[t0_rXc];
	
	bgrPxl e0_imgDiff_pxls01[t0_rXc];
	bgrPxl e0_imgDiff_pxls12[t0_rXc];
	
	bgrPxlF e0_imgDivPxlf_01[ t0_rXc ];
	bgrPxlF e0_imgDivPxlf_12[ t0_rXc ];
	
	bgrPxl e0_imgMulPxl_01[ t0_rXc ];
	bgrPxl e0_imgMulPxl_12[ t0_rXc ];
	
	for (int y=0; y < t0_rows; y++)
	{
		for (int x=0; x < t0_cols; x++)
		{
			e0_imgDiff_mats01[y*t0_cols + x].blue = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
			e0_imgDiff_mats01[y*t0_cols + x].green = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
			e0_imgDiff_mats01[y*t0_cols + x].red = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
					
			e0_imgDiff_mats12[y*t0_cols + x].blue = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			e0_imgDiff_mats12[y*t0_cols + x].green = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			e0_imgDiff_mats12[y*t0_cols + x].red = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			
			
			e0_imgDiff_pxls01[y*t0_cols + x].blue = 
				t0_imgPxls_0[0][y*t0_cols+x].blue - t0_imgPxls_0[0][y*t0_cols+x].blue;
			e0_imgDiff_pxls01[y*t0_cols + x].green = 
				t0_imgPxls_0[1][y*t0_cols+x].green - t0_imgPxls_0[0][y*t0_cols+x].green;
			e0_imgDiff_mats01[y*t0_cols + x].red = 
				t0_imgPxls_0[1][y*t0_cols+x].red - t0_imgPxls_0[0][y*t0_cols+x].red;
					
			e0_imgDiff_pxls12[y*t0_cols + x].blue = 
				t0_imgPxls_0[2][y*t0_cols+x].blue - t0_imgPxls_0[1][y*t0_cols+x].blue;
			e0_imgDiff_pxls12[y*t0_cols + x].green = 
				t0_imgPxls_0[2][y*t0_cols+x].green - t0_imgPxls_0[1][y*t0_cols+x].green;
			e0_imgDiff_pxls12[y*t0_cols + x].red = 
				t0_imgPxls_0[2][y*t0_cols+x].red - t0_imgPxls_0[1][y*t0_cols+x].red;
				
				
			e0_imgDivPxlf_01[y*t0_cols + x].blue = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_01[y*t0_cols + x].green = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_01[y*t0_cols + x].red = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
					
			e0_imgDivPxlf_12[y*t0_cols + x].blue = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_12[y*t0_cols + x].green = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_12[y*t0_cols + x].red = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
				
			e0_imgMulPxl_01[y*t0_cols + x].blue = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_01[y*t0_cols + x].green = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_01[y*t0_cols + x].red = 
				mats[1]->at<vec3b>(y,x).val[0] - mats[0]->at<vec3b>(y,x).val[0];
					
			e0_imgDivPxlf_12[y*t0_cols + x].blue = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_12[y*t0_cols + x].green = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
			e0_imgDivPxlf_12[y*t0_cols + x].red = 
				mats[2]->at<vec3b>(y,x).val[0] - mats[1]->at<vec3b>(y,x).val[0];
		}
	}
	
	int *e0_imgDiffs_mats[t0_count-1] = {e0_imgDiff_mats01, e0_imgDiff_mats12};
	int *e0_imgDiffs_pxls[t0_count-1] = {e0_imgDiff_pxls01, e0_imgDiff_pxls12};
	
	
	

	

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














