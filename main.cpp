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

struct depthPlane {
	int depth;
	int number;
};

bool compareByNumber(const depthPlane& a, const depthPlane& b)
{
	return a.number > b.number;
}

bool compareByDepth(const depthPlane& a, const depthPlane& b)
{
	return a.depth > b.depth;
}

void main() {
	int baseline = 50;
	int cameraDistance = 500;
	Mat depth = imread("./Vintage_depth.png", IMREAD_GRAYSCALE);
	Mat ref = imread("./Vintage.png");
	for (int width = 0; width < depth.cols; width++) {
		for (int height = 0; height < depth.rows; height++) {
			depth.at<uchar>(height, width) = 255 - depth.at<uchar>(height, width);
		}
	}
	imshow("depth", depth);
	vector<depthPlane> disparityArray;
	for (int width = 0; width < depth.cols; width++) {
		for (int height = 0; height < depth.rows; height++) {
			int Z = depth.at<uchar>(height, width);
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
		Mat MPI = Mat(ref.rows, ref.cols, CV_8UC4, Scalar(0, 0, 0, 0));
		int syntheticSize = baseline * (disparityArray[i].depth - disparityArray[disparityArray.size()-1].depth) / cameraDistance;
		for (int width = 0; width < depth.cols; width++) {
			for (int height = 0; height < depth.rows; height++) {
				if (depth.at<uchar>(height, width) == disparityArray[i].depth) {
					MPI.at<Vec4b>(height, width) = Vec4b(ref.at<Vec3b>(height, width)[0], ref.at<Vec3b>(height, width)[1], ref.at<Vec3b>(height, width)[2],255);
					
					for (int xOffset = -syntheticSize+1; xOffset < syntheticSize; xOffset++) {
						for (int yOffset = -syntheticSize + 1; yOffset < syntheticSize; yOffset++) {
							if (height + yOffset < depth.rows && height + yOffset > 0 && width + xOffset < depth.cols && width + xOffset>0) {
								if (depth.at<uchar>(height + yOffset, width+xOffset) < disparityArray[i].depth) {
									MPI.at<Vec4b>(height + yOffset, width + xOffset) = Vec4b(255, 255, 255, 255);
								}
							}
						}
					}
				}
			}
		}
		std::ostringstream oss;
		oss << "MPI/" << disparityArray[i].depth << ".png";
		imwrite(oss.str(), MPI);
	}
	waitKey(0);
}