#include "myLib.h"
#include <stdlib.h>
#include "maze2.h"
#include "text.h"
#include "runner.h"
#include "won.h"
#include <stdio.h>
#include "gameover.h"

#define NUMOBJS 5
typedef struct
{
	int row;
	int col;
	int size;
	int speed;
	u16 color;
} MOVOBJ;

typedef struct
{
	int row;
	int col;
	int size;
	int speed;
	u16 color;
} player;

enum GBAState {
	START,
	START_NODRAW,
	START_NODRAW2,
};


void drawimage3(int row, int col, int height, int width, const unsigned short *image) {
	for(int r = 0; r < height; r++) {
		DMA[3].src = &(*image) + ((r)*(width));
		DMA[3].dst = (videoBuffer) + ((row + r)*(240) + col);
		DMA[3].cnt = (width) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}

int xCollision(int x, int y, int speed) {
    if (speed > 0) {
        if (x + y - 1> 240) {
            return 1;
        }
    } else if (speed < 0) {
        if (x + speed < 0) {
            return 1;
        }
    }
    return 0;
}

int yCollision(int y, int x, int speed) {
    if (speed > 0) {
        if (y + x -1 > 160) {
            return 1;
        }
    } else if (speed < 0) {
        if (y + speed < 0) {
            return 1;
        }
    }
    return 0;
}


int horizontalCollision(int y, int x, int size, int speed) {
	if (speed > 0) {
		for(int i = 0; i < size-2; i++) {
			u16 color = getLineColor(y + i, x+size-2);
			if(color == BLACK) {
				return 1;
			}
			if (color == BLUE) {
				return 2;
			}
			if (color == GREEN) {
				return 3;
			}
		}
	} else if (speed < 0) {
		for(int i = 0; i < size-2; i++) {
			u16 color = getLineColor(y + i, x-1);
			if(color == BLACK) {
				return 1;
			}
			if (color == BLUE) {
				return 2;
			}
			if (color == GREEN) {
				return 3;
			}
		}
	}
	return 0;
}


int verticalCollision(int y, int x, int size, int speed) {
	if (speed > 0) {
		for(int i = 0; i < size-2; i++) {
			u16 color = getLineColor(y + size -2, x + i);
			if(color == BLACK) {
				return 1;
			}
			if (color == BLUE) {
				return 2;
			}
			if (color == GREEN) {
				return 3;
			}
		}
	} else if (speed < 0) {
		for(int i = 0; i < size-2; i++) {
			u16 color = getLineColor(y - 1, x + i);
			if(color == BLACK) {
				return 1;
			}
			if (color == BLUE) {
				return 2;
			}
			if (color == GREEN) {
				return 3;
			}
		}
	}
	return 0;
}



int main()
{

	REG_DISPCTL = MODE3 | BG2_ENABLE;
	enum GBAState state = START;
	char message[] = "Press Enter To Start the Game!";
	int enable = 1;	
	int previous = 0;
	int gameflag = 1;
	drawimage3(0 , 0, RUNNER_HEIGHT, RUNNER_WIDTH, runner);
	while(1) {
		switch(state) {
		case START:
			waitForVblank();
			if (previous == 1) {
				if(KEY_DOWN_NOW(BUTTON_START)) {
					if (enable == 0) {
					state = START_NODRAW;
					previous = 1;
					enable = 1;
					}
				} else if (!(KEY_DOWN_NOW(BUTTON_START))) {
					waitForVblank();
					if (enable) {
						drawimage3(0 , 0, RUNNER_HEIGHT, RUNNER_WIDTH, runner);
						drawString(110, 30, message, YELLOW);
						enable  = 0;
					} 
				}
			} else if(previous == 0) {
					waitForVblank();
					if(KEY_DOWN_NOW(BUTTON_START)) {
						state = START_NODRAW;
						previous = 1;
						enable = 1;
					}
					drawString(110, 30, message, YELLOW);
			}
			break;
		case START_NODRAW:
			if(!(KEY_DOWN_NOW(BUTTON_SELECT))) {
				waitForVblank();
				if (enable) {
				player player1;	
				player1.row = 80;
				player1.col = 5;
				player1.speed = 1;
				player1.size = 7;
				player1.color = RED;
				player oldplayer = player1;

				MOVOBJ gate;
				gate.row = 75;
				gate.col = 80;
				gate.color = GREEN;
				gate.size = 5;


				MOVOBJ objs[NUMOBJS];
				MOVOBJ oldobjs[NUMOBJS];
				MOVOBJ *cur;
				int bgcolor = WHITE;
				drawimage3(0 , 0, MAZE2_HEIGHT, MAZE2_WIDTH, maze2);
				for(int i=0; i<NUMOBJS; i++)
				{	
				if (i == 0) {
					objs[i].row = 40;
					objs[i].col = 90;
					objs[i].speed = 1;	
				}

				if (i == 1) {
					objs[i].row = 115;
					objs[i].col = 105;
					objs[i].speed = 2;
				}

				if (i == 2) {
					objs[i].row = 40;
					objs[i].col = 120;
					objs[i].speed = 1;
				}

				if (i == 3) {
					objs[i].row = 115;
					objs[i].col = 135;
					objs[i].speed = 2;
				}
	
				if (i == 4) {
					objs[i].row = 40;
					objs[i].col = 150;
					objs[i].speed = 1;
				}

				objs[i].color = BLUE;
				objs[i].size = 5;
				oldobjs[i] = objs[i];
			}
			int size = 5;	
			int oldsize = size;
				while(1) {
					if ((KEY_DOWN_NOW(BUTTON_SELECT))) {
							gameflag = 4;
						
					} 
					if(KEY_DOWN_NOW(BUTTON_RIGHT))
					{
						if(horizontalCollision(player1.row, player1.col, player1.size, player1.speed) == 1) {

						} else if(horizontalCollision(player1.row, player1.col, player1.size, player1.speed) == 2) {
							gameflag = 0;
						} else if(horizontalCollision(player1.row, player1.col, player1.size, player1.speed) == 3) {
							gameflag = 2;
						} else if (xCollision(player1.col, player1.size, player1.speed) == 1) {

						}  else {
							player1.col += player1.speed;
						}
					}
					if(KEY_DOWN_NOW(BUTTON_LEFT))
					{	
						if(horizontalCollision(player1.row, player1.col, player1.size, -1) == 1) {

						} else if (horizontalCollision(player1.row, player1.col, player1.size, -1) == 2) {
							gameflag = 0;
						} else if (horizontalCollision(player1.row, player1.col, player1.size, -1) == 3) {
							gameflag = 2;
						} else if (xCollision(player1.col, player1.size, (-1 *player1.speed)) == 1){

						} else {
							player1.col -= player1.speed;
						}
					}

					if(KEY_DOWN_NOW(BUTTON_UP))
					{
						if(verticalCollision(player1.row, player1.col, player1.size, -1) == 1) {

						} else if(verticalCollision(player1.row, player1.col, player1.size,-1) == 2) {
							gameflag = 0;
						} else if(verticalCollision(player1.row, player1.col, player1.size,-1) == 3) {
							gameflag = 2;
						} else if (yCollision(player1.row, player1.size, (-1 *player1.speed)) == 1){

						} else {
							player1.row -= player1.speed;
						}
					}

					if(KEY_DOWN_NOW(BUTTON_DOWN))
					{
						if(verticalCollision(player1.row, player1.col, player1.size, player1.speed) == 1) {

						} else if(verticalCollision(player1.row, player1.col, player1.size, player1.speed) == 2) {
							gameflag = 0;
						} else if(verticalCollision(player1.row, player1.col, player1.size, player1.speed) == 3) {
							gameflag = 2;
						} else if (yCollision(player1.row, player1.size, player1.speed) == 1){

						} else {
							player1.row += player1.speed;
						}
					}

					for(int i=0; i<NUMOBJS; i++)
					{
						cur = objs + i;
						if (i == 1 || i == 3) {
							if(verticalCollision(cur->row, cur->col, cur->size+2, cur->speed)) {
							cur->speed = -1 * cur->speed;
							} else if(verticalCollision(player1.row, player1.col, player1.size,cur->speed) == 2) {
								gameflag = 0;
							}  else {
								cur->row += cur->speed;
							}
						} else {
							if(verticalCollision(cur->row, cur->col, cur->size+3, cur->speed)) {
								cur->speed = -1 * cur->speed;
							}  else if(verticalCollision(player1.row, player1.col, player1.size,cur->speed) == 2) {
								gameflag = 0;
							} else {
								cur->row += cur->speed;
							}
						}
					}
					waitForVblank();
					for(int i=0; i<NUMOBJS; i++)
					{
						drawRect(oldobjs[i].row, oldobjs[i].col, oldsize, oldsize, bgcolor);
					}
					for(int i=0; i<NUMOBJS; i++)
					{
						cur = objs+i;
						drawRect(cur->row, cur->col, size, size, cur->color);
						oldobjs[i] = objs[i];
					}
					drawRect(oldplayer.row, oldplayer.col, size, size, bgcolor);
					drawRect(player1.row, player1.col, size, size, player1.color);
					drawRect(gate.row, gate.col, size+5, size, gate.color);
					oldplayer = player1;
					oldsize = size;
					if(gameflag == 0 || gameflag == 2) {
						state = START_NODRAW2;
						enable = 1;
						break;
					}  else if (gameflag == 4) {
						state = START;
						enable = 1;
						state = START;
						previous = 1;
						enable = 1;
						gameflag = 1;
						break;
					}
					}; 
				}	
			}
			break;
		case START_NODRAW2:
			 waitForVblank();
			 if ((KEY_DOWN_NOW(BUTTON_SELECT))) {
				if (enable == 0) {
					state = START;
					previous = 1;
					enable = 1;
					
				}
			} else if(!(KEY_DOWN_NOW(BUTTON_SELECT))) {
				if (enable) {
					if (gameflag == 0) {
						drawimage3(0 , 0, GAMEOVER_HEIGHT, GAMEOVER_WIDTH, gameover);
						char message[] = "GAME OVER";
						drawString(150, 5, message, RED);
						enable  = 0;
						gameflag = 1;
					} else if(gameflag == 2) {
						drawimage3(0 , 0, WON_HEIGHT, WON_WIDTH, won);
						char message[] = "Woho Good Job!!!:D";
						drawString(150, 5, message, RED);
						enable  = 0;
						gameflag = 1;
					}
				}
			}
			break;

		
		}
	}
}



