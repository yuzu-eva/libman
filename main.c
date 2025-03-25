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

void select_from_table(sqlite3 *db, char *table_name, char *q_param)
{
    char *zErrMsg = 0;
    char *sql;
    int rc;

    const unsigned char *name, *episode, *status;
    char *type;

    if (!strcmp(table_name, "anime")) {
        strcpy(type, "Episode");
        sql = "SELECT NAME, EPISODE, STATUS FROM anime WHERE NAME LIKE ?1 ORDER BY ID;";
    } else if (!strcmp(table_name, "manga")) {
        strcpy(type, "Chapter");
        sql = "SELECT NAME, CHAPTER, STATUS FROM manga WHERE NAME LIKE ?1 ORDER BY ID;";
    } else {
        fprintf(stderr, "table does not exist...\n");
        sqlite3_close(db);
        exit(69);
    }

    sqlite3_stmt *stmt;
    sprintf(q_param, "%s%%", q_param);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        exit_with_error(db, "failure fetching data: ");
    }

    sqlite3_bind_text(stmt, 1, q_param, -1, SQLITE_STATIC);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        name = sqlite3_column_text(stmt, 0);
        episode = sqlite3_column_text(stmt, 1);
        status = sqlite3_column_text(stmt, 2);
        printf("%s, %s %s, %s\n", name, type, episode, status);
    }

    sqlite3_finalize(stmt);
}

void update_ep(sqlite3 *db, char *table_name, char *q_param, char *value)
{
    char *zErrMsg = 0;
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    if (!strcmp(table_name, "anime")) {
        sql = "UPDATE anime SET EPISODE=?1 WHERE NAME=?2;";
    } else if (!strcmp(table_name, "manga")) {
        sql = "UPDATE manga SET CHAPTER=?1 WHERE NAME=?2;";
    } else {
        fprintf(stderr, "table does not exist...\n");
        sqlite3_close(db);
        exit(69);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        exit_with_error(db, "failure fetching data: ");
    }

    sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, q_param, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void update_status(sqlite3 *db, char *table_name, char *q_param, char *value)
{
    char *zErrMsg = 0;
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    if (!strcmp(table_name, "anime")) {
        sql = "UPDATE anime SET STATUS=?1 WHERE NAME=?2;";
    } else if (!strcmp(table_name, "manga")) {
        sql = "UPDATE manga SET STATUS=?1 WHERE NAME=?2;";
    } else {
        fprintf(stderr, "table does not exist...\n");
        sqlite3_close(db);
        exit(69);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        exit_with_error(db, "failure fetching data: ");
    }

    sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, q_param, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void add_entry(sqlite3 *db, char *table_name, char *name, char *episode, char *status)
{
    char *zErrMsg = 0;
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    if (!strcmp(table_name, "anime")) {
        sql = "INSERT INTO anime (NAME, EPISODE, STATUS) VALUES (?1, ?2, ?3);";
    } else if (!strcmp(table_name, "manga")) {
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
    sqlite3_bind_text(stmt, 2, episode, -1, SQLITE_STATIC);
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
    char *mode, *target, *name, *episode, *status;
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
        episode = argv[4];
        update_ep(db, target, name, episode);
    } else if (!strcmp(mode, "status")) {
        if (argc < 5) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        status = argv[4];
        update_status(db, target, name, status);
    } else if (!strcmp(mode, "add")) {
        if (argc < 6) {
            fprintf(stderr, "missing argument...\n");
            sqlite3_close(db);
            exit(69);
        }
        episode = argv[4];
        status = argv[5];
        add_entry(db, target, name, episode, status);
    } else {
        fprintf(stderr, "unknown option...\n");
        sqlite3_close(db);
        exit(69);
    }

    sqlite3_close(db);
    return 0;
}
