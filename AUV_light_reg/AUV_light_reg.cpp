#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat frame, src;
VideoCapture cap(0); //open video for reading
bool bSuccess;

double cap_read()
{
	if (cap.isOpened() == false) // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	String window_name = "video";
	namedWindow(window_name, WINDOW_NORMAL); //create a window

	while (true)
	{
		bSuccess = cap.read(frame); // read a new frame from video 
		if (bSuccess == false) 		//Breaking the while loop at the end of the video
		{
			cout << "Found the end of the video" << endl;
			break;
		}

		imshow(window_name, frame); //show the frame in the created window

		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}
}

Mat RGB2HSV(Mat src)
{
	int row = src.rows; int col = src.cols;
	Mat dst(row, col, CV_32FC3);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			float b = src.at<Vec3b>(i, j)[0] / 255.0;
			float g = src.at<Vec3b>(i, j)[1] / 255.0;
			float r = src.at<Vec3b>(i, j)[2] / 255.0;

			float minn = min(r, min(g, b));
			float maxx = max(r, max(g, b));

			dst.at<Vec3f>(i, j)[2] = maxx; //V
			float delta = maxx - minn; float h, s;

			if (maxx != 0)
			{
				s = delta / maxx;
			}
			else
			{
				s = 0;
			}
			if (r == maxx)
			{
				h = (g - b) / delta;
			}
			else if (g == maxx)
			{
				h = 2 + (b - r) / delta;
			}
			else if (b == maxx)
			{
				h = 4 + (r - g) / delta;
			}
			else
			{
				h = 0;
			}
			h *= 60;
			if (h < 0)
				h += 360;
			dst.at<Vec3f>(i, j)[0] = h; dst.at<Vec3f>(i, j)[1] = s;
		}
	}
	return dst;
}

int main(int argc, char* argv[])
{
	cap_read();

	src = cap.read(frame);
	if (src.empty())
	{
		return -1;
	}

	Mat dst, dst1, dst2;
	cvtColor(src, dst1, RGB2HSV);
	dst = RGB2HSV(src);
	return 0;
}
