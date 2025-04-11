# libman - Library Manager

## Overview

Library Manager is a CLI-Tool to manage a library of anime and manga.  
I used to track which anime I have watched and what episode I'm on by
writing in a text file, which got annoying after a while.  
This is why I created this tool to keep everything organized in a
sqlite3 database.  

## Quickstart

Only requires a C compiler and sqlite3. Should be installed on every
system be default.  
You need a database with tables `anime` and `manga`. Attached in this
repo is a setup script for the database. Simply execute
```
sqlite3 library.db <db-setup.sql
```

This will create the following tables:  
anime(ID integer pk, NAME text, EPISODE text, STATUS text)  
manga(ID integer pk, NAME text, CHAPTER text, STATUS text)  

Chapters and episodes are stored as text, because sometimes the chapter
isn't just a number but rather in the form "Volume 6 Chapter 4",
shortened to "V6CH4". When I'm done with an anime or manga, I set the
episode or chapter to 0 and set the status to "done".  

The database file is expected to be in
"~/.local/share/sqlite/library.db".  
To compile, do
```bash
make
```

&nbsp;

To install it in /usr/local/bin/, do
```bash
sudo make install
```

## Usage

Currently there are 3 modes:
- get, which retrieves one or more anime or manga by the given name
argument
- set, which updates the episode or chapter and/or the status of a given
entry
- add, which adds a new entry to a given table

Here are examples for each mode:

```
libman get anime jojo
```

```
libman set manga "Nikubami Honegishimi" 3
libman set anime "Lucky Star" done
libman set anime "Durarara!!" 13 "watching"
```

```
libman add anime "Detective Conan" 1 "not started"
```
