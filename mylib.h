// Guy Fulwiler

#ifndef MYLIB_H
#define MYLIB_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef volatile signed char vs8;
typedef volatile signed short vs16;
typedef volatile signed int vs32;

#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))
#define REG_DISPCTL *((u16 *)0x4000000)
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define RGB(r,g,b) ((r)|((g)<<5)|((b)<<10))
#define SCANLINECOUNTER (*(vu16 *)0x4000006)

/* Buttons */

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

#define BUTTONS (*(vu16*)0x04000130)

#define BUTTON_HELD(mask) (((mask) & buttons) == (mask))
#define BUTTON_PRESSED(mask) ((((mask) & lastButtons) != (mask)) && (BUTTON_HELD(mask)))
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

typedef struct {
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;


typedef struct {
	int row;
	int col;
	int shape[4][4];
} BLOCK;

#define DMA ((volatile DMAREC *)0x040000B0)

/* DMA channel 0 register definitions*/
#define REG_DMA0SAD         *(vu32*)0x40000B0  /* source address*/
#define REG_DMA0DAD         *(vu32*)0x40000B4  /* destination address*/
#define REG_DMA0CNT         *(vu32*)0x40000B8  /* control register*/

/* DMA channel 1 register definitions*/
#define REG_DMA1SAD         *(vu32*)0x40000BC  /* source address*/
#define REG_DMA1DAD         *(vu32*)0x40000C0  /* destination address*/
#define REG_DMA1CNT         *(vu32*)0x40000C4  /* control register*/

/* DMA channel 2 register definitions*/
#define REG_DMA2SAD         *(vu32*)0x40000C8  /* source address*/
#define REG_DMA2DAD         *(vu32*)0x40000CC  /* destination address*/
#define REG_DMA2CNT         *(vu32*)0x40000D0  /* control register*/

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(vu32*)0x40000D4   /* source address*/
#define REG_DMA3DAD         *(vu32*)0x40000D8  /* destination address*/
#define REG_DMA3CNT         *(vu32*)0x40000DC  /* control register*/

/* Defines*/
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000

extern u16* videoBuffer;

int jtet[4][4];
int ltet[4][4];
int longtet[4][4];
int squaretet[4][4];
int toplefttet[4][4];
int toprighttet[4][4];

void waitForVblank();
void rotateBlock(BLOCK block);
void fillScreen(u16 color);
const unsigned short* getTypeImage(int num);
void drawFullTetris(BLOCK block);
void drawSingleTetrisBlock(int row, int col, const unsigned short* type);
void drawRow(const unsigned short* type, void* destination, int width);
void drawImage3 (int r, int c, int width, int height, const u16* image);
void setPixel(int row, int col, u16 color);
void drawString3(int row, int col, char *str, u16 color);
void drawChar3(int row, int col, char ch, unsigned short color);
void spawnNewBlock();
void clearScreen();
void drawStart();
void drawHome();
void redraw();
int checkCollision(int addRow, int addCol, BLOCK block);
void checkForFullRow();
void clearRow(int row);
void setBlock();
void move();
void emptyBoard();
int checkEmpty();
void rotateCurrentBlockClock(BLOCK block);
void rotateCurrentBlockCounter(BLOCK block);
void rotateGhostBlockClock(BLOCK block);
void rotateGhostBlockCounter(BLOCK block);
void drawClearTetris(BLOCK block);
void updateText();
void gameOver();
void createNext();

#endif
