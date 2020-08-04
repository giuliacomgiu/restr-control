#include "pessoa.h"

Pessoa::Pessoa(){
	//name.fname = ' ';
	//name.lname = ' ';
	credit = 0;
	is_enrolled = false;
};

Pessoa::~Pessoa(){
};

full_name Pessoa::getName(void){
	return name;
};

void Pessoa::setName(void){
	string input;
	bool in_alpha = false;
	
	//Setting first name
	while (!in_alpha){
		
		cout << "\n\rEnter first and middle name (characters only):" << endl;
		getline(cin,input,'\n');
		in_alpha = true;

		//If even a single character isnt alpha, repeat question
		for (int i = 0; i < input.length(); i++){
			if (!isalpha(input[i]) && input[i] != ' '){ in_alpha = false; }
		}

		if (!in_alpha) cout << "All characters must be alphabetic." << endl;
	}
 
	cout << "Input name was " << input << endl;
	name.fname = input;

	//Setting last name
	in_alpha = false;
	while (!in_alpha){
		
		cout << "\n\rEnter last name (characters only):" << endl;
		getline(cin,input,'\n');
		in_alpha = true;

		//If even a single character isnt alpha, repeat question
		for (int i = 0; i < input.length(); i++){
			if (!isalpha(input[i]) && input[i] != ' '){ in_alpha = false; }
		}

		if (!in_alpha) cout << "All characters must be alphabetic." << endl;
	}
 
	cout << "Input name was " << input << endl;
	name.lname = input;
	cout << "Creating member " << name.fname << " " << name.lname << endl;
	return;
};

bool Pessoa::getEnrolled(void){
	return is_enrolled;
};

void Pessoa::setEnrolled(void){
	bool in_valid = false;
	string input;

	while (!in_valid){
		cout << "\n\rIs this person currently enrolled in the university?(y/n)" << endl;
		getline(cin,input,'\n');

		//Checking for valid answer
		if(input == "y" || input == "Y") {
			in_valid = true;
			is_enrolled = true;
		} else if(input == "n" || input == "N") {
			in_valid = true;
			is_enrolled = false;
		} 

		if(!in_valid) cout << "Type only y or n" << endl;
	}
	cout << "Saving " << input << endl;
	return;
};

void Pessoa::addCash(void){
	if (is_enrolled){
		bool in_valid = false;
		string input;

		//Checking for how much money
		while (!in_valid){
			bool has_sep = false;
			string dot = ".";
			string comm = ",";
			in_valid = true;

			cout << "\n\rHow much would you like to add?" << endl;
			getline(cin,input,'\n');
			

			/* FAIL CODE LINE 109
			// Chencking if its valid
			for (int i = 0; i < input.length(); i++){
				
				// Is it one and only cents separator?
				if ((input[i] == dot || input[i] == comm) && !has_sep){
						has_sep = true;
						input[i] = dot;

				//Not number or multiple separators, invalid
				} else if (!isdigit(input[i])){
					in_valid = false; 
				}
			}

			if (!in_valid) cout << "Invalid input." << endl;*/
			try{
				//fix commas
				credit = stof(input); //If this doesnt work, throws an exception
				in_valid = true;
			}
			catch(exception& e){
				//e.what() displays error
				cout << "Invalid input. Type digits and . only." << endl;
				in_valid = false;
			}
		}

		cout << "Added R$" << credit << endl;
	} else cout << "Person not enrolled in uni. Forbinned action." <<endl;
}