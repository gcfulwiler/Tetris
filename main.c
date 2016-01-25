// Guy Fulwiler

#include "mylib.h"
#include "j.h"
#include "l.h"
#include "long.h"
#include "square.h"
#include "t.h"
#include "topleft.h"
#include "topright.h"
#include "blank.h"
#include "start.h"
#include "home.h"
#include "font.h"
#include "start.h"
#include "blank.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "ghost.h"

int level;
int score;
int lines;
int levelCounter;
int aPushed;
int lPushed;
int rPushed;
int upPushed;
int over;
int drawn;
char buffer[4][41]; 


int board[18][10] = {{0}};

BLOCK current;
BLOCK next;
BLOCK ghostBlock;

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	int playing = 0;
	while(1) {
		if(!playing) {
			clearScreen();
			drawStart();
			while(!KEY_DOWN_NOW(BUTTON_START));
			over = 0;
			playing = 1;
			level = 1;
			score = 0;
			lines = 0;
			levelCounter = 90;
			clearScreen();
			drawHome();
			createNext();
			spawnNewBlock();
			drawn = 0;
			drawFullTetris(current);
		} else if (!over){
			waitForVblank();
			move();
			if(checkEmpty()) {
				spawnNewBlock();
				drawFullTetris(current);
				if(checkCollision(0, 0, current)) {
					over = 1;
				}
			}
			if(levelCounter > 0) {
				levelCounter -= level * 3;
			} else if(checkCollision(1, 0, current)) {
				setBlock();
				checkForFullRow();
				levelCounter = 90;
				level = (lines / 10) + 1;
			} else {
				drawClearTetris(current);
				current.row += 1;
				drawFullTetris(current);
				levelCounter = 90;
			}
			updateText();
		} else if (!drawn) {
			emptyBoard();
			redraw();
			gameOver();
			drawn = 1;
			level = 1;
			lines = 0;
			score = 0;
			levelCounter = 90;
			while(!KEY_DOWN_NOW(BUTTON_START) && !KEY_DOWN_NOW(BUTTON_SELECT));
			if (KEY_DOWN_NOW(BUTTON_START)) {
				playing = 1;
				over = 0;
				clearScreen();
				drawHome();
				spawnNewBlock();
				drawn = 0;
				drawFullTetris(current);
			}
		}
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			playing = 0;
			drawn = 0;
		} else if (KEY_DOWN_NOW(BUTTON_START)) {
			playing = 1;
		}
	}
}

int checkEmpty() {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(current.shape[i][j]){
				return 0;
			}
		}
	}
	return 1;
}

void gameOver() {
	drawString3(40, 68, "GAME OVER", RGB(0, 0, 0));
	drawString3(56, 62, "PRESS START", RGB(0, 0, 0));
}

void updateGhost() {
	if(!checkCollision(1, 0, current) || !checkCollision(0, 0, current)) {
		ghostBlock.row = current.row;
		ghostBlock.col = current.col;
		while(!checkCollision(1, 0, ghostBlock)) {
			ghostBlock.row += 1;
		}
		drawFullTetris(ghostBlock);
	}
}

void updateText() {
	for(int i = 0; i < 6; i++) {
		drawSingleTetrisBlock(32, 148 + 8 * i, blank);
	}
	for(int i = 0; i < 4; i++) {
		drawSingleTetrisBlock(64, 156 + 8 * i, blank);
		drawSingleTetrisBlock(88, 156 + 8 * i, blank);
	}
	int starter = 0;
	if(score > 9) {
		starter = 4;
	}
	if(score > 99) {
		starter = 8;
	}
	if(score > 999) {
		starter = 12;
	}
	if(score > 9999) {
		starter = 16;
	}
	sprintf(buffer[0], "%d", score);
	drawString3(32, 169 - starter, buffer[0], RGB(0, 0, 0));
	starter = 0;
	if(level > 9) {
		starter = 4;
	}
	if(level > 99) {
		starter = 8;
	}
	if(level > 999) {
		starter = 12;
	}
	if(level > 9999) {
		starter = 16;
	}
	sprintf(buffer[0], "%d", level);
	drawString3(64, 169 - starter, buffer[0], RGB(0, 0, 0));
	starter = 0;
	if(lines > 9) {
		starter = 4;
	}
	if(lines > 99) {
		starter = 8;
	}
	if(lines > 999) {
		starter = 12;
	}
	if(lines > 9999) {
		starter = 16;
	}
	sprintf(buffer[0], "%d", lines);
	drawString3(88, 169 - starter, buffer[0], RGB(0, 0, 0));
	starter = 0;
}

void spawnNewBlock() {
	drawClearTetris(next);
	current.row = 0;
	ghostBlock.row = 0;
	current.col = 3;
	ghostBlock.col = 3;
	int random = rand() % 6 + 1;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(random == 1) {
				current.shape[i][j] = next.shape[i][j];
				if(next.shape[i][j]) {
					ghostBlock.shape[i][j] = 7;
				} else {
					ghostBlock.shape[i][j] = 0;
				}
				next.shape[i][j] = jtet[i][j];
			}
			if(random == 2) {
				current.shape[i][j] = next.shape[i][j];
				if(next.shape[i][j]) {
					ghostBlock.shape[i][j] = 7;
				} else {
					ghostBlock.shape[i][j] = 0;
				}
				next.shape[i][j] = ltet[i][j];
			}
			if(random == 3) {
				current.shape[i][j] = next.shape[i][j];
				if(next.shape[i][j]) {
					ghostBlock.shape[i][j] = 7;
				} else {
					ghostBlock.shape[i][j] = 0;
				}
				next.shape[i][j] = longtet[i][j];
			}
			if(random == 4) {
				current.shape[i][j] = next.shape[i][j];
				if(next.shape[i][j]) {
					ghostBlock.shape[i][j] = 7;
				} else {
					ghostBlock.shape[i][j] = 0;
				}
				next.shape[i][j] = squaretet[i][j];
			}
			if(random == 5) {
				current.shape[i][j] = next.shape[i][j];
				if(next.shape[i][j]) {
					ghostBlock.shape[i][j] = 7;
				} else {
					ghostBlock.shape[i][j] = 0;
				}
				next.shape[i][j] = toplefttet[i][j];
			}
			if(random == 6) {
				current.shape[i][j] = next.shape[i][j];
				if(next.shape[i][j]) {
					ghostBlock.shape[i][j] = 7;
				} else {
					ghostBlock.shape[i][j] = 0;
				}
				next.shape[i][j] = toprighttet[i][j];
			}
		}
	}
	updateGhost();
	drawFullTetris(next);
}

void createNext() {
	next.row = 13;
	next.col = 13;
	int random = rand() % 6 + 1;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(random == 1) {
				next.shape[i][j] = jtet[i][j];
			}
			if(random == 2) {
				next.shape[i][j] = ltet[i][j];
			}
			if(random == 3) {
				next.shape[i][j] = longtet[i][j];
			}
			if(random == 4) {
				next.shape[i][j] = squaretet[i][j];
			}
			if(random == 5) {
				next.shape[i][j] = toplefttet[i][j];
			}
			if(random == 6) {
				next.shape[i][j] = toprighttet[i][j];
			}
		}
	}
}

void clearScreen() {
	DMA[3].src = RGB(0,0,0);
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (160*240) | DMA_ON | DMA_DESTINATION_INCREMENT;
}

void emptyBoard() {
	for(int i = 0; i < 18; i++) {
		for(int j = 0; j < 10; j++) {
			board[i][j] = 0;
		}
	}
}

void drawStart() {
	for (int i = 0; i < 160; i++) {
		DMA[3].src = (Start + (240 * i));
		DMA[3].dst = videoBuffer + OFFSET(i, 0 ,240);
		DMA[3].cnt = 240 | DMA_ON;
	}
}

void drawHome() {
	for (int i = 0; i < 160; i++) {
		DMA[3].src = (Home + (240 * i));
		DMA[3].dst = videoBuffer + OFFSET(i, 0, 240);
		DMA[3].cnt = 240 | DMA_ON;
	}
}

void redraw() {
	for(int i = 0; i < 18; i++) {
		for(int j = 0; j < 10; j++) {
			if(board[i][j] != 0) {
				const unsigned short* type = getTypeImage(board[i][j]);
				drawSingleTetrisBlock(8 + 8 * i, 56 + 8 * j, type);
			} else {
				drawSingleTetrisBlock(8 + 8 * i, 56 + 8 * j, blank);
			}
		}
	}
}

int checkCollision(int addRow, int addCol, BLOCK block) {
	if(block.row + addRow > 17) {
		return 1;
	} 
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block.shape[j][i]) {
				if(block.col + i + addCol > 9 || block.col + i + addCol < 0 || block.row + j + addRow > 17) {
					return 1;
				}
				if(board[j + block.row + addRow][i + block.col + addCol]) {
					return 1;
				}
			}
		}
	}
	return 0;
}

void checkForFullRow() {
	int full = 1;
	for(int i = 0; i < 18; i++) {
		for(int j = 0; j < 10; j++) {
			if(!board[i][j]) {
				full = 0;
			}
		}
		if(full == 1) {
			clearRow(i);
			redraw();
		}
		full = 1;
	}
}

void clearRow(int row) {
	for(int i = row; i > 0; i--) {
		for(int j = 0; j < 10; j++) {
			board[i][j] = board[i - 1][j];
		}
	}
	score += 25 * level;
	lines += 1;
}

void rotateCurrentBlockClock(BLOCK block) {
	for (int i = 0; i < 4; i++) {
        	for (int j = 0; j < 4; j++) {
            		current.shape[i][j] = block.shape[3 - j][i];
        	}
    	}
}

void rotateCurrentBlockCounter(BLOCK block) {
	for (int i = 0; i < 4; i++) {
        	for (int j = 0; j < 4; j++) {
            		current.shape[i][j] = block.shape[j][3 - i];
        	}
    	}
}

void rotateGhostBlockClock(BLOCK block) {
	for (int i = 0; i < 4; i++) {
        	for (int j = 0; j < 4; j++) {
            		ghostBlock.shape[i][j] = block.shape[3 - j][i];
        	}
    	}
}

void rotateGhostBlockCounter(BLOCK block) {
	for (int i = 0; i < 4; i++) {
        	for (int j = 0; j < 4; j++) {
            		ghostBlock.shape[i][j] = block.shape[j][3 - i];
        	}
    	}
}

void setBlock() {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(current.shape[i][j]) {
				board[current.row + i][current.col + j] = current.shape[i][j];
				current.shape[i][j] = 0;
			}
		}
	}
	current.row = 0;
	current.col = 3;
}

void move() {
	if(KEY_DOWN_NOW(BUTTON_LEFT)) {
		if(!checkCollision(0, -1, current) && !lPushed) {
			drawClearTetris(current);
			current.col -= 1;
			drawClearTetris(ghostBlock);
			ghostBlock.col -= 1;
			updateGhost();
			drawFullTetris(current);
			lPushed = 1;
			if(checkCollision(1, 0, current)) {
				levelCounter = 150;
			}
		}
	} else {
		lPushed = 0;
	}
	if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
		if(!checkCollision(0, 1, current) && !rPushed) {
			drawClearTetris(current);
			current.col += 1;
			drawClearTetris(ghostBlock);
			ghostBlock.col += 1;
			updateGhost();
			drawFullTetris(current);
			rPushed = 1;
			if(checkCollision(1, 0, current)) {
				levelCounter = 150;
			}
		}
	} else {
		rPushed = 0;
	}
	if(KEY_DOWN_NOW(BUTTON_DOWN)) {
		if(levelCounter > 15) {
			levelCounter = 15;
			score++;
		}
		drawFullTetris(current);
	}
	if(KEY_DOWN_NOW(BUTTON_A)) {
		if(!aPushed) {
			while(!checkCollision(1, 0, current)) {
				drawClearTetris(current);
				current.row += 1;
				drawFullTetris(current);
				score += level;
			}
			aPushed = 1;
		}
	} else {
		aPushed = 0;
	}
	if(KEY_DOWN_NOW(BUTTON_UP)) {
		if(!upPushed) {
			upPushed = 1;
			rotateCurrentBlockClock(current);
			if(checkCollision(0, 0, current)) {
				rotateCurrentBlockCounter(current);
			} else {
				rotateCurrentBlockCounter(current);
				drawClearTetris(current);
				rotateCurrentBlockClock(current);
				drawClearTetris(ghostBlock);
				rotateGhostBlockClock(ghostBlock);
				updateGhost();
				drawFullTetris(current);
			}
		}
	} else {
		upPushed = 0;
	}
}
