#include "globals.h"
#include "uart_functions.h"
#include "ScreenFunctions.h"
#include <time.h>

static bool lowered;
static uint8_t leftMostCol = 0;
static uint8_t rightMostCol = 10;

#define MOTHERSHIP_MIN 600 //6 seconds between spawns
#define MOTHERSHIP_MAX 2000 //20 seconds between spawns

void moveTankLeft(){
  unsigned short tankPosition;/*{{{*/
  tankPosition = globals_getTankPosition()-TANK_MOVEMENT;
  if(tankPosition+TANK_SIDE_SPACE < STOP_DISTANCE){//do nothing if tank is at far left of screen
    return;
  }
  else{
    globals_setTankPosition(tankPosition);
  }
  //redraw tank
  write_tank_to_memory();
  return;/*}}}*/
}

void moveTankRight(){
  unsigned short tankPosition;/*{{{*/
  tankPosition = globals_getTankPosition() + TANK_MOVEMENT;
  if(tankPosition > X_MAX - STOP_DISTANCE - WIDTH_TANK - TANK_SIDE_SPACE){//do nothing if tank is at far right of screen, reference by left side of tank
    return;
  }
  else{
    globals_setTankPosition(tankPosition);
  }
  //redraw tank
  write_tank_to_memory();
  return;/*}}}*/
}


void moveAlienBlock(){
/*{{{*/
  point_t alienBlockLocation;
  alienBlockLocation = globals_getAlienBlockPosition();

  if(blockMovingRight){
    alienBlockLocation.x += BLOCK_MOVEMENT_X;
  }else{
    alienBlockLocation.x -= BLOCK_MOVEMENT_X;
  }
  uint16_t right_edge = globals_getAlienPosition(rightMostCol).x+ALIEN_WIDTH+WIDTH_ALIEN_COL_SPACE;
  //reference block by left side. If all the way right, change direction to left
  //move down if all the way at right of screen and change direction
  //if(alienBlockLocation.x > X_MAX - STOP_DISTANCE - BLOCK_WIDTH - BLOCK_SIDE_SPACE-1 && !lowered && blockMovingRight){
  if(right_edge > X_MAX - STOP_DISTANCE - 1 && !lowered && blockMovingRight){
    alienBlockLocation.y += BLOCK_MOVEMENT_Y;
    alienBlockLocation.x -= BLOCK_MOVEMENT_X;//don't move horizontally
    lowered = true;//the block did not just move down. Can do so next time
    blockMovingRight = false;
  }
  //move down if all the way at left of screen and change direction
  //else if(alienBlockLocation.x+BLOCK_SIDE_SPACE+1 < STOP_DISTANCE  && !lowered && !blockMovingRight){
  else if(alienBlockLocation.x < STOP_DISTANCE && !lowered && !blockMovingRight){
	xil_printf("left edge: %d\n\r",alienBlockLocation.x);
    alienBlockLocation.y += BLOCK_MOVEMENT_Y;
    alienBlockLocation.x += BLOCK_MOVEMENT_X;//don't move horizontally
    lowered = true;//block just moved down. Next move must be to the side
    blockMovingRight = true;
  }
  else{
    lowered = false;//the block did not just move down. Can do so next time
  }
  globals_setAlienBlockPosition(alienBlockLocation);//update horizontal position
  alienBlockState = !alienBlockState;
  write_alien_block_to_memory();
  return;/*}}}*/
}

/* call this function from updateBullets. No longer killed by user input. Passed x,y arguments from update bullets
 * must find out which alien is represented by those coordinates, then update boolean array. Increase score
*/

void killAlien(unsigned short x, unsigned short y){
  /*{{{*/
  static bool newDeadCol;
  int i;
  unsigned int col,row;
  col = (x - globals_getAlienBlockPosition().x) / (WIDTH_ALIENS + WIDTH_ALIEN_COL_SPACE);
  row = (y - globals_getAlienBlockPosition().y) / (ALIEN_HEIGHT + ALIEN_ROW_SEPARATION);
  globals_alien = col + (row * numberOfCol);
  globals_DeadAliens[globals_alien] = true; //kill the alien
  //last alien in column? If so, adjust column globals
  /*
   * 0
   * 11
   * 22
   * 33
   * 44
   */
  newDeadCol = true;
  for(i = col; i < col + (numberOfCol*4+1); i+=numberOfCol){
	//if any are alive, stop checking
	if(globals_DeadAliens[i] == false){
		newDeadCol = false;
		break;
	}
  }
  if(newDeadCol){
	  globals_deadColumns[col] = DEAD;
	  if(globals_deadColumns[leftMostCol] == DEAD){
	    leftMostCol++;
	    numberOfCol--;
	    rightMostCol--;
	    point_t alienBlockLocation= globals_getAlienBlockPosition();
	    alienBlockLocation.x = globals_getAlienPosition(leftMostCol).x;
	    globals_setAlienBlockPosition(alienBlockLocation);
	  }
	  if(globals_deadColumns[rightMostCol] == DEAD){
	    rightMostCol--;
	  }
  }
  beginAlienExplosion = true;
  write_explosion_to_memory(globals_alien);
  score += ALIEN_SCORE;
  write_score_to_memory(score);
  return;/*}}}*/
}

void newTankBullet(){
  //is there already a bullet on screen for the tank?/*{{{*/
  if(!tankBulletOffscreen){
    return;
  }
  tankBulletOffscreen = false;
  unsigned short tankPosition = globals_getTankPosition();
  point_t bulletPosition;
  bulletPosition.x = tankPosition + (WIDTH_TANK / 2) + TANK_SIDE_SPACE - 2; //set x position to be at center of tank
  bulletPosition.y = TANK_Y_POSITION - HEIGHT_BULLET; //one pixel above tank. Origin at top left
  globals_setTankBulletPosition(bulletPosition);
  //draw bullet
  write_new_tank_bullet_to_memory();
  return;/*}}}*/
}

static bool deadColumnFt(uint8_t Column){
  int i;/*{{{*/
  for(i = Column; i <= Column+44;i+=numberOfCol){
    if(!globals_DeadAliens[i]){
      return false;//at least one alien in column is alive
    }
  }
  return true;//all aliens in column are dead/*}}}*/
}

void newAlienBullet(){
  bullet_t bullet;/*{{{*/
  uint8_t i = 0;
  //see if 4 live bullets on screen already
  for(i = 0; i < 4; i++){
    if(globals_bullets[i].offScreen == true){
      break;//at least one bullet is offscreen. Okay to make a new one. 
    }
    if(i == 3){
      return;//4 bullets are active on screen. Do not make a new bullet
    }
  }
  uint8_t bulletArrayFreeSpace = i;
  uint8_t bulletType;
  uint8_t alienColumn;

  point_t alienBullet;
  //initialize random generator
  bool deadColumn;
  bulletType = rand() % 4; //randomly choose number from 0-3 to represent bullet types
  do{
    alienColumn = rand() % numberOfCol; //choose which of the 11 columns will shoot the bullet
    deadColumn = deadColumnFt(alienColumn);
  }while(deadColumn);
  //xil_printf("column: %d dead: %d\n\r",alienColumn,deadColumn);
  //find first live alien in column
  /*
   * 0  @ @ @ @ @ @ @ @ @ @ @
   * 11 @ @ @ @ @ @ @ @ @ @ @
   * 22 @ @ @ @ @ @ @ @ @ @ @
   * 33 @ @ @ @ @ @ @ @ @ @ @
   * 44 @ @ @ @ @ @ @ @ @ @ @
  */
  uint8_t bestLiveAlien;
  for(i = alienColumn+(4*numberOfCol); i >= alienColumn; i-=numberOfCol){//advance through rows in column
    if(globals_DeadAliens[i] == false){
      bestLiveAlien = i;
      break;
    }
  }
  //xil_printf("best alien: %d\n\r",bestLiveAlien);
  //form bullet right below chosen alien in the center of the column
  point_t alienPosition = globals_getAlienPosition(bestLiveAlien);
  //xil_printf("alien y position: %d\n\r",alienPosition.y);
  alienBullet.x = alienPosition.x + (ALIEN_WIDTH/2+1);
  alienBullet.y = alienPosition.y + ALIEN_HEIGHT + 1;//one pixel below alien
  //update bullet with above parameters
  bullet.type = bulletType;
  bullet.position = alienBullet;
  bullet.offScreen = false;
  //add new bullet to free spot in array
  globals_bullets[bulletArrayFreeSpace] = bullet;
  //draw bullet at location
  write_alien_bullets_to_memory();
  return;/*}}}*/
}

/*set bullets to offscreen when they collide with anything. Check pixel color. If != BLACK, bullet hit something. Consider the following:
 * if WHITE: hit alien. Pass x,y location to kill alien
 * if GREEN: could be tank or bunker. If y >= 400 then it must be then tank. If bunker, pass location to erodeBunker(). Otherwise, call killTank()
 * if RED: mothership. Call killMothership()
 * change offscreen limits to not include score and live otherwise the above colors will exist in other places than just aliens/bunkers/tank
*/

void updateBullets(){
  //move all alien bullets down/*{{{*/
  int i,j;
  int color[6];
  int tankColor[4];
  for(i = 0; i<4;i++){//iterate through alien bullet array
    //only update position if bullet is onscreen
    if(globals_bullets[i].offScreen == false){
      globals_bullets[i].position.y += BULLET_MOVEMENT_DISTANCE;//preemptively grab new position
      if(globals_bullets[i].position.y > GROUND_OFFSET-1){
        globals_bullets[i].offScreen = true;
      }else{
		  for(j = 0; j < 6; j++){//check all six pixels of alien bullet for collision
			  color[j] = get_pixel_color(globals_bullets[i].position.x+j,(globals_bullets[i].position.y + 18));
			  if(color[j]==GREEN){//hit tank or bunker
				(globals_bullets[i].position.y > (BUNKER_HALF_ROW + BUNKER_ROW_OFFSET)) ? killTank() : erodeBunker(globals_bullets[i].position.x,globals_bullets[i].position.y);
				//xil_printf("bullet.y: %d\n\r",globals_bullets[i].position.y);
				globals_bullets[i].offScreen = true;
				break;
			  }
		  }
      }
    }
    write_alien_bullets_to_memory();
  }

  //move tank bullets up
  if(!tankBulletOffscreen){
	  point_t tankBullet = globals_getTankBulletPosition();
	  tankBullet.y -= BULLET_MOVEMENT_DISTANCE;
	  if(tankBullet.y <= TOP_OF_SCREEN){//offscreen
		  tankBulletOffscreen = true;
	  }else{
		  for(i = 1; i < 3; ++i){//only pixels 2 and 3 in the tank bullet are white
			  tankColor[i] = get_pixel_color(tankBullet.x+i, tankBullet.y);
			  if(tankColor[i] != BLACK){
				//xil_printf("tankbullet color: %d\n\r",tankColor[i]);
				tankBulletOffscreen = true;
				if(tankColor[i] == GREEN){
					erodeBunker(tankBullet.x+i,tankBullet.y);
				}
				if(tankColor[i] == RED){
					killMothership();
				}
				if(tankColor[i] == WHITE){
					killAlien(tankBullet.x+i,tankBullet.y);
				}
			  }
		  }
	  }
	  globals_setTankBulletPosition(tankBullet);
	  write_tank_bullet_to_memory();
  }

  return;/*}}}*/
}

/* take in x,y parameters from updateBullets. Find out which part of the bunker was hit and change the destruction level of that portion*/

void erodeBunker(unsigned short x, unsigned short y){
/*{{{*/
	//xil_printf("entered erode bunker\n\r");
  unsigned int id;
  unsigned int bunker_x;
  unsigned int region;
  unsigned int row,col;
  //find out which bunker was hit

  if(x >= BUNKER_3 - 2){
    id = 3;
    bunker_x = BUNKER_3;
  }
  else if(x >= BUNKER_2 - 2){
    id = 2;
    bunker_x = BUNKER_2;
  }
  else if(x >= BUNKER_1 - 2){
    id = 1;
    bunker_x = BUNKER_1;
  }
  else{
    id = 0;
    bunker_x = BUNKER_0;
  }

  //find out which region of the bunker was hit. 3 rows, 4 cols
 // col = (x - globals_getAlienBlockPosition().x) / (WIDTH_ALIENS + WIDTH_ALIEN_COL_SPACE);
 // row = (y - globals_getAlienBlockPosition().y) / (ALIEN_HEIGHT + ALIEN_ROW_SEPARATION);
  //globals_alien = col + (row * 11);
  col = (x - bunker_x ) / (BUNKER_WIDTH / 4);
  row = (y - BUNKER_ROW_OFFSET) / (BUNKER_HEIGHT / 3);
  region = col + (row << 2); //row * 4
  globals_bunkers[id].quadrants[region].destruction_level += 1;
  //xil_printf("Bunker id: %d   destruction_level: %d\n\r",id,globals_bunkers[id].quadrants[region].destruction_level);
  write_an_erosion_to_memory(row, col, globals_bunkers[id].quadrants[region].destruction_level);
  return;/*}}}*/
}

/* make new functions:
 * killTank(): destroys the tank and decrements lives. 
 * killMothership(): destroy mothership, restart mothership spawn counter, increase points
*/

//ensure game pauses. Do 'splosion animation. Decrease lives. 
//    ^ to do this, set global boolean here that will be looked at by FIT. FIT will loop in animation until completed

void killTank(){
/*{{{*/
	//xil_printf("kill tank entered\n\r");
  int i;
  --lives;
  write_lives_to_memory();  
  globals_tankDeath = running;
  for(i = 0; i < 4; i++){
    globals_bullets[i].offScreen = true;
  }
  write_alien_bullets_to_memory();
  tankBulletOffscreen = true;
  write_tank_bullet_to_memory();

  return;/*}}}*/
}

void killMothership(){
  /*{{{*/
  write_mothership_hit_score_to_memory();
  write_mothership_black_to_memory();
  score += MOTHERSHIP_SCORE;
  write_score_to_memory(score);
  //assign new random spawn value for mothership
  mothershipSpawnCounter = rand() % (MOTHERSHIP_MAX + 1 - MOTHERSHIP_MIN) + MOTHERSHIP_MIN;
  globals_mothershipState = DEAD;
  return;/*}}}*/
}
