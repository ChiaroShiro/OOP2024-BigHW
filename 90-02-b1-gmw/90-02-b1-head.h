/* 2351871 ¿…»Ùπ» º∆ø∆ */

#pragma once

#define COLOR_CATES			5

const char* const GRAPH[] = { "®X®T®[",
							  "®U°Ô®U",
							  "®^®T®a" };

int isErasable(int n, int m, int x, int y, const int map[][MAP_SIZE], int sta[][MAP_SIZE]);
void drawStatus(int n, int m, int sta[][MAP_SIZE], const char* s);
void gaming(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], int optChoose, StyleCSS style, int &xborder, int &yborder);
bool isPossible(int n, int m, const int map[][MAP_SIZE], const int sta[][MAP_SIZE]);
void squeezeBall(int n, int m, int map[][MAP_SIZE], int sta[][MAP_SIZE], bool showGraph = 0, void (*slideLeftBall)(int, int, int, int, int) = NULL);

void AnimeOption(int n, int m, int optChoos);