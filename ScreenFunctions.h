

#ifndef SCREENFUNCTIONS_H_
#define SCREENFUNCTIONS_H_

#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define SCORE_WORD_SIZE 5

void init_monitor();
void init_screen();
void write_alien_block_to_memory();
void write_top_row_aliens();
void write_middle_rows_aliens();
void write_bottom_rows_aliens();
void write_pixel_array(int row_offset, int col_offset, int row_array_max, int col_array_max, const int pixel_array[], int color);
void write_tank_to_memory();
void write_bunkers_to_memory();
void write_right_bunker(int row_offset, int col_offset, int row_array_max, int col_array_max, const int pixel_array[], int color);
void write_tank_bullet_to_memory();
void write_alien_bullets_to_memory();
void write_score_to_memory();
void write_lives_to_memory();
void write_bottom_line_to_memory();
void write_mothership_to_memory();
void write_mothership_hit_score_to_memory();
void write_mothership_black_to_memory();
void write_explosion_to_memory(int alien_index);
void write_score_word_to_memory();
void write_lives_word_to_memory();
int const* get_int_bitmap(int i);
int const* get_erosion_bitmap(int i);
void write_new_tank_bullet_to_memory();
void write_tank_explosion1();
void write_tank_explosion2();
void write_alien_dead_to_memory(int alien_index);
void write_an_erosion_to_memory(int bunker, int quadrant);

int alien_block_x;
int alien_block_y;
int current_alien;
unsigned int * framePointer0;
unsigned int * framePointer1;

//added by Keith. Grabs color of pixel
int get_pixel_color(int row, int col);

#endif /* SCREENFUNCTIONS_H_ */
