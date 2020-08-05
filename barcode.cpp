#include "barcode.h"

using namespace std;

Barcode::Barcode(){
	code = 0;
};

void Barcode::setCode(unsigned int c){
	code = c;
	return;
};

unsigned int Barcode::getCode(void){
	return code;
};