# myal - My Anime Library

My Anime Library is a cli-tool written in C to manage an anime and manga list.  
I used to track which anime I have watched and what episode I'm on by writing in
a text file, which got annoying after a while.  
This is why I created this tool to keep anime and manga organized in a sqlite3
database.  

Is it a complete overkill to write this in C? Yes  
But why not? I wanted some practice with C, so if you see any glaring errors or
unsafe code feel free to point it out and roast me.  

You need a database with tables `anime` and `manga`. Attached in this repo
is a setup script for the database. Simply execute
```
sqlite3 <db-setup.sql
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
