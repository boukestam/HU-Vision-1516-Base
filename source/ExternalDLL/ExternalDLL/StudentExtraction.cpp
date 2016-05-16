#include "StudentExtraction.h"

bool StudentExtraction::stepExtractEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentExtraction::stepExtractNose(const IntensityImage &image, FeatureMap &features) const {
	Point2D<double> noseLeft = features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0];
	Point2D<double> noseRight = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0];

	return false;
}

bool StudentExtraction::stepExtractMouth(const IntensityImage &image, FeatureMap &features) const {
	return false;
}