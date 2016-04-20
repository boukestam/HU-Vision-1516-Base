#include "Mask.h"

Mask::Mask(double * data, int width, int height) : data{ data }, width{ width }, height{height}{
	
}

int Mask::getWidth(){
	return width;
}

int Mask::getHeight(){
	return height;
}

double Mask::getWeight(int x, int y){
	return data[x + y * width];
}

double Mask::getTotalWeight(){
	const int numWeights = width*height;
	double total=0;
	for (int i = 0; i < numWeights; i++){
		total += data[i];
	}
	return total;
}