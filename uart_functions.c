#include "globals.h"
#include "uart_functions.h"
#include "ScreenFunctions.h"
#include <time.h>

static bool lowered;
static uint8_t rightMostCol = 10;

#define MOTHERSHIP_MIN 600 //6 seconds between spawns
#define MOTHERSHIP_MAX 2000 //20 seconds between spawns
#define DEAD_COL_OFFSET 4
#define TOP_ALIEN_SCORE 40
#define MID_ALIEN_SCORE 20
#define LOW_ALIEN_SCORE 10
#define MAX_ALIEN_BULLETS 4
#define ALIEN_BULLET_PIXELS 6
#define HEIGHT_OF_ALIEN_BULLET 18
#define REGION_COL_NUM 4
#define REGION_ROW_NUM 3

void moveTankLeft(){
  unsigned short tankPosition;/*{{{*/
  tankPosition = globals_getTankPosition()-TANK_MOVEMENT; //preemptively update tankPosition
  if(tankPosition+TANK_SIDE_SPACE < STOP_DISTANCE){//do nothing if tank is at far left of screen
    return;
  }
  else{
    globals_setTankPosition(tankPosition); //set the global tank position
  }
  //redraw tank
  write_tank_to_memory(); //redraw the tank
  return;/*}}}*/
}

void moveTankRight(){
  /*{{{*/
  unsigned short tankPosition;
  tankPosition = globals_getTankPosition() + TANK_MOVEMENT; //preemptively update tankPosiiton
  if(tankPosition > X_MAX - STOP_DISTANCE - WIDTH_TANK - TANK_SIDE_SPACE){//do nothing if tank is at far right of screen, reference by left side of tank
    return;
  }
  else{
    globals_setTankPosition(tankPosition); //set the global tank position
  }
  //redraw tank
  write_tank_to_memory(); //redraw the tank
  return;/*}}}*/
}


void moveAlienBlock(){
/*{{{*/
  point_t alienBlockLocation;
  alienBlockLocation = globals_getAlienBlockPosition(); //get current block locaiton

  //preemptively move block based on direction
  if(blockMovingRight){
    alienBlockLocation.x += BLOCK_MOVEMENT_X; 
  }else{
    alienBlockLocation.x -= BLOCK_MOVEMENT_X;
  }
  //calculate the very right edge of the block, regardless of the number of columns
  uint16_t right_edge = globals_getAlienPosition(rightMostCol).x+ALIEN_WIDTH+WIDTH_ALIEN_COL_SPACE;
  //reference block by left side. If all the way right, change direction to left
  //move down if all the way at right of screen and change direction
  if(right_edge > X_MAX - STOP_DISTANCE - 1 && !lowered && blockMovingRight){
    alienBlockLocation.y += BLOCK_MOVEMENT_Y;
    alienBlockLocation.x -= BLOCK_MOVEMENT_X;//don't move horizontally
    lowered = true;//the block did not just move down. Can do so next time
    blockMovingRight = false;
    globals_setAlienBlockPosition(alienBlockLocation);//update horizontal position
    write_dead_aliens_to_memory();

  }
  //move down if all the way at left of screen and change direction. This takes into account lost columns
  else if(alienBlockLocation.x < STOP_DISTANCE && !lowered && !blockMovingRight){
    alienBlockLocation.y += BLOCK_MOVEMENT_Y;
    alienBlockLocation.x += BLOCK_MOVEMENT_X;//don't move horizontally
    lowered = true;//block just moved down. Next move must be to the side
    blockMovingRight = true;
    globals_setAlienBlockPosition(alienBlockLocation);//update horizontal position
    write_dead_aliens_to_memory();
  }
  else{
	globals_setAlienBlockPosition(alienBlockLocation);//update horizontal position
    lowered = false;//the block did not just move down. Can do so next time
  }
  alienBlockState = !alienBlockState;
  write_alien_block_to_memory();
  return;/*}}}*/
}

/* call this function from updateBullets. No longer killed by user input. Passed x,y arguments from update bullets
 * must find out which alien is represented by those coordinates, then update boolean array. Increase score
*/

void killAlien(unsigned short x, unsigned short y){
  /*{{{*/
  dead_alien_count++; //used for deciding the speed of the aliens. Increases as more die
  static bool newDeadCol; //causes sever conditions. See below
  int i;
  unsigned int col,row;
  col = (x - globals_getAlienBlockPosition().x) / (WIDTH_ALIENS + WIDTH_ALIEN_COL_SPACE); //according to NEW arrangement
  row = (y - globals_getAlienBlockPosition().y) / (ALIEN_HEIGHT + ALIEN_ROW_SEPARATION); //regardless of arrangement
  globals_alien = col + (row * MAXIMUM_COL)+offset; //use offset to change to ORIGINAL number of col
  globals_DeadAliens[globals_alien] = true; //kill the alien
  //last alien in column? If so, adjust column globals number of col number of col
  newDeadCol = true;
  //see if col is dead according to ORIGINAL arrangement
  for(i = col+offset; i <= col + (MAXIMUM_COL*DEAD_COL_OFFSET+1); i+=MAXIMUM_COL){
	//if any are alive, stop checking
	if(globals_DeadAliens[i] == false){
		newDeadCol = false;
		break;
	}
  }
  //since a new col died, sever things need to be checked. Was the column the far left column? If so, the block
  //reference needs to be adjusted. The far right column? Adjust that
  if(newDeadCol){
	  globals_deadColumns[col+offset] = DEAD; //set col to dead according to ORIGINAL arrangement
          //was the first column destroyed?
	  if(globals_deadColumns[offset] == DEAD){
	    numberOfCol--;//decrease the number of columns for proper position finding
	    point_t alienBlockLocation= globals_getAlienBlockPosition();
	    alienBlockLocation.x = globals_getAlienPosition(offset + 1).x; //using ORIGINAL arrangement, set the new block reference
	    globals_setAlienBlockPosition(alienBlockLocation); 
	    offset++; //inc the offset to know how many columns the new block position is from the original
	  }
          //was the last column destroyed?
	  if(globals_deadColumns[rightMostCol] == DEAD){
	    rightMostCol--; //decrement for use in right_edge
	  }
  }
  beginAlienExplosion = true; //signal used in FIT. Gives enough time to see explosion
  write_explosion_to_memory(globals_alien); //draw the explosion
  //give scores based on alien type
  if(row == 0){
	  score += TOP_ALIEN_SCORE;
  }else if(row <= 2){
	  score += MID_ALIEN_SCORE;
  }else
	  score += LOW_ALIEN_SCORE;
  write_score_to_memory(score);
  return;/*}}}*/
}

void newTankBullet(){
  /*{{{*/
  //is there already a bullet on screen for the tank? if so, don't make another
  if(!tankBulletOffscreen){
    return;
  }
  tankBulletOffscreen = false; //there is now a tank bullet onscreen
  unsigned short tankPosition = globals_getTankPosition(); //get the position of the tank
  point_t bulletPosition;
  bulletPosition.x = tankPosition + (WIDTH_TANK / 2) + TANK_SIDE_SPACE - 2; //set x position to be at center of tank
  bulletPosition.y = TANK_Y_POSITION - HEIGHT_BULLET; //one pixel above tank. Origin at top left
  globals_setTankBulletPosition(bulletPosition); //set the new bullet position
  //draw bullet
  write_new_tank_bullet_to_memory();
  return;/*}}}*/
}

//helper funciton for determing if a column is dead or not
static bool deadColumnFt(uint8_t Column){
  int i;/*{{{*/
  //check every spot in a column based on the original block
  for(i = Column; i <= Column+(MAXIMUM_COL<<2);i+=MAXIMUM_COL){
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
  for(i = 0; i < MAX_ALIEN_BULLETS; i++){
    if(globals_bullets[i].offScreen == true){
      break;//at least one bullet is offscreen. Okay to make a new one. 
    }
    if(i == MAX_ALIEN_BULLETS-1){
      return;//4 bullets are active on screen. Do not make a new bullet
    }
  }
  uint8_t bulletArrayFreeSpace = i;
  uint8_t bulletType;
  uint8_t alienColumn;

  point_t alienBullet;
  //initialize random generator
  bool deadColumn;
  bulletType = rand() % MAX_ALIEN_BULLETS; //randomly choose number from 0-3 to represent bullet types
  do{
    alienColumn = rand() % MAXIMUM_COL; //choose which of the 11 columns will shoot the bullet
    deadColumn = deadColumnFt(alienColumn);
  }while(deadColumn);
  uint8_t bestLiveAlien;
  for(i = alienColumn+(MAX_ALIEN_BULLETS*MAXIMUM_COL); i >= alienColumn; i-=MAXIMUM_COL){//advance through rows in column
    if(globals_DeadAliens[i] == false){
      bestLiveAlien = i;
      break;
    }
  }
  //form bullet right below chosen alien in the center of the column
  point_t alienPosition = globals_getAlienPosition(bestLiveAlien);
  alienBullet.x = alienPosition.x + (ALIEN_WIDTH/2+1); //center on alien
  alienBullet.y = alienPosition.y + ALIEN_HEIGHT + 1;//one pixel below alien
  //update bullet with above parameters
  bullet.type = bulletType; //set bullet type
  bullet.position = alienBullet; //set bullet position
  bullet.offScreen = false; //set bullet to onScreen
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
  int temp;
  int i,j = 0;
  for(i = 0; i<MAX_ALIEN_BULLETS;i++){//iterate through alien bullet array
    //only update position if bullet is onscreen
    if(globals_bullets[i].offScreen == false){
      globals_bullets[i].position.y += BULLET_MOVEMENT_DISTANCE;//preemptively grab new position
      if(globals_bullets[i].position.y > GROUND_OFFSET-1){
        globals_bullets[i].offScreen = true;
      }else{
          for(j = 0; j < ALIEN_BULLET_PIXELS; j++){//check all six pixels of alien bullet for collision
            temp = get_pixel_color(globals_bullets[i].position.x+j,(globals_bullets[i].position.y + HEIGHT_OF_ALIEN_BULLET));
            if(temp == GREEN){//hit tank or bunker
              //did the bullet hit our tank or a bunker? Call the correct function
              (globals_bullets[i].position.y > (BUNKER_HALF_ROW + BUNKER_ROW_OFFSET)) ? killTank() : erodeBunker(globals_bullets[i].position.x+j,globals_bullets[i].position.y+HEIGHT_OF_ALIEN_BULLET + ALIEN_BULLET_PIXELS);
              globals_bullets[i].offScreen = true;
              break;
            }
          }
      }
    }
    write_alien_bullets_to_memory();
  }

  //move tank bullets up if a bullet is onscreen
  if(!tankBulletOffscreen){
	  point_t tankBullet = globals_getTankBulletPosition();
	  tankBullet.y -= BULLET_MOVEMENT_DISTANCE; //get the new locaiton
	  globals_setTankBulletPosition(tankBullet);
	  for(i = 1; i < 3; ++i){//only pixels 2 and 3 in the tank bullet are white
                  //grab the correct pixel color
		  temp = get_pixel_color(tankBullet.x+i, tankBullet.y); //function is found in screenfunctions.c
                  //stop looking for colors once one is found
		  if(temp != BLACK){
			  break;
		  }
	  }
          //a color was hit. What was it?
	  if(temp != BLACK ){
		  tankBulletOffscreen = true; //the bullet will be deleted
		  write_tank_bullet_to_memory(); //black out bullet
		  switch(temp){
                    //hit bunker. Call erodeBunker
		  case COLOR_GREEN:
			  erodeBunker(tankBullet.x+i,tankBullet.y);
			  break;
                    //hit alien. Call killAlien
		  case COLOR_WHITE:
			  killAlien(tankBullet.x+i,tankBullet.y);
			  break;
                    //hit mothership. Call killMothership
		  case COLOR_RED:
			  killMothership();
			  break;
		  default:
			  break;
		  }
	  }
          //did the bullet make it to the top of screen (below score and lives)?
	  else if(tankBullet.y <= TOP_OF_SCREEN){//offscreen
		  tankBulletOffscreen = true; //delete bullet
		  write_tank_bullet_to_memory();
          //bullet did not hit anything. Simply move up. 
	  }else{
		  write_tank_bullet_to_memory();
	  }
  }
  return;/*}}}*/
}

/* take in x,y parameters from updateBullets. Find out which part of the bunker was hit and change the destruction level of that portion*/

void erodeBunker(unsigned short x, unsigned short y){
/*{{{*/
  unsigned int id; //which bunker it was
  unsigned int bunker_x; //starting x position of the bunker
  unsigned int region; //which of the 12 regions of the bunker was hit
  unsigned int row,col; //the row and col used to calculate the region

  //find out which bunker was hit
  if(x >= BUNKER_3){
    id = 3;
    bunker_x = BUNKER_3;
  }
  else if(x >= BUNKER_2){
    id = 2;
    bunker_x = BUNKER_2;
  }
  else if(x >= BUNKER_1){
    id = 1;
    bunker_x = BUNKER_1;
  }
  else{
    id = 0;
    bunker_x = BUNKER_0;
  }

  //find out which region of the bunker was hit. 3 rows, 4 cols
  col = (x - bunker_x ) / (BUNKER_WIDTH / REGION_COL_NUM);
  row = (y - BUNKER_ROW_OFFSET) / (BUNKER_HEIGHT / REGION_ROW_NUM);
  region = col + (row << 2); //row * REGION_COL_NUM 
  globals_bunkers[id].quadrants[region].destruction_level += 1; //increase the level of destruciton
  write_an_erosion_to_memory(id, region);//draw destruciton

  return;/*}}}*/
}

void killTank(){
/*{{{*/
  int i;
  //looks like you're out of lives. Get gud nub
  if(lives == 1){
      write_game_over_to_memory();
      exit(0);
  }
  --lives; //decrement lives count
  write_lives_to_memory(); //erase one life 
  globals_tankDeath = running; //signal FIT to start the explosion animation
  //remove all alien bullets
  for(i = 0; i < MAX_ALIEN_BULLETS; i++){
    globals_bullets[i].offScreen = true;
  }
  write_alien_bullets_to_memory();
  //remove tank bullet
  tankBulletOffscreen = true;
  write_tank_bullet_to_memory();

  return;/*}}}*/
}

void killMothership(){
  /*{{{*/
  beginMotherExplosion = true;//signal FIT to begin score animation
  score += MOTHERSHIP_SCORE; //increase score and update on screen
  write_score_to_memory(score);
  globals_mothershipState = DEAD;//allow spawn counter to begin again
  return;/*}}}*/
}
