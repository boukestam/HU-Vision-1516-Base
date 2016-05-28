#include "StudentExtraction.h"
#include "StudentPreProcessing.h"
#include "ImageIO.h"

bool StudentExtraction::stepExtractEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentExtraction::stepExtractNose(const IntensityImage &image, FeatureMap &features) const {
	Point2D<double> noseLeft = features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0];
	Point2D<double> noseRight = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0];

	// cut nostrils image and threshold to only keep nostrils
	StudentPreProcessing spp;

	int noseHeight = noseLeft.y;

	int noseWidth = noseRight.x - noseLeft.x;
	int nostrilHeight = noseWidth / 4;
	const IntensityImage* thresholdedNostrilImage = spp.stepDynamicThresholding(image, noseLeft.x + 3, noseHeight + nostrilHeight, noseRight.x - 3, noseHeight + nostrilHeight * 2);
	ImageIO::showImage(*thresholdedNostrilImage);

	// cluster

	// determine center of gravity of two biggest clusters

	Point2D<double> leftNostrilMid(0, 0);
	Point2D<double> rightNostrilMid(0, 0);

	features.putFeature(Feature(Feature::FEATURE_NOSTRIL_LEFT, leftNostrilMid));
	features.putFeature(Feature(Feature::FEATURE_NOSTRIL_RIGHT, rightNostrilMid));

	return false;
}

bool StudentExtraction::stepExtractMouth(const IntensityImage &image, FeatureMap &features) const {
	return false;
}