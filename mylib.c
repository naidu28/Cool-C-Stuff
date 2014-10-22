/* AUTHOR: Saiprasad Naidu
 * v1.0
 */

typedef unsigned short u16;
#define RGB(r, g, b) ((r) | (g) << 5 | (b) << 10)

u16 *videoBuffer = (u16 *)0x6000000;

void setPixel(int, int, u16);
void drawRect(int, int, int, int, u16);
void drawHollowRect(int, int, int, int, u16);
void plotLine(int, int, int, int, u16); //this is for the assignment
void plotLine2(int, int, int, int, u16); //covers other octants
void drawCirc(int, int, int, u16);
void drawHollowTri(int, int, int, int, int, int, u16);
void drawTri(int, int, int, int, int, int, u16, u16);
void fillIn(int, int, u16, u16);
int checkPixel(int, int, u16);
void drawLetters(int, int, int, char, u16);
void drawSword(int, int);
void delay(int);

/*
 * Sets a pixel
 */
void setPixel(int r, int c, u16 color) {
	videoBuffer[r*240+c] = color; 
}

/*
 * Draws a filled-in rectangle
 */
void drawRect(int r, int c, int w, int h, u16 color) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			setPixel(r+j, c+i, color);
		}
	}
}

/*
 * Draws a hollow rectangle
 */
void drawHollowRect(int r, int c, int w, int h, u16 color) {
	int i = 0;
	int j = 0;

	for (i = 0; i < w; i++)
		setPixel(r, c+i, color);
	for (j = 0; j < h; j++)
		setPixel(r+j, c, color);
	for (i = 0; i < w; i++)
		setPixel(r+j, c+i, color);
	for (j = 0; j < h; j++)
		setPixel(r+j, c+i, color);
}

/*
 * Alternate implementation of plotLine() - used
 * for octants in which plotLine() will not work
 */
void plotLine2(int r0, int c0, int r1, int c1, u16 color) {
	int dc = c1 - c0;
	int dr = r1 - r0;
	int r = r0;
	int c = c0;
	int bool = 0;
	int cSign = 0;
	int rSign = 0;

	if (dc < 0) { //because dc and dr will be set equal to their absolute values soon
		cSign = -1;
		dc = -dc;
	} else
		cSign = 1;
	if (dr < 0) {
		rSign = -1;
		dr = -dr;
	} else
		rSign = 1;

	setPixel(r, c, color);

	if (dc >= dr) {
		if (cSign == -1) {
			int temp = 0;
			temp = c1;
			c1 = c0;
			c0 = temp;
			if (rSign == -1) {
				temp = r1;
				r1 = r0;
				r0 = temp;
			} else
				bool = 1;
		} else {
			if (rSign == -1)
				bool = 1;
		}
		
		int D = (dr << 1) - dc;

		for (c = c + 1; c <= c1; c++) {
			if (D > 0) {
				if (bool == 1)
					r = r - 1;
				else
					r = r + 0;
				setPixel(r, c, color);
				D = D + ((dr << 1) - (dc << 1));
			} else {
				setPixel(r, c, color);
				D = D + (dr << 1);
			}
		}
	} else {
		if (rSign == -1) {
			int temp = 0;
			temp = r1;
			r1 = r0;
			r0 = temp;
			if (cSign == -1) {
				temp = c1;
				c1 = c0;
				c0 = temp;
			} else
				bool = 1;
		} else {
			if (cSign == -1)
				bool = 1;
		}

		int D = (dc << 1) - dr;

		for (r = r + 1; r <= r1; r++) {
			if (D > 0) {
				if (bool == 1)
					c = c - 1;
				else
					c = c + 1;
				setPixel(r, c, color);
				D = D + ((dc << 1) - (dr << 1));
			} else {
				setPixel(r, c, color);
				D = D + (dc << 1);
			}
		}
	}
}

/*
 * Plots a line between two points using Bresingham's
 * algorithm - works for octant outlined in the pseudocode
 * as well as certain others
 */
void plotLine(int r0, int c0, int r1, int c1, u16 color) {
	int dc = c1 - c0;
	int dr = r1 - r0;
	int r = r0;
	int c = c0;

	if (dc < 0)
		dc = -dc;
	if (dr < 0)
		dr = -dr;

	if (c0 < c1) {
		c = c0;
		if (r0 < r1)
			r = r0;
		else if (r0 > r1) {
			r = r1;
			r1 = r0;
		}
	} else if (c0 > c1) {
		c = c1;
		c1 = c0;
		if (r0 < r1)
			r = r0;
		else if (r0 > r1) {
			r = r1;
			r1 = r0;
		}
	}

	setPixel(r, c, color);

	if (dc >= dr) {
		int D = (dr << 1) - dc;

		for (c = c + 1; c <= c1; c++) {
			if (D > 0) {
				r = r + 1;
				setPixel(r, c, color);
				D = D + ((dr << 1) - (dc << 1));
			} else {
				setPixel(r, c, color);
				D = D + (dr << 1);
			}
		}
	} else {
		int D = (dc << 1) - dr;

		for (r = r + 1; r <= r1; r++) {
			if (D > 0) {
				c = c + 1;
				setPixel(r, c, color);
				D = D + ((dc << 1) - (dr << 1));
			} else {
				setPixel(r, c, color);
				D = D + (dc << 1);
			}
		}
	}
}

/*
 * Draws a hollow triangle using plotLine2() for greater
 * effectiveness
 */
void drawHollowTri(int r0, int c0, int r1, int c1, int r2, int c2, u16 color) {
	plotLine2(r0, c0, r1, c1, color);
	plotLine2(r0, c0, r2, c2, color);
	plotLine2(r1, c1, r2, c2, color);
}

/*
 * Calls drawHollowTri(), then fills in the resulting space
 */
void drawTri(int r0, int c0, int r1, int c1, int r2, int c2
	, u16 extColor, u16 inColor) {
	drawHollowTri(r0, c0, r1, c1, r2, c2, extColor);
	int rmin = r0;
	int cmin = c0;
	
	if (r1 < rmin)
		rmin = r1;
	if (r2 < rmin)
		rmin = r2;

	if (c1 < cmin)
		cmin = c1;
	if (c2 < cmin)
		cmin = c2;

	fillIn(rmin, cmin, extColor, inColor);
}

/*
 * Fills in spaces by checking "sentinel" colors.
 * Greatest function ever created. Period.
 * Note: a bit slow for large areas
 */
void fillIn(int r0, int c0, u16 extColor, u16 inColor) {
	int checkCount = 0;
	for (int i = c0; i < 240; i++) {
		for (int j = r0; j < 160; j++) {
			int check = checkPixel(j, i, extColor);	
			if (check == 1)
				checkCount++;
		}
		if (checkCount >= 2) {
			checkCount = 0;
			for (int j = r0; j < 160; j++) {
				int check = checkPixel(j, i, extColor);	
				if (check == 1)
					checkCount++;
				else {
					if (checkCount == 1)
						setPixel(j, i, inColor);
					else if (checkCount == 2)
						j = 160;
				}
			}
		}
		checkCount = 0;
	}

}

/*
 * Checks if the color of a pixel matches
 * an inputted color by accessing memory
 */
int checkPixel(int r, int c, u16 color) {
	if (videoBuffer[r*240+c] == color)
		return 1;
	else
		return 0; 
}

/*
 * Unused function with potential for future use
 */
void drawLetters(int r, int c, int h, char letter, u16 color) {
	int h2 = h*2;
	switch(letter) {
		case 'A' :
			plotLine(r, c, r-h, c+h, color); //left line
			plotLine(r, c+h, r+h, c+h2, color); //right line
			break;
		default:
			break;
	}
}

/*
 * Behold the grandaddy of all legendary swords.
 */
void drawSword(int rTip, int cTip) {
	//u16 gold = RGB(31, 24, 0);
	/*u16 purp = RGB(9, 0, 14);
	u16 darkPurp = RGB(7, 0, 11);
	u16 superDark = RGB(4, 0, 7);*/
	u16 gold = RGB(31, 24, 0);
	u16 GB = RGB(18, 14, 1);
	u16 darkGB = RGB(13, 10, 2);
	u16 gray = RGB(15, 15, 15);
	u16 lightGray = RGB(21, 21, 21);
	u16 darkGray = RGB(8, 8, 8);
	u16 red = RGB(31, 0, 0);
	u16 darkRed = RGB(15, 0, 0);
	//u16 black = RGB(0, 0, 0);

	int rTEdge = rTip - 7;
	int rBEdge = rTip + 7;
	int cStart = cTip - 25;

	plotLine(rTEdge, cStart, rTip, cTip, gray); //top edge
	plotLine2(rBEdge, cStart, rTip, cTip, gray); //bottom edge
	plotLine(rBEdge, cStart, rBEdge, cStart - 120, gray); //upper bevel
	plotLine(rTEdge, cStart, rTEdge, cStart - 120, gray); //lower bevel
	plotLine(rTip, cTip, rTip, cStart - 110, gray); //middle line-thing
	fillIn(rTEdge, cStart - 119, gray, lightGray); //fills in the upper blade
	fillIn(rTip, cStart - 110, gray, darkGray); //fills in the lower blade
	drawHollowRect(rTEdge + 4, cStart - 161, 30, 6, darkGB); //almighty hilt
	fillIn(rTEdge + 4, cStart - 160, darkGB, gold); //fills in the hilt
	drawRect(rTEdge - 10, cStart - 130, 10, 35, darkGB); //edge of crossguard
	drawRect(rTEdge - 8, cStart - 128, 6, 31, gold); //more crossguard
	drawRect(rTEdge - 6, cStart - 126, 2, 27, GB); //interior of crossguard
	drawHollowRect(rTEdge - 15, cStart - 133, 16, 5, darkRed);
	drawHollowRect(rBEdge + 10, cStart - 133, 16, 5, darkRed);
	drawRect(rTEdge - 14, cStart - 132, 15, 4, red);
	drawRect(rBEdge + 11, cStart - 132, 15, 4, red);
	//fillIn(rTEdge - 15, cStart - 133, darkRed, red);
	//fillIn(rBEdge + 10, cStart - 133, darkRed, red);
}

/*
 * Simple delay function used for video buffering
 */
void delay(int n) {
	volatile int x = 0;
	for (int i = 0; i < n*100; i++)
		x++;
}