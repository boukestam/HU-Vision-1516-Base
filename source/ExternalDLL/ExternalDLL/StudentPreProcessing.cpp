#include "StudentPreProcessing.h"
#include "Mask.h"
#include "IntensityImageStudent.h"

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::maskImage(const IntensityImage &intensityImage, const Mask maskImage) const {
	IntensityImage * newIntensityImage = new IntensityImageStudent();
	return nullptr;
}