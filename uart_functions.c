#include "globals.h"
#include "uart_functions.h"
#include "ScreenFunctions.h"
#include <time.h>

static bool lowered;

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
  //TODO find left and right most columns based on globals_deadColumns[10]. Move block accordingly
  point_t alienBlockLocation;/*{{{*/
  alienBlockLocation = globals_getAlienBlockPosition();
  if(blockMovingRight){
    alienBlockLocation.x += BLOCK_MOVEMENT_X;
  }else{
    alienBlockLocation.x -= BLOCK_MOVEMENT_X;
  }
  //reference block by left side. If all the way right, change direction to left
  //move down if all the way at right of screen and change direction
  if(alienBlockLocation.x > X_MAX - STOP_DISTANCE - BLOCK_WIDTH - BLOCK_SIDE_SPACE-1 && !lowered && blockMovingRight){
    alienBlockLocation.y += BLOCK_MOVEMENT_Y;
    alienBlockLocation.x -= BLOCK_MOVEMENT_X;//don't move horizontally
    lowered = true;
    blockMovingRight = false;
  }
  //move down if all the way at left of screen and change direction
  else if(alienBlockLocation.x+BLOCK_SIDE_SPACE+1 < STOP_DISTANCE  && !lowered && !blockMovingRight){
    alienBlockLocation.y += BLOCK_MOVEMENT_Y;
    alienBlockLocation.x += BLOCK_MOVEMENT_X;//don't move horizontally
    lowered = true;
    blockMovingRight = true;
  }
  else{
    lowered = false;
  }
  globals_setAlienBlockPosition(alienBlockLocation);//update horizontal position
  alienBlockState = !alienBlockState;
  write_alien_block_to_memory();
  return;/*}}}*/
}

/* call this function from updateBullets. No longer killed by user input. Passed x,y arguments from update bullets
 * must find out which alien is represented by those coordinates, then update boolean array. Increase score
*/
//TODO add draw alien explosion function
void killAlien(unsigned short x, unsigned short y){
  /*{{{*/
  int i;
  col = (x - globals_getAlienBlockPosition().x) / (WIDTH_ALIENS + WIDTH_ALIEN_COL_SPACE);
  row = (y - globals_getAlienBlockPosition().y) / (ALIEN_HEIGHT + ALIEN_ROW_SEPARATION);
  unsigned int alien = col + (row * 11);
  globals_DeadAliens[alien] = true; //kill the alien
  //last alien in column? If so, adjust column globals
  for(i = col; i < col + 33; i+=11){
    if(globals_DeadAliens[i] = true){
      continue;
    }
    else
      break;
    globals_deadColumns[col] = DEAD;
  }
    
  //TODO draw alien 'splosion
  //redraw alien block (preferably only the alien that died)
  write_alien_block_to_memory();
  score += ALIEN_SCORE;
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
  for(i = Column; i <= Column+44;i+=1){
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
    alienColumn = rand() % 11; //choose which of the 11 columns will shoot the bullet
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
  for(i = alienColumn+44; i >= alienColumn; i-=11){//advance through rows in column
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
/*TODO test. Otherwise completed */
void updateBullets(){
  //move all alien bullets down/*{{{*/
  int i;
  int color;
  for(i = 0; i<4;i++){//iterate through alien bullet array
    //only update position if bullet is onscreen
    if(globals_bullets[i].offscreen = false){
      globals_bullets[i].position.y += BULLET_MOVEMENT_DISTANCE;
      color = get_pixel_color(globals_bullets[i].position.x,globals_bullets[i].position.y)
      //if bullet hits something or goes off screen change bullet state in array
      if(  color != BLACK || globals_bullets[i].position.y > Y_MAX){
        globals_bullets[i].offScreen = true;
        //alien bullet can hit bunker or tank
        if(color == GREEN){ //is color offscreen black? Probably not. Extra check for green
          (globals_bullets[i].position.y > TANK_BUFFER) ? killTank() : killBunker(globals_bullets[i].position.x,globals_bullets[i].position.y);
        }
      }
    }
  }
  //move tank bullets up
  point_t tankBullet = globals_getTankBulletPosition();
  tankBullet.y -= BULLET_MOVEMENT_DISTANCE;
  color = get_pixel_color(tankBullet.x, tankBullet.y);
  globals_setTankBulletPosition(tankBullet);
  //offscreen if hit an object or when off top of screen
  if(tankBullet.y <= 7 || color != BLACK){
    tankBulletOffscreen = true;
    switch(color){
      case GREEN:
        killBunker(tankBullet.x,tankBullet.y);
        break;
      case RED:
        killMothership();
        break;
      case WHITE:
        killAlien(tankBullet.x,tankBullet.y);
        break;
      default:
        break;
    }
  }

  write_alien_bullets_to_memory();
  write_tank_bullet_to_memory();
  return;/*}}}*/
}

/* take in x,y parameters from updateBullets. Find out which part of the bunker was hit and change the destruction level of that portion*/
//TODO test. Otherwise completed
void erodeBunker(unsigned short x, unsigned short y){
/*{{{*/
  unsigned int id;
  unsigned int bunker_x;
  unsigned int region;
  //find out which bunker was hit
  if(x > BUNKER_3){
    id = 3;
    bunker_x = BUNKER_3;
  }
  else if(x > BUNKER_2){
    id = 2;
    bunker_x = BUNKER_2;
  }
  else if(x > BUNKER_1){
    id = 1;
    bunker_x = BUNKER_1;
  }
  else{
    id = 0;
    bunker_x = BUNKER_0;
  }
  //find out which region of the bunker was hit. 3 rows, 4 cols
  col = (x - bunker_x) / WIDTH_BUNKER;
  row = (y - BUNKER_ROW_OFFSET) / BUNKER_HEIGHT;
  region = col + (row * 4);
  globals_bunkers[id].quadrants[region].destruction_level += 1;
  write_bunkers_to_memory();
  return;/*}}}*/
}

/* make new functions:
 * killTank(): destroys the tank and decrements lives. 
 * killMothership(): destroy mothership, restart mothership spawn counter, increase points
*/

//ensure game pauses. Do 'splosion animation. Decrease lives. 
//    ^ to do this, set global boolean here that will be looked at by FIT. FIT will loop in animation until completed
void killTank(){
  --lives;  
  tank_death = running;
  return;
}

//TODO add erase mothership function
void killMothership(){
  //erase mothership
  score += MOTHERSHIP_SCORE;
  mothershipSpawnCounter = 0;
  return;
}
