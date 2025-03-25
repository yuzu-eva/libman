#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

const char *filename = "./library.db";

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i = 0; i < argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void exit_with_error(sqlite3 *db, const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(69);
}

void select_from_table(sqlite3 *db, char *tblName, char *qp)
{
    char *sql;
    int rc;

    const unsigned char *name, *value, *status;
    const char *type;
    char *query_param = malloc(strlen(qp) + 2);
    int id;


    if (!strcmp(tblName, "anime")) {
        type = "Episode";
        sql = "SELECT * FROM anime WHERE NAME LIKE ?1 ORDER BY ID;";
    } else if (!strcmp(tblName, "manga")) {
        type = "Chapter";
        sql = "SELECT * manga WHERE NAME LIKE ?1 ORDER BY ID;";
    } else {
        fprintf(stderr, "table does not exist...\n");
        sqlite3_close(db);
        exit(69);
    }

    sqlite3_stmt *stmt;
    snprintf(query_param, strlen(qp) + 2, "%s%%", qp);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        exit_with_error(db, "failure fetching data: ");
    }

    sqlite3_bind_text(stmt, 1, query_param, -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
        name = sqlite3_column_text(stmt, 1);
        value = sqlite3_column_text(stmt, 2);
        status = sqlite3_column_text(stmt, 3);
        printf("%03d: %s, %s %s, %s\n", id, name, type, value, status);
    }

    sqlite3_finalize(stmt);
    free(query_param);
}

void update_entry(sqlite3 *db, char *tblName, char *qp, char *value, char *status)
{
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    if (!strcmp(tblName, "anime")) {
        if (status == NULL) {
            sql = "UPDATE anime SET EPISODE=?1 WHERE NAME=?2;";
        } else {
            sql = "UPDATE anime SET EPISODE=?1, STATUS=?2 WHERE NAME=?3;";
        }
    } else if (!strcmp(tblName, "manga")) {
        if (status == NULL) {
            sql = "UPDATE manga SET CHAPTER=?1 WHERE NAME=?2;";
        } else {
            sql = "UPDATE manga SET CHAPTER=?1, STATUS=?2 WHERE NAME=?3;";
        }
    } else {
        fprintf(stderr, "table does not exist...\n");
        sqlite3_close(db);
        exit(69);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        exit_with_error(db, "failure fetching data: ");
    }

    if (status == NULL) {
        sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, qp, -1, SQLITE_STATIC);
    } else {
        sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, status, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, qp, -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void add_entry(sqlite3 *db, char *tblName, char *name, char *value, char *status)
{
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    if (!strcmp(tblName, "anime")) {
        sql = "INSERT INTO anime (NAME, EPISODE, STATUS) VALUES (?1, ?2, ?3);";
    } else if (!strcmp(tblName, "manga")) {
        sql = "INSERT INTO manga (NAME, CHAPTER, STATUS) VALUES (?1, ?2, ?3);";
    } else {
        fprintf(stderr, "table does not exist...\n");
        sqlite3_close(db);
        exit(69);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        exit_with_error(db, "failure fetching data: ");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, status, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr, "missing argument...\n");
        exit(69);
    }
    char *mode, *target, *name, *value, *status;
    mode = argv[1];
    target = argv[2];
    name = argv[3];

    sqlite3 *db;
    int rc;

    rc = sqlite3_open(filename, &db);

    if (rc) {
        exit_with_error(db, "Can't open database: ");
    }

    if (!strcmp(mode, "get")) {
        select_from_table(db, target, name);
    } else if (!strcmp(mode, "set")) {
        if (argc < 5) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        if (argc == 5) {
            value = argv[4];
            update_entry(db, target, name, value, NULL);
        } else if (argc == 6) {
            value = argv[4];
            status = argv[5];
            update_entry(db, target, name, value, status);
        }
    } else if (!strcmp(mode, "add")) {
        if (argc < 6) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        value = argv[4];
        status = argv[5];
        add_entry(db, target, name, value, status);
    } else {
        fprintf(stderr, "unknown option...\n");
        sqlite3_close(db);
        exit(69);
    }

    sqlite3_close(db);
    return 0;
}
