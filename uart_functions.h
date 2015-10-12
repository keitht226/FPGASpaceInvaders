#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include <stdbool.h>

bool blockMovingRight;

void uart_commands();
void moveTankLeft();
void moveTankRight();
void moveAlienBlock();
void killAlien(unsigned short x, unsigned short y);
void newTankBullet();
void newAlienBullet();
void updateBullets();
void erodeBunker(unsigned short x, unsigned short y);
void killTank();
void killMothership();



#endif /* UART_FUNCTIONS_H_ */
