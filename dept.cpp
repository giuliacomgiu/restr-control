#include "dept.h"

using namespace std;


Dept::Dept(){
	dept = "NON";
};

bool Dept::setDept(string input){
	bool error = true;

	if (input == "CTC" || input == "CCE" ||\
		input == "CFM" || input == "CCB"){
		dept = input;
		error = false;
	}
	else {
		dept = "NON";
	}

	return error;
};

string Dept::getDept(void){
	return dept;
};