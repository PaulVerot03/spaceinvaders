//ldd: -lncurses
#include <curses.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// https://invisible-island.net/ncurses/ncurses-intro.html#stdscr
// https://fr.wikipedia.org/wiki/Table_des_caract%C3%A8res_Unicode/U25A0 

//en testant sur plusieur écrans : 1080*1920 => 62*272 || 1050*1680 => 62*237 || 1600*900 (vertical) => 96*126 ||| {stty size}
//penser a chercher le shmilblick dans gnome-characters
//j'ai toujours paul@localhost:~/Documents/Ratrapages C/Workspace> qui s'affiche quand je lance, j'essaie direct dans un tty en vm (suse)

//git 1


void INIT() {
  WINDOW *win = initscr();
  curs_set(0);
  nonl();
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_BLUE);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(3, COLOR_GREEN, COLOR_GREEN);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_WHITE, COLOR_BLACK);
  init_pair(6, COLOR_BLACK, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_WHITE);
  raw();
  keypad(stdscr, TRUE); 
  noecho();
  
  nodelay(win, 1);
  srand(time(NULL));
  init_ship();
}

void DONE() {
  endwin();
}

void wait_key() {
  while (getch() == ERR) {
    usleep(1000);
  }
}

#define COLOR_ALIENS COLOR_PAIR(1)
#define COLOR_SHIELDS COLOR_PAIR(2)
#define COLOR_SHIP COLOR_PAIR(3)
#define COLOR_BOMBS COLOR_PAIR(4)
#define COLOR_SHOTS COLOR_PAIR(5)
#define COLOR_DARK COLOR_PAIR(6)
#define COLOR_LIGHT COLOR_PAIR(7)
#define SECOND 1000000
int shots_rate = 10;
int main_rate = 100;
int aliens_rate = 4;
int bombs_rate = 8;
int bombs_chance = 12;

int ship;
void init_ship(){
  ship = COLS/2; //normalement, si COLS est impair, la division de deux ints reste un int
}

void print_ship(){
  mvaddch(LINES/2,COLS/2,'B');
}

int **bombs;
void init_bombs();
void print_bombs();

int **shots;
void init_shots();
void print_shots();

int **shields;
void init_shields(int count, int width);
void print_shields();

int **aliens, aliens_count;
char **frames;
char *frames_data;
int frames_count, frames_height, frames_width;
void init_aliens(char *path);
void print_alien_frame(int frame, int row, int col);
void print_aliens();

void init_game(char *aliens_path,
               int shields_count,
               int shields_width);


void print_game();
void game_over(int won);


void main(int argc, char *argv[]){
  INIT();
  //printf(0xE2);
  print_ship();
  refresh();
 // DONE();


}
