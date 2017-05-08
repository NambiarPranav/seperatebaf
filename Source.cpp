
#include <iostream>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include <math.h>


using namespace std;
using namespace cv;

int DFS(int z, int n, float *pixels, float *diff, float *values, long int number,float *elements);
int max(int z, int n, float *pixels, float *diff, float *values, long int number,float *elements);



int main()
{
	//Load both the images
	Mat img4 = imread("white.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img3 = imread("ironman.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//Change size of both the images
	Size size(700, 700);
	Mat img1,img2;
	resize(img4, img1, size);
	resize(img3, img2, size);


	//Calculating the no. of pixels
	int m = img1.rows;
	int n = img1.cols;
	int k = m*n;

	//Allocating space to the arrays
	//pixels->all pixels,diff->between the pixels of both images,values->all edge capacity in the row,elements->all elements in the path
	float *pixels, *values, *diff;
	float *elements;

	(float *)pixels = (float *)malloc(sizeof(float)*k * 10 * 2);
	(float *)values = (float *)malloc(sizeof(float)*k);
	(float *)diff = (float *)malloc(sizeof(float)*k);
	(float *)elements = (float *)malloc(sizeof(float)*k);

	//here *10 because 1st is no. of adjacent pixels,2nd is capacity of edge to sink
	//alternate contain next nose and it's value
	long int i = 0, j = 0,z=0;
	int loc,mid;
	float mid1, mid2, mid3;
	for (i = 0; i < img1.rows; i++)
	{
		for (j = 0; j < img1.cols; j++)
		{
			loc = 2;
			mid = img1.at<uchar>(i, j) - img2.at<uchar>(i, j);
			if (mid < 0)
			{
				mid = -mid;
			}
			//difference between pixels to array diff
			*(diff + z) = mid;


			//edge to the sink
			
			*(pixels + z * 10 + 1) = 0;
			

			//weight of each normal edge
			mid1 = 15 * pow(2,-mid);
			
			
			
			//values as node then it's weight
			if (i - 1 >= 0)
			{
				mid3 = (i - 1)*n + j;
				*(pixels + loc + z * 10) = mid3+k;
				loc += 1;
				*(pixels + z * 10 + loc) = mid1;
				loc += 1;
				*(pixels + (z + k) * 10 + loc - 2) = mid3;
				*(pixels + (z + k) * 10 + loc - 1) = 0;

			
			}
			if (j - 1 >= 0)
			{
				mid3 = (i)*n + j-1;
				*(pixels + loc + z * 10) = mid3 + k;
				loc += 1;
				*(pixels + z * 10 + loc) = mid1;
				loc += 1;
				*(pixels + (z + k) * 10 + loc - 2) = mid3;
				*(pixels + (z + k) * 10 + loc - 1) = 0;


			}
			if (i + 1 < n)
			{
				mid3 = (i+1)*n + j;
				*(pixels + loc + z * 10) = mid3 + k;
				loc += 1;
				*(pixels + z * 10 + loc) = mid1;
				loc += 1;
				*(pixels + (z + k) * 10 + loc - 2) = mid3;
				*(pixels + (z + k) * 10 + loc - 1) = 0;
			}
			if (j + 1 < m)
			{
				mid3 = (i)*n + j + 1;
				*(pixels + loc + z * 10) = mid3 + k;
				loc += 1;
				*(pixels + z * 10 + loc) = mid1;
				loc += 1;
				*(pixels + (z + k) * 10 + loc - 2) = mid3;
				*(pixels + (z + k) * 10 + loc - 1) = 0;
			}
			*(pixels + z * 10) = loc;
			z += 1;
		}
	}
	//cout << *(pixels + 2) <<" "<< *(pixels + 3) <<" "<< *(pixels + 4) << endl;

	z = 0;
	while (z < 2 * k)
	{
		if (z < k)
		{
			*(pixels + z * 10 + 1) = 0;
		}
		else
		{
			*(pixels + z * 10 + 1) = 10;
		}
		z += 1;
	}




	//1st position is no of elements
	*(elements) = 2;

	*(values) = 2;
	z = 0;
	while (z < k)
	{
		//finding path to the sink
		*(values) = 2;
		*(values + 1) = *(diff + z);
		*(elements) = 2;
		*(elements + 1) = z;
		DFS (z, k, pixels , diff , values , 0,elements);
		
		z++;
	}


	namedWindow("original_image", CV_WINDOW_AUTOSIZE);
		imshow("original_image", img2);
		waitKey(0);

		destroyWindow("original_image");

	int l1, l2;
	float comp = 5;
	while (i < k)
	{
		

		//cout << *(diff + i) << endl;
		if (*(diff + i) <comp)
		{
			l1 = i / n;
			l2 = i%n;
			img2.at<uchar>(l1, l2) = 0;
		}
		i++;
	}
	/*namedWindow("b", CV_WINDOW_AUTOSIZE);
	imshow("b", img2);
	waitKey(0);

	destroyWindow("b");
	*/
	Size size1(700, 700);
	Mat a1;
	resize(img2, a1, size1);

	
	


	//Display the images
	namedWindow("new", CV_WINDOW_AUTOSIZE);
	
	imshow("new", a1);
	
	waitKey(0);
	
	destroyWindow("new");


	return 0;
}


//function to find path to end
int DFS(int z, int n, float *pixels, float *diff, float *values, long int number, float *elements)
{

	long int i = 2,mid,mid1;
	float mid2;
	if (number < n)
	{
		if (*(pixels + 1 + z * 10) != 0)
		{
			max(z, n, pixels, diff, values, 0, elements);
		}
		mid = *(elements + 1);
		if (*(diff + mid) > 0)
		{
			while (i < *(pixels + z * 10))
			{
				if (*(pixels + z * 10 + i + 1) > 0)
				{
					mid1 = *(elements);
					*(elements + mid1) = *(pixels +z*10+ i);
					*(elements) += 1;
					mid = *(values);
					i += 1;
					*(values + mid) = *(pixels + i+z*10);
					i += 1;
					*(values) += 1;
					mid1 = *(pixels + z * 10 + i - 2);
					DFS(mid1, n, pixels, diff, values, number + 1, elements);
					*(elements) -= 1;
					*(values) -= 1;

				}
				else
				{
					i += 2;
				}
				
				
			}
		}
		
	}

	return 0;

}

int max(int z, int n, float *pixels, float *diff, float *values, long int number, float *elements)
{
	long int mid1, mid2,mid3;
	float mid4,mid5,min;
	mid1 = *(elements);
	mid2 = *(elements + mid1 - 1);

	min = *(pixels+mid2*10+1);
	
	long int i = 1;
	mid2 = *(values);
	while (i < mid2)
	{
		if (min > *(values + i))
		{
			min = *(values+i);
		}
		i++;
	}

	mid1 = *(elements + 1);
	*(diff + mid1) -= min;
	mid1 = *(elements);
	mid2 = *(elements + mid1 - 1);
	*(pixels + mid2 * 10 + 1) -= min;
	
	i = 0;
	int j = 0;
	while (i < mid1-1)
	{
		mid4 = *(elements + i);
		mid2 = mid4;
		i += 1;
		mid5 = *(elements + i);
		mid3 = mid5;
		j = 2;
		while (j < *(pixels +mid2*10))
		{
			if (*(pixels + mid2 * 10 + j) == mid5)
			{
				break;
			}
			j++;
		}
		*(pixels + mid2 * 10 + j + 1) -= min;
		j = 2;
		while (j < *(pixels +mid3*10))
		{

			if (*(pixels + mid3 * 10 + j) == mid4)
			{
				break;
			}
			j++;
		}
		*(pixels + mid3 * 10 + j + 1) += min;

		


		
	}
	


	return 0;
}
