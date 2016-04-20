#include "StudentPreProcessing.h"
#include "ValueGrid.h"
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

ValueGrid * StudentPreProcessing::maskImage(const IntensityImage &intensityImage, const ValueGrid maskImage) const {
	const int totalWeight = maskImage.getTotalValue();
	const int offsetX = (maskImage.getWidth() - 1) / 2;
	const int offsetY = (maskImage.getHeight() - 1) / 2;

	const int newImageWidth = intensityImage.getWidth() - offsetX * 2;
	const int newImageHeight = intensityImage.getHeight() - offsetY * 2;

	double * data = new double[newImageWidth*newImageHeight];
	for (int x = 0; x < newImageWidth; x++){
		for (int y = 0; y < newImageHeight; y++){
			int totalValue = 0;
			for (int maskX = 0; maskX < maskImage.getWidth(); maskX++){
				for (int maskY = 0; maskY < maskImage.getHeight(); maskY++){
					totalValue += intensityImage.getPixel(x + maskX, y + maskY) * maskImage.getValue(maskX, maskY);
				}
			}
			data[x + y*newImageHeight] = totalValue;
		}
	}

	return new ValueGrid(data, newImageWidth, newImageHeight);
}