# myal - My Anime Library

## Overview

My Anime Library is a CLI-Tool to manage an anime and manga list.  
I used to track which anime I have watched and what episode I'm on by writing in
a text file, which got annoying after a while.  
This is why I created this tool to keep anime and manga organized in a sqlite3
database.  

## Quickstart

You need a database with tables `anime` and `manga`. Attached in this repo
is a setup script for the database. Simply execute
```
sqlite3 library.db <db-setup.sql
```

This will create the following tables:  
anime(ID integer pk, NAME text, EPISODE text, STATUS text)  
manga(ID integer pk, NAME text, CHAPTER text, STATUS text)  

Chapters and episodes are stored as text, because sometimes the chapter isn't
just a number but rather in the form "Volume 6 Chapter 4", shortened to "V6CH4".
When I'm done with an anime or manga, I set the episode or chapter to 0 and set
the status to "done".  

You need to set the filepath to your db-file in the main.c, but everything else
should work out of the box.  
The Makefile uses clang, because that's what I prefer, but you can change it
to gcc in the second line. Or you could simply do  
```
gcc -o myal main.c -lsqlite3
```

## Usage

Currently there are 3 modes:
- get, which retrieves one or more anime or manga by the given name argument
- set, which updates the episode or chapter and/or the status of a given entry
- add, which adds a new entry to a given table

Here are examples for each mode:

```
myal get anime jojo
```

```
myal set manga "Nikubami Honegishimi" 3
myal set anime "Lucky Star" done
myal set anime "Durarara!!" 13 "watching"
```

```
myal add anime "Detective Conan" 1 "not started"
```
