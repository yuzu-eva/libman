#include "dbhandling.h"
#include "enum.h"

void exit_with_error(sqlite3 *db, const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(69);
}

void select_from_table(sqlite3 *db, target_e target, char *qp)
{
    char *sql;
    int rc;
    int found = 0;

    const unsigned char *name, *value, *status;
    const char *type;
    char *query_param = malloc(strlen(qp) + 2);

    switch (target) {
    case ANIME:
        type = "Episode";
        sql = "SELECT * FROM anime WHERE NAME LIKE ?1 ORDER BY ID;";
        break;
    case MANGA:
        type = "Chapter";
        sql = "SELECT * FROM manga WHERE NAME LIKE ?1 ORDER BY ID;";
        break;
    default:
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
        name = sqlite3_column_text(stmt, 1);
        value = sqlite3_column_text(stmt, 2);
        status = sqlite3_column_text(stmt, 3);
        printf("%s, %s %s, %s\n", name, type, value, status);
        found = 1;
    }

    if (!found) {
        printf("no entry found...\n");
    }

    sqlite3_finalize(stmt);
    free(query_param);
}

void update_entry(sqlite3 *db, target_e target, char *qp, char *value, char *status)
{
    char *sql;
    int rc;
    int res;

    sqlite3_stmt *stmt;

    switch (target) {
    case ANIME:
        if (status == NULL) {
            sql = "UPDATE anime SET EPISODE=?1 WHERE NAME=?2;";
        } else {
            sql = "UPDATE anime SET EPISODE=?1, STATUS=?2 WHERE NAME=?3;";
        }
        break;
    case MANGA:
        if (status == NULL) {
            sql = "UPDATE manga SET CHAPTER=?1 WHERE NAME=?2;";
        } else {
            sql = "UPDATE manga SET CHAPTER=?1, STATUS=?2 WHERE NAME=?3;";
        }
        break;
    default:
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
    res = sqlite3_changes(db);
    if (!res) {
        printf("no entry found...\n");
    }
    sqlite3_finalize(stmt);
}

void add_entry(sqlite3 *db, target_e target, char *name, char *value, char *status)
{
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    switch (target) {
    case ANIME:
        sql = "INSERT INTO anime (NAME, EPISODE, STATUS) VALUES (?1, ?2, ?3);";
        break;
    case MANGA:
        sql = "INSERT INTO manga (NAME, CHAPTER, STATUS) VALUES (?1, ?2, ?3);";
        break;
    default:
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
    printf("Entry %s added!\n", name);
    sqlite3_finalize(stmt);
}
