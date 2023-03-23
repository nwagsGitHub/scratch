#ifndef __scratchfs_h__
#define __scratchfs_h__


typedef struct {
	int blue,
	int green,
	int red
} bgrPxl;

typedef struct {
	float blue,
	float green,
	float red
} bgrPxlF;

int
idx_cmp (	int x, 
			int idx_x, 
			int idx_next, 
			int xs_size, 
			int *xs, 
			int (*compar) (const void*, const void*)	);

int
gt_cmp (const void *a, 
		const void *b	);

int
lt_cmp (const void *a, 
		const void *b	);

int
bgr_sv_idx (int blue,
				int green,
				int red	);

unsigned long *
mats_to_sums (	int size,
					Mat **mats	);
					
unsigned long *
mats_into_sums (	int size,
					Mat **mats,
					unsigned long *sums	);

int *
mat_to_imgSum (	Mat mat	);

int *
mat_to_imgSum (	Mat mat,
				int *imgSum	);

int *
img3_to_imgSum (	int rows,
					int cols,
					bgrPxl *img3	);

int *
img3_into_imgSum (	int rows,
					int cols,
					bgrPxl *img3,
					int *imgSum	);

int **
mats_to_imgSums (	int size,
					Mat *mats	);

int **
mats_to_imgSums (	int size,
					Mat *mats,
					int **imgSums	);

int **
img3s_to_imgSums (	int size,
					int rows,
					int cols,
					bgrPxl **img3s	);

int *
img1s_imgIdx (	int size,
				int rows,
				int cols,
				int (*compar) (const void*, const void*),
				int **img1s	);

bgrPxl **
matsBGR_to_img3Diffs (	int size,
						Mat **mats	);

bgrPxl **
matsBGR_into_img3Diffs (	int size,
							Mat **mats,
							bgrPxl **img3Diffs	);

bgrPxl **
img3s_to_img3Diffs (	int size,
						int rows,
						int cols,
						bgrPxl **img3s	);

bgrPxl **
img3s_into_img3Diffs (	int size,
						int rows,
						int cols,
						bgrPxl **img3s,
						bgrPxl **img3Diffs	);

bgrPxl
img3s_cmp_bgrPxl ( 	int size,
				int rows,
				int cols,
				int (*compar) (const void*, const void*),
				bgr_pxl **img3s	);

bgrPxl *
acc_img3_img3 (	int rows,
			int cols,
			bgrPxl *cum,
			bgrPxl *add	);

bgrPxl *
acc_img3_mat (	bgrPxl *cum,
				Mat mat	);

bgrPxlF*
divf_img3_by_img3_to_img3f (	int rows,
								int cols,
								bgrPxl *end,
								bgrPxl *sor	);

bgrPxlF*
divf_img3_by_img3_into_img3f (	int rows,
								int cols,
								bgrPxl *end,
								bgrPxl *sor,
								bgrPxl *quo	);

bgrPxl *
mul_img3_by_img3f (	int rows,
					int cols,
					bgrPxl *lier,
					bgrPxlF *cand	);

bgrPxl *
mul_img3_by_img3f_to_img3 (	int rows,
							int cols,
							bgrPxl *lier,
							bgrPxlF *cand	);

bgrPxl *
mul_img3_by_img3f_into_img3 (	int rows,
							int cols,
							bgrPxl *lier,
							bgrPxlF *cand,
							bgrPxl *prod	);

Mat
img3_into_mat ( int rows,
				int cols,
				bgrPxl *img3,
				Mat mat	);


#endif

