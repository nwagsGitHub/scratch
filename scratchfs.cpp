#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "scratchfs.h"




int 
idx_cmp (	int x, 
		int idx_x, 
		int idx_next, 
		int xs_size, 
		int *xs, 
		int (*compar) (const void*, const void*)	)
{
	int next_x = xs[0];
	if (xs_size==1)
		return compar(x, next_x) ? idx_x : idx_next;
	else
	{
		if ( compar(x, next_x) )
			return idx_cmp (    x,     idx_x, idx_next+1, xs_size-1, xs+1, compar);
		else
			return idx_cmp (next_x, idx_next, idx_next+1, xs_size-1, xs+1, compar);
	}
}



int 
gt_cmp (const void *a, 
	const void *b	)
{
	return ( *(int *)a > *(int *)b );
}



int 
lt_cmp (const void *a,
	const void *b	)
{
	return ( *(int *)a < *(int *)b );
}



int
bgr_sv_idx (int blue,
		int green,
		int red	)
{
	int colors[5] = {	green + green - ( blue + red  ),
						red   + red   - ( blue + green),
						blue  + green - (  red + red  ),
						blue  + red   - (green + green),
						green + red   - ( blue + blue )	};
						
	return idx_cmp( blue+blue-(green+red), 0, 1, 5, colors, gt_cmp );
}



unsigned long *
mats_to_sums (	int size,
			Mat **mats	)
{
	int chans = mats->channels;
	int cols = mats->cols;
	int rows = mats->rows;
	unsigned long *sums = (unsigned long *) calloc( rows * cols, sizeof(unsigned long) );
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
mats_into_sums (	int size,
				Mat **mats,
				unsigned long *sums	)
{
	int chans = mats->channels;
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
mat_to_imgSum (	Mat mat	)
{
	int chans = mat->channels;
	int cols = mat->cols;
	int rows = mat->rows;
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
mat_into_imgSum (	Mat mat,
				int *imgSum	)
{
	int chans = mat->channels;
	int cols = mat->cols;
	int rows = mat->rows;
	
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
img3_to_imgSum (	int rows,
				int cols,
				bgrPxl *img3	)
{
	int *imgSum = (int *) calloc( rows * cols, sizeof(int) );
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			*(imgSum+ y*cols +x) = (img3+ y*cols +x)->blue +
									(img3+ y*cols +x)->green + 
									(img3+ y*cols +x)->red;
		}
	}
	
	return imgSum;
}


int *
img3_into_imgSum (	int rows,
				int cols,
				bgrPxl *img3	
				int *imgSum	)
{
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			*(imgSum+ y*cols +x) = (img3+ y*cols +x)->blue +
									(img3+ y*cols +x)->green + 
									(img3+ y*cols +x)->red;
		}
	}
	
	return imgSum;
}


int **
mats_to_imgSums (	int size,
				Mat *mats	)
{
	int **imgSums = (int **) calloc( size, sizeof(int **) );
	
	for (int i=0; i < size; i++)
	{
		*(imgSums+i) = mat_to_imgSum( *(mats+i) );
	}
	
	return imgSums;
}


int **
mats_into_imgSums (	int size,
				Mat *mats,
				int	**imgSums	)
{
	for (int i=0; i < size; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				*(*(imgSums+i)+ y*cols +x) = 
					(img3+ y*cols +x)->blue +
					(img3+ y*cols +x)->green + 
					(img3+ y*cols +x)->red;
			}
		}
	}
	
	return imgSums;
}



int **
img3s_to_imgSums (	int size,
				int rows,
				int cols,
				bgrPxl **img3s	)
{
	int **imgSums = (int **) calloc( size, sizeof(int **) );
	
	for (int i=0; i < size; i++)
	{
		*(imgSums+i) = img3_to_imgSum( cols, rows, *(img3s+i) );
	}
	
	return imgSums;
}


int **
img3s_into_imgSums (	int size,
					int rows,
					int cols,
					bgrPxl **img3s,
					int **imgSums	)
{	
	for (int i=0; i < size; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				*(*(imgSums+i)+ y*cols +x) = 
					(*(img3s+i)+ y*cols +x)->blue +
					(*(img3s+i)+ y*cols +x)->green + 
					(*(img3s+i)+ y*cols +x)->red;
			}
		}
	}
	
	return imgSums;
}


int *
img1s_imgIdx (	int size,
			int rows,
			int cols,
			int (*compar) (const void*, const void*),
			int **img1s	)
{
	int pxls[size-1];
	int *imgIdx = (int *) calloc( size, sizeof(int) );
	
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			for (int i=1; i < size; i++)
			{
				pxls[i-1] = *(*(img1s+i)+ y*cols +x);
			}
			*(imgIdx+ y*cols +x) = idx_cmp( *(*img1s+ y*cols +x), 0, 1, size-1, pxls, compar );
		}
	}
	
	return imgIdx;
}



bgrPxl **
matsBGR_to_img3Diffs (	int size,
					Mat **mats	)
{
	int chans = mats->channels;
	int cols = mats->cols;
	int rows = mats->rows;
	bgrPxl **img3Diffs = (bgrPxl **) calloc( size-1, sizeof(bgrPxl *) );
	
	for (int i=0; i < size-1; i++)
	{
		bgrPxl *img3Diff = (bgrPxl *) calloc( cols * rows, sizeof(bgrPxl) );
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(img3Diff+ y*cols +x)->blue = 
					mats[i+1]->at<Vec3b>(y,x).val[0] -
					mats[i]->at<Vec3b>(y,x).val[0];
					
				(img3Diff+ y*cols +x)->green = 
					mats[i+1]->at<Vec3b>(y,x).val[1] -
					mats[i]->at<Vec3b>(y,x).val[1];
					
				(img3Diff+ y*cols +x)->red = 
					mats[i+1]->at<Vec3b>(y,x).val[2] -
					mats[i]->at<Vec3b>(y,x).val[2];
			}
		}
		*(img3Diffs+i) = img3Diff;
	}
	
	return img3Diffs;
}



bgrPxl **
matsBGR_into_img3Diffs (	int size,
						Mat **mats,
						bgrPxl **img3Diffs	)
{
	int chans = mats->channels;
	int cols = mats->cols;
	int rows = mats->rows;
	
	for (int i=0; i < size-1; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(*(img3Diffs+i)+ y*cols +x)->blue = 
					mats[i+1]->at<Vec3b>(y,x).val[0] -
					mats[i]->at<Vec3b>(y,x).val[0];
					
				(*(img3Diffs+i)+ y*cols +x)->green = 
					mats[i+1]->at<Vec3b>(y,x).val[1] -
					mats[i]->at<Vec3b>(y,x).val[1];
					
				(*(img3Diffs+i)+ y*cols +x)->red = 
					mats[i+1]->at<Vec3b>(y,x).val[2] -
					mats[i]->at<Vec3b>(y,x).val[2];
			}
		}
	}
	
	return img3Diffs;
}



bgrPxl **
img3s_to_img3Diffs (	int size,
					int rows,
					int cols,
					bgrPxl **img3s	)
{
	bgrPxl **img3Diffs = (bgrPxl **) calloc( size-1, sizeof(bgrPxl *) );
	
	for (int i=0; i < size-1; i++)
	{
		bgrPxl *img3Diff = (bgrPxl *) calloc( cols * rows, sizeof(bgrPxl) );
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(img3Diff+ y*cols +x)->blue = 
					(*(img3s+i+1)+ y*cols +x)->blue -
					(*(img3s+i)+ y*cols +x)->blue;
					
				(img3Diff+ y*cols +x)->green = 
					(*(img3s+i+1)+ y*cols +x)->green -
					(*(img3s+i)+ y*cols +x)->green;
					
				(img3Diff+ y*cols +x)->red = 
					(*(img3s+i+1)+ y*cols +x)->red -
					(*(img3s+i)+ y*cols +x)->red;
			}
		}
		*(img3Diffs+i) = img3Diff;
	}
	
	return img3Diffs;
}



bgrPxl **
img3s_into_img3Diffs (	int size,
					int rows,
					int cols,
					bgrPxl **img3s,
					bgrPxl **img3Diffs	)
{
	for (int i=0; i < size-1; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(*(img3Diffs+i)+ y*cols +x)->blue = 
					(*(img3s+i+1)+ y*cols +x)->blue -
					(*(img3s+i)+ y*cols +x)->blue;
					
				(*(img3Diffs+i)+ y*cols +x)->green = 
					(*(img3s+i+1)+ y*cols +x)->green -
					(*(img3s+i)+ y*cols +x)->green;
					
				(*(img3Diffs+i)+ y*cols +x)->red = 
					(*(img3s+i+1)+ y*cols +x)->red -
					(*(img3s+i)+ y*cols +x)->red;
			}
		}
	}
	
	return img3Diffs;
}



bgrPxl
img3s_cmp_bgrPxl ( 	int size,
				int rows,
				int cols,
				int (*compar) (const void*, const void*),
				bgr_pxl **img3s	)
{
	bgrPxl pxl = {0,0,0};
	for (int i=0; i < size; i++)
	{
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				if ( compar( (*(img3s+i)+ y*cols +x)->blue, pxl.blue) )
					pxl.blue = (*(img3s+i)+ y*cols +x)->blue;
				if ( compar( (*(img3s+i)+ y*cols +x)->green, pxl.green) )
					pxl.green = (*(img3s+i)+ y*cols +x)->green;
				if ( compar( (*(img3s+i)+ y*cols +x)->red, pxl.red) )
					pxl.red = (*(img3s+i)+ y*cols +x)->red;
			}
		}
	}
	
	return pxl;
}



bgrPxl *
acc_img3_img3 (	int rows,
			int cols,
			bgrPxl *cum,
			bgrPxl *add	)
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
acc_img3_mat (	bgrPxl *cum,
			Mat mat	)
{
	int rows = mat.rows;
	int cols = mat.rows;
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(cum+ y*cols +x)->blue += mat.at<Vec3b>(y,x).val[0];
			(cum+ y*cols +x)->green += mat.at<Vec3b>(y,x).val[1];
			(cum+ y*cols +x)->red += mat.at<Vec3b>(y,x).val[2];
		}
	}
	
	return cum;
}



bgrPxlF *
divf_img3_by_img3_to_img3f (	int rows,
							int cols,
							bgrPxl *end,
							bgrPxl *sor	)
{
	bgrPxlF *img3f = (bgrPxlF *) calloc( rows * cols, sizeof(bgrPxlF) );
	for (int y=0; y < rows; y++)
	{
		for (int x=0; x < cols; x++)
		{
			(img3f+ y*cols + x)->blue = 
				(float) (end+ y*cols +x)->blue /
				(float) (sor+ y*cols +x)->blue;
				
			(img3f+ y*cols + x)->green = 
				(float) (end+ y*cols +x)->green /
				(float) (sor+ y*cols +x)->green;
				
			(img3f+ y*cols + x)->red = 
				(float) (end+ y*cols +x)->red /
				(float) (sor+ y*cols +x)->red;
		}
	}
	
	return img3f;
}



bgrPxlF *
divf_img3_by_img3_into_img3f (	int rows,
							int cols,
							bgrPxl *end,
							bgrPxl *sor,
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
mul_img3_by_img3f (	int rows,
				int cols,
				bgrPxl *lier,
				bgrPxlF *cand	)
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
mul_img3_by_img3f_to_img3 (	int rows,
						int cols,
						bgrPxl *lier,
						bgrPxlF *cand	)
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
mul_img3_by_img3f_into_img3 (	int rows,
							int cols,
							bgrPxl *lier,
							bgrPxlF *cand,
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
img3_into_mat ( int rows,
			int cols,
			bgrPxl *img3,
			Mat mat	)
{
	for (int y=0; y < rows; y++) 
	{
		for (int x=0; x < cols; x++)
		{
			mat.at<Vec3b>[0] = (img3+ y*cols +x)->blue;
			mat.at<Vec3b>[1] = (img3+ y*cols +x)->green;
			mat.at<Vec3b>[2] = (img3+ y*cols +x)->red;
		}
	}

	return mat;
}
















