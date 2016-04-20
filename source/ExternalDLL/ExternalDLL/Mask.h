#pragma once

class Mask{
public:
	Mask(double * data, int width, int height);
	int getWidth();
	int getHeight();
	double getWeight(int x, int y);
	double getTotalWeight();
private:
	double * data;
	int width;
	int height;
};