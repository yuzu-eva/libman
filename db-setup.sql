CREATE TABLE anime(
       id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
       name TEXT NOT NULL,
       episode TEXT NOT NULL,
       status TEXT NOT NULL);

CREATE TABLE manga(
       id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
       name TEXT NOT NULL,
       chapter TEXT NOT NULL,
       status TEXT NOT NULL);

CREATE TABLE book(
       id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
       name TEXT NOT NULL,
       author TEXT NOT NULL,
       chapter TEXT NOT NULL,
       status TEXT NOT NULL);
