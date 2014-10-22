/* AUTHOR: Saiprasad Naidu
 * v1.0
 */

typedef unsigned short u16;

#define REG_DISPCNT *(u16 *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define RGB(r, g, b) ((r) | (g) << 5 | (b) << 10)

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

int main(void) {
	//code here
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	u16 gold = RGB(31, 24, 0);
	u16 blue = RGB(0, 0, 20);
	u16 grassGreen = RGB(0, 13, 0);
	u16 purp = RGB(9, 0, 14);
	u16 darkPurp = RGB(7, 0, 11);
	u16 superDark = RGB(4, 0, 7);
	//Writes a background color
	drawRect(0, 0, 240, 160, grassGreen);
	//drawHollowRect(104, 0, 240, 56, superDark);

	//Next couple statements used for drawSword
	int rTip = 132;
	int cTip = 200;
	int c = cTip;

	while(1) { //Loop to provide permanence for the program

		// Next couple lines makes the TriForce
		drawTri(52, 90, 96, 60, 96, 120, blue, gold);
		drawTri(52, 150, 96, 120, 96, 180, blue, gold);
		drawTri(8, 120, 52, 90, 52, 150, blue, gold);
		drawTri(52, 90, 52, 150, 96, 120, gold, grassGreen);

		c = c + 3; //Controls the speed of sword animation
		if (c >= 240)
			c = 0;
		// Next couple lines reset image after sword writes over it
		drawHollowRect(104, 0, 239, 56, superDark);
		drawHollowRect(105, 1, 237, 54, superDark);
		drawHollowRect(106, 2, 235, 52, darkPurp);
		drawRect(107, 3, 234, 50, purp);
		// Here is where each image of sword is drawn
		drawSword(rTip, c);
		// delay not currently in use
		delay(0);
	}
}