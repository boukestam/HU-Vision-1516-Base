#include "StudentPreProcessing.h"
#include "ValueGrid.h"
#include "IntensityImageStudent.h"

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	return &image.toGrayScale();
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

	Mask mask = { &maskData[0][0], maskWidth, maskHeight };

	return maskImage(image, mask);
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	const int maskWidth = 3;
	const int maskHeight = 3;

	double maskData[maskHeight][maskWidth] = {
		{ 0.5, 1, 0.5 },
		{ 1,  -6, 1   },
		{ 0.5, 1, 0.5 }
	};

	Mask mask = { &maskData[0][0], maskWidth, maskHeight };

	IntensityImage* blurredImage = stepBlur(image);

	return maskImage(*blurredImage, mask);
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::maskImage(const IntensityImage &intensityImage, const ValueGrid maskImage) const {
	const int totalWeight = maskImage.getTotalValue();
	const int offsetX = (maskImage.getWidth() - 1) / 2;
	const int offsetY = (maskImage.getHeight() - 1) / 2;

	const int newImageWidth = intensityImage.getWidth() - offsetX * 2;
	const int newImageHeight = intensityImage.getHeight() - offsetY * 2;

	IntensityImage * newIntensityImage = new IntensityImageStudent(newImageWidth, newImageHeight);
	for (int x = 0; x < newImageWidth; x++){
		for (int y = 0; y < newImageHeight; y++){
			int totalValue = 0;
			for (int maskX = 0; maskX < maskImage.getWidth(); maskX++){
				for (int maskY = 0; maskY < maskImage.getHeight(); maskY++){
					totalValue += intensityImage.getPixel(x + maskX, y + maskY) * maskImage.getValue(maskX, maskY);
				}
			}
			newIntensityImage->setPixel(x, y, totalValue);
		}
	}
	

	return newIntensityImage;
}