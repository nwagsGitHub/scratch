#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "scratchfs.h"


int ps_mat_i(const Mat mat) {
	int chans = mat.channels();
	int rows = mat.rows;
	int cols = mat.cols;
	
	int y_lim = 2;
	int x_lim = 2;
	
	int y_dots, x_dots;
	y_dots = 0;
	printf("MAT| rows: %d, cols: %d, channels: %d\n", rows, cols, chans);
	for (int y=0; y < rows; y++)
	{
		if (y < y_lim || (rows - y) < y_lim+1)
		{	
			x_dots = 0;
			for (int x=0; x < cols; x++)
			{
				if (x < x_lim || (cols - x) < x_lim+1)
				{	
					printf(" (");
					for (int c=0; c < chans; c++)
					{
						printf(" %i", mat.at<Vec3b>(y,x).val[c] );
						if (c < chans-1)
							printf(",");
					}
					printf(") ");//(%d,%d) ",y,x);
				} else {
					if (x_dots < 3) 
					{
						printf("%c", '.');
						x_dots++;
					}
				}
			}
		} else {
			if (y_dots < 3) 
			{
				printf("%c", '.');
				y_dots++;
			}
		}
		printf("\n");
	}
	
	return 0;
}

int ps_imgInt(const int rows,
			const int cols,
			const int *imgInt) {
	
	int y_lim = 2;
	int x_lim = 2;
	
	int y_dots, x_dots;
	y_dots = 0;
	printf("imgInt| rows: %d, cols: %d\n", rows, cols);
	for (int y=0; y < rows; y++)
	{
		if (y < y_lim || (rows - y) < y_lim+1)
		{	
			x_dots = 0;
			for (int x=0; x < cols; x++)
			{
				if (x < x_lim || (cols - x) < x_lim+1)
				{	
					printf(" ( %i) ", imgInt[y*cols + x]);
				} else {
					if (x_dots < 3) 
					{
						printf("%c", '.');
						x_dots++;
					}
				}
			}
		} else {
			if (y_dots < 3) 
			{
				printf("%c", '.');
				y_dots++;
			}
		}
		printf("\n");
	}
	
	return 0;
}

int ps_imgPxl(const int rows,
			const int cols,
			const bgrPxl *imgPxl) {
	
	int y_lim = 2;
	int x_lim = 2;
	
	int y_dots, x_dots;
	y_dots = 0;
	printf("imgPxl| rows: %d, cols: %d\n", rows, cols);
	for (int y=0; y < rows; y++)
	{
		if (y < y_lim || (rows - y) < y_lim+1)
		{	
			x_dots = 0;
			for (int x=0; x < cols; x++)
			{
				if (x < x_lim || (cols - x) < x_lim+1)
				{	
					printf(" ( %i, %i, %i) ", 
						imgPxl[y*cols + x].blue, 
						imgPxl[y*cols + x].green,
						imgPxl[y*cols + x].red);
				} else {
					if (x_dots < 3) 
					{
						printf("%c", '.');
						x_dots++;
					}
				}
			}
		} else {
			if (y_dots < 3) 
			{
				printf("%c", '.');
				y_dots++;
			}
		}
		printf("\n");
	}
	
	return 0;
}

int ps_imgPxlF(const int rows,
			const int cols,
			const bgrPxlF *imgPxlf) {
	
	int y_lim = 2;
	int x_lim = 2;
	
	int y_dots, x_dots;
	y_dots = 0;
	printf("imgPxl| rows: %d, cols: %d\n", rows, cols);
	for (int y=0; y < rows; y++)
	{
		if (y < y_lim || (rows - y) < y_lim+1)
		{	
			x_dots = 0;
			for (int x=0; x < cols; x++)
			{
				if (x < x_lim || (cols - x) < x_lim+1)
				{	
					printf(" ( %f, %f, %f) ", 
						imgPxlf[y*cols + x].blue, 
						imgPxlf[y*cols + x].green,
						imgPxlf[y*cols + x].red);
				} else {
					if (x_dots < 3) 
					{
						printf("%c", '.');
						x_dots++;
					}
				}
			}
		} else {
			if (y_dots < 3) 
			{
				printf("%c", '.');
				y_dots++;
			}
		}
		printf("\n");
	}
	
	return 0;
}




int 
idx_cmp (	const int x, 
		const int idx_x, 
		const int idx_next, 
		const int xs_size, 
		const int *xs, 
		int (*compar) (const void*, const void*)	)
{
	int next_x = xs[0];
	if (xs_size==1)
		return compar(&x, &next_x) ? idx_x : idx_next;
	
	else
	{
		if ( compar(&x, &next_x) )
			return idx_cmp (    x,     idx_x, idx_next+1, xs_size-1, xs+1, compar);
		else
			return idx_cmp (next_x, idx_next, idx_next+1, xs_size-1, xs+1, compar);
	}
}



int 
cmp_gt (const void *a, 
	const void *b	)
{
	return ( *(int *)a > *(int *)b );
}



int 
cmp_lt (const void *a,
	const void *b	)
{
	return ( *(int *)a < *(int *)b );
}



int
bgr_sv_idx (const int blue,
		const int green,
		const int red	)
{
	int colors[5] = {	green + green - ( blue + red  ),
						red   + red   - ( blue + green),
						blue  + green - (  red + red  ),
						blue  + red   - (green + green),
						green + red   - ( blue + blue )	};
						
	return idx_cmp( blue+blue-(green+red), 0, 1, 5, colors, cmp_gt );
}



unsigned long *
mats_to_sums (	const int size,
			const Mat *mats	)
{
	int chans = mats->channels();
	int cols = mats->cols;
	int rows = mats->rows;
	unsigned long *sums = (unsigned long *) calloc( size, sizeof(unsigned long) );
	unsigned long sum = 0;

	for (int i=0; i < size; i++)
	{
		sum = 0;
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				for (int c=0; c < chans; c++)
				{
					sum += (mats+i)->at<Vec3b>(y,x).val[c];
				}
			}
		}
		*(sums+i) = sum;
	}
	
	return sums;
}


unsigned long *
mats_into_sums (const int size,
			const Mat *mats,
			unsigned long *sums	)
{
	int chans = mats->channels();
	int cols = mats->cols;
	int rows = mats->rows;
	unsigned long sum = 0;	

	for (int i=0; i < size; i++)
	{
		sum = 0;
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				for (int c=0; c < chans; c++)
				{
					sum += (mats+i)->at<Vec3b>(y,x).val[c];
				}
			}
		}
		*(sums+i) = sum;
	}
	
	return sums;
}



int *
mat_to_imgSum (	const Mat mat	)
{
	int chans = mat.channels();
	int cols = mat.cols;
	int rows = mat.rows;
	int *imgSum = (int *) calloc( rows * cols, sizeof(int) );
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			for (int c=0; c < chans; c++)
			{
				*(imgSum+ y*cols +x) += mat.at<Vec3b>(y,x).val[c];
			}
		}
	}
	
	return imgSum;
}

int *
mat_into_imgSum (	const Mat mat,
				int *imgSum	)
{
	int chans = mat.channels();
	int cols = mat.cols;
	int rows = mat.rows;
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			*(imgSum+ y*cols +x) = 0;
			for (int c=0; c < chans; c++)
			{
				*(imgSum+ y*cols +x) += mat.at<Vec3b>(y,x).val[c];
			}
		}
	}
	
	return imgSum;
}



int *
imgPxl_to_imgSum (	const int rows,
				const int cols,
				const bgrPxl *imgPxl	)
{
	int *imgSum = (int *) calloc( rows * cols, sizeof(int) );
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			*(imgSum+ y*cols +x) = (imgPxl+ y*cols +x)->blue +
									(imgPxl+ y*cols +x)->green + 
									(imgPxl+ y*cols +x)->red;
		}
	}
	
	return imgSum;
}


int *
imgPxl_into_imgSum (	const int rows,
					const int cols,
					const bgrPxl *imgPxl,
					int *imgSum	)
{
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			*(imgSum+ y*cols +x) = (imgPxl+ y*cols +x)->blue +
									(imgPxl+ y*cols +x)->green + 
									(imgPxl+ y*cols +x)->red;
		}
	}
	
	return imgSum;
}


int **
mats_to_imgSums (	const int size,
				const Mat *mats	)
{
	int **imgSums = (int **) calloc( size, sizeof(int **) );
	
	for (int i=0; i < size; i++)
	{
		*(imgSums+i) = mat_to_imgSum( *(mats+i) );
	}
	
	return imgSums;
}


int **
mats_into_imgSums (	const int size,
				const Mat *mats,
				int	**imgSums	)
{
	int chans = mats->channels();
	int rows = mats->rows;
	int cols = mats->cols;
	for (int i=0; i < size; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				*(*(imgSums+i)+ y*cols +x) = 0;
				for (int c=0; c < chans; c++)
					*(*(imgSums+i)+ y*cols +x) += mats[i].at<Vec3b>(y,x)[c];
			}
		}
	}
	
	return imgSums;
}



int **
imgPxls_to_imgSums (const int size,
				const int rows,
				const int cols,
				bgrPxl **imgPxls	)
{
	int **imgSums = (int **) calloc( size, sizeof(int **) );
	
	for (int i=0; i < size; i++)
	{
		*(imgSums+i) = imgPxl_to_imgSum( cols, rows, *(imgPxls+i) );
	}
	
	return imgSums;
}


int **
imgPxls_into_imgSums (	const int size,
					const int rows,
					const int cols,
					bgrPxl **imgPxls,
					int **imgSums	)
{	
	for (int i=0; i < size; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				*(*(imgSums+i)+ y*cols +x) = 
					(*(imgPxls+i)+ y*cols +x)->blue +
					(*(imgPxls+i)+ y*cols +x)->green + 
					(*(imgPxls+i)+ y*cols +x)->red;
			}
		}
	}
	
	return imgSums;
}


int *
imgInts_imgIdx (const int size,
			const int rows,
			const int cols,
			int (*compar) (const void*, const void*),
			const int **imgInts	)
{
	int pxls[size-1];
	int *imgIdx = (int *) calloc( size, sizeof(int) );
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			for (int i=1; i < size; i++)
			{
				pxls[i-1] = *(*(imgInts+i)+ y*cols +x);
			}
			*(imgIdx+ y*cols +x) = idx_cmp( *(*imgInts+ y*cols +x), 0, 1, size-1, pxls, compar );
		}
	}
	
	return imgIdx;
}



bgrPxl **
matsBGR_to_imgPxlDiffs (	const int size,
						const Mat *mats	)
{
	int chans = mats->channels();
	int cols = mats->cols;
	int rows = mats->rows;
	bgrPxl **imgPxlDiffs = (bgrPxl **) calloc( size-1, sizeof(bgrPxl *) );
	
	for (int i=0; i < size-1; i++)
	{
		bgrPxl *imgPxlDiff = (bgrPxl *) calloc( cols * rows, sizeof(bgrPxl) );
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(imgPxlDiff+ y*cols +x)->blue = 
					mats[i+1].at<Vec3b>(y,x).val[0] -
					mats[i].at<Vec3b>(y,x).val[0];
					
				(imgPxlDiff+ y*cols +x)->green = 
					mats[i+1].at<Vec3b>(y,x).val[1] -
					mats[i].at<Vec3b>(y,x).val[1];
					
				(imgPxlDiff+ y*cols +x)->red = 
					mats[i+1].at<Vec3b>(y,x).val[2] -
					mats[i].at<Vec3b>(y,x).val[2];
			}
		}
		*(imgPxlDiffs+i) = imgPxlDiff;
	}
	
	return imgPxlDiffs;
}



bgrPxl **
matsBGR_into_imgPxlDiffs (	const int size,
						const Mat *mats,
						bgrPxl **imgPxlDiffs	)
{
	int chans = mats->channels();
	int cols = mats->cols;
	int rows = mats->rows;
	
	for (int i=0; i < size-1; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(*(imgPxlDiffs+i)+ y*cols +x)->blue = 
					mats[i+1].at<Vec3b>(y,x).val[0] -
					mats[i].at<Vec3b>(y,x).val[0];
					
				(*(imgPxlDiffs+i)+ y*cols +x)->green = 
					mats[i+1].at<Vec3b>(y,x).val[1] -
					mats[i].at<Vec3b>(y,x).val[1];
					
				(*(imgPxlDiffs+i)+ y*cols +x)->red = 
					mats[i+1].at<Vec3b>(y,x).val[2] -
					mats[i].at<Vec3b>(y,x).val[2];
			}
		}
	}
	
	return imgPxlDiffs;
}



bgrPxl **
imgPxls_to_imgPxlDiffs (	const int size,
						const int rows,
						const int cols,
						bgrPxl **imgPxls	)
{
	bgrPxl **imgPxlDiffs = (bgrPxl **) calloc( size-1, sizeof(bgrPxl *) );
	
	for (int i=0; i < size-1; i++)
	{
		bgrPxl *imgPxlDiff = (bgrPxl *) calloc( cols * rows, sizeof(bgrPxl) );
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(imgPxlDiff+ y*cols +x)->blue = 
					(*(imgPxls+i+1)+ y*cols +x)->blue -
					(*(imgPxls+i)+ y*cols +x)->blue;
					
				(imgPxlDiff+ y*cols +x)->green = 
					(*(imgPxls+i+1)+ y*cols +x)->green -
					(*(imgPxls+i)+ y*cols +x)->green;
					
				(imgPxlDiff+ y*cols +x)->red = 
					(*(imgPxls+i+1)+ y*cols +x)->red -
					(*(imgPxls+i)+ y*cols +x)->red;
			}
		}
		*(imgPxlDiffs+i) = imgPxlDiff;
	}
	
	return imgPxlDiffs;
}



bgrPxl **
imgPxls_into_imgPxlDiffs (	const int size,
						const int rows,
						const int cols,
						bgrPxl **imgPxls,
						bgrPxl **imgPxlDiffs	)
{
	for (int i=0; i < size-1; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(*(imgPxlDiffs+i)+ y*cols +x)->blue = 
					(*(imgPxls+i+1)+ y*cols +x)->blue -
					(*(imgPxls+i)+ y*cols +x)->blue;
					
				(*(imgPxlDiffs+i)+ y*cols +x)->green = 
					(*(imgPxls+i+1)+ y*cols +x)->green -
					(*(imgPxls+i)+ y*cols +x)->green;
					
				(*(imgPxlDiffs+i)+ y*cols +x)->red = 
					(*(imgPxls+i+1)+ y*cols +x)->red -
					(*(imgPxls+i)+ y*cols +x)->red;
			}
		}
	}
	
	return imgPxlDiffs;
}



bgrPxl
imgPxls_cmp_bgrPxl ( 	const int size,
					const int rows,
					const int cols,
					int (*compar) (const void*, const void*),
					bgrPxl **imgPxls	)
{
	bgrPxl pxl = { (*imgPxls)->blue, (*imgPxls)->green, (*imgPxls)->red };
	for (int i=0; i < size; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				if ( compar( &((*(imgPxls+i)+ y*cols +x)->blue), &(pxl.blue) ) )
					pxl.blue = (*(imgPxls+i)+ y*cols +x)->blue;
				if ( compar( &((*(imgPxls+i)+ y*cols +x)->green), &(pxl.green) ) )
					pxl.green = (*(imgPxls+i)+ y*cols +x)->green;
				if ( compar( &((*(imgPxls+i)+ y*cols +x)->red), &(pxl.red) ) )
					pxl.red = (*(imgPxls+i)+ y*cols +x)->red;
			}
		}
	}
	
	return pxl;
}



bgrPxl *
acc_imgPxl_imgPxl (	const int rows,
				const int cols,
				bgrPxl *cum,
				const bgrPxl *add	)
{
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(cum+ y*cols +x)->blue += (add+ y*cols +x)->blue;
			(cum+ y*cols +x)->green += (add+ y*cols +x)->green;
			(cum+ y*cols +x)->red += (add+ y*cols +x)->red;
		}
	}
	
	return cum;
}



bgrPxl *
acc_imgPxl_mat (bgrPxl *cum,
			const Mat mat	)
{
	int rows = mat.rows;
	int cols = mat.cols;

	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(cum+ y*cols +x)->blue  += mat.at<Vec3b>(y,x).val[0];
			(cum+ y*cols +x)->green += mat.at<Vec3b>(y,x).val[1];
			(cum+ y*cols +x)->red   += mat.at<Vec3b>(y,x).val[2];
		}
	}
	
	return cum;
}



bgrPxlF *
divf_imgPxl_by_imgPxl_to_imgPxlf (	const int rows,
								const int cols,
								const bgrPxl *end,
								const bgrPxl *sor	)
{
	bgrPxlF *imgPxlf = (bgrPxlF *) calloc( rows * cols, sizeof(bgrPxlF) );
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(imgPxlf+ y*cols + x)->blue = 
				(float) (end+ y*cols +x)->blue /
				(float) (sor+ y*cols +x)->blue;
				
			(imgPxlf+ y*cols + x)->green = 
				(float) (end+ y*cols +x)->green /
				(float) (sor+ y*cols +x)->green;
				
			(imgPxlf+ y*cols + x)->red = 
				(float) (end+ y*cols +x)->red /
				(float) (sor+ y*cols +x)->red;
		}
	}
	
	return imgPxlf;
}



bgrPxlF *
divf_imgPxl_by_imgPxl_into_imgPxlf (	const int rows,
									const int cols,
									const bgrPxl *end,
									const bgrPxl *sor,
									bgrPxlF *quo)
{
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(quo+ y*cols + x)->blue = 
				(float) (end+ y*cols +x)->blue /
				(float) (sor+ y*cols +x)->blue;
				
			(quo+ y*cols + x)->green = 
				(float) (end+ y*cols +x)->green /
				(float) (sor+ y*cols +x)->green;
				
			(quo+ y*cols + x)->red = 
				(float) (end+ y*cols +x)->red /
				(float) (sor+ y*cols +x)->red;
		}
	}
	
	return quo;
}



bgrPxl *
mul_imgPxl_by_imgPxlf (	const int rows,
					const int cols,
					bgrPxl *lier,
					const bgrPxlF *cand	)
{
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(lier+ y*cols + x)->blue = 
				(lier+ y*cols +x)->blue *
				(cand+ y*cols +x)->blue;
				
			(lier+ y*cols + x)->green = 
				(lier+ y*cols +x)->green *
				(cand+ y*cols +x)->green;
				
			(lier+ y*cols + x)->red = 
				(lier+ y*cols +x)->red *
				(cand+ y*cols +x)->red;
		}
	}
	
	return lier;
}



bgrPxl *
mul_imgPxl_by_imgPxlf_to_imgPxl (	const int rows,
								const int cols,
								const bgrPxl *lier,
								const bgrPxlF *cand	)
{
	bgrPxl *prod = (bgrPxl *) calloc( rows * cols, sizeof(bgrPxl) );
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(prod+ y*cols + x)->blue = 
				(lier+ y*cols +x)->blue *
				(cand+ y*cols +x)->blue;
				
			(prod+ y*cols + x)->green = 
				(lier+ y*cols +x)->green *
				(cand+ y*cols +x)->green;
				
			(prod+ y*cols + x)->red = 
				(lier+ y*cols +x)->red *
				(cand+ y*cols +x)->red;
		}
	}
	
	return prod;
}



bgrPxl *
mul_imgPxl_by_imgPxlf_into_imgPxl (	const int rows,
								const int cols,
								const bgrPxl *lier,
								const bgrPxlF *cand,
								bgrPxl *prod	)
{
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(prod+ y*cols + x)->blue = 
				(lier+ y*cols +x)->blue *
				(cand+ y*cols +x)->blue;
				
			(prod+ y*cols + x)->green = 
				(lier+ y*cols +x)->green *
				(cand+ y*cols +x)->green;
				
			(prod+ y*cols + x)->red = 
				(lier+ y*cols +x)->red *
				(cand+ y*cols +x)->red;
		}
	}
	
	return prod;
}



Mat
imgPxl_into_mat ( const int rows,
				const int cols,
				const bgrPxl *imgPxl,
				Mat mat	)
{
	for (int y=0; y < rows; y++) 
	{
		for (int x=0; x < cols; x++)
		{
			mat.at<Vec3b>(y,x).val[0] = (imgPxl+ y*cols +x)->blue;
			mat.at<Vec3b>(y,x).val[1] = (imgPxl+ y*cols +x)->green;
			mat.at<Vec3b>(y,x).val[2] = (imgPxl+ y*cols +x)->red;
		}
	}

	return mat;
}
















