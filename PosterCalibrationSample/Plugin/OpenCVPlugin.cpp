#include <windows.h>
#include "ImageMatcher.h"

using namespace cv;

struct InteropVector2
{
	float X;
	float Y;
};


struct PosterMatchData
{
	bool found;
	int inliers;
	InteropVector2 Corners[4];
};


static bool objectImageDirty = false;
static Mat objectImage;
static ImageMatcher* imageMatcher;

static void DrawBoundingBox(Mat image, vector<Point2f> bb)
{
	for (unsigned i = 0; i < bb.size() - 1; i++)
	{
		line(image, bb[i], bb[i + 1], Scalar(0, 255, 0), 7);
	}
	line(image, bb[bb.size() - 1], bb[0], Scalar(0, 255, 0), 7);
}


extern "C" __declspec(dllexport) bool PosterDetector_SetPosterObjectData(BYTE *  pixels, int width, int height)
{
	objectImage = Mat(height, width, CV_8UC4, pixels);

	objectImageDirty = true;
	return objectImage.rows > 0;
}


extern "C" __declspec(dllexport) int PosterDetector_GetPosterMatchData(BYTE *  pixels, int width, int height, bool flipImage, int minimumInliers, PosterMatchData *posterMatchData)
{
	if (objectImage.cols == 0)
	{
		return 0;
	}

	Mat mainImg = Mat(height, width, CV_8UC4, pixels);

	if (flipImage)
		flip(mainImg, mainImg, 0);

	Stats stats;

	if (imageMatcher == nullptr || !imageMatcher->IsValid)
	{
		//Ptr<BRISK> detector = BRISK::create(50, 2);
		Ptr<ORB> detector = ORB::create(500);

		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
		imageMatcher = new ImageMatcher(detector, matcher);
		imageMatcher->IsValid = true;
	}
	if (objectImageDirty)
	{
		objectImageDirty = false;
		std::vector<Point2f> bb;
		bb.push_back(Point2f(1, 1));
		bb.push_back(Point2f(objectImage.cols - 1, 1));
		bb.push_back(Point2f(objectImage.cols - 1, objectImage.rows - 1));
		bb.push_back(Point2f(1, objectImage.rows - 1));

		imageMatcher->SetObjectImage(objectImage, bb, "", stats);
	}

	vector<Point2f> result = imageMatcher->ProcessFrame(mainImg, stats);
	posterMatchData->inliers = stats.inliers;
	if (stats.inliers >= minimumInliers)
	{
		if (stats.inliers >= minimumInliers)
		{
			DrawBoundingBox(mainImg, result);
		}
		posterMatchData->found = true;
		posterMatchData->Corners[0].X = result.at(0).x;
		posterMatchData->Corners[0].Y = result.at(0).y;
		posterMatchData->Corners[1].X = result.at(1).x;
		posterMatchData->Corners[1].Y = result.at(1).y;
		posterMatchData->Corners[2].X = result.at(2).x;
		posterMatchData->Corners[2].Y = result.at(2).y;
		posterMatchData->Corners[3].X = result.at(3).x;
		posterMatchData->Corners[3].Y = result.at(3).y;
	}
	else
	{
		posterMatchData->found = false;
	}
	return stats.inliers;
}


