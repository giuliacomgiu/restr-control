//g++ main.cpp -lsqlite3
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <ctype.h>
#include "pessoa.cpp"

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
    

    Pessoa membro;
    membro.setName();
    membro.setEnrolled();
    membro.addCash();

    return 0;
}