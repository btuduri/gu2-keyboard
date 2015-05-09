#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_ttf.h>

#define CURVCURS 60
#define RED 71
#define GREEN 75
#define BLUE 207
#define RED2 191
#define GREEN2 192
#define BLUE2  235
#define WIDTH 1024
#define HEIGHT 768
#define NB_RECORDS 100000

SDL_Window *p_window;
SDL_Surface *p_kenet;
SDL_Surface *p_fond_blanc;
SDL_Surface *p_fond;
SDL_Surface *p_cursor;
SDL_Surface *p_cursor_2;
SDL_Surface *p_car;
SDL_Surface *p_car_2;
SDL_Renderer *p_renderer;
SDL_Texture *p_texture;
SDL_Texture *p_texture_kenet;
SDL_Texture *p_texture_fond_blanc;
SDL_Texture *p_texture_cursor;
SDL_Texture *p_texture_cursor_2;
SDL_Texture *p_texture_car;
SDL_Texture *p_texture_car_2;
SDL_Color color_car;
SDL_Color color_omb;
SDL_Color color1, color2;
//
FILE *fichier;
//
SDL_Event event;
//
TTF_Font *font;

//
int esc;
int x, y, last_x, w_cursor, h_cursor, width, height, max_height, nb_lines;
int array_curv_curs[CURVCURS][3];
int index_curv_curs;
const char* car;
char actual_char;
char* array_car;
char* new_char;
char* line_char;
char* line_char_2;
char last_chars[3];
int ind_car_x, ind_car_y;
int typing;
int upper;
Uint32 pixel_format;
int last_index;
int car_ok, delay;
unsigned int tick, wait_val;
unsigned int* array_record;
int index_record;


void init();
void quit();
void keyboard();
void flash_screen();
void display_kenet();
void display_cursor();
void test_keyboard();
void display_text(SDL_Color color1, SDL_Color color2, int alpha);
void record_char(char c_char);
void init_coords();
int widthOK();
void make_enter();
void key_up();
void key_down();
void key_backspace();
void record();
void new_screen();
void save_record();

#endif // FUNCTIONS_H_INCLUDED
