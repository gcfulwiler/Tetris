#ifndef FONT_H
#define FONT_H

void drawString(int row, int col, char *str, u16 color);
void drawChar(int row, int col, char ch, u16 color);

extern const unsigned char fontdata_6x8[12288];

#endif
