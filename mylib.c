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
#include "ghost.h"

u16* videoBuffer = (u16*) 0x6000000;

int jtet[4][4] = {
{0, 0, 0, 0},
{0, 1, 1, 0},
{0, 0, 1, 0},
{0, 0, 1, 0}
};

int ltet[4][4] = {
{0, 0, 0, 0},
{0, 2, 2, 0},
{0, 2, 0, 0},
{0, 2, 0, 0}
};

int longtet[4][4] = {
{0, 0, 0, 0},
{3, 3, 3, 3},
{0, 0, 0, 0},
{0, 0, 0, 0}
};

int squaretet[4][4] = {
{0, 0, 0, 0},
{0, 4, 4, 0},
{0, 4, 4, 0},
{0, 0, 0, 0}
};

int toplefttet[4][4] = {
{0, 0, 0, 0},
{5, 5, 0, 0},
{0, 5, 5, 0},
{0, 0, 0, 0}
};

int toprighttet[4][4] = {
{0, 0, 0, 0},
{0, 6, 6, 0},
{6, 6, 0, 0},
{0, 0, 0, 0}
};

const unsigned short* getTypeImage(int num) {
	if(num == 1) {
		return jblock;
	}
	if(num == 2) {
		return lblock;
	}
	if(num == 3) {
		return longblock;
	}
	if(num == 4) {
		return squareblock;
	}
	if(num == 5) {
		return topleftblock;
	}
	if(num == 6) {
		return toprightblock;
	}
	if(num == 7) {
		return ghost;
	}
	return 0;
}

void drawFullTetris(BLOCK block) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block.shape[i][j] != 0) {
				const unsigned short* type = getTypeImage(block.shape[i][j]);
				drawSingleTetrisBlock(8 + 8 * block.row + 8 * i, 56 + 8 * block.col + 8 * j, type);
			}
		}
	}
}

void drawClearTetris(BLOCK block) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(block.shape[i][j] != 0) {
				drawSingleTetrisBlock(8 + 8 * block.row + 8 * i, 56 + 8 * block.col + 8 * j, blank);
			}
		}
	}
}

void drawSingleTetrisBlock(int row, int col, const unsigned short* type) {
	for(int i = 0; i < 8; i ++) {
		drawRow(&type[OFFSET(i, 0, 8)], &videoBuffer[OFFSET(row + i, col, 240)], 8);
	}
}

void drawRow(const unsigned short* type, void* destination, int width) {
	DMA[3].src = type;
	DMA[3].dst = destination;
	DMA[3].cnt = DMA_ON | width;
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void fillScreen(volatile u16 color)
{	
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | 38400;	
}

void drawImage3 (int r, int c, int width, int height, const u16* image) {
	for (int i = 0; i < height; i++) {
		DMA[3].src = (image + (width * i));
		DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
		DMA[3].cnt = width | DMA_ON;
	}
}

void setPixel(int r, int c, u16 color) {
	videoBuffer[240 * r + c] = color;
}
