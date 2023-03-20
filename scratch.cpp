//#include <opencv2/opencv.hpp>
 
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iomanip>

#include <iostream>

#include <cstdarg>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>

#define MAX_IMAGES 5


#define GRAD_M 2
#define LIMIT_ADJ

using namespace cv;


typedef struct {
	unsigned long int tot;
	int rows;
	int cols;
	Mat *image;
} Mat_info;

typedef struct {
	int blue;
	int green;
	int red;
} Pxl_int_info;

typedef struct {
	int rows;
	int cols;
	Pxl_int_info *image;
} Img_info;

typedef struct {
	float blue;
	float green;
	float red;
	int clr_idx;
} Pxl_flt_info;

typedef struct {
	int rows;
	int cols;
	Pxl_flt_info *image;
} Flt_info;

typedef struct {
	float lp;
} Lp_info;

int max_b, max_g, max_r;


int dbg=0;

/*
	MAX	(b+b)-(g+r) , (b+g)-(r+r) , (b+r)-(g+g) , g+g-(b+r) , (g+r)-(b+b) , (r+r)-(b+g)
	
	MAX defined image --> base image
	
	fraction to go from pxl(base) to pxl(max)
	fraction to go from pxl(base) to pxl(min)
*/

int max_idx(int x, int idx_x, int idx_next, int xs_sz, int *xs)
{
	int cur_x = xs[0];
	if (xs_sz == 1)
		return x > cur_x ? idx_x : idx_next;
	else 
	{
		if (x > cur_x)
			return max_idx(x, idx_x, idx_next+1, xs_sz-1, xs+1);
		else
			return max_idx(cur_x, idx_next, idx_next+1, xs_sz-1, xs+1);
	}
}

int max_clr(int b, int g, int r)
{
	int ret = 0;
	
	int bb = b+b;
	int bg = b+g;
	int br = b+r;
	int gg = g+g;
	int gr = g+r;
	int rr = r+r;
	int bb_ = bb-gr; // 0
	int bg_ = bg-rr; // 1
	int br_ = br-gg; // 2
	int gg_ = gg-br; // 3
	int gr_ = gr-bb; // 4
	int rr_ = rr-bg; // 5

/*	
	Bgr		|..
	bGr		.|.
	bgR		..|
	
	BGr		||.
	BgR		|.|
	bGR		.||
	
	BGR		|||
	
	---------------------
	
	|..
	|..
	
	||_ vs _||
	
	bb_	255+255		 			- 			(254+		 253)	= 3
   *bg_	255+	254				- 					(253+253)	= 3		3/3 = 1
   *br_	255+			253		-			(254+254)			= 0
	gg_			254+254			- (255+				 253)		= 0
   *gr_			254+	253 	- (255+255)						= -3
	rr_					253+253	- (255+		 254)				= -3
	
	bb_	255+255		 			- 			(254+		 250)	= 6
   *bg_	255+	254				- 					(250+250)	= 9		9/6 = 1.5
   *br_	255+			250		-			(254+254)			= -3
	gg_			254+254			- (255+				 250)		= 3
   *gr_			254+	250 	- (255+255)						= -6
	rr_					250+250	- (255+		 254)				= -9	
	
	bb_	255+255		 			- 			(254+		 200)	= 56
   *bg_	255+	254				- 					(200+200)	= 109	109/56 = 1.95
   *br_	255+			200		-			(254+254)			= -53
	gg_			254+254			- (255+				 200)		= 53
   *gr_			254+	200 	- (255+255)						= -56
	rr_					200+200	- (255+		 254)				= -109
	
		bb_	255+255		 			- 			(254+	  50)	= 206
   *bg_	255+	254				- 					( 50+ 50)	= 409	409/206 = 1.99
   *br_	255+			 50		-			(254+254)			= -203
	gg_			254+254			- (255+				  50)		= 203
   *gr_			254+	 50 	- (255+255)						= -206
	rr_					 50+ 50	- (255+		 254)				= -409
	
	bb_	255+255		 			- 			(254+	   0)		= 256
   *bg_	255+	254				- 					(  0+  0)	= 509	509/256 = 1.99
   *br_	255+			  0		-			(254+254)			= -253
	gg_			254+254			- (255+				   0)		= 253
   *gr_			254+	  0 	- (255+255)						= -256
	rr_					  0+  0	- (255+		 254)				= -509
	
	
	
	
	
*/	
	
	
	

	int colors[5] = { bg_, br_, gg_, gr_, rr_ };
	return max_idx(bb_, 0, 1, 5, colors);
}


int ii_cmp(const void *v1, const void *v2)
{
	Mat_info *i1 = *(Mat_info **) v1;
	Mat_info *i2 = *(Mat_info **) v2;
	
	if (i1->tot < i2->tot)
		return -1;
	if (i1->tot > i2->tot)
		return 1;
	return 0;
}

int main(int argc, char *argv[])
{
	printf("GRAD_M: %d\n", GRAD_M);
	
	char wd_path[PATH_MAX];
	getcwd(wd_path, PATH_MAX);
	printf("wd_path: %s\n", wd_path);
	
	
	const char *art_fldr = "artifacts";
	const char *test_jpg = "test.jpg";
	char art_path[PATH_MAX];
	strcpy(art_path, wd_path);
	
	struct dirent *art_ptr;
	
	char const *jpg = ".jpg";
	
	struct dirent *ent_ptr;
	
	
	Mat_info *mat_infos[MAX_IMAGES];
	Mat mats[MAX_IMAGES];
	Mat temp_mat;
	int img_count = 0;
	
	int sharpness_idx;
	double max_sharpness;
	double cur_sharpness;
	
	Img_info *img_infos[MAX_IMAGES-1];
	int rows, cols;
	
	Flt_info flt_info;
	
	max_b = max_g = max_r = 0;
	
	Mat lp_mats[MAX_IMAGES-1];
	
	
	
	strcat(art_path, "/");
	DIR *art_dir_ptr = opendir(strcat(art_path, art_fldr)); 
	DIR *dir_ptr = opendir(wd_path);
	
	
	printf("art_dir_ptr: %s\n", art_path);
	while (art_ptr = readdir(art_dir_ptr) )
	{
		printf("looking at %s\n", art_ptr->d_name);
		printf("\tstrcmp(..) : %d , strcmp(.) : %d\n",
			strcmp("..", art_ptr->d_name),
			strcmp(".", art_ptr->d_name) );
		if (!strcmp("..", art_ptr->d_name) || !strcmp(".", art_ptr->d_name) ) { }
		else
		{
			char w[strlen(art_fldr)+1+strlen(art_ptr->d_name)+1];
			strcpy(w, art_fldr);
			strcat(w, "/");
			strcat(w, art_ptr->d_name);
			printf("\tnot '..' or '.' so removing.\n");
			printf("\tsuccess?: %s\n", remove(w)?"no":"yes");
		}
	}
	closedir(art_dir_ptr);
	
	for (int i=0; i < MAX_IMAGES; i++)
	{
		mat_infos[i] = (Mat_info *) calloc(1, sizeof(Mat_info *));
	}
	
	while ( (img_count < MAX_IMAGES) && (ent_ptr = readdir(dir_ptr)) )
	{
		printf(" checking %s \n", ent_ptr->d_name);
		char *extension = strrchr(ent_ptr->d_name, '.');
		if (img_count < MAX_IMAGES && extension && !strcmp(extension, jpg) )
		{
			printf(" is a jpg file, processing...\n\n");
			
			std::string image_path(ent_ptr->d_name);
			//mats[img_count] = imread(image_path, IMREAD_UNCHANGED);
			//temp_mats[img_count] = imread(image_path, IMREAD_UNCHANGED);
			temp_mat = imread(image_path, IMREAD_UNCHANGED);
			
			if (temp_mat.empty() )//(mats[img_count].empty() )
			{
				std::cout << "Could not read the image: " << image_path << std::endl;
				//return 1;
			} else {
				GaussianBlur(temp_mat, mats[img_count], Size(3,3), 0, 0, BORDER_DEFAULT);
				//temp_mats[img_count].copyTo(mats[img_count]);
				(mat_infos[img_count])->image = &mats[img_count];

				unsigned long int total_sum = 0;
				
				rows = mat_infos[img_count]->rows = mats[img_count].rows;
				cols = mat_infos[img_count]->cols = mats[img_count].cols;
				
				for (int y=0; y < rows; y++) //mats[img_count].rows; y++)
				{

					for (int x=0; x < cols; x++) //mats[img_count].cols; x++) 
					{

						for (int c=0; c < mats[img_count].channels(); c++) 
						{
							
							total_sum+=mats[img_count].at<Vec3b>(y,x).val[c];
							mat_infos[img_count]->tot = total_sum;
							
						}
						
					}
					
				}
				
				img_count++;
				printf("img_count+: %d\n", img_count);
				
			} //(mats[img_count].empty() )
			temp_mat.release();
		} else {
			printf(" ...not a jpg file or some other issue\n\n");
		}
	}
	printf("img_count: %d\n", img_count);
	closedir(dir_ptr);
	
	if (img_count > 0)
	{
		for (int i=0; i < img_count; i++)
			std::cout << "mat_infos[" << i << "] tot: " << mat_infos[i]->tot \
			<< " | rows: " << mat_infos[i]->rows << " | cols: "<< mat_infos[i]->cols << std::endl;
		qsort(mat_infos, img_count, sizeof(mat_infos[0]), ii_cmp);
		puts("");
		for (int i=0; i < img_count; i++)
			std::cout << "mat_infos[" << i << "] tot: " << mat_infos[i]->tot \
			<< " | rows: " << mat_infos[i]->rows << " | cols: "<< mat_infos[i]->cols << std::endl;
		puts("\n");
		
		
		printf("calculating diffs 1\n");
		for (int i=0; i < img_count-1; i++)
		{
			img_infos[i] = (Img_info *) calloc(1, sizeof(Img_info *));
			img_infos[i]->image = (Pxl_int_info *) calloc(rows*cols, sizeof(Pxl_int_info));
			img_infos[i]->rows = rows;
			img_infos[i]->cols = cols;
			printf("rows: %d\n", rows);
			printf("cols: %d\n", cols);
			for (int y=0; y < rows; y++) //mat_infos[i]->image->rows; y++)
			{		
				
				for (int x=0; x < cols; x++) //mat_infos[i]->image->cols; x++)
				{
				
					(img_infos[i]->image+(y*cols)+x)->blue =
						(int) mat_infos[i+1]->image->at<Vec3b>(y,x).val[0] - 
						(int) mat_infos[i]->image->at<Vec3b>(y,x).val[0];
					(img_infos[i]->image+(y*cols)+x)->green =
						(int) mat_infos[i+1]->image->at<Vec3b>(y,x).val[1] - 
						(int) mat_infos[i]->image->at<Vec3b>(y,x).val[1];
					(img_infos[i]->image+(y*cols)+x)->red =
						(int) mat_infos[i+1]->image->at<Vec3b>(y,x).val[2] - 
						(int) mat_infos[i]->image->at<Vec3b>(y,x).val[2];
				

				}
				if (y<1)
				{
					printf("chk: ");
					for (int x=0; x < 5; x++)
					{
						printf("(%i) ", (img_infos[i]->image+(y*cols)+x)->blue);
					}
					printf(" ,| ");
					for (int x=0; x < 5; x++)
					{
						printf("(%i) ", ((img_infos[i]->image+(y*cols)+x)->green));
					}
					printf(" `| ");
					for (int x=0; x < 5; x++)
					{
						printf("(%i) ", ((img_infos[i]->image+(y*cols)+x)->red));
					}
					printf("\n");
				}
				
				if (y<5)
				{
					printf("%d.| ", y);	
					for (int x=0; x < 5; x++)	//mat_infos[i]->image->cols; x++)
					{
						printf("(%d) ",
							(int) mat_infos[i+1]->image->at<Vec3b>(y,x).val[0] - 
							(int) mat_infos[i]->image->at<Vec3b>(y,x).val[0] );
					}
					printf(" .| ");
					for (int x=0; x < 5; x++)	//mat_infos[i]->image->cols; x++)
					{
						printf("(%d) ",
							(int) mat_infos[i+1]->image->at<Vec3b>(y,x).val[1] - 
							(int) mat_infos[i]->image->at<Vec3b>(y,x).val[1] );
					}
					printf(" '| ");
					for (int x=0; x < 5; x++)	//mat_infos[i]->image->cols; x++)
					{
						printf("(%d) ",
							(int) mat_infos[i+1]->image->at<Vec3b>(y,x).val[2] - 
							(int) mat_infos[i]->image->at<Vec3b>(y,x).val[2] );
					}
					printf("\n");
				}
			}
			printf("\n\n\n");
		}
		
		//Flatten the highest diffs down into img_infos[0]
		for (int i=(img_count-2); i > 0; i--)
		{
			for (int y=0; y < rows; y++)
			{
				for (int x=0; x < cols; x++)
				{
					if ( (img_infos[i]->image+(y*cols)+x)->blue > 
						 (img_infos[i-1]->image+(y*cols)+x)->blue )
					{
						(img_infos[i-1]->image+(y*cols)+x)->blue = 
							(img_infos[i]->image+(y*cols)+x)->blue;
					}
					
					if ( (img_infos[i]->image+(y*cols)+x)->green > 
						 (img_infos[i-1]->image+(y*cols)+x)->green )
					{
						(img_infos[i-1]->image+(y*cols)+x)->green = 
							(img_infos[i]->image+(y*cols)+x)->green;
					}
					if ( (img_infos[i]->image+(y*cols)+x)->red > 
						 (img_infos[i-1]->image+(y*cols)+x)->red )
					{
						(img_infos[i-1]->image+(y*cols)+x)->red = 
							(img_infos[i]->image+(y*cols)+x)->red;
					}
				}
			}
			if (i==1)
			{
				for (int y=0; y < rows; y++)
				{
					for (int x=0; x < cols; x++)
					{
						if ( (img_infos[0]->image+(y*cols)+x)->blue  > max_b)
							max_b = (img_infos[0]->image+(y*cols)+x)->blue;
						if ( (img_infos[0]->image+(y*cols)+x)->green > max_g)
							max_g = (img_infos[0]->image+(y*cols)+x)->green;
						if ( (img_infos[0]->image+(y*cols)+x)->red   > max_r)
							max_r = (img_infos[0]->image+(y*cols)+x)->red;
					}
				}
			}
		}
		
		
		
		#ifndef GRAD_M
		for (int i=0; i < img_count; i++)
		{
			Mat dx, dy, src_gray;
			
			cvtColor( *mat_infos[i]->image, src_gray, COLOR_BGR2GRAY );
			Sobel( src_gray, dx, CV_32F, 1, 0, -1 );
			Sobel( src_gray, dy, CV_32F, 0, 1, -1 );
			//Sobel( *mat_infos[i]->image, dx, CV_32F, 1, 0, -1 );
			//Sobel( *mat_infos[i]->image, dy, CV_32F, 0, 1, -1 );
			
			magnitude( dx, dy, dx );
			cur_sharpness = sum(dx)[0];
			printf("%d. %e\n", i, cur_sharpness);
			if (cur_sharpness > max_sharpness)
			{
				max_sharpness = cur_sharpness;
				sharpness_idx = i;
			}
			puts("");
			std::cout << "dx[" << i << "] rows: " << dx.rows << " , cols: " << dx.cols << std::endl;
			std::cout << "dy[" << i << "] rows: " << dx.rows << " , cols: " << dx.cols << std::endl;
			puts("");
			dx.release();
			dy.release();
			src_gray.release();
		}
		
		#elif GRAD_M == 1
		
		for (int i=0; i < img_count; i++)
		{
			Mat lp, src_gray;
			cvtColor( *mat_infos[i]->image, src_gray, COLOR_BGR2GRAY );
			Laplacian( src_gray, lp, CV_32F, 3 );
			cur_sharpness = sum(lp)[0];
			printf("%d. %e\n", i, cur_sharpness);
			if (cur_sharpness > max_sharpness)
			{
				max_sharpness = cur_sharpness;
				sharpness_idx = i;
			}
			puts("");
			std::cout << "lp[" << i << "] rows: " << lp.rows << " , cols: " << lp.cols << std::endl;
			puts("");
			lp.release();
			src_gray.release();
		}
		
		#elif GRAD_M == 2
		
		puts("\nLaplacian...");
		for (int i=0; i < img_count; i++)
		{
			Mat src_gray;
			cvtColor( *mat_infos[i]->image, src_gray, COLOR_BGR2GRAY );
			Laplacian( src_gray, lp_mats[i], CV_32F, 3 ); //CV_32F
			std::cout << i << ". " << std::endl;
			for (int y=0; y < 5; y++)
			{
				for (int x=0; x < 4; x++)
				{
					std::cout << "(" << std::setw(3) << (float)lp_mats[i].at<Vec3b>(x,y)[0] << ", "
						<< std::setw(3) << (float)lp_mats[i].at<Vec3b>(y,x)[1] << ", "
						<< std::setw(3) << (float)lp_mats[i].at<Vec3b>(y,x)[2] << ")  ";
				}
				puts("");
			}
			puts("\n");
			namedWindow("lp window", WINDOW_AUTOSIZE);
			resize(lp_mats[i], lp_mats[i], Size(lp_mats[i].cols/4, lp_mats[i].rows/4) );
			imshow( "lp_window", lp_mats[i] );
			waitKey(0);
			src_gray.release();
			
		}
		
		#elif GRAD_M == 3
		
		puts("\nSobel dx dy ...");
		for (int i=0; i < img_count; i++)
		{
			Mat dx, dy, src_gray;
			
			cvtColor( *mat_infos[i]->image, src_gray, COLOR_BGR2GRAY );
			Sobel( src_gray, dx, CV_32F, 1, 0, -1 );
			Sobel( src_gray, dy, CV_32F, 0, 1, -1 );
			//Sobel( *mat_infos[i]->image, dx, CV_32F, 1, 0, -1 );
			//Sobel( *mat_infos[i]->image, dy, CV_32F, 0, 1, -1 );
			
			std::cout << "dx_" << i << ". " << std::endl;
			for (int y=0; y < 5; y++)
			{
				for (int x=0; x < 4; x++)
				{
					std::cout << "(" << std::setw(3) << (float)dx.at<Vec3b>(x,y)[0] << ", "
						<< std::setw(3) << (float)dx.at<Vec3b>(y,x)[1] << ", "
						<< std::setw(3) << (float)dx.at<Vec3b>(y,x)[2] << ")  ";
				}
				puts("");
			}
			puts("\n");
			namedWindow("dx", WINDOW_AUTOSIZE);
			resize(dx, dx, Size(dx.cols/4, dx.rows/4) );
			imshow( "dx", dx );
			waitKey(0);
			
			std::cout << "dy_" << i << ". " << std::endl;
			for (int y=0; y < 5; y++)
			{
				for (int x=0; x < 4; x++)
				{
					std::cout << "(" << std::setw(3) << (float)dy.at<Vec3b>(x,y)[0] << ", "
						<< std::setw(3) << (float)dy.at<Vec3b>(y,x)[1] << ", "
						<< std::setw(3) << (float)dy.at<Vec3b>(y,x)[2] << ")  ";
				}
				puts("");
			}
			puts("\n");
			namedWindow("dy", WINDOW_AUTOSIZE);
			resize(dy, dy, Size(dy.cols/4, dy.rows/4) );
			imshow( "dy", dy );
			waitKey(0);
			
			dx.release();
			dy.release();
			src_gray.release();
		}
		
		#endif
		
		
		
		//filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
		
		
		#if GRAD_M == 2
		#elif GRAD_M == 3
		#else
		
		printf("max sharpness: %e\n", max_sharpness);
		printf("sharpness idx: %d\n", sharpness_idx);
		
		flt_info = { rows, cols };
		flt_info.image = (Pxl_flt_info *) calloc(rows*cols, sizeof(Pxl_flt_info));
		
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				(flt_info.image+(y*cols)+x)->blue = 
					(float)(img_infos[0]->image+(y*cols)+x)->blue / (float)max_b;
				(flt_info.image+(y*cols)+x)->green = 
					(float)(img_infos[0]->image+(y*cols)+x)->green / (float)max_g;
				(flt_info.image+(y*cols)+x)->red = 
					(float)(img_infos[0]->image+(y*cols)+x)->red / (float)max_r;
				(flt_info.image+(y*cols)+x)->clr_idx = 
					max_clr( (img_infos[0]->image+(y*cols)+x)->blue,
								(img_infos[0]->image+(y*cols)+x)->green,
								(img_infos[0]->image+(y*cols)+x)->red );
			}
		}

		printf("test flts\n");
		for (int y=0; y < 2; y++)
		{
			for (int x=0; x < 4; x++)
			{
				printf("(%3.3f, %3.3f, %3.3f, %i) ", (flt_info.image+(y*cols)+x)->blue,
											(flt_info.image+(y*cols)+x)->green,
											(flt_info.image+(y*cols)+x)->red,
											(flt_info.image+(y*cols)+x)->clr_idx);
			}
			printf("\n");
		}
		printf("\nend test flts\n");
		
		
		
		uchar max_c = 255;
		for (int y=0; y < rows; y++)
		{
			for (int x=0; x < cols; x++)
			{
				#ifdef LIMIT_ADJ
				if ( (flt_info.image+(y*cols)+x)->clr_idx < 3 )
				{
				#endif
					mat_infos[sharpness_idx]->image->at<Vec3b>(y,x).val[0] += 
				 		(uchar)( (max_c - mat_infos[sharpness_idx]->image->at<Vec3b>(y,x).val[0]) *
						(flt_info.image+(y*cols)+x)->blue );
				#ifdef LIMIT_ADJ
				}
				if ( (flt_info.image+(y*cols)+x)->clr_idx == 1 ||
					 (flt_info.image+(y*cols)+x)->clr_idx == 3 ||
					 (flt_info.image+(y*cols)+x)->clr_idx == 4 )
				{
				#endif
				
					mat_infos[sharpness_idx]->image->at<Vec3b>(y,x).val[1] += 
				 		(uchar)( (max_c - mat_infos[sharpness_idx]->image->at<Vec3b>(y,x).val[1]) *
						(flt_info.image+(y*cols)+x)->green );	
				#ifdef LIMIT_ADJ
				}
				if ( (flt_info.image+(y*cols)+x)->clr_idx == 2 ||
					 (flt_info.image+(y*cols)+x)->clr_idx == 4 ||
					 (flt_info.image+(y*cols)+x)->clr_idx == 5 )
				{
				#endif
					mat_infos[sharpness_idx]->image->at<Vec3b>(y,x).val[2] += 
				 		(uchar)( (max_c - mat_infos[sharpness_idx]->image->at<Vec3b>(y,x).val[2]) *
						(flt_info.image+(y*cols)+x)->red );		
				#ifdef LIMIT_ADJ
				}
				#endif
				
			} 
		}
		
		
		const char* window_name = "Demo ONE";
		char where[strlen(art_fldr)+1+strlen(test_jpg)+1];
		strcpy(where, art_fldr);
		strcat(where, "/");
		strcat(where, test_jpg);
		printf("where?: %s\n", where);
		imwrite(where, *mat_infos[sharpness_idx]->image);
		resize(*mat_infos[sharpness_idx]->image, *mat_infos[sharpness_idx]->image, Size((mat_infos[sharpness_idx]->image)->cols/4, (mat_infos[sharpness_idx]->image)->rows/4));
		namedWindow(window_name, WINDOW_AUTOSIZE);
		imshow( window_name, *mat_infos[sharpness_idx]->image );
		waitKey(0);
		#endif //GRAD_M < 2
	}
	printf("the end\n");
	return 0;
}
