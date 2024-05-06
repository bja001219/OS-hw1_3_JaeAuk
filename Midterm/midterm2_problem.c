#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "Console.h"


//#define DEBUG	// comment-out this line after completion

#define TRUE 1
#define FALSE 0

#define ESC 27


#define FIGHTER_LEN 3
char fighter[] = "=B>";

typedef struct {
	int width, height;
	int key;
} FighterInfo;

int cont = TRUE;

void* MoveFighter(void *vparam);

void Erase(int x, int y, int len);
void DrawObject(int x, int y, char *object);


#define MISSLE_LEN 3
char missile[] = "-->";

typedef struct {
	int width;		// screen width
	int sx, sy;		// start coordinate
} MissileInfo;

void Shoot(int sx, int sy, int width);

int main()
{
	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	clrscr();
	gotoxy(1, screen_height + 1);
	printf("screen size = %d x %d. ", screen_width, screen_height);
	printf("press ESC to quit.");

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG

	// create fighter thread
	FighterInfo fighterInfo = {screen_width, screen_height, 0 };
	pthread_t ftid = 0;

	pthread_create(&ftid, NULL, MoveFighter, &fighterInfo);

	while(fighterInfo.key != ESC){
		gotoxy(1, screen_height + 1);
		fighterInfo.key = getch();
	}

	cont = FALSE;

	gotoxy(1, screen_height);
	printf("Terminating fighter thread...");
	fflush(stdout);

	pthread_join(ftid, NULL);
	gotoxy(1, screen_height + 2);
	printf("Bye!\n");

#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void* MoveFighter(void *vparam)
{
	FighterInfo *param = (FighterInfo *)vparam;

	int x = param->width / 2;
	int y = param->height / 2;
	int dx = 0, dy = 0;

	int powerup = FALSE;

	while(cont){
		int prevx = x;
		int prevy = y;
		
		if(param->key != 0){
			switch(param->key){
			case 'i':
				dx = 0;
				dy = -1;
				break;

			case 'j':
				dx = -1;
				dy = 0;
				break;

			case 'k':
				dx = 0;
				dy = 1;
				break;

			case 'l':
				dx = 1;
				dy = 0;
				break;

			case 'p':
				powerup = 1 - powerup;
				break;

			case ' ':
				Shoot(x + FIGHTER_LEN, y, param->width);

				if(powerup){
					if(y > 1)
						Shoot(x + FIGHTER_LEN, y-1, param->width);
					if(y < param->height)
						Shoot(x + FIGHTER_LEN, y+1, param->width);
				}
				break;

			default:
				dx = dy = 0;
				break;
			}

			param->key = 0;
		}

		if(x + dx < 1 || x + dx + FIGHTER_LEN - 1 > param->width)
			dx = 0;
		if(y + dy < 1 || y + dy > param->height)
			dy = 0;

		x += dx;
		y += dy;

		Erase(prevx, prevy, FIGHTER_LEN);
		DrawObject(x, y, fighter);
		fflush(stdout);

		usleep(50000);
	}
}

void Erase(int x, int y, int len)
{
	LockDisplay();

	gotoxy(x, y);
	for(int i = 0; i < len; i++)
		putchar(' ');

	UnlockDisplay();
}

void DrawObject(int x, int y, char *object)
{
	LockDisplay();

	gotoxy(x, y);
	printf("%s", object);

	UnlockDisplay();
}

void Shoot(int sx, int sy, int width)
{
	// implement this function
	MissileInfo missile1 = {sx, sy, width};
	int screen_width = getWindowWidth();
	while(1){
		if(sx < screen_width){
			gotoxy(sx, sy);
			DrawObject(sx, sy, missile);
			sx += 3;
		}else{
			break;
		}
		int prevx = sx;
		usleep(1000);
		gotoxy(sx-3, sy);
		printf(" ");
		gotoxy(sx-2, sy);
		printf(" ");
		gotoxy(sx-1, sy);
		printf(" ");
	}
}
