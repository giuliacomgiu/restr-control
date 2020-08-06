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


/*Defining functions for registration*/
bool cleanName(string &input) {
    bool is_valid = true;

    for (int i = 0; i < input.length(); i++){
        if (!isalpha(input[i]) && !isspace(input[i])){ 
            is_valid = false; 
        }
    }

    if (!is_valid) cout << "All characters must be alphabetic." << endl;
    return is_valid;
}

string stringSQL( string &s ) {
    return string("'") + s + string("'");
}


void getName(Person * member){
    
    /*Registering first name*/
    bool is_valid = false;
    string input;

    while (!is_valid){
        
        cout << "\n\rEnter first name (characters only):" << endl;
        getline(cin,input,'\n');
        
        is_valid = cleanName(input);
    }
    member->setFName(input);

    /*Registering last name*/
        is_valid = false;
    while (!is_valid){
        
        cout << "\n\rEnter middle and last names (characters only):" << endl;
        getline(cin,input,'\n');
        
        is_valid = cleanName(input);
    }
    member->setLName(input);

    return;
}

void getCPF(Person * member){
    /*Considera que CPF nao pode ser 0*/
    bool is_valid = false;
    int cpf;
    string input;

    while(!is_valid){
        cout << "\n\rEnter de CPF. Digits only." << endl;
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

void getIsent(Student & stud){
    bool is_valid = false;
    bool is_isent = false;
    string input;

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

    return;
}

/*UPSERT (insert, on conflict update), is not
available on this sqlite3 version. I failed to
update it, but when its sucessful, heres the proper way

    sql = "INSERT INTO Pessoa(fname, lname, cpf," 
    " credito, is_enr, enr_mode, is_isento) VALUES ("
    + stringSQL(fname_s) + ","
    + stringSQL(lname_s) + ","
    + to_string(cpf) + ","
    + to_string(0) + ","
    + to_string(isenr_s) + ","
    + to_string(mode) + ","
    + to_string(is_isent)+ ") "
    "ON CONFLICT(cpf) DO UPDATE SET "
    "is_enr=" + to_string(isenr_s) + ","
    +"enr_mode=" + to_string(mode) + ","
    +"is_isento=" + to_string(is_isent)+
    ";";
*/

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
    string sql;

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
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
        "fname TEXT NOT NULL," \
        "lname TEXT NOT NULL," \
        "cpf INTEGER UNIQUE," \
        "credito REAL ," \
        "is_enr INTEGER ," \
        "enr_mode INTEGER ," \
        "is_isento INTEGER  ); "\
        \
        "CREATE TABLE IF NOT EXISTS Centro("  \
        "ID INTEGER PRIMARY KEY NOT NULL," \
        "name TEXT NOT NULL," \
        "population INTEGER ); " \
        \
        "CREATE TABLE IF NOT EXISTS Funcao("  \
        "id INTEGER PRIMARY KEY NOT NULL," \
        "name TEXT NOT NULL );";


   /* Commit SQL Table */
   rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }


    /*Registering people*/

    /*Error variable*/
    bool inputErr = false;

    /*Parsing variables*/
    Staff stff;
    Student stud;
    Person * new_pers;
    short int mode;
    enum modeEnroll {staff = 1, student = 2};
    bool is_isent = 0;
    string input;

    cout << "Are you registering staff or student?\n\
    \r(1) for staff\n\r(2) for student" << endl;
    getline(cin,input,'\n');
    stringstream(input) >> mode;
    
    /*Choosing between staff or student*/    
    switch (mode){
        
        /*If staff*/
        case staff:
            new_pers = &stff;
            getCPF(new_pers);
            getName(new_pers);
            
            /*This script is for enrolled people only*/
            new_pers->setEnrolled(true);
            break;

        /*If student*/
        case student:
            new_pers = &stud;
            getCPF(new_pers);
            getName(new_pers);

            /*This script is for enrolled people only*/
            new_pers->setEnrolled(true);

            /*Getting isentment from payments*/
            getIsent(stud);
            is_isent = stud.getIsent();

            break;
        
        default:
            cout << "Invalid input." << endl;
            inputErr = true;
            break;
    }

    if (!inputErr){

        string fname_s, lname_s;
        const short int isenr_s = 1;
        unsigned int cpf;

        /*Getting common data from input*/
        fname_s = (new_pers->getName()).fname;
        lname_s = (new_pers->getName()).lname;
        cpf = new_pers->getCPF();

        /*Inserting person on database*/
        //Use upsertSQL when upsert available
        sql = "INSERT INTO Pessoa(fname, lname, cpf," 
        " credito, is_enr, enr_mode, is_isento) VALUES ("
        + stringSQL(fname_s) + ","
        + stringSQL(lname_s) + ","
        + to_string(cpf) + ","
        + to_string(0) + ","
        + to_string(isenr_s) + ","
        + to_string(mode) + ","
        + to_string(is_isent)+ ");";
        
        cout << sql.c_str() << endl;
    
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
       
        if( rc == SQLITE_CONSTRAINT ){
        
        /*If cpf wasnt unique, update */
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);

            sql = "UPDATE Pessoa SET "
            "enr_mode=" + to_string(mode) + ", "
            "is_isento=" + to_string(is_isent) + " "
            "WHERE cpf = " + to_string(cpf) + ";";
            cout << "\n" << sql.c_str() << endl;

            rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

            if(rc != SQLITE_OK){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                fprintf(stdout, "Records created successfully\n");
            }


        } else if (rc != SQLITE_OK){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records created successfully\n");
        }
    }
       
    sqlite3_close(db);

    return 0;
}