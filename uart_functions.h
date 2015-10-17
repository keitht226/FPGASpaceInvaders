#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include <stdbool.h>

bool blockMovingRight;

void moveTankLeft(); //moves the tank left by pressing the left button
void moveTankRight(); //moves the tank right by pressing the right button
void moveAlienBlock(); //moves the alien block with FIT. Moves down at edge. Account for destroyed edge columns
void killAlien(unsigned short x, unsigned short y); //called by updateBullets. Updates deadAlien array and score. Signal explosion animation
void newTankBullet(); //makes a new tank bullet if none already exist by pressing center button
void newAlienBullet(); //randomly creates a random type of bullet unless there are already 4 present. 
void updateBullets(); //move all bullets the appropriate direction. Call erodeBunker, killTank, killMothership, or killAlien if bullet hits something
void erodeBunker(unsigned short x, unsigned short y); //find appropriate region based on bullets (12 regions, 4 col, 3 row) and destroy it one level
void killTank(); //signal tank animation and decrement lives. 
void killMothership(); //signal mothership score animation and increase score. Reset spawn counter



#endif /* UART_FUNCTIONS_H_ */
