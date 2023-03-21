#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>




int 
idx_cmp (	int x, 
			int idx_x, 
			int idx_next, 
			size_t xs_size, 
			int *xs, 
			int (*compar) (const void*, const void*)	)
{
	int next_x = xs[0];
	if (xs_size==1)
		return compar(x, next_x) ? idx_x : idx_next;
	else
	{
		if ( compar(x, next_x) )
			return idx_cmp (    x,       idx_x, idx_next+1, xs_size-1, xs+1, compar);
		else
			return idx_cmp (next_x, next_idx_x, idx_next+1, xs_size-1, xs+1, compar);
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
max_rgb_vector (int blue,
				int green,
				int red	)
{
	int colors[5] = {	blue  + green - (  red + red  ),
						blue  + red   - (green + green),
						green + green - ( blue + red  ),
						green + red   - ( blue + blue ),
						red   + red   - ( blue + green)	};
	return idx_cmp( blue+blue-(green+red), 0, 1, 5, colors, gt_cmp );
}




