# myal - My Anime Library

My Anime Library is a cli-tool written in C to manage an anime watchlist.  
I used to track which anime I have watched and what episode I'm on by writing in
a text file, which got annoying after a while.  
This is why I created a simple program to quickly search and change a
csv file.  

Is it a complete overkill to write this in C? Yes  
Could this have been a simple bash script? Definitely  

But why not? I wanted some practice with C, so if you see any glaring errors or
unsafe code feel free to point it out and roast me.

This cli-tool works with csv files in the format

name,episode  

For example:

Samurai Champloo,14  
Bakemonogatari,done  
Steins; Gate,1  

and so on. The episode is actually stored as a string, so you can put "done" or
"abandoned" or anything else you want in there. The string allows for a maximum
length of 11.  

You need to set the filepath to your csv in the main.c, but everything else
should work out of the box.  
The Makefile uses clang, but you can change it to gcc in the second line. Or
you could simply do

```
gcc -o myal main.c
```