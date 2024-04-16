README.txt
Written by Erik Losin and Isak Ahlberg in "time4io.c" (inputs.c) and "labwork.S". 
The following files has been modified: 
"mipslabwork.c", "mipslabmain.c" (flappymain.c), "mipslabfunc.c"(flappyfunctions.c), "mipslabdata.c"(flappydata.c), "mipslab.h"(flappy.h) All files was written 2015 by F Lundevall
 Some parts are original code written by Axel Isaksson. The files "vectors.S", "Stubs.c" is not modified and was also written by F lundevall and Axel Isaksson.

The game was created to be played on the chipKIT Uno32 board and the chipKit basic I/O.

Run and compile:
To run and compile the game you need hte MSYS terminal before you look following link: 
"http://github.com/is1200-example-projects/mcb32tools/releases/"

After you have followed every step in the tutorial you should see "[Mcb32]" above the command line. When you have located the exact file directory for the game you run this command:
make && make install TTYDEV=/dev/ttyS(number depending on which port on your computer for the chipKIT).

The game should compile without a error code. 

How to play the game:
Btn 4 gives the character the ability to jump.
At startup a menu will be displayed to play the game you need to hold down btn4 for 1 sec. When you die, your score is shown on the screen to play the game again you need to hold down the same button for 1 second.   
