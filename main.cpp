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
#include <iostream>
#include <filesystem>
using namespace cv;
using namespace std;
namespace fs = std::filesystem;

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
	/*
	int baseline = 50;
	int cameraDistance = 500;
	Mat depth = imread("./Printer_depth.png", IMREAD_GRAYSCALE);
	Mat ref = imread("./Printer.png");
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
					//SHOW REAL PIXELS
					MPI.at<Vec4b>(height, width) = Vec4b(ref.at<Vec3b>(height, width)[0], ref.at<Vec3b>(height, width)[1], ref.at<Vec3b>(height, width)[2],255);
					
					for (int xOffset = -syntheticSize+1; xOffset < syntheticSize; xOffset++) {
						for (int yOffset = -syntheticSize + 1; yOffset < syntheticSize; yOffset++) {
							if (height + yOffset < depth.rows && height + yOffset > 0 && width + xOffset < depth.cols && width + xOffset>0) {
								if (depth.at<uchar>(height + yOffset, width+xOffset) < disparityArray[i].depth) {
									//SHOW MASKS
									//MPI.at<Vec4b>(height + yOffset, width + xOffset) = Vec4b(255, 255, 255, 255);
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
	
	*/


	// INPAINT_TELEA INPAINT_TELEA INPAINT_TELEA INPAINT_TELEA
	/*
	std::string path = "./MPI";
	for (const auto& entry : fs::directory_iterator(path)) {
		String pathStr = entry.path().string();
		Mat mask = imread(pathStr, IMREAD_GRAYSCALE);
		Mat dst;
		Mat output = Mat(ref.rows, ref.cols, CV_8UC4, Scalar(0, 0, 0, 0));
		cv::inpaint(ref, mask, dst, 9, INPAINT_TELEA);
		for (int width = 0; width < mask.cols; width++) {
			for (int height = 0; height < mask.rows; height++) {
				if (mask.at<uchar>(height, width) != 0) {
					output.at<Vec4b>(height, width)[0] = dst.at<Vec3b>(height, width)[0];
					output.at<Vec4b>(height, width)[1] = dst.at<Vec3b>(height, width)[1];
					output.at<Vec4b>(height, width)[2] = dst.at<Vec3b>(height, width)[2];
					output.at<Vec4b>(height, width)[3] = 255;
				}
			}
		}
		imwrite(pathStr, output);
	}
	*/



	// INPAINT_PEELING INPAINT_PEELING INPAINT_PEELING
	std::string path = "./MPI";
	for (const auto& entry : fs::directory_iterator(path)) {
		String MPIString = entry.path().string();
		int MPIdepth = stoi(MPIString.substr(6, MPIString.find(".", 6) - 6));
		String MaskString = "./MPI_Mask/" + to_string(MPIdepth);
		MaskString.append(".png");
		String OutputString = "./MPI_Result/" + to_string(MPIdepth);
		OutputString.append(".png");

		//Mat REF = imread("./Printer.png");
		Mat MPI = imread(MPIString, IMREAD_UNCHANGED);
		Mat Mask = imread(MaskString, IMREAD_GRAYSCALE);
		Mat output = Mat(MPI.rows, MPI.cols, CV_8UC4, Scalar(0, 0, 0, 0));
		output = MPI;

		while (true) {
			bool upInpainted = true;
			bool downInpainted = true;
			bool leftInpainted = true;
			bool rightInpainted = true;

			for (int width = 0; width < MPI.cols; width++) {
				for (int height = 0; height < MPI.rows; height++) {
					if (output.at<Vec4b>(height, width)[3] != 0) {
						if (height + 1 < MPI.rows) {
							if (Mask.at<uchar>(height + 1, width) != 0) {
								downInpainted = false;
								output.at<Vec4b>(height + 1, width) = output.at<Vec4b>(height, width);
								Mask.at<uchar>(height + 1, width) = 0;
							}
						}
						if (height - 1 >= 0) {
							if (Mask.at<uchar>(height - 1, width) != 0) {
								upInpainted = false;
								output.at<Vec4b>(height - 1, width) = output.at<Vec4b>(height, width);
								Mask.at<uchar>(height - 1, width) = 0;
							}
						}
						if (width + 1 < MPI.cols) {
							if (Mask.at<uchar>(height, width + 1) != 0) {
								leftInpainted = false;
								output.at<Vec4b>(height, width + 1) = output.at<Vec4b>(height, width);
								Mask.at<uchar>(height, width + 1) = 0;
							}
						}
						if (width - 1 >= 0) {
							if (Mask.at<uchar>(height, width - 1) != 0) {
								rightInpainted = false;
								output.at<Vec4b>(height, width - 1) = output.at<Vec4b>(height, width);
								Mask.at<uchar>(height, width - 1) = 0;
							}
						}
					}
				}
			}

			printf("%s", "loop");
			printf("UP: %d\n", upInpainted);
			printf("DOWN: %d\n", downInpainted);
			printf("LEFT: %d\n", leftInpainted);
			printf("RIGHT: %d\n", rightInpainted);
			printf("\n");

			//imwrite(OutputString, output);

			if (upInpainted&&downInpainted&&leftInpainted&&rightInpainted) {
				printf("%d",MPIdepth);
				printf("\n");
				break;
			}
		}
		
		imwrite(OutputString, output);
	}

	waitKey(0);
}