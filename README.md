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

name,episode,status  

For example:

Samurai Champloo,14,watching  
Bakemonogatari,0,done  
Steins; Gate,1,watching  

and so on. If I'm done watching a series, the episode is set to '0'.  

You need to set the filepath to your csv in the main.c, but everything else
should work out of the box.  
The Makefile uses clang, because that's what I prefer, but you can change it
to gcc in the second line. Or you could simply do  
```
gcc -o myal main.c
```