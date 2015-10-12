#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include <stdbool.h>

bool blockMovingRight;

void uart_commands();
void moveTankLeft();
void moveTankRight();
void moveAlienBlock();
void killAlien();
void newTankBullet();
void newAlienBullet();
void updateBullets();
void erodeBunker();



#endif /* UART_FUNCTIONS_H_ */
