#include "ScreenFunctions.h"
#include "globals.h"

#define GROUND_BUFFER 410

void init_monitor() {

	// The variables framePointer and framePointer1 are pointers to the base address of frame 0 and frame 1
	framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
	//framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640 * 480;
	// Just paint some large red, green, blue, and white squares in different quadrants
	int row = 0;
	int col = 0;
	for (row = 0; row < Y_MAX; row++) {
		for (col = 0; col < X_MAX; col++) {
			if (row < Y_MAX/2) {
				if (col < X_MAX/2) {
					// upper left corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = GREEN;
				} else {
					// upper right corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = BLUE;
				}
			} else {
				if (col < Y_MAX/2) {
					// lower left corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = YELLOW;
				} else {
					// lower right corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = RED;
				}
			}
		}
	}//end of row for loop*/

	init_screen();
	movedDown = false;
}

void init_screen() {
	point_t alien_start;
	alien_start.x = ALIEN_START;
	alien_start.y = ALIEN_START;
	globals_setAlienBlockPosition(alien_start);
	//globals_setTankPosition((unsigned short) 60);

	//Write our alien block to memory where it then is used to print to the screen
	write_alien_block_to_memory();

	write_tank_to_memory();
	write_bunkers_to_memory();
	write_tank_bullet_to_memory();
	write_alien_bullets_to_memory();

	write_score_word_to_memory();
	write_lives_word_to_memory();
	write_lives_to_memory();
	write_bottom_line_to_memory();

}

//Alien Block ----------------------------------------------------------------
void write_alien_block_to_memory() {
	movedDown = 0;
	write_top_row_aliens();
	write_middle_rows_aliens();
	write_bottom_rows_aliens();

}

void write_dead_aliens_to_memory() {
	movedDown = 1;
	write_top_row_aliens();
	write_middle_rows_aliens();
	write_bottom_rows_aliens();
}

//Alien Rows top/middle/bottom ----------------------------------------------------
void write_top_row_aliens() {
	current_alien = 0;
	while (current_alien < COLUMNS_OF_ALIENS) {
		point_t p = globals_getAlienPosition(current_alien);
		int row_offset = p.y;
		int col_offset = p.x;
		//If alien is dead, skip and don't draw it
		if (!globals_DeadAliens[current_alien]) {
			//check in or out state
			if (alienBlockState) { //subtracted 8 because the top row aliens have extra black pixels on top
				write_pixel_array(row_offset - TOP_ROW_ALIENS_EXTRA_PIXELS, col_offset, ALIEN_TOP_ROW, ALIEN_COL, alien_top_out, WHITE);
			} else {
				write_pixel_array(row_offset - TOP_ROW_ALIENS_EXTRA_PIXELS, col_offset, ALIEN_TOP_ROW, ALIEN_COL, alien_top_in, WHITE);
			}
		}
		else if (movedDown){
			//Write black space
			//write_pixel_array(row_offset - TOP_ROW_ALIENS_EXTRA_PIXELS, col_offset, ALIEN_TOP_ROW, ALIEN_COL, alien_dead_24x16, BLACK);
		}
		++current_alien;
	}
}
void write_middle_rows_aliens() {
	//Second row of aliens
	//current_alien is now 11
	while (current_alien < MAX_ALIEN_MID_ROW) {
		point_t p = globals_getAlienPosition(current_alien);
		int row_offset = p.y;
		int col_offset = p.x;
		//If alien is dead, skip and don't draw it
		if (!globals_DeadAliens[current_alien]) {
			//check in or out state
			if (alienBlockState) {
				write_pixel_array(row_offset, col_offset, ALIEN_ROW, ALIEN_COL, alien_middle_out, WHITE);
			}
			else {
				write_pixel_array(row_offset, col_offset, ALIEN_ROW, ALIEN_COL, alien_middle_in, WHITE);
			}
		}
		else if(movedDown){
			//Write black space
			write_pixel_array(row_offset + TOP_ROW_ALIENS_EXTRA_PIXELS/2, col_offset, ALIEN_ROW, ALIEN_COL, alien_dead_24x16, BLACK);
		}
		++current_alien;
	}
}
void write_bottom_rows_aliens() {
	//4rd row of aliens	
	//current_alien is now 33
	while (current_alien < MAX_ALIEN_BOTTOM) {
		point_t p = globals_getAlienPosition(current_alien);
		int row_offset = p.y;
		int col_offset = p.x;
		//If alien is dead, skip and don't draw it
		if (!globals_DeadAliens[current_alien]) {
			//check in or out state
			if (alienBlockState) {
				write_pixel_array(row_offset, col_offset, ALIEN_ROW, ALIEN_COL, alien_bottom_out, WHITE);
			} else {
				write_pixel_array(row_offset, col_offset, ALIEN_ROW, ALIEN_COL, alien_bottom_in, WHITE);
			}
		}
		else if(movedDown){
			//Write black space
			write_pixel_array(row_offset + TOP_ROW_ALIENS_EXTRA_PIXELS, col_offset, ALIEN_ROW, ALIEN_COL, alien_dead_24x16, BLACK);
		}
		++current_alien;
	}
}


//static unsigned short block_x,block_y;


//Alien Explosion ------------------------------------------------------

void write_explosion_to_memory(int alien_index) {
//	point_t alien_block = globals_getAlienBlockPosition();
//	block_x = alien_block.x;
//	block_y = alien_block.y;
	point_t p = globals_getAlienPosition(alien_index);
	int row_offset = p.y;
	int col_offset = p.x;
//	row_offset = block_y + (alien_index / COLUMNS_OF_ALIENS) * ALIEN_ROW +(alien_index/COLUMNS_OF_ALIENS);// ALIEN_ROW_SEPARATION * (alien_index/numberOfCol);
//	col_offset = (block_x + (alien_index % COLUMNS_OF_ALIENS) * ALIEN_COL + (alien_index%COLUMNS_OF_ALIENS) * ALIEN_COLUMN_SEPARATION) - (offset * ALIEN_COL + offset * ALIEN_COLUMN_SEPARATION);
	write_pixel_array(row_offset, col_offset, ALIEN_ROW, ALIEN_COL, alien_explosion_24x20, WHITE);
	//write_pixel_array(row_offset + alien_block_y, col_offset, ALIEN_ROW, ALIEN_COL, alien_dead_24x16, BLACK);


}

void write_alien_dead_to_memory(int alien_index) {
	//	point_t alien_block = globals_getAlienBlockPosition();
	//	block_x = alien_block.x;
	//	block_y = alien_block.y;
		point_t p = globals_getAlienPosition(alien_index);
		//point_t p = current_dead_alien;//
		int row_offset = p.y;
		int col_offset = p.x;
	//	row_offset = block_y + (alien_index / COLUMNS_OF_ALIENS) * ALIEN_ROW +(alien_index/COLUMNS_OF_ALIENS);// ALIEN_ROW_SEPARATION * (alien_index/numberOfCol);
	//	col_offset = (block_x + (alien_index % COLUMNS_OF_ALIENS) * ALIEN_COL + (alien_index%COLUMNS_OF_ALIENS) * ALIEN_COLUMN_SEPARATION) - (offset * ALIEN_COL + offset * ALIEN_COLUMN_SEPARATION);
		write_pixel_array(row_offset, col_offset, ALIEN_ROW, ALIEN_COL, alien_dead_24x16, BLACK);
		//write_pixel_array(row_offset + alien_block_y, col_offset, ALIEN_ROW, ALIEN_COL, alien_dead_24x16, BLACK);
}

//Tank ---------------------------------------------------------------------

void write_tank_to_memory() {
	int row_offset = TANK_ROW_OFFSET;
	int col_offset = (int) globals_getTankPosition();
	write_pixel_array(row_offset, col_offset, TANK_ROW, TANK_COL, tank_left, GREEN);
	write_pixel_array(row_offset, col_offset + BIT_32, TANK_ROW, TANK_COL, tank_right, GREEN);

}

void write_tank_explosion1() {
	int row_offset = TANK_ROW_OFFSET;
	int col_offset = (int) globals_getTankPosition() + BIT_32/2;
	write_pixel_array(row_offset, col_offset, TANK_ROW, TANK_COL, tank_explosion_1, GREEN);
	//write_pixel_array(row_offset, col_offset - BIT_32, TANK_ROW, TANK_COL, tank_right, GREEN);

}

void write_tank_explosion2() {
	int row_offset = TANK_ROW_OFFSET;
	int col_offset = (int) globals_getTankPosition() + BIT_32/2;
	write_pixel_array(row_offset, col_offset, TANK_ROW, TANK_COL, tank_explosion_2, GREEN);
	//write_pixel_array(row_offset, col_offset - BIT_32, TANK_ROW, TANK_COL, tank_right, GREEN);

}

void write_tank_black() {
	int row_offset = TANK_ROW_OFFSET;
	int col_offset = (int) globals_getTankPosition() + BIT_32/2;
	write_pixel_array(row_offset, col_offset, TANK_ROW, TANK_COL, tank_black, BLACK);
	//write_pixel_array(row_offset, col_offset - BIT_32, TANK_ROW, TANK_COL, tank_right, GREEN);

}

//Mothership ---------------------------------------

void write_mothership_to_memory() {
	int row_offset = MOTHERSHIP_ROW_OFFSET;
	int col_offset = (int) mothershipPosition;
	write_pixel_array(row_offset, col_offset, MOTHERSHIP_ROW, MOTHERSHIP_COL + M_SHIP_LEFT_OFFSET, mothership_left, RED);
	write_pixel_array(row_offset, col_offset + BIT_32, MOTHERSHIP_ROW + M_SHIP_RIGHT_OFFSET, MOTHERSHIP_COL, mothership_right, RED);

}

void write_mothership_hit_score_to_memory() {
	int row_offset = MOTHERSHIP_ROW_OFFSET;
	int col_offset = (int) mothershipPosition;
	write_pixel_array(row_offset, col_offset + BIT_32/2, MOTHERSHIP_ROW, BIT_32, mothership_score, WHITE);
	//write_pixel_array(row_offset, col_offset - BIT_32, MOTHERSHIP_ROW, MOTHERSHIP_COL, mothership_right, WHITE);

}

void write_mothership_black_to_memory() {
	int row_offset = MOTHERSHIP_ROW_OFFSET;
	int col_offset = (int) mothershipPosition;
	write_pixel_array(row_offset, col_offset + M_SHIP_BLACK_OFFSET, MOTHERSHIP_ROW, BIT_32, mothership_black, BLACK);

}



//Bunkers ----------------------------------------------------------------------
void write_bunkers_to_memory() {

	int i;
	//Iterate through each bunker
	for (i = 0; i < NUM_BUNKERS; ++i) {
		int bunker_col_offset = BUNKER_COL_OFFSET + (BUNKER_COL_OFFSET * i) + ((BUNKER_LEFT_COL + BUNKER_RIGHT_COL) * i);
		//Write full intact bunkers to memory
		write_pixel_array(BUNKER_ROW_OFFSET, bunker_col_offset, BUNKER_HALF_ROW, BUNKER_LEFT_COL, left_bunker_24x36, GREEN);
		write_pixel_array(BUNKER_ROW_OFFSET, bunker_col_offset + BUNKER_LEFT_COL, BUNKER_HALF_ROW, BUNKER_RIGHT_COL, right_bunker_24x36, GREEN);
	}//end of bunker for loop


	//Bunker half 36x24(rowXcol) -> 36x48 whole bunker
	//Bunker damage is 12x12
	//So each bunker has a 3x4 grid //xxxx 0 is top left
									//xxxx 11 is bottom right
									//x  x 9&10 don't need to be touched
}

//Erosion -------------------------------------------------------------------------
void write_an_erosion_to_memory(int bunker, int quadrant){



	//Row
	int erosion_row_offset = BUNKER_ROW_OFFSET + EROSION_ROWCOL * (quadrant / EROSION_QUAD_COLS);
	//Col
	int bunker_col_offset = BUNKER_COL_OFFSET + (BUNKER_COL_OFFSET * bunker) + ((BUNKER_LEFT_COL + BUNKER_RIGHT_COL) * bunker);
	int erosion_col_offset = bunker_col_offset + EROSION_ROWCOL * (quadrant % EROSION_QUAD_COLS);
	//If not zero draw the erosion over the bunker, 0 = no damage, 3 = gone
	int level;
	level = (int)globals_bunkers[bunker].quadrants[quadrant].destruction_level;
	if(level >= MAX_EROSION){
		write_pixel_array(erosion_row_offset, erosion_col_offset, EROSION_ROWCOL, EROSION_ROWCOL, bunkerDamage3_12x12, BLACK);
	}
	else{
		write_pixel_array(erosion_row_offset, erosion_col_offset, EROSION_ROWCOL, EROSION_ROWCOL, get_erosion_bitmap(quadrant, level), GREEN);
	}
	//xil_printf("eroded a bunker\n\r");
}

//Tank Bullets -------------------------------------------------------------
void write_tank_bullet_to_memory() {
	point_t position = globals_getTankBulletPosition();
	if(!tankBulletOffscreen) {
		write_pixel_array(position.y, position.x, NEW_BULLET_SPACER, TANK_BULLET_COL, tankBullet_4x18, YELLOW);
	}
	else {
		write_pixel_array(position.y, position.x, TANK_BULLET_ROW, TANK_BULLET_COL, tankBulletBlack, BLACK);
		//xil_printf("printed a black bullet at x: %d y: %d\n\r",position.x,position.y);
	}
}

//New Tank Bullets -------------------------------------------------------------
void write_new_tank_bullet_to_memory() {
	point_t position = globals_getTankBulletPosition();
	write_pixel_array(position.y, position.x, TANK_BULLET_NEW_ROW, TANK_BULLET_COL, tankBulletNew, YELLOW);
}

void write_alien_bullets_to_memory() {
	/*
	 typedef struct{
	 uint8_t type;  0,1,2,3
	 point_t position;
	 bool offScreen; //true if offScreen;
	 } bullet_t;
	 bullet_t globals_bullets[4];
	 */
	//need to make alien/tank bullet bitmaps, there's 4 different types? I only remember seeing 2 different looking ones
	int i;
	for(i = 0; i < MAX_ALIEN_BULLETS; ++i) {
		if(!globals_bullets[i].offScreen){
			switch(i){
			case 0:
				write_pixel_array(globals_bullets[i].position.y, globals_bullets[i].position.x, ALIEN_BULLET_ROW, ALIEN_BULLET_COL, alienBulletT0, YELLOW);
				break;
			case 1:
				write_pixel_array(globals_bullets[i].position.y, globals_bullets[i].position.x, ALIEN_BULLET_ROW, ALIEN_BULLET_COL, alienBulletT1, YELLOW);
				break;
			case 2:
				write_pixel_array(globals_bullets[i].position.y, globals_bullets[i].position.x, ALIEN_BULLET_ROW, ALIEN_BULLET_COL, alienBulletZ0, YELLOW);
				break;
			case 3:
				write_pixel_array(globals_bullets[i].position.y, globals_bullets[i].position.x, ALIEN_BULLET_ROW, ALIEN_BULLET_COL, alienBulletZ1, YELLOW);
				break;
			default:
				write_pixel_array(globals_bullets[i].position.y, globals_bullets[i].position.x, ALIEN_BULLET_ROW, ALIEN_BULLET_COL, alienBulletZ1, YELLOW);
			}
		}
		else {
			write_pixel_array(globals_bullets[i].position.y, globals_bullets[i].position.x, ALIEN_BULLET_ROW, ALIEN_BULLET_COL, alienBulletBlack, BLACK);
		}
	}
}





//Score -------------------------------------------------------------------------
void write_score_to_memory(int current_score) {
	int score_array[MAX_SCORE_SIZE];

	int j;
	for(j = 0; j < MAX_SCORE_SIZE; ++j){
		int digit;
		digit = current_score % TENS;
		score_array[j] = digit;
		current_score = current_score / TENS;
	}


	int i;
	i = MAX_SCORE_SIZE-1;
	while(i) {
		if(score_array[i]) {
			int offset_multiplier = 0;
			while(i){
				write_pixel_array(SCORE_ROW_OFFSET, SCORE_NUM_COL_OFFSET + (CHAR_WIDTH * offset_multiplier) + offset_multiplier*SCORE_MULTIPLIER, CHAR_HEIGHT, CHAR_WIDTH, get_int_bitmap(score_array[i]), YELLOW);
				--i;
				++offset_multiplier;
			}
			write_pixel_array(SCORE_ROW_OFFSET, SCORE_NUM_COL_OFFSET + (CHAR_WIDTH * offset_multiplier) + offset_multiplier*SCORE_MULTIPLIER, CHAR_HEIGHT, CHAR_WIDTH, get_int_bitmap(score_array[0]), YELLOW);
		}
		if(i == 0){
			break;
		}
		--i;
	}
}



void write_score_word_to_memory() {
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET, CHAR_HEIGHT, CHAR_WIDTH, letterS, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + CHAR_WIDTH + LETTER_SPACER, CHAR_HEIGHT, CHAR_WIDTH, letterC, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 2*CHAR_WIDTH + LETTER_SPACER*2, CHAR_HEIGHT, CHAR_WIDTH, letterO, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 3*CHAR_WIDTH + LETTER_SPACER*3, CHAR_HEIGHT, CHAR_WIDTH, letterR, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 4*CHAR_WIDTH + LETTER_SPACER*4, CHAR_HEIGHT, CHAR_WIDTH, letterE, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + START_0 + 6*CHAR_WIDTH + LETTER_SPACER*20, CHAR_HEIGHT, CHAR_WIDTH, num0, YELLOW);
}


//Lives -----------------------------------------------------------------------------------------
void write_lives_to_memory() {
	int i;
	//Write each of the lives left
	for(i = 0; i < STARTING_LIVES; ++i){
		if(i < lives){
			write_pixel_array(LIVES_ROW_OFFSET, LIVES_COL_OFFSET + (i * BIT_32 * 2), TANK_ROW, TANK_COL, tank_left, BLUE);
			write_pixel_array(LIVES_ROW_OFFSET, LIVES_COL_OFFSET + BIT_32 + (i * BIT_32 * 2), TANK_ROW, TANK_COL, tank_right, BLUE);
		}
		else{
			write_pixel_array(LIVES_ROW_OFFSET, LIVES_COL_OFFSET + (i * BIT_32 * 2), TANK_ROW, TANK_COL, tank_left, BLACK);
			write_pixel_array(LIVES_ROW_OFFSET, LIVES_COL_OFFSET + BIT_32 + (i * BIT_32 * 2), TANK_ROW, TANK_COL, tank_right, BLACK);
		}
	}


}

void write_lives_word_to_memory() {
	write_pixel_array(SCORE_ROW_OFFSET, LIVES_WORD_COL_OFFSET, CHAR_HEIGHT, CHAR_WIDTH, letterL, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, LIVES_WORD_COL_OFFSET + CHAR_WIDTH + LETTER_SPACER, CHAR_HEIGHT, CHAR_WIDTH, letterI, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, LIVES_WORD_COL_OFFSET + 2*CHAR_WIDTH + LETTER_SPACER*2, CHAR_HEIGHT, CHAR_WIDTH, letterV, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, LIVES_WORD_COL_OFFSET + 3*CHAR_WIDTH + LETTER_SPACER*3, CHAR_HEIGHT, CHAR_WIDTH, letterE, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, LIVES_WORD_COL_OFFSET + 4*CHAR_WIDTH + LETTER_SPACER*4, CHAR_HEIGHT, CHAR_WIDTH, letterS, PURPLE);
}


//Bottom Line -----------------------------------------------------------------------------------------
void write_bottom_line_to_memory() {
	int row;
	int col;
	for (row = GROUND_OFFSET; row < GROUND_OFFSET + GROUND_WIDTH; ++row) {
		for (col = 0; col < X_MAX; ++col) {
			//make the pixel at the location appear
			framePointer0[row * X_MAX + col] = RED;
			//framePointer1[row * X_MAX + col] = GREEN;
		}
	}
}


//End Game -----------------------------------------------------------------------------------------
void write_game_over_to_memory(){
	int row = 0;
	int col = 0;
	for (row = 0; row < Y_MAX; row++) {
		for (col = 0; col < X_MAX; col++) {
			if (row < Y_MAX/2) {
				if (col < X_MAX/2) {
					// upper left corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = GREEN;
				} else {
					// upper right corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = BLUE;
				}
			} else {
				if (col < Y_MAX/2) {
					// lower left corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = YELLOW;
				} else {
					// lower right corner.
					framePointer0[row * X_MAX + col] = BLACK;
					//framePointer1[row * 640 + col] = RED;
				}
			}
		}
	}//end of row for loop*/
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET, CHAR_HEIGHT, CHAR_WIDTH, letterG, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + CHAR_WIDTH + LETTER_SPACER, CHAR_HEIGHT, CHAR_WIDTH, letterA, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 2*CHAR_WIDTH + LETTER_SPACER*2, CHAR_HEIGHT, CHAR_WIDTH, letterM, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 3*CHAR_WIDTH + LETTER_SPACER*3, CHAR_HEIGHT, CHAR_WIDTH, letterE, PURPLE);

	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 5*CHAR_WIDTH + LETTER_SPACER*5, CHAR_HEIGHT, CHAR_WIDTH, letterE, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 6*CHAR_WIDTH + LETTER_SPACER*6, CHAR_HEIGHT, CHAR_WIDTH, letterR, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 7*CHAR_WIDTH + LETTER_SPACER*7, CHAR_HEIGHT, CHAR_WIDTH, letterR, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 8*CHAR_WIDTH + LETTER_SPACER*8, CHAR_HEIGHT, CHAR_WIDTH, letterO, PURPLE);
	write_pixel_array(SCORE_ROW_OFFSET, SCORE_COL_OFFSET + 9*CHAR_WIDTH + LETTER_SPACER*9, CHAR_HEIGHT, CHAR_WIDTH, letterR, PURPLE);
}




int const* get_int_bitmap(int i) {
	switch(i) {
		case 1:
			return num1;
		case 2:
			return num2;
		case 3:
			return num3;
		case 4:
			return num4;
		case 5:
			return num5;
		case 6:
			return num6;
		case 7:
			return num7;
		case 8:
			return num8;
		case 9:
			return num9;
		case 0:
			return num0;
		default:
			return alien_explosion_24x20;
	}
}

int const* get_erosion_bitmap(int quad, int level){
	switch(quad) {
		case 0://upper left
		{
			switch(level){
				case 1:
					return bunkerDamage2_left_corner;
				case 2:
					return bunkerDamage1_left_corner;
				case 3:
					return bunkerDamage0_left_corner;
				default:
					return bunkerDamage3_12x12;
			}
		}
		case 3://upper right
		{
			switch(level){
				case 1:
					return bunkerDamage2_right_corner;
				case 2:
					return bunkerDamage1_right_corner;
				case 3:
					return bunkerDamage0_right_corner;
				default:
					return bunkerDamage3_12x12;
			}
		}
		case 5://inner left
		{
			switch(level){
				case 1:
					return bunkerDamage2_inner_left;
				case 2:
					return bunkerDamage1_inner_left;
				case 3:
					return bunkerDamage0_inner_left;
				default:
					return bunkerDamage3_12x12;
			}
		}
		case 6://inner right
		{
			switch(level){
				case 1:
					return bunkerDamage2_innner_right;
				case 2:
					return bunkerDamage1_innner_right;
				case 3:
					return bunkerDamage0_innner_right;
				default:
					return bunkerDamage3_12x12;
			}
		}
		default:
		{
			switch(level){
				case 1:
					return bunkerDamage2_12x12;
				case 2:
					return bunkerDamage1_12x12;
				case 3:
					return bunkerDamage0_12x12;
				default:
					return bunkerDamage3_12x12;
			}
		}

	}
}







//Actual writing of pixel maps ------------------------------
void write_pixel_array(int row_offset, int col_offset, int row_array_max, int col_array_max, const int pixel_array[], int color) {
	int row;
	int col;

	for (row = 0; row < row_array_max; ++row) {
		for (col = 0; col < col_array_max; ++col) {
			//check bit map and get each bit from the number stored by masking for the bit in the unsigned int
			if (pixel_array[row] & (1 << (BIT_32 - 1 - col))) {
				//make the pixel at the location appear //with the color we want
				framePointer0[(row + row_offset) * X_MAX + col + col_offset] = color;
				//framePointer1[(row + row_offset) * X_MAX + col + col_offset] = color;
			}
			else{
			 //Pixel was not colored so make it black
			 framePointer0[(row + row_offset)* X_MAX + col + col_offset] = BLACK;
			 //framePointer1[(row + row_offset)* X_MAX + col + col_offset] = BLACK;
			 }
		}
	}
}

int get_pixel_color(int col, int row){
  int color;
  color = framePointer0[row * X_MAX + col];
  return color;
}
