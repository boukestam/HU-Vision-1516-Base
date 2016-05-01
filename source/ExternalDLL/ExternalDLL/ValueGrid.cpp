#include "ValueGrid.h"

ValueGrid::ValueGrid(double * data, int width, int height) : data{ data }, width{ width }, height{ height }{
	
}

int ValueGrid::getWidth() const{
	return width;
}

int ValueGrid::getHeight() const{
	return height;
}

int ValueGrid::getSize() const{
	return width*height;
}

double ValueGrid::getValue(int x, int y) const{
	return data[x + y * width];
}

double ValueGrid::getValue(int i) const{
	return data[i];
}

double ValueGrid::getTotalValue() const{
	const int numWeights = width*height;
	double total=0;
	for (int i = 0; i < numWeights; i++){
		total += data[i];
	}
	return total;
}

void ValueGrid::setValue(int x, int y, double value) const{
	data[x + y * width] = value;
}

void ValueGrid::setValue(int i, double value) const{
	data[i] = value;
}

double * ValueGrid::getData() const{
	return data;
}
