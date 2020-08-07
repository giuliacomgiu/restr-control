/*Lib headers*/
//g++ main.cpp -l sqlite3
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <ctype.h>
#include <sstream>


/*Classes headers or cpps*/
#include "wallet.cpp"
#include "person.cpp"
#include "dept.cpp"
#include "barcode.cpp"
#include "student.cpp"
#include "staff.cpp"

/*Callback from database*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void getBarcode(Person * member){
    /*Considera que CPF nao pode ser 0*/
    bool is_valid = false;
    int cpf;
    string input;

    while(!is_valid){
        cout << "\n\rEnter Barcode number. Digits only." << endl;
        getline(cin,input,'\n');    
        stringstream(input) >> cpf;
        
        is_valid = true; // exits loop if ok

        if (cpf <= 0){
            is_valid = false;
            cout << "Invalid input. Enter digits only." << endl;
        }
    }

    member->setCPF(cpf);
    return;
}

float getCash(void){
    bool is_valid = false;
    float amount;
    string input;

    while(!is_valid){
        cout << "How much cash to add?" << endl;
        getline(cin,input,'\n');

        try{
            amount = stof(input);
            is_valid = true;
        } catch(const exception& e){
            cout << "Invalid input:" << e << endl;
        }
    }

    return amount;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;

   /* Open database */
    rc = sqlite3_open("user.sqlite", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    Person * new_pers;
    //get barcode
    //select name and credit from database
    //set name and credit to new_pers;
    //get amount of cash to add
    float amount = getCash();
    //Sum and add to db
    //Display XX$ was added to name

    /*Select id (right now, same as barcode)*/
    sql = ""

    sqlite3_close(db);

    return 0;
}