#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>


#include "Console.h"


//#define DEBUG			// comment out this line after completion


#define TRUE 1
#define FALSE 0

void DropWord(char *word, int idx, int no_word, int width, int height);
void Erase(int sx, int sy, int len);

int main(int argc, char *argv[])
{
	if(argc == 1){
		printf("Usage: %s <word1> <word2> ...\n", argv[0]);
		return 0;
	}
	clrscr();

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	gotoxy(1, screen_height + 1);
	printf("screen size = %dx%d.\n", screen_width, screen_height);

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG


	// write your code here and do not modify other parts
	int no_word = 1;
	int index = 0;
	for(int i = 1; i <= argc; i++){
		DropWord(argv[i], index, no_word, screen_width, screen_height);
		no_word++;
		index++;
	}

	gotoxy(1, screen_height + 2);
	printf("Bye!\n");

#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void DropWord(char *word, int idx, int no_word, int width, int height)
// drops a word from top to bottom.
// read the code to understand the parameters
{
	int len = strlen(word);
	int x = width * idx / (no_word + 1);
	for(int y = 1; y < height; y++){

		LockDisplay();

		if(y > 1)
			Erase(x, y - 1, len);

		gotoxy(x, y);
		printf("%s", word);

		fflush(stdout);

		UnlockDisplay();

		usleep(100000);
	}
}

void Erase(int sx, int sy, int len)
{
	gotoxy(sx, sy);
	for(int i = 0; i < len; i++)
		putchar(' ');
}
