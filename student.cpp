#include "student.h"

Student::Student(){
	is_isent = false;
};

bool Student::getIsent(void){
	return is_isent;
};

void Student::setIsent(bool i){
	is_isent = i;
	return;
};