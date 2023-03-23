#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "dbg.h"
#include "minunit.h"

#include "scratchfs.h"

/*
bgrPxl *img3_input1, *img3_input2, *img3_expected1;
bgrPxlF *img3F_input1, *img3F_expected1;
bgrPxl **img3s_input1, **img3s_expected1;
Mat mat_input1, mat_expected;
Mat *mats_input1;
int *img1_input1, *img1_input2;
*/

int test_int;
float test_float;

int count_t = 3;
int rows_t = 3;
int cols_t = 4;
int rxc_t = rows_t * cols_t;

int lesser_int = 1;
int greater_int = lesser_int + 1;



int img1_input1[5] = {  1,  2,  3,  4,  5 };
int img1_input2[5] = {  4,  0,  1,  4,  3 };

Mat mat_8u = Mat::ones(rows_t, cols_t, CV_8U);
Mat mat_16u = Mat::ones(rows_t, cols_t, CV_16U)*2;
Mat mat_32u = Mat::ones(rows_t, cols_t, CV_32U)*100;
Mat mats[3] = {mat_8u, mat_16u, mat_32u};


/*

idx_cmp
	gt_cmp
	lt_cmp
bgr_sv_idx
mats_to_sums
mats_into_sums
mat_to_imgSum
mat_into_imgSum
img3_to_imgSum
img3_into_imgSum
mats_to_imgSums
mats_into_imgSums
img3s_to_imgSums
img3s_into_imgSums
img1s_imgIdx
matsBGR_to_img3Diffs
matsBGR_into_img3Diffs
img3s_to_img3Diffs
img3s_into_img3Diffs
img3s_cmp_bgrPxl
acc_img3_img3
acc_img3_mat
divf_img3_by_img3_to_img3f
divf_img3_by_img3_into_img3f
mul_img3_by_img3f
mul_img3_by_img3f_to_img3
mul_img3_by_img3f_into_img3
img3_into_mat

*/


char *
test_gt_cmp()
{
	mu_assert( gt_cmp(greater_int, lesser_int), "should be a > b, but not");
	mu_assert( !gt_cmp(lesser_int, greater_int), "should be a < b, but not"); 
}


char *
test_lt_cmp()
{
	mu_assert( lt_cmp(lesser_int, greater_int), "should be a < b, but not");
	mu_assert( !lt_cmp(greater_int, lesser_int), "should be a > b, but not");
}


char *
test_idx_cmp()
{
	int *img1_t = &img1_input1[1];
	int int_t = img1_input1[0];
	int int_r = idx_cmp(int_t, 0, 1, 4, img1_t, gt_cmp);
	int int_e = 4;
	mu_assert( int_e == int_r, "gt_cmp, already ordered");
	
	int int_r = idx_cmp(int_t, 0, 1, 4, img1_t, lt_cmp);
	int_e = 0;
	mu_assert( int_e == int_r, "lt_cmp, already ordered");
	
	int_r = idx_cmp(int_t, 0, 3, 4, img1_t, gt_cmp);
	int_e = 6;
	mu_assert( int_e == int_r, "gt_cmp, idx_next, already ordered");
	
	int_r = idx_cmp(int_t, 0, 3, 4, img1_t, lt_cmp);
	int_e = 0;
	mu_assert( int_e == int_r, "lt_cmp, idx_next, already ordered");
	
	img1_t = &img1_input2[1];
	int_t = img1_input2[0];
	int_r = idx_cmp(int_t, 0, 1, 4, img1_t, gt_cmp);
	int_e = 3;
	mu_assert( int_e == int_r, "gt_cmp, x high, same as val at idx 3");
	
	int_r = idx_cmp(int_t, 0, 1, 4, img1_t, lt_cmp);
	int_e = 1;
	mu_assert( int_e == int_r, "lt_cmp, val at idx 1 low");
	
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
	unsigned long int sum_e1 = rows_t * cols_t;
	unsigned long int sum_e2 = rows_t * cols_t * 2;
	unsigned long int sum_e3 = rows_t * cols_t * 100;
	unsigned long int sums_e[3] = {sum_e1, sum_e2, sum_e3};
	unsigned long int *sums_r = mats_to_sums(3, mats_ones);
	mu_assert( !memcmp(sums_e, sums_r), "sums mismatch");
	
	free(sums_r);
	
	return NULL;
}


char *
test_mats_into_sums()
{
	
	return NULL;
}

char *
test_mat_to_imgSum()
{
	int int_e1[rows*cols];
	for (int i=0; i < rows*cols; i++)
	{
		int_e1[i] = 3;
	}
	
	int *imgSum = mat_to_imgSum(mat_8u);
	
	mu_assert( !memcmp(int_e1,imgSum), "imgSum mismatch");
	
	free(imgSum);
	
	return NULL;
}


char *
test_mat_into_imgSum()
{
	int int_e1[rows*cols];
	for (int i=0; i < rows*cols; i++)
	{
		int_e1[i] = 6;
	}
	
	int *imgSum1 = (int *) calloc( rows * cols, sizeof(int) );
	mat_into_imgSum(mat_16u, imgSum1);
	
	mu_assert( !memcmp(int_e1,imgSum1), "imgSum mismatch");
	
	int *imgSum2 = mat_to_imgSum(mat_16u);
	mu_assert( !memcmp(imgSum1, imgSum2), "imgSum1 <> imgSum2");
	
	
	free(imgSum1);
	free(imgSum2);
}


char *
test_img3_to_imgSum()
{

}


char *
test_img3_into_imgSum()
{

}


char *
test_mats_to_imgSums()
{

}


char *
test_mats_into_imgSums()
{

}


char *
test_img3s_to_imgSums()
{

}

char *
test_img3s_into_imgSums()
{

}


char *
test_img1s_imgIdx()
{

}


char *
test_matsBGR_to_img3Diffs()
{

}

char *
test_matsBGR_into_img3Diffs()
{

}

char *
test_img3_to_img3Diffs()
{

}

char *
test_img3_into_img3Diffs()
{

}


char *
test_img3s_cmp_bgrPxl()
{

}


char *
test_acc_img3_img3()
{

}


char *
test_acc_img3_mat()
{

}


char *
test_divf_img3_by_img3_to_img3f()
{

}

char *
test_divf_img3_by_img3_into_img3f()
{

}


char *
test_mul_img3_by_img3f()
{

}


char *
test_mul_img3_by_img3f_to_img3()
{

}

char *
test_mul_img3_by_img3f_into_img3()
{

}

char *
test_img3_into_mat()
{

}


char *
all_tests()
{
	int *img1_input1 = (int *) calloc( count_t, sizeof(int) );
	int *img1_expected1 = (int *) calloc( count_t, sizeof(int) );

	for (int i=0; i < count_t; i++) {
		img1_expected1 = i;
	}
	
	

	mu_suite_start();
	
	mu_run_test(test_gt_cmp);
	mu_run_test(test_lt_cmp);
	mu_run_test(test_idx_cmp);
	mu_run_test(test_bgr_sv_idx);
	mu_run_test(test_mats_to_sums);
	mu_run_test(test_mats_into_sums);
	mu_run_test(test_mat_to_imgSum);
	mu_run_test(test_mat_into_imgSum);
	mu_run_test(test_img3_to_imgSum);
	mu_run_test(test_img3_into_imgSum);
	mu_run_test(test_mats_to_imgSums);
	mu_run_test(test_mats_into_imgSums);
	mu_run_test(test_img3s_to_imgSums);
	mu_run_test(test_img3s_into_imgSums);
	mu_run_test(test_img1s_imgIdx);
	mu_run_test(test_matsBGR_to_img3Diffs);
	mu_run_test(test_matsBGR_into_img3Diffs);
	mu_run_test(test_img3s_to_img3Diffs);
	mu_run_test(test_img3s_into_img3Diffs);
	mu_run_test(test_img3s_cmp_bgrPxl);
	mu_run_test(test_acc_img3_img3);
	mu_run_test(test_acc_img3_mat);
	mu_run_test(test_divf_img3_by_img3_to_img3f);
	mu_run_test(test_divf_img3_by_img3_into_img3f);
	mu_run_test(test_mul_img3_by_img3f);
	mu_run_test(test_mul_img3_by_img3f_to_img3);
	mu_run_test(test_mul_img3_by_img3f_into_img3);
	mu_run_test(test_img3_into_mat);
	
	free(img1_expected1);
	
	return NULL;
}


RUN_TESTS(all_tests);














