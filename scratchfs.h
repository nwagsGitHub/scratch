#ifndef __scratchfs_h__
#define __scratchfs_h__



using namespace cv;

typedef struct {
	int blue;
	int green;
	int red;
} bgrPxl;

typedef struct {
	float blue;
	float green;
	float red;
} bgrPxlF;

int
idx_cmp (	int x, 
		int idx_x, 
		int idx_next, 
		int xs_size, 
		const int *xs, 
		int (*compar) (const void*, const void*)	);

int
cmp_gt (const void *a, 
	const void *b	);

int
cmp_lt (const void *a, 
	const void *b	);

int
bgr_sv_idx (int blue,
		int green,
		int red	);

unsigned long *
mats_to_sums (	int size,
			const Mat *mats	);
					
unsigned long *
mats_into_sums (	int size,
				const Mat *mats,
				unsigned long *sums	);

int *
mat_to_imgSum (	Mat mat	);

int *
mat_into_imgSum (	Mat mat,
			int *imgSum	);

int *
imgPxl_to_imgSum (	int rows,
					int cols,
					bgrPxl *imgPxl	);

int *
imgPxl_into_imgSum (	int rows,
					int cols,
					bgrPxl *imgPxl,
					int *imgSum	);

int **
mats_to_imgSums (	int size,
				const Mat *mats	);

int **
mats_into_imgSums (	int size,
				const Mat *mats,
				int **imgSums	);

int **
imgPxls_to_imgSums (	int size,
					int rows,
					int cols,
					bgrPxl **imgPxls	);

int **
imgPxls_to_imgSums (	int size,
					int rows,
					int cols,
					bgrPxl **imgPxls	);

int **
imgPxls_into_imgSums (	int size,
					int rows,
					int cols,
					bgrPxl **imgPxls,
					int **imgSums	);

int *
imgInts_imgIdx (	int size,
				int rows,
				int cols,
				int (*compar) (const void*, const void*),
				const int **imgInts	);

bgrPxl **
matsBGR_to_imgPxlDiffs (	int size,
						const Mat *mats	);

bgrPxl **
matsBGR_into_imgPxlDiffs (	int size,
						const Mat *mats,
						bgrPxl **imgPxlDiffs	);

bgrPxl **
imgPxls_to_imgPxlDiffs (	int size,
						int rows,
						int cols,
						bgrPxl **imgPxls	);

bgrPxl **
imgPxls_into_imgPxlDiffs (	int size,
						int rows,
						int cols,
						bgrPxl **imgPxls,
						bgrPxl **imgPxlDiffs	);

bgrPxl
imgPxls_cmp_bgrPxl ( 	int size,
					int rows,
					int cols,
					int (*compar) (const void*, const void*),
					bgrPxl **imgPxls	);

bgrPxl *
acc_imgPxl_imgPxl (	int rows,
				int cols,
				bgrPxl *cum,
				bgrPxl *add	);

bgrPxl *
acc_imgPxl_mat (	bgrPxl *cum,
				Mat mat	);

bgrPxlF*
divf_imgPxl_by_imgPxl_to_imgPxlf (	int rows,
								const int cols,
								const bgrPxl *end,
								const bgrPxl *sor	);

bgrPxlF*
divf_imgPxl_by_imgPxl_into_imgPxlf (	const int rows,
									const int cols,
									const bgrPxl *end,
									const bgrPxl *sor,
									bgrPxlF *quo	);

bgrPxl *
mul_imgPxl_by_imgPxlf (	int rows,
					int cols,
					bgrPxl *lier,
					bgrPxlF *cand	);

bgrPxl *
mul_imgPxl_by_imgPxlf_to_imgPxl (	int rows,
								int cols,
								bgrPxl *lier,
								bgrPxlF *cand	);

bgrPxl *
mul_imgPxl_by_imgPxlf_into_imgPxl (	int rows,
								int cols,
								bgrPxl *lier,
								bgrPxlF *cand,
								bgrPxl *prod	);

Mat
imgPxl_into_mat (	int rows,
				int cols,
				bgrPxl *imgPxl,
				Mat mat	);


#endif

