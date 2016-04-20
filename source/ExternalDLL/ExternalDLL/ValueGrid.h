#pragma once

class ValueGrid{
public:
	ValueGrid(double * data, int width, int height);
	int getWidth() const;
	int getHeight() const;
	double getValue(int x, int y) const;
	double getTotalValue() const;
private:
	double * data;
	int width;
	int height;
};