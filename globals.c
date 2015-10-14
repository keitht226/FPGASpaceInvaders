#include "globals.h"

//static global makes local to this file. Other files can only access variables through functions
static unsigned short tankPosition;
static point_t tankBulletPosition;
static point_t AlienBlockPosition;
bool globals_DeadAliens[55] = {false}; //initialize all to be alive. True is dead, false is alive
bool globals_deadColumns[10] = {ALIVE}; //initialize all columns to be alive
bool tankBulletOffscreen = true;
bool alienBlockState = LEGS_IN;
unsigned int score = 0;
unsigned int lives = 3;
unsigned short mothershipSpawnCounter = 0;
globals_bullets[0].offScreen = true;
globals_bullets[1].offScreen = true;
globals_bullets[2].offScreen = true;
globals_bullets[3].offScreen = true;
bool mothershipState = DEAD;

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
const int TANK_BULLET_COL = 4;
const int TANK_BULLET_NEW_ROW = 12;
const int ALIEN_BULLET_ROW = 18;
const int ALIEN_BULLET_COL = 6;
const int GROUND_OFFSET = 463;
const int BIT_32 = 32;

const int WHITE = 16777215;
const int YELLOW = 16050970;
const int RED = 16711680;
const int GREEN = 65280;
const int BLUE = 255;
const int BLACK = 0;


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
  int col = alien % 11;
  int row = alien / 11;
  int y = globals_getAlienBlockPosition().y + (row * (ALIEN_HEIGHT+ALIEN_ROW_SEPARATION));
  int x = globals_getAlienBlockPosition().x + (col * (WIDTH_ALIENS + WIDTH_ALIEN_COL_SPACE));

  alienPosition.x = x;
  alienPosition.y = y;
  return alienPosition;
}

const int saucer_32x14[14]=
{
packword32(0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packword32(0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0),
packword32(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packword32(0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0),
packword32(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword32(0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0),
packword32(0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0),
packword32(0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0),
packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword32(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword32(0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0),
packword32(0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0),
packword32(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0),
packword32(0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0)
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
packwordBunkerDamage(0,0,0,1,1,1,0,0,1,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,0,0,1,1,0,0),
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
packwordBunkerDamage(0,0,0,1,1,1,1,1,1,0,0,0),
packwordBunkerDamage(0,0,1,1,1,1,1,1,1,1,0,0),
packwordBunkerDamage(0,1,0,0,1,1,1,1,0,0,1,0),
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

const int alienBulletBlack[18] =
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
