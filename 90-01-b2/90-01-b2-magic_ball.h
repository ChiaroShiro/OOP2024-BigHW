/*2351871 ¿…»Ùπ» º∆ø∆*/
#pragma once

#include "../include/matrix.h"

#define INIT_X_SIZE			100
#define INIT_Y_SIZE			25
#define X_LIM				40
#define Y_LIM				14
#define SMALL_GAP			5
#define MID_GAP				30
#define BIG_GAP				400

#define BALL_CATEGORY_SIZE	9

#define CLICK_UNABLE		0
#define CLICK_SINGLE		1
#define CLICK_DOUBLE		2



struct Pair {
	int x;
	int y;
};

void mainWork(int n, int m, bool isLoop = 1);

void baseOptionInit(int n, int m);
void baseOptionDelete(int n, int m);
void baseOptionDeleteHint(int n, int m);

void graphOptionInitBorder(int n, int m, bool showBorder);
void graphOptionDelete(int n, int m);
void graphOptionDeleteHint(int n, int m);

bool findBlock(int n, int m, int map[][MAP_SIZE], int res[][MAP_SIZE]);
bool findAvailable(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE]);
bool tryFindBlock(int n, int m, int map[][MAP_SIZE]);
bool tryFindAvailable(int n, int m, int map[][MAP_SIZE]);

void eliminateBall(int x, int y, int val);
void slideDownBall(int n, int m, int x, int y, int val);