#ifndef _DBHANDLING_
#define _DBHANDLING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

#include "enum.h"

void exit_with_error(sqlite3 *db, const char *msg);
void select_from_table(sqlite3 *db, target_e target, char *qp);
void update_entry(sqlite3 *db, target_e target, char *qp, char *value, char *status);
void add_entry(sqlite3 *db, target_e target, char *name, char *value, char *status);

#endif
