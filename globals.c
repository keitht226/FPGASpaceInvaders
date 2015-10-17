#include "globals.h"

//static global makes local to this file. Other files can only access variables through functions
static unsigned short tankPosition;
static point_t tankBulletPosition;
static point_t AlienBlockPosition;
bool globals_DeadAliens[55] = {false}; //initialize all to be alive. True is dead, false is alive
bool globals_deadColumns[11] = {ALIVE}; //initialize all columns to be alive
bool globals_tankDeath = stopped;
bool tankBulletOffscreen = true;
bool alienBlockState = LEGS_IN;
bool beginAlienExplosion = false;
unsigned int dead_alien_count = 0;
unsigned int score = 0;
unsigned int lives = 3;
unsigned short mothershipSpawnCounter = 2000;//mothership appears first time after 20 seconds. All following is random
unsigned int alienExplodeCounter = 1;
unsigned int numberOfCol = 11;
bool mothershipState = DEAD;
unsigned int offset = 0;

const int ALIEN_START = 50;
const int ROWS_OF_ALIENS = 5;
const int COLUMNS_OF_ALIENS = 11;
const int TOP_ROW_ALIENS_EXTRA_PIXELS = 8;
const int ALIEN_ROW = 20;
const int ALIEN_TOP_ROW = 28;
const int ALIEN_COL = 32;
const int TANK_ROW_OFFSET = 400;
const int TANK_ROW = 16;
const int TANK_COL = 32;
const int NUM_BUNKERS = 4;
const int MAX_ALIEN_BULLETS = 4;
const int BUNKER_HALF_ROW = 36;
const int BUNKER_RIGHT_COL = 24;
const int BUNKER_LEFT_COL = 24;
const int BUNKER_SEPERATOR = 40;
const int BUNKER_ROW_OFFSET = 340;
const int BUNKER_COL_OFFSET = 80;
const int EROSION_ROWCOL = 12;
const int EROSION_QUAD_ROWS = 3;
const int EROSION_QUAD_COLS = 4;
const int MIN_EROSOIN = 0;
const int MAX_EROSION = 4;
const int TANK_BULLET_ROW = 18;
const int TANK_BULLET_COL = 3;
const int TANK_BULLET_NEW_ROW = 12;
const int ALIEN_BULLET_ROW = 18;
const int ALIEN_BULLET_COL = 6;
const int GROUND_OFFSET = 455;
const int BIT_32 = 32;
const int MAX_SCORE_SIZE = 6;
const int MOTHERSHIP_ROW_OFFSET = 50;
const int MOTHERSHIP_ROW = 14;
const int MOTHERSHIP_COL = 20;

const int WHITE = 16777215;
const int YELLOW = 16050970;
const int RED = 16711680;
const int GREEN = 65280;
const int BLUE = 255;
const int BLACK = 0;
const int PURPLE = 10905297;
const int PINK = 16711935;


void globals_setTankPosition(unsigned short val){
  tankPosition = val;
  //xil_printf("Setting Tank Position: %d\n\r", tankPosition);
}

unsigned short globals_getTankPosition(){
  return tankPosition;
}

void globals_setTankBulletPosition(point_t val){
  tankBulletPosition.x = val.x;
  tankBulletPosition.y = val.y;
  //xil_printf("Setting bullet x = %d, y = %d\n\r", val.x, val.y);
}

point_t globals_getTankBulletPosition(){
  return tankBulletPosition;
}

void globals_setAlienBlockPosition(point_t val){
  AlienBlockPosition.x = val.x;
  AlienBlockPosition.y = val.y;
 // xil_printf("Setting Alien x = %d, y = %d\n\r", val.x, val.y);
}

point_t globals_getAlienBlockPosition(){
  return AlienBlockPosition;
}

void globals_setAlienBuilletPosition(point_t val,uint8_t index){
  globals_bullets[index].position.x = val.x;
  globals_bullets[index].position.y = val.y;
}

point_t globals_getAlienBulletPosition(uint8_t index){
  return globals_bullets[index].position;
}

point_t globals_getAlienPosition(uint8_t alien){
  point_t alienPosition;
  int col = alien % 11 - offset;
  int row = alien / 11;
  int y = globals_getAlienBlockPosition().y + (row * (ALIEN_HEIGHT+ALIEN_ROW_SEPARATION));
  int x = globals_getAlienBlockPosition().x + (col * (WIDTH_ALIENS + WIDTH_ALIEN_COL_SPACE));

  alienPosition.x = x;
  alienPosition.y = y;
  return alienPosition;
}

const int mothership_left[14]=
{
packword_mothership_left(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1),
packword_mothership_left(0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1),
packword_mothership_left(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1),
packword_mothership_left(0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0),
packword_mothership_left(0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0)
};

const int mothership_right[14] =
{
  packword_mothership_right(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0),
  packword_mothership_right(1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
  packword_mothership_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
  packword_mothership_right(1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
  packword_mothership_right(1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
  packword_mothership_right(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0),
  packword_mothership_right(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0)
};

const int mothership_black[14] =
{
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
};

const int mothership_score[14] =
{
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0),
  packword32(0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
};




const int alien_explosion_24x20[20]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0),
packword24(1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword24(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0),
packword24(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1),
packword24(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1),
packword24(0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0)
};

const int alien_dead_24x16[16]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
};

const int alien_top_in[28]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0)
};

const int alien_top_out[28]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0)
};

const int alien_middle_in[20]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1),
packword24(0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0)
};

const int alien_middle_out[20]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1),
packword24(0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1),
packword24(0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1),
packword24(0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1),
packword24(0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1),
packword24(0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0),
packword24(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0)
};

const int alien_bottom_in[20]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0),
packword24(0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0),
packword24(0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0)
};

const int alien_bottom_out[20]=
{
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0),
packword24(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0),
packword24(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword24(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1)
};

const int tank_left[16]=
{
packword_tank_left(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1),
packword_tank_left(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1),
packword_tank_left(0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword_tank_left(0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword_tank_left(0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword_tank_left(0,0,0,0,0,0,0,0,0,0,0,1,1,1,1),
packword_tank_left(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_left(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

const int tank_right[16]=
{
packword_tank_right(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword_tank_right(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packword_tank_right(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0),
packword_tank_right(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0),
packword_tank_right(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0),
packword_tank_right(1,1,1,1,0,0,0,0,0,0,0,0,0,0,0),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword_tank_right(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};


const int tank_explosion_1[16] =
{
  packword32(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0),
  packword32(0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0),
  packword32(0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0),
  packword32(0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1),
  packword32(0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1),
  packword32(1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
  packword32(1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
  packword32(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
  packword32(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
  packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
  packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
  packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

const int tank_explosion_2[16] =
{
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0),
  packword32(0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1),
  packword32(0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1),
  packword32(1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0),
  packword32(1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0),
  packword32(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
  packword32(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
  packword32(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
  packword32(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
  packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
  packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
  packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

const int tank_black[16] =
{
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
  packword32(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
};



// Bunker Left half
const int left_bunker_24x36[36]=
{
packwordBunkerLeft(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerLeft(1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0)
};

//bunker right half
const int right_bunker_24x36[36]=
{
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerRight(0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1)
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
const int bunkerDamage0_12x12[12]=
{
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,0,0,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,1,1,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,1,1,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,0,0)
};

const int bunkerDamage1_12x12[12]=
{
packwordBunkerDamage(1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,1,1,0,0,1,1,1,0),
packwordBunkerDamage(0,1,0,0,1,1,0,0,0,0,1,0),
packwordBunkerDamage(1,1,0,0,1,1,0,0,0,0,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,0,0,1,1,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,1,1,0,0)
};

const int bunkerDamage2_12x12[12]=
{
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,0,0,1,1,1,1,0,0,1,0),
packwordBunkerDamage(1,1,0,0,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1)
};

//Bunker corners damage 2 ---------------------------------------------------------
const int bunkerDamage2_left_corner[12]=
{
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,1,1,1,1,0,0,1,0),
packwordBunkerDamage(0,0,0,0,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1)
};


const int bunkerDamage2_right_corner[12]=
{
packwordBunkerDamage(1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,1,1,1,1,0,0,0,0),
packwordBunkerDamage(1,1,0,0,1,1,1,1,0,0,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1)
};

const int bunkerDamage2_innner_right[12]=
{
packwordBunkerDamage(0,0,0,0,0,1,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,0,0,1,1,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,1,1,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1)
};

const int bunkerDamage2_inner_left[12]=
{
packwordBunkerDamage(1,1,1,1,1,1,1,0,0,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,0,0,1,1,1),
packwordBunkerDamage(1,1,0,0,1,1,1,1,1,1,1,0),
packwordBunkerDamage(1,1,0,0,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerDamage(1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0)
};

//Bunker corners damage 1 ---------------------------------------------------------

const int bunkerDamage1_left_corner[12]=
{
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,1,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,1,1,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0)
};


const int bunkerDamage1_right_corner[12]=
{
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,1,1,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,1,1,0,0,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,0,0,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,0,0,0,0,1,1,1,1,1,1),
packwordBunkerDamage(1,1,0,0,0,0,1,1,1,1,1,1)
};

const int bunkerDamage1_innner_right[12]=
{
packwordBunkerDamage(0,0,0,0,0,1,1,0,0,1,1,1),
packwordBunkerDamage(0,0,0,0,0,1,1,0,0,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,0,0)
};

const int bunkerDamage1_inner_left[12]=
{
packwordBunkerDamage(0,0,1,1,1,1,1,0,0,1,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,0,0,1,1,1),
packwordBunkerDamage(1,1,0,0,1,0,0,1,1,1,1,1),
packwordBunkerDamage(1,1,0,0,1,0,0,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0)
};

//Bunker corners damage 0 ---------------------------------------------------------

const int bunkerDamage0_left_corner[12]=
{
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1),
packwordBunkerDamage(0,0,0,0,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,0,0,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,1,1,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,1,1,1,1,1,1,1,1)
};


const int bunkerDamage0_right_corner[12]=
{
packwordBunkerDamage(1,1,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,1,1,1,1),
packwordBunkerDamage(0,0,0,0,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,0,0,1,1,0,0,1,1),
packwordBunkerDamage(0,0,1,1,0,0,1,1,0,0,1,1)
};

const int bunkerDamage0_innner_right[12]=
{
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,1,1,1,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,1,1,0,0,0,0),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,1,1,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1),
packwordBunkerDamage(0,0,0,0,0,0,0,0,0,0,1,1)
};

const int bunkerDamage0_inner_left[12]=
{
packwordBunkerDamage(0,0,1,1,1,1,1,0,0,1,1,1),
packwordBunkerDamage(0,0,1,1,1,1,1,0,0,1,1,1),
packwordBunkerDamage(1,1,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,0,0,0,0,1,1,1,1,0,0),
packwordBunkerDamage(1,1,1,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,1,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(0,0,1,1,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0),
packwordBunkerDamage(1,1,0,0,0,0,0,0,0,0,0,0)
};

const int bunkerDamage3_12x12[12]=
{
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1),
packwordBunkerDamage(1,1,1,1,1,1,1,1,1,1,1,1)
};

const int tankBullet_4x18[18] =
{
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0)
};


const int tankBulletNew[12] =
{
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
packwordBullet(0,1,1,0),
};

const int tankBulletBlack[18] =
{
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0),
packwordBullet(0,0,0,0)
};

const int alienBulletT0[18] =
{
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(1,1,1,1,1,1),
packwordAlienBullet(1,1,1,1,1,1),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0)
};

const int alienBulletT1[18] =
{
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(1,1,1,1,1,1),
packwordAlienBullet(1,1,1,1,1,1),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0)
};

const int alienBulletZ0[18] =
{
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(1,1,0,0,0,0),
packwordAlienBullet(1,1,0,0,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,0,0,1,1),
packwordAlienBullet(0,0,0,0,1,1),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(1,1,0,0,0,0),
packwordAlienBullet(1,1,0,0,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0)
};

const int alienBulletZ1[18] =
{
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,1,1),
packwordAlienBullet(0,0,0,0,1,1),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(1,1,0,0,0,0),
packwordAlienBullet(1,1,0,0,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,0,0,1,1),
packwordAlienBullet(0,0,0,0,1,1),
packwordAlienBullet(0,0,1,1,0,0),
packwordAlienBullet(0,0,1,1,0,0)
};

const int alienBulletBlack[24] =
{
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0),
packwordAlienBullet(0,0,0,0,0,0)
};


const int letterS[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int letterC[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int letterO[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int letterR[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1)
};

const int letterL[15]=
{
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

const int letterE[15]=
{
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int letterI[15]=
{
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0)
};

const int letterV[15]=
{
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0)
};

const int letterG[15]=
{
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};

const int letterA[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1)
};

const int letterM[15]=
{
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1),
packWord(1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1)
};

const int num1[15]=
{
packWord(0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0),
packWord(0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0)
};

const int num2[15]=
{
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};


const int num3[15]=
{
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int num4[15]=
{
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1)
};

const int num5[15]=
{
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int num6[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int num7[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0)
};

const int num8[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};

const int num9[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};


const int num0[15]=
{
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packWord(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0)
};
