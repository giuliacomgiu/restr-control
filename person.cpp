#include "person.h"

Person::Person(){
	is_enrolled = false;
};

Person::~Person(){
};

full_name Person::getName(void){
	return name;
};

void Person::setFName(string input){
	name.fname = input;
	return;
};

void Person::setLName(string input){
	name.lname = input;
	return;
};

bool Person::getEnrolled(void){
	return is_enrolled;
};

void Person::setEnrolled(bool e){
	is_enrolled = e;
	return;
};