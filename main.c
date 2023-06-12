//ldd: -lncurses
#include <curses.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// https://invisible-island.net/ncurses/ncurses-intro.html#stdscr
// https://fr.wikipedia.org/wiki/Table_des_caract%C3%A8res_Unicode/U25A0 

//5/6 en testant sur plusieur écrans : 1080*1920 => 62*272 || 1050*1680 => 62*237 || 1600*900 (vertical) => 96*126 ||| {stty size}
//5/6 penser a chercher le shmilblick dans gnome-characters
//5/6 j'ai toujours paul@localhost:~/Documents/Ratrapages C/Workspace> qui s'affiche quand je lance, 10/6 j'essaie direct dans un tty en vm (suse), marche pas jsp pourquoi (ncurses est chiant a installer sur suse sans gui, c'est pratique yast 1 click)
//12/6 j'essaie sur mon serv, par defaut gcc n'est pas installé sur ubuntu server, curieux.
//6/6 git : https://github.com/PaulVerot03/spaceinvaders //je l'ai mis sur github histoire de plus facillement tester sur d'autres machines, nottament la vm foireuse


void INIT() {
  //initscr();  
  WINDOW *win = initscr();
  curs_set(0);
  nonl();
  //scrollok(); //main.c:23:3: error: too few arguments to function ‘scrollok’
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

int ship;

void wait_key() {
  while (getch() == ERR) {
    switch (ERR){
      case KEY_LEFT:
        ship -= 1;
        break;
      case KEY_RIGHT:
        ship += 1;
        break;
    }
    refresh();
    //usleep(1000);
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

void init_ship(){
  ship = COLS/2; //normalement, si COLS est impair, la division de deux ints reste un int
}

void print_ship(){
  attron(COLOR_SHIP);
  mvaddch(LINES - 1,ship,ACS_BLOCK); //ACS_BLOCK apparait comme un # 
  //en cherchant dans curses.h je trouve #define ACS_BLOCK   NCURSES_ACS('0') /*solid square block*/, donc bah, je sais pas 
  // trouvé, ya pas de couleur par défaut, faut en definir une pour ACS ? Quand j'essaie avec une lettre ca s'affiche en blanc par defaut
}

int **bombs;
//int rows = LINES; //main.c:78:12: error: initializer element is not constant  
//int columns = COLS;  //main.c:79:15: error: initializer element is not constant; bon bah ca degage
   
void init_bombs(){
  bombs = (int **)malloc(sizeof(int *)*LINES); //j'ai beaucoup de mal avec malloc, ducoup j'ai demandé de l'aide a une amie : ici, j'attribut une première dimension au tableau
  for(int i = 0 ; i < LINES; i++){
    bombs[i] = (int*)malloc(COLS * sizeof(int)); //j'ai pas tout compris mais ca à l'air de marcher
  }
  for (int i = 0; i < LINES; i++) {
        for (int j = 0; j < COLS; j++) { //initialise toutes les bombes à 0. elles seront attribuée plus tard.
           bombs[i][j]= 0;
        }
        
    }

}
    
void print_bombs(){ 
  attron(COLOR_BOMBS);
  
}

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
  print_ship();
  wait_key();
  refresh();
 // DONE();
 free(bombs);


}
