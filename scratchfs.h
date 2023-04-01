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



int ps_mat_i(const Mat mat);

int ps_imgInt(const int rows,
			const int cols,
			const bgrPxl *imgInt);
			
int ps_imgPxl(const int rows,
			const int cols,
			const bgrPxl *imgPxl);

int ps_imgPxlF(const int rows,
			const int cols,
			const bgrPxlF *imgPxlf);



int
idx_cmp (	const int x, 
		const int idx_x, 
		const int idx_next, 
		const int xs_size, 
		const int *xs, 
		int (*compar) (const void*, const void*)	);

int
cmp_gt (const void *a, 
	const void *b	);

int
cmp_lt (const void *a, 
	const void *b	);

int
bgr_sv_idx (const int blue,
		const int green,
		const int red	);

unsigned long *
mats_to_sums (	const int size,
			const Mat *mats	);
					
unsigned long *
mats_into_sums (	int size,
				const Mat *mats,
				unsigned long *sums	);

int *
mat_to_imgSum (	Mat mat	);

int *
mat_into_imgSum (const Mat mat,
			int *imgSum	);

int *
imgPxl_to_imgSum (	const int rows,
					const int cols,
					const bgrPxl *imgPxl	);

int *
imgPxl_into_imgSum (const int rows,
				const int cols,
				const bgrPxl *imgPxl,
				int *imgSum	);

int **
mats_to_imgSums (	const int size,
				const Mat *mats	);

int **
mats_into_imgSums (	const int size,
				const Mat *mats,
				int **imgSums	);

int **
imgPxls_to_imgSums (const int size,
				const int rows,
				const int cols,
				const bgrPxl **imgPxls	);

int **
imgPxls_to_imgSums (const int size,
				const int rows,
				const int cols,
				bgrPxl **imgPxls	);

int **
imgPxls_into_imgSums (	const int size,
					const int rows,
					const int cols,
					bgrPxl **imgPxls,
					int **imgSums	);

int *
imgInts_imgIdx (const int size,
			const int rows,
			const int cols,
			int (*compar) (const void*, const void*),
			const int **imgInts	);

bgrPxl **
matsBGR_to_imgPxlDiffs (const int size,
					const Mat *mats	);

bgrPxl **
matsBGR_into_imgPxlDiffs (	const int size,
						const Mat *mats,
						bgrPxl **imgPxlDiffs	);

bgrPxl **
imgPxls_to_imgPxlDiffs (const int size,
					const int rows,
					const int cols,
					bgrPxl **imgPxls	);

bgrPxl **
imgPxls_into_imgPxlDiffs (	const int size,
						const int rows,
						const int cols,
						bgrPxl **imgPxls,
						bgrPxl **imgPxlDiffs	);

bgrPxl
imgPxls_cmp_bgrPxl (const int size,
				const int rows,
				const int cols,
				int (*compar) (const void*, const void*),
				bgrPxl **imgPxls	);

bgrPxl *
acc_imgPxl_imgPxl (	const int rows,
				const int cols,
				bgrPxl *cum,
				const bgrPxl *add	);

bgrPxl *
acc_imgPxl_mat (bgrPxl *cum,
			const Mat mat	);

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
mul_imgPxl_by_imgPxlf (	const int rows,
					const int cols,
					bgrPxl *lier,
					const bgrPxlF *cand	);

bgrPxl *
mul_imgPxl_by_imgPxlf_to_imgPxl (	const int rows,
								const int cols,
								const bgrPxl *lier,
								const bgrPxlF *cand	);

bgrPxl *
mul_imgPxl_by_imgPxlf_into_imgPxl (	const int rows,
								const int cols,
								const bgrPxl *lier,
								const bgrPxlF *cand,
								bgrPxl *prod	);

Mat
imgPxl_into_mat (	const int rows,
				const int cols,
				const bgrPxl *imgPxl,
				Mat mat	);


#endif

