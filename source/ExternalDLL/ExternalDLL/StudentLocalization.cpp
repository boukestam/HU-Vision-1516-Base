#include "StudentLocalization.h"

bool StudentLocalization::stepFindHead(const IntensityImage &image, FeatureMap &features) const {
	
	// Find top of head
	const int halfWidth = image.getWidth() / 2;
	Histogram & topHeadHistogram = getPixelCountHistogram(image, Axis::Y, halfWidth - halfWidth / 4, halfWidth + halfWidth / 4);
	
	int topOfHeadY = -1;
	for (int i = 0; i < topHeadHistogram.getSize(); i++) {
		if (topHeadHistogram.getValue(i) > 2) {
			topOfHeadY = i;
			break;
		}
	}
	if (topOfHeadY == -1) {
		std::cout << "Could not find topOfHeadY!" << std::endl;
		return false;
	}
	Feature * featureHeadTop = new Feature(Feature::FEATURE_HEAD_TOP, Point2D<double>(image.getWidth() / 2, topOfHeadY));
	features.putFeature(*featureHeadTop);

	// Find sides of head
	
	int previousLeftX = -1;
	int previousRightX = -1;
	for (int y = topOfHeadY; y < image.getHeight(); y+=10) {
		Histogram & sideHeadHistogram = getPixelCountHistogram(image, Axis::X, y, y+10);
		//Left
		int leftX = -1;
		for (int i = 0; i < sideHeadHistogram.getSize(); i++) {
			int value = sideHeadHistogram.getValue(i);
			if (value > 2) {
				leftX = value;
				break;
			}
		}
		//Right
		int rightX = -1;
		for (int i = sideHeadHistogram.getSize()-1; i >= 0; i--) {
			int value = sideHeadHistogram.getValue(i);
			if (value > 2) {
				
				rightX = value;
				break;
			}
		}
		int headWidth = rightX - leftX;
		int previousWidth = previousRightX - previousLeftX;
		if (headWidth < previousWidth) {
			Feature * featureHeadLeft = new Feature(Feature::FEATURE_HEAD_LEFT_SIDE, Point2D<double>(previousLeftX, y));
			Feature * featureHeadRight = new Feature(Feature::FEATURE_HEAD_RIGHT_SIDE, Point2D<double>(previousRightX, y));
			features.putFeature(*featureHeadLeft);
			features.putFeature(*featureHeadRight);
			return true;
		}

		previousLeftX = leftX;
		previousRightX = rightX;
	}

	return false;
}

if (rightY == -1 || leftY == -1) {
	std::cout << "Could not find rightY or leftY!" << std::endl;
	return false;
}

return true;

bool StudentLocalization::stepFindNoseMouthAndChin(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindChinContours(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseEndsAndEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

Histogram & StudentLocalization::getPixelCountHistogram(const IntensityImage & image, Axis axis, int start, int end) const {
	int otherAxisSize = axis == Axis::Y ? image.getWidth() : image.getHeight();
	Histogram & histogram = *new Histogram(axis, otherAxisSize);
	for (int pos1 = 0; pos1 < otherAxisSize; pos1++) {
		int value = 0;
		for (int pos2 = start; pos2 < end; pos2++) {
			int x = axis == Axis::Y ? pos2 : pos1;
			int y = axis == Axis::Y ? pos1 : pos2;
			value += (int)image.getPixel(x, y) > 127 ? 1 : 0;
		}
		histogram.setValue(pos1, value);
	}
	return histogram;
}

//
// ----------- X                    | | Y
//									| |
// -----------						| |