#include "dbhandling.h"
#include "enum.h"

void exit_with_error(sqlite3 *db, const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(69);
}

void select_from_table(sqlite3 *db, args_e target, char *qp)
{
    char *sql;
    int rc;
    int found = 0;

    const unsigned char *name, *author, *value, *status;
    const char *type;
    char *query_param = malloc(strlen(qp) + 2);

    switch (target) {
    case ANIME:
        type = "Episode";
        sql = "SELECT * FROM anime WHERE name LIKE ?1 ORDER BY id;";
        break;
    case MANGA:
        type = "Chapter";
        sql = "SELECT * FROM manga WHERE name LIKE ?1 ORDER BY id;";
        break;
    case BOOK:
        type = "Chapter";
        sql = "SELECT * FROM book WHERE name LIKE ?1 ORDER BY id;";
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

    if (target == BOOK) {
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            name = sqlite3_column_text(stmt, 1);
            author = sqlite3_column_text(stmt, 2);
            value = sqlite3_column_text(stmt, 3);
            status = sqlite3_column_text(stmt, 4);
            printf("%s - %s, %s %s, %s\n", author, name, type, value, status);
            found = 1;
        }
    } else {
        while(sqlite3_step(stmt) == SQLITE_ROW) {
            name = sqlite3_column_text(stmt, 1);
            value = sqlite3_column_text(stmt, 2);
            status = sqlite3_column_text(stmt, 3);
            printf("%s, %s %s, %s\n", name, type, value, status);
            found = 1;
        }
    }

    if (!found) {
        printf("no entry found...\n");
    }

    sqlite3_finalize(stmt);
    free(query_param);
}

void update_entry(sqlite3 *db, args_e target, char *qp, char *value, char *status)
{
    char *sql;
    int rc;
    int res;

    sqlite3_stmt *stmt;

    switch (target) {
    case ANIME:
        if (status == NULL) {
            sql = "UPDATE anime SET episode=?1 WHERE name=?2;";
        } else {
            sql = "UPDATE anime SET episode=?1, status=?2 WHERE name=?3;";
        }
        break;
    case MANGA:
        if (status == NULL) {
            sql = "UPDATE manga SET chapter=?1 WHERE name=?2;";
        } else {
            sql = "UPDATE manga SET chapter=?1, status=?2 WHERE name=?3;";
        }
        break;
    case BOOK:
        if (status == NULL) {
            sql = "UPDATE book SET chapter=?1 WHERE name=?2;";
        } else {
            sql = "UPDATE book SET chapter=?1, status=?2 WHERE name=?3;";
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

void add_entry(sqlite3 *db, args_e target, char *name, char *author, char *value, char *status)
{
    char *sql;
    int rc;

    sqlite3_stmt *stmt;

    switch (target) {
    case ANIME:
        sql = "INSERT INTO anime (name, episode, status) VALUES (?1, ?2, ?3);";
        break;
    case MANGA:
        sql = "INSERT INTO manga (name, chapter, status) VALUES (?1, ?2, ?3);";
        break;
    case BOOK:
        sql = "INSERT INTO book (name, author, chapter, status) VALUES (?1, ?2, ?3, ?4);";
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

    if (target == BOOK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, author, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, value, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, status, -1, SQLITE_STATIC);
    } else {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, status, -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    printf("Entry %s added!\n", name);
    sqlite3_finalize(stmt);
}
