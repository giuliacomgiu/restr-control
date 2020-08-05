/*Lib headers*/
//g++ main.cpp -lsqlite3
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


/*Defining functions for registration*/
bool cleanName(string input) {
    bool is_valid = true;

    for (int i = 0; i < input.length(); i++){
        if (!isalpha(input[i]) && !isspace(input[i])){ 
            is_valid = false; 
        }
    }

    if (!is_valid) cout << "All characters must be alphabetic." << endl;
    return is_valid;
}

/*Callback from database*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

   /* Open database */
    rc = sqlite3_open("user.sqlite", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL tables */
    sql = "CREATE TABLE IF NOT EXISTS Pessoa("  \
        "id INT PRIMARY KEY NOT NULL," \
        "name TEXT NOT NULL," \
        "credito REAL NOT NULL," \
        "is_enrolled INTEGER NOT NULL," \
        "is_isento INTEGER NOT NULL ); "\
        \
        "CREATE TABLE IF NOT EXISTS Centro("  \
        "ID INT PRIMARY KEY NOT NULL," \
        "name TEXT NOT NULL," \
        "population INTEGER ); " \
        \
        "CREATE TABLE IF NOT EXISTS Funcao("  \
        "id INT PRIMARY KEY NOT NULL," \
        "name TEXT NOT NULL );";


   /* Commit SQL Table */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
    


    /*Registering people*/
    Staff staff;
    Student stud;
    Person * new_pers;
    string input;
    bool is_valid, is_isent;
    int mode;
    
    cout << "Are you registering staff or student?\n\
    \r(1) for staff\n\r(2) for student" << endl;
    getline(cin,input,'\n');
    stringstream(input) >> mode;
    
    /*Choosing between staff or student*/    
    switch (mode){
        
        /*If staff*/
        case 1:

            /*Registering first name
            Checking for input validity*/
            is_valid = false;
            while (!is_valid){
                cout << "\n\rEnter first name (characters only):" << endl;
                getline(cin,input,'\n');
                
                is_valid = cleanName(input);
                if (!is_valid) cout << "All characters must be alphabetic." << endl;
            }
            staff.setFName(input);

            /*Registering last name
            Checking for input validity*/
            is_valid = false;
            while (!is_valid){
                cout << "\n\rEnter middle and last names (characters only):" << endl;
                getline(cin,input,'\n');
                
                is_valid = cleanName(input);
                if (!is_valid) cout << "All characters must be alphabetic." << endl;
            }
            staff.setLName(input);

            /*Setting enrollment status.
            This script is for enrolled people only*/
            staff.setEnrolled(true);
            break;

        /*If student*/
        case 2:
            /*Registering first name
            Checking for input validity*/
            is_valid = false;
            while (!is_valid){
                
                cout << "\n\rEnter first name (characters only):" << endl;
                getline(cin,input,'\n');
                
                is_valid = cleanName(input);
                if (!is_valid) cout << "All characters must be alphabetic." << endl;
            }

            stud.setFName(input);

            /*Registering last name
            Checking for input validity*/
            is_valid = false;
            while (!is_valid){
                
                cout << "\n\rEnter middle and last names (characters only):" << endl;
                getline(cin,input,'\n');
                
                is_valid = cleanName(input);
                if (!is_valid) cout << "All characters must be alphabetic." << endl;
            }

            stud.setLName(input);
            //Person *p_stud = &stud;

            /*Setting enrollment status.
            This script is for enrolled people only*/
            stud.setEnrolled(true);

            /*Getting isentment from payments*/
            is_valid = false;
            while(!is_valid){
                cout << "\n\rIs the student isent?\n\
                \r(0) for no\n\r(1) for yes" << endl;
                getline(cin,input,'\n');
                
                if(input == "0" || input == "1"){ 
                    stringstream(input) >> is_isent;
                    is_valid = true;
                }
            }
            stud.setIsent(is_isent);

            break;
        
        default:
            cout << "Invalid input." << endl;
            break;
    }

    return 0;
}