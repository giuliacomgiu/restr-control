//g++ main.cpp -l sqlite3
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <ctype.h>
#include <sstream>


#include "./classes/wallet.cpp"
#include "./classes/person.cpp"
#include "./classes/dept.cpp"
#include "./classes/barcode.cpp"
#include "./classes/student.cpp"
#include "./classes/staff.cpp"


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


void promptName(Person * member){
    
    //FIRST NAME
    bool is_valid = false;
    string input;

    while (!is_valid){
        
        cout << "\n\rEnter first name (characters only):" << endl;
        getline(cin,input,'\n');
        
        is_valid = cleanName(input);
    }
    member->setFName(input);

    //LAST NAME
    is_valid = false;
    while (!is_valid){
        
        cout << "\n\rEnter middle and last names (characters only):" << endl;
        getline(cin,input,'\n');
        
        is_valid = cleanName(input);
    }
    member->setLName(input);

    return;
}

void promptCPF(Person * member){
    
    bool is_valid = false;
    int cpf;
    string input;

    while(!is_valid){
        cout << "\n\rEnter de CPF. Digits only." << endl;
        getline(cin,input,'\n');    
        stringstream(input) >> cpf;
        
        if (cpf > 0) is_valid = true;
        else cout << "Invalid input. Enter digits only." << endl;
    }

    member->setCPF(cpf);
    return;
}

void promptIsent(Student & new_stud){
    
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

    new_stud.setIsent(is_isent);

    return;
}

/*UPSERT (insert, on conflict update), is not
available on this sqlite3 version. I failed to
update it, but when its sucessful, heres the proper way

    sql = "INSERT INTO Pessoa(fname, lname, cpf," 
    " credito, is_enr, enr_mode, is_isento) VALUES ("
    + stringSQL(fname) + ","
    + stringSQL(lname) + ","
    + to_string(cpf) + ","
    + to_string(0) + ","
    + to_string(is_enrolled) + ","
    + to_string(mode) + ","
    + to_string(is_isent)+ ") "
    "ON CONFLICT(cpf) DO UPDATE SET "
    "is_enr=" + to_string(is_enrolled) + ","
    +"enr_mode=" + to_string(mode) + ","
    +"is_isento=" + to_string(is_isent)+
    ";";
*/

static int callbackSQL(void *NotUsed, int argc, char **argv, char **azColName) {
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
    bool DB_OPEN_ERR = false;
    bool DB_EXEC_ERR = false;
    bool INPUT_ERR = false;


    //DATABASE HANDLING
    rc = sqlite3_open("user.sqlite", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        DB_OPEN_ERR = true;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    if (!DB_OPEN_ERR) {
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


        rc = sqlite3_exec(db, sql.c_str(), callbackSQL, 0, &zErrMsg);
       
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            DB_EXEC_ERR = true;
        } else {
            fprintf(stdout, "Table created successfully\n");
        }
    }


    //PROMPTING FOR PEOPLE TO REGISTER
    if (!DB_OPEN_ERR && !DB_EXEC_ERR){

        string input;
    
        Staff new_staff;
        Student new_stud;
        Person * new_pers;
        
        short int mode;
        enum enroll_mode {staff = 1, student = 2};
        bool is_isent = 0;
    
        cout << "Are you registering staff or student?\n\
        \r(1) for staff\n\r(2) for student" << endl;
        getline(cin,input,'\n');
        stringstream(input) >> mode;
        
        switch (mode){
            
            case staff:
                new_pers = &new_staff;
                
                promptCPF(new_pers);
                promptName(new_pers);
                new_pers->setEnrolled(true);
                
                break;
    
            case student:
                new_pers = &new_stud;
                
                promptCPF(new_pers);
                promptName(new_pers);
                new_pers->setEnrolled(true);
                promptIsent(new_stud);
                is_isent = new_stud.getIsent();
    
                break;
            
            default:
                cout << "Invalid input." << endl;
                INPUT_ERR = true;
                break;
        }
    

        if (!INPUT_ERR){

            string fname, lname;
            const short int is_enrolled = 1;
            unsigned int cpf;

            /*Getting common data from input*/
            fname = (new_pers->getName()).fname;
            lname = (new_pers->getName()).lname;
            cpf = new_pers->getCPF();

            /*Inserting person on database*/
            //Use upsertSQL when upsert available
            sql = "INSERT INTO Pessoa(fname, lname, cpf," 
            " credito, is_enr, enr_mode, is_isento) VALUES ("
            + stringSQL(fname) + ","
            + stringSQL(lname) + ","
            + to_string(cpf) + ","
            + to_string(0) + ","
            + to_string(is_enrolled) + ","
            + to_string(mode) + ","
            + to_string(is_isent)+ ");";
            
            cout << sql.c_str() << endl;
        
            rc = sqlite3_exec(db, sql.c_str(), callbackSQL, 0, &zErrMsg);
            
            //IF CPF NOT UNIQUE, UPDATE
            if( rc == SQLITE_CONSTRAINT ){
            
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);

                sql = "UPDATE Pessoa SET "
                "enr_mode=" + to_string(mode) + ", "
                "is_isento=" + to_string(is_isent) + " "
                "WHERE cpf = " + to_string(cpf) + ";";
                cout << "\n" << sql.c_str() << endl;

                rc = sqlite3_exec(db, sql.c_str(), callbackSQL, 0, &zErrMsg);

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
    }

    sqlite3_close(db);

    return 0;
}