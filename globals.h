#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define packword32(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b31 << 31) | (b30 << 30) | (b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) |						  \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
(\
 (b23 << 27) | (b22 << 26) | (b21 << 25) | (b20 << 24) | (b19 << 23) | (b18 << 22) | (b17 << 21) | (b16 << 20) |						  \
 (b15 << 19) | (b14 << 18) | (b13 << 17) | (b12 << 16) | (b11 << 15) | (b10 << 14) | (b9  << 13 ) | (b8  << 12 ) |						  \
 (b7  << 11 ) | (b6  << 10 ) | (b5  << 9 ) | (b4  << 8 ) | (b3  << 7 ) | (b2  << 6 ) | (b1  << 5 ) | (b0  << 4 ))

#define packwordBunkerLeft(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
(\
 (b23 << 31) | (b22 << 30) | (b21 << 29) | (b20 << 28) | (b19 << 27) | (b18 << 26) | (b17 << 25) | (b16 << 24) |						  \
 (b15 << 23) | (b14 << 22) | (b13 << 21) | (b12 << 20) | (b11 << 19) | (b10 << 18) | (b9  << 17 ) | (b8  << 16 ) |						  \
 (b7  << 15 ) | (b6  << 14 ) | (b5  << 13 ) | (b4  << 12 ) | (b3  << 11 ) | (b2  << 10 ) | (b1  << 9 ) | (b0  << 8 ))

#define packwordBunkerRight(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
(\
 (b23 << 31) | (b22 << 30) | (b21 << 29) | (b20 << 28) | (b19 << 27) | (b18 << 26) | (b17 << 25) | (b16 << 24) |						  \
 (b15 << 23) | (b14 << 22) | (b13 << 21) | (b12 << 20) | (b11 << 19) | (b10 << 18) | (b9  << 17 ) | (b8  << 16 ) |						  \
 (b7  << 15 ) | (b6  << 14 ) | (b5  << 13 ) | (b4  << 12 ) | (b3  << 11 ) | (b2  << 10 ) | (b1  << 9 ) | (b0  << 8 ))

#define packword30(b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) |						  \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

/*#define packword48(b47,b46,b45,b44,b43,b42,b41,b40,b39,b38,b37,b36,b35,b34,b33,b32,b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b47 << 47) | (b46 << 46) | (b45 << 45) | (b44 << 44) | (b43 << 43) | (b42 << 42) | (b41 << 41) | (b41 << 41) | \
 (b40 << 40) | (b39 << 39) | (b38 << 38) | (b37 << 37) | (b36 << 36) | (b35 << 35) | (b34 << 34) | (b33 << 33) | (b32 << 32) | \
 (b31 << 31) | (b30 << 30) | (b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) |						  \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )
*/

#define packword_tank_right(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 31) | (b13 << 30) | (b12 << 29) |(b11 << 28) | (b10 << 27) | (b9  << 26 ) | (b8  << 25 ) |						  \
(b7  << 24 ) | (b6  << 23 ) | (b5  << 22 ) | (b4  << 21 ) | (b3  << 20 ) | (b2  << 19 ) | (b1  << 18 ) | (b0  << 17 ) )


#define packword_tank_left(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) |(b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
(b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )


#define packwordBunkerDamage(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b11 << 31) | (b10 << 30) | (b9  << 29 ) | (b8  << 28 ) |						  \
(b7  << 27 ) | (b6  << 26 ) | (b5  << 25 ) | (b4  << 24 ) | (b3  << 23 ) | (b2  << 22 ) | (b1  << 21 ) | (b0  << 20 ))

#define packwordAlienBullet(b5,b4,b3,b2,b1,b0) \
((b5  << 31 ) | (b4  << 30 ) | (b3  << 29 ) | (b2  << 28 ) | (b1  << 27 ) | (b0  << 26 ))

#define packwordBullet(b3,b2,b1,b0) \
((b3  << 31 ) | (b2  << 30 ) | (b1  << 29 ) | (b0  << 28 ))

#define TANK_WIDTH 30//duplicated w/ const int TANK_COL
#define ALIEN_WIDTH 24//duplicated w/ const int ALIEN_COL
#define ALIEN_HEIGHT 16//duplicated w/ const int ALIEN_ROW
#define ALIEN_BLOCK_WIDTH (ALIEN_WIDTH * 11)
#define ALIEN_BLOCK_HEIGHT (ALIEN_HEIGHT * 5)
#define X_MAX 640
#define Y_MAX 480
#define MAX_BLOCK_POSITION ((X_MAX-1)-ALIEN_BLOCK_WIDTH)
#define MAX_TANK_POSITION ((X_MAX-1)-TANK_WIDTH)
#define ALIEN_ROW_SEPARATION 4
#define ALIEN_COLUMN_SEPARATION 0
#define BULLET_HEIGHT 18
#define BULLET_MOVEMENT_DISTANCE 6
#define LEGS_IN 0
#define LEGS_OUT 1
#define TANK_POSITION_Y 400

//macros for proper positioning
#define WIDTH_ALIENS 24
#define WIDTH_ALIEN_COL_SPACE 8
#define WIDTH_TANK 30
#define TANK_SIDE_SPACE 17
#define TANK_Y_POSITION 400
#define HEIGHT_BULLET 12
#define TANK_MOVEMENT 8
#define ALIEN_INBETWEEN_SPACE 8
#define BLOCK_WIDTH ((ALIEN_WIDTH+ALIEN_INBETWEEN_SPACE) * 10 + ALIEN_WIDTH)
#define BLOCK_SIDE_SPACE 4
#define STOP_DISTANCE 32 //all movement MUST be less than this or else overflow
#define BLOCK_MOVEMENT_Y 8
#define BLOCK_MOVEMENT_X 4
//
/*****************      lab4 macros and globals       ********************************************************************/
#define Y_MIN 30 //considered offscreen if below this for bullets. They will disappear before hitting score or lives
unsigned int score; //updated in killAliens() and killMothership()
/****************       end lab4      *************/

bool globals_DeadAliens[55]; //initialize all to be alive. True is dead, false is alive
bool tankBulletOffscreen;
bool alienBlockState;

typedef struct{
  unsigned short x;
  unsigned short y;
} point_t;

typedef struct{
  uint8_t type;
  point_t position;
  bool offScreen; //true if offScreen;
} bullet_t;

typedef struct{
  uint8_t id;//identification from 0-3
  uint8_t destruction_level;//0-4. 0 is undamaged, 4 is completely destroyed
} quadrant;

typedef struct{
  quadrant quadrants[12];
} bunker;

bullet_t globals_bullets[4];

bunker globals_bunkers[4];

void globals_setTankPosition(unsigned short val);//only care about x coordinate
unsigned short globals_getTankPosition();

void globals_setTankBulletPosition(point_t val);
point_t globals_getTankBulletPosition();

void globals_setAlienBlockPosition(point_t val);
point_t globals_getAlienBlockPosition();

void globals_setAlienBulletPosition(point_t val,uint8_t index);
point_t globals_getAlienBulletPosition(uint8_t index);

point_t globals_getAlienPosition(uint8_t alien);

//all shapes
// Must define packword for each of the different bit-widths.
const int saucer_32x14[14];
const int alien_explosion_24x20[20];
 
const int alien_dead_24x16[16];
const int alien_top_in[28];
const int alien_top_out[28];
const int alien_middle_in[20];
const int alien_middle_out[20];
const int alien_bottom_in[20];
const int alien_bottom_out[20];
const int tank_right[16];
const int tank_left[16];
 
// Shape of the entire bunker.
const int left_bunker_24x36[36];
const int right_bunker_24x36[36];
 
// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
const int bunkerDamage0_12x12[12];
const int bunkerDamage1_12x12[12];
const int bunkerDamage2_12x12[12];
const int bunkerDamage3_12x12[12];
const int tankBullet_4x18[18];
const int tankBulletNew[12];
const int tankBulletBlack[18];
const int alienBulletT1[18];
const int alienBulletT0[18];
const int alienBulletZ0[18];
const int alienBulletZ1[18];
const int alienBulletBlack[18];


//col & row of pixel maps
const int ALIEN_START;
const int ROWS_OF_ALIENS;
const int COLUMNS_OF_ALIENS;
const int TOP_ROW_ALIENS_EXTRA_PIXELS;
const int ALIEN_ROW;
const int ALIEN_TOP_ROW;
const int ALIEN_COL;
const int TANK_ROW;
const int TANK_COL;
const int TANK_ROW_OFFSET;
const int NUM_BUNKERS;
const int MAX_ALIEN_BULLETS;
const int BUNKER_HALF_ROW;
const int BUNKER_RIGHT_COL;
const int BUNKER_LEFT_COL;
const int BUNKER_SEPERATOR;
const int BUNKER_ROW_OFFSET;
const int BUNKER_COL_OFFSET;
const int EROSION_ROWCOL;
const int EROSION_QUAD_ROWS;
const int EROSION_QUAD_COLS;
const int MIN_EROSION;
const int MAX_EROSION;
const int TANK_BULLET_ROW;
const int TANK_BULLET_COL;
const int TANK_BULLET_NEW_ROW;
const int ALIEN_BULLET_ROW;
const int ALIEN_BULLET_COL;
const int GROUND_OFFSET;
const int BIT_32;

const int WHITE;
const int YELLOW;
const int RED;
const int GREEN;
const int BLUE;
const int BLACK;


#endif /* GLOBALS_H_ */
