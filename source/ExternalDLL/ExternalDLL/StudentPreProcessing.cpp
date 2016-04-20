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

	double maskData[maskHeight][maskWidth] = {
		{ 0.5, 1, 0.5 },
		{ 1,  -6, 1   },
		{ 0.5, 1, 0.5 }
	};

	double maskMin = -6 * 256;
	double maskScale = 12;

	ValueGrid mask = { &maskData[0][0], maskWidth, maskHeight };

	IntensityImage* blurredImage = stepBlur(image);

	ValueGrid* edgeValues = maskImage(*blurredImage, mask);
	
	int numPixels = edgeValues->getWidth() * edgeValues->getHeight();

	for (int i = 0; i < numPixels; i++){
		edgeValues->setValue(i, edgeValues->getValue(i) - maskMin / 12);
	}

	return stepToIntensityImage(*edgeValues);
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	IntensityImage* newImage = new IntensityImageStudent(image.getWidth(), image.getHeight());

	int numPixels = image.getWidth() * image.getHeight();

	for (int i = 0; i < numPixels; i++){
		Intensity value = image.getPixel(i);

		if (value > 150){
			newImage->setPixel(i, 255);
		}
		else{
			newImage->setPixel(i, 0);
		}
	}

	return newImage;
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
			data[x + y*newImageWidth] = totalValue;
		}
	}

	return new ValueGrid(data, newImageWidth, newImageHeight);
}