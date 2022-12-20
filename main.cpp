#include<stdio.h>
#include<opencv2/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include<stdio.h>
#include<math.h>
#include<vector>
#include<Windows.h>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

struct depthUnit {
	int depth;
	int number;
};

bool compareByNumber(const depthUnit& a, const depthUnit& b)
{
	return a.number > b.number;
}

bool compareByDepth(const depthUnit& a, const depthUnit& b)
{
	return a.depth > b.depth;
}

void main() {
	Mat ref = imread("./refImage.png");
	Mat mask = imread("./mask.png", IMREAD_GRAYSCALE);
	Mat dst;
	cv::inpaint(ref, mask, dst, 1, INPAINT_NS);
	imshow("result", dst);
	waitKey(0);

	/*
	int baseline = 50;
	int cameraDistance = 500;
	Mat depth = imread("./depth.png");
	Mat ref = imread("./refImage.png");

	vector<depthUnit> disparityArray;
	for (int width = 0; width < depth.cols; width++) {
		for (int height = 0; height < depth.rows; height++) {
			int Z = depth.at<Vec3b>(height, width)[0];
			bool ifDisExists = false;
			for (int i = 0; i < disparityArray.size(); i++) {
				if (Z == disparityArray[i].depth) {
					disparityArray[i].number++;
					ifDisExists = true;
					break;
				}
			}
			if (!ifDisExists) {
				disparityArray.push_back({Z,0});
			}
		}
	}

	sort(disparityArray.begin(), disparityArray.end(), compareByDepth);

	for (int i = 0; i < disparityArray.size(); i++) {
		Mat MPI = Mat(ref.rows, ref.cols, CV_8UC3, Scalar(0, 0, 0));
		int syntheticSize = baseline * (disparityArray[i].depth - disparityArray[disparityArray.size()-1].depth) / cameraDistance;
		printf("%i\n", disparityArray.size());
		for (int width = 0; width < depth.cols; width++) {
			for (int height = 0; height < depth.rows; height++) {
				if (depth.at<Vec3b>(height, width)[0] == disparityArray[i].depth) {
					MPI.at<Vec3b>(height, width) = ref.at<Vec3b>(height, width);
				}
			}
		}
		std::ostringstream oss;
		oss << "MPI/" << disparityArray[i].depth << ".png";
		imwrite(oss.str(), MPI);
	}
	*/
}