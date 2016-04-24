/*
* Copyright (c) 2015 DottedEye Designs, Alexander Hustinx, NeoTech Software, Rolf Smit - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*/

#pragma once
#include "Localization.h"
#include "IntensityImageStudent.h"

enum class Axis {
	X, Y
};

class Histogram {
public:
	Histogram(Axis axis, int size) : axis{ axis }, data{ new int[size] }, size{ size } {}

	~Histogram() {
		delete[] data;
	}

	int getValue(int index) {
		return data[index];
	}

	void setValue(int index, int value) {
		data[index] = value;
	}

	Axis getAxis() {
		return axis;
	}

	int getSize() {
		return size;
	}
private:
	Axis axis;
	int * data;
	int size;
};

class StudentLocalization : public Localization {
public:
	bool stepFindHead(const IntensityImage &image, FeatureMap &features) const;
	bool stepFindNoseMouthAndChin(const IntensityImage &image, FeatureMap &features) const;
	bool stepFindChinContours(const IntensityImage &image, FeatureMap &features) const;
	bool stepFindNoseEndsAndEyes(const IntensityImage &image, FeatureMap &features) const;
	bool stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const;
	Histogram & getPixelCountHistogram(const IntensityImage & image, Axis axis, int start, int end) const;
};