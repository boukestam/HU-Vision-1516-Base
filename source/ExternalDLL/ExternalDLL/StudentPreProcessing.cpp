#include "StudentPreProcessing.h"
#include "ValueGrid.h"
#include "IntensityImageStudent.h"
#include <algorithm>

#include <iostream>

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	return &image.toGrayScale();
}

IntensityImage * StudentPreProcessing::stepToIntensityImage(const ValueGrid &grid) const {
	IntensityImage * newIntensityImage = new IntensityImageStudent(grid.getWidth(), grid.getHeight());
	int gridSize = grid.getSize();
	for (int i = 0; i < gridSize; i++){
		newIntensityImage->setPixel(i, (Intensity)grid.getValue(i));
	}
	
	return newIntensityImage;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepBlur(const IntensityImage & image) const{
	const int maskWidth = 3;
	const int maskHeight = 3;

	double maskData[maskHeight][maskWidth] = {
		{ 1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 1, 1 }
	};

	ValueGrid mask = { &maskData[0][0], maskWidth, maskHeight };

	ValueGrid* maskedValues = maskImage(image, mask);

	int numPixels = maskedValues->getWidth() * maskedValues->getHeight();
	for (int i = 0; i < numPixels; i++){
		maskedValues->setValue(i, maskedValues->getValue(i) / 9);
	}

	return stepToIntensityImage(*maskedValues);
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	const int maskWidth = 3;
	const int maskHeight = 3;

	static double maskData[maskHeight][maskWidth] = {
		{ 0.5, 1, 0.5 },
		{ 1,  -6, 1   },
		{ 0.5, 1, 0.5 }
	};

	static const double maskMin = -6 * 256;
	static const double maskScale = 12;

	static const ValueGrid mask = { &maskData[0][0], maskWidth, maskHeight };

	IntensityImage* blurredImage = stepBlur(image);

	ValueGrid* edgeValues = maskImage(*blurredImage, mask);
	
	int numPixels = edgeValues->getWidth() * edgeValues->getHeight();

	for (int i = 0; i < numPixels; i++){
		edgeValues->setValue(i, edgeValues->getValue(i) - maskMin / 12);
	}

	return stepToIntensityImage(*edgeValues);
}

IntensityImage * thresholdImage(const IntensityImage &image, int threshold) {
	IntensityImage* newImage = new IntensityImageStudent(image.getWidth(), image.getHeight());

	int numPixels = image.getWidth() * image.getHeight();

	for (int i = 0; i < numPixels; i++) {
		Intensity value = image.getPixel(i);

		if (value > threshold) {
			newImage->setPixel(i, 0);
		} else {
			newImage->setPixel(i, 255);
		}
	}

	return newImage;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	IntensityImage* blurredImage = stepBlur(image);
	return thresholdImage(*blurredImage, 135);
}

int otsuGetThreshold(int grayScaleHistogram[], int imageSize) {
	int sum = 0;
	for (int i = 1; i < 256; ++i) {
		sum += i * grayScaleHistogram[i];
	}

	int sumB = 0;
	int wB = 0;
	int wF = 0;
	int mB = 0;
	int mF = 0;
	double max = 0.0;
	double between = 0.0;
	double threshold1 = 0.0;
	double threshold2 = 0.0;
	for (int i = 0; i < 256; ++i) {
		wB += grayScaleHistogram[i];
		if (wB == 0) {
			continue;
		}
		int wF = imageSize - wB;
		if (wF == 0) {
			break;
		}
		sumB += i * grayScaleHistogram[i];
		mB = sumB / wB;
		mF = (sum - sumB) / wF;
		between = wB * wF * (mB - mF) * (mB - mF);
		if (between >= max) {
			threshold1 = i;
			if (between > max) {
				threshold2 = i;
			}
			max = between;
		}
	}
	return (int)((threshold1 + threshold2) / 2.0);
}

IntensityImage * StudentPreProcessing::stepDynamicThresholding(const IntensityImage &image) const {
	// Convert to histogram
	const int grayScaleSize = 256;
	int grayScaleHistogram[grayScaleSize];
	for (int i = 0; i < grayScaleSize; i++) {
		grayScaleHistogram[i] = 0;
	}

	const int imageSize = image.getWidth() * image.getHeight();
	for (int i = 0; i < imageSize; i++) {
		grayScaleHistogram[image.getPixel(i)] += 1;
	}

	// Get threshold
	int newThreshold = otsuGetThreshold(grayScaleHistogram, imageSize);

	return thresholdImage(image, newThreshold);
}

IntensityImage * StudentPreProcessing::stepDynamicThresholding(const IntensityImage &image, int startX, int startY, int endX, int endY) const {
	IntensityImage * newImage = new IntensityImageStudent(endX - startX, endY - startY);

	for (int x = 0; x < newImage->getWidth(); x++) {
		for (int y = 0; y < newImage->getHeight(); y++) {
			newImage->setPixel(x, y, image.getPixel(x + startX, y + startY));
		}
	}

	return stepDynamicThresholding(*newImage);
}

ValueGrid * StudentPreProcessing::maskImage(const IntensityImage &intensityImage, const ValueGrid maskImage) const {
	const int totalWeight = maskImage.getTotalValue();
	const int offsetX = (maskImage.getWidth() - 1) / 2;
	const int offsetY = (maskImage.getHeight() - 1) / 2;
	
	const int imageSize = intensityImage.getWidth() * intensityImage.getHeight();
	//const int offset = offsetX + offsetY * maskImage.getWidth();
	const int newImageWidth = intensityImage.getWidth() - offsetX * 2;
	const int newImageHeight = intensityImage.getHeight() - offsetY * 2;
	const int newImageSize = newImageWidth * newImageHeight;

	const int maskImageSize = maskImage.getSize();
	const int maskImageWidth = maskImage.getWidth();
	const int maskImageHeight = maskImage.getHeight();

	double * oldImageData = new double[imageSize];
	for (int i = 0; i < imageSize; i++){
		oldImageData[i] = intensityImage.getPixel(i);
	}

	double * maskData = maskImage.getData();
	double * newData = new double[newImageSize];
	for (int i = 0; i < newImageSize; i++){
		newData[i] = 0;
	}
	int maskIndex = 0;
	for (int maskX = 0; maskX < maskImageWidth; maskX++){
		for (int maskY = 0; maskY < maskImageHeight; maskY++){
			const double maskValue = maskData[maskIndex];
			const int maskOffset = maskX + maskY * intensityImage.getWidth();
			int index = 0;
			oldImageData += maskOffset;
			for (int newImageY = 0; newImageY < newImageHeight; newImageY++){
				const int translateY = newImageY * intensityImage.getWidth();
				for (int newImageX = 0; newImageX < newImageWidth; newImageX++){
					newData[index] += oldImageData[newImageX + translateY] * maskValue;
					index++;
				}
			}
			oldImageData -= maskOffset;
			maskIndex++;
		}
	}

	return new ValueGrid(newData, newImageWidth, newImageHeight);
}