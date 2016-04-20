#include "ValueGrid.h"

ValueGrid::ValueGrid(double * data, int width, int height) : data{ data }, width{ width }, height{ height }{
	
}

int ValueGrid::getWidth() const{
	return width;
}

int ValueGrid::getHeight() const{
	return height;
}

double ValueGrid::getValue(int x, int y) const{
	return data[x + y * width];
}

double ValueGrid::getTotalValue() const{
	const int numWeights = width*height;
	double total=0;
	for (int i = 0; i < numWeights; i++){
		total += data[i];
	}
	return total;
}