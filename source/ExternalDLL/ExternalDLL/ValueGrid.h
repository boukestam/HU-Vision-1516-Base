#pragma once

class ValueGrid{
public:
	ValueGrid(double * data, int width, int height);

	int getWidth() const;
	int getHeight() const;

	double getValue(int x, int y) const;
	double getValue(int i) const;

	double getTotalValue() const;

	void setValue(int x, int y, double value) const;
	void setValue(int i, double value) const;

	double * getData() const;

	int getSize() const;
private:
	double * data;
	int width;
	int height;
};