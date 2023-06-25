// ldd: -lncurses
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

// https://invisible-island.net/ncurses/ncurses-intro.html#stdscr
// https://fr.wikipedia.org/wiki/Table_des_caract%C3%A8res_Unicode/U25A0

// 5/6 en testant sur plusieur écrans : 1080*1920 => 62*272 || 1050*1680 =>
// 62*237 || 1600*900 (vertical) => 96*126 ||| {stty size} 5/6 penser a chercher
// le shmilblick dans gnome-characters 5/6 j'ai toujours
// paul@localhost:~/Documents/Ratrapages C/Workspace> qui s'affiche quand je lance, mais ca s'affiche pas sur cow... //14/6 avec scrollok ca disparait mais si j'appui sur une touche ca re-apparait si je scroll, ca ferme le programme, ok.
// 12/6 j'essaie sur mon serv, par defaut gcc n'est pas installé sur ubuntu server, curieux. 6/6 git : https://github.com/PaulVerot03/spaceinvaders

void INIT() {
  initscr();
  WINDOW *win = initscr();
  curs_set(0);
  nonl();
  //scrollok(win,TRUE); // main.c:23:3: error: too few arguments to function ‘scrollok’ trouvé, la doc est foireuse et indique pas comment ca marche.
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_BLUE);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(3, COLOR_GREEN, COLOR_GREEN);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_WHITE, COLOR_BLACK);
  init_pair(6, COLOR_BLACK, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_WHITE);
  int screen_height, screen_width;
  getmaxyx(stdscr, screen_height, screen_width);
  cbreak();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(win, 1);
  srand(time(NULL));
  init_ship();
  init_bombs();
  init_shots();
  init_shields();
}

void DONE() { endwin(); }

int ship;

void wait_key() {
  while (getch() == ERR) {
    switch (ERR) {
    case KEY_LEFT:
      ship -- ;
      print_ship();
      break;
    case KEY_RIGHT:
      ship ++;
      print_ship();
      break;
    default:
      break;
    }
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
#define SHIELD_HEIGHT 2
int shots_rate = 10;
int main_rate = 100;
int aliens_rate = 4;
int bombs_rate = 8;
int bombs_chance = 12;

void init_ship() {
  ship = COLS / 2;
}

void print_ship() {
  attron(COLOR_SHIP);
  mvaddch(LINES - 1, ship, ACS_BLOCK); // ACS_BLOCK apparait comme un #
  //trouvé, ya pas de couleur pardéfaut, faut en definir une pour ACS ? Quand j'essaie avec une lettre ca s'affiche en blanc par defaut
}

int **bombs;
// int rows = LINES; //main.c:78:12: error: initializer element is not constant
// int columns = COLS;  //main.c:79:15: error: initializer element is not constant; bon bah ca degage. Je comprend pas, sur des exemples en ligne ils font ca et ca marche

void init_bombs() {
  if(bombs != NULL){
  bombs = (int **)malloc(
      sizeof(int *) *
      LINES); // j'ai beaucoup de mal avec malloc, ducoup j'ai demandé de l'aide
              // a une amie : ici, j'attribut une première dimension au tableau
  for (int i = 0; i < LINES; i++) {
    bombs[i] = (int *)malloc(
        COLS * sizeof(int)); // j'ai pas tout compris mais ca à l'air de marcher
  }
  for (int i = 0; i < LINES; i++) {
    for (int j = 0; j < COLS; j++) { // initialise toutes les bombes à 0. il n'y a pas de bombes immédiatement au début.
      bombs[i][j] = 0;
    }
  }
  }
}

void print_bombs() {
  attron(COLOR_BOMBS);
  for (int i = 0; i < LINES; i++) {
    for (int j = 0; j < COLS; j++) {
      if (bombs[i][j] == 1) {
        mvaddch(i, j, ACS_DIAMOND);
        refresh();
      }
    }
  }
}

int **shots;
void init_shots() {
  if (shots != NULL){
  shots = (int **)malloc(sizeof(int *) * LINES); // copié collé des bombes
  for (int i = 0; i < LINES; i++) {
    shots[i] = (int *)malloc(COLS * sizeof(int));
  }
  for (int i = 0; i < LINES; i++) {
    for (int j = 0; j < COLS; j++) {
      shots[i][j] = 0;
    }
  }
  }
  
}
void print_shots() {
  attron(COLOR_SHOTS);
  for (int i = 0; i < LINES; i++) {
    for (int j = 0; j < COLS; j++) {
      if (shots[i][j] == 1) {
        mvaddch(i, j, ACS_DEGREE);
        refresh();
      }
    }
  }
}

int **shields;
int shield_count = 8;
int shield_width = 2;
int shield_spacing; 

void init_shields(int count, int width) {
  int flip = 0;
  if (shields != NULL){
    shields = (int **)malloc(sizeof(int *) * LINES);
  }
  for (int i = 0; i < LINES; i++) {
    shields[i] = (int *)malloc(COLS * sizeof(int));
  }
  shield_spacing = COLS/(shield_count*2);
  for (int i = LINES - 5; i < LINES - 3; i++) {
    for (int j = 0; j < COLS; j++) {
      if (flip < shield_width +1){
        shields[i][j] = 1;
        flip += 1;
      }
      else if (flip > shield_width){
        shields[i][j] = 0;
        flip += 1 ;
        if (flip > shield_spacing){
          flip = 0;
        }
      }
      
    }
  }
    //Segmentation fault (core dumped) //bon bah on va pas faire ca
  /*shields = (int **)malloc(sizeof(int *) * 2); // comme il n'y a que deux lignes de bombes 
  for (int i = 0; i < 2; i++) {
    shields[i] = (int *)malloc(COLS * sizeof(int));
  }
  for (int i = LINES - 5; i < LINES - 3; i++) {
    for (int j = 0; j < COLS; j++) {
      shields[i][j] = 1;
    }
  }*/
}


/*
void init_shields(int count, int width) {
    shield_count = count;
    shield_width = width;

    // Calculer l'espacement entre les boucliers pour les répartir sur la largeur de l'écran ///mega foireux
    int total_width = shield_count * shield_width;
    int available_width = COLS - total_width;
    shield_spacing = available_width / (shield_count + 1);

    
    shields = (int **)malloc(sizeof(int *) * LINES); 

    for (int i = 0; i < LINES; i++) {     // Initialise les boucliers dans le tableau
        shields[i] = (int *)malloc(sizeof(int) * COLS);

        for (int j = 0; j < COLS; j++) { 
          //ici trouver une maniere de faire 1/2
           shields[i][j] = 0;
        }

       // int start_col = (i + 1) * shield_spacing + i * shield_width; //ok, donc ca c'est cassé 
        //int end_col = start_col + shield_width - 1;

        for (int j = 0; j <= COLS; j++) {
           shields[i][j] = 1;
        }
    }
}
*/

void print_shields() { 
  attron(COLOR_SHIELDS);
  for (int i = LINES - 5; i < LINES - 3; i++) {
    for (int j = 0; j < COLS; j++) {
      if (shields[i][j] == 1){
          mvaddch(i,j, ACS_BLOCK);
          refresh();
      }
    }
  }
  
}

int **aliens, aliens_count;
char **frames ;
char *frames_data;
int frames_count, frames_height = 3, frames_width = 6;/*
void init_aliens(char *path){ //ca pareil, j'ai demandé de l'aide, je ne suis pas sur que ca marche, je fais confiance a @satan's queen sur discord
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    exit(1);
  }

    // compte les lignes
    int line_count = 0;
    int max_width = 0;
    char buffer[256]; //je voulais faire moin, mais je me suis fait engueuler parce que c'est "pas propre"
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int len = strlen(buffer);
        if (len > max_width) {
            max_width = len;
        }
        line_count++;
    }

    //remet la tete de lecture à 0
    fseek(file, 0, SEEK_SET);

    frames_data = malloc(sizeof(char) * line_count * max_width);

    //stocke les frazmes
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        strcpy(frames_data + i * max_width, buffer);
        i++;
    }

    fclose(file);

    frames_count = line_count / frames_height;
    frames_width = max_width;
    frames = malloc(sizeof(char *) * frames_count);

    for (i = 0; i < frames_count; i++) {
      frames[i] = frames_data + i * frames_height * frames_width;
    }
}*/
void print_alien_frame(int frame, int row, int col);
void print_aliens();

void init_game(char *aliens_path, int shields_count, int shields_width);

void print_game() {}
void game_over(int won);

int  main(void) {
  //init_shields(shield_count, shield_width);
  //char *path = "aliens.txt";
  //init_aliens(path);
  INIT();
  print_ship();
  print_bombs();
  print_shots();
  print_shields();
  wait_key();
  refresh();
  //DONE(); // celui là ferme correctement la page mais pas ceux que j'ai mis plus haut avec une touche
  /*for (int i = 0; i < shield_count; i++) {
    free(shields[i]);
  }*/
//je comprend pas, j'ai  free(): double free detected in tcache 2 Aborted (core dumped) ; ducoup je check d'abbord si c'est occupé avant de liberer et ca marche pas quand meme
  if(shots != NULL){free(shots);}
  if(frames != NULL){free(frames);}
  if(frames_data != NULL){free(frames_data);}
  if(bombs != NULL){free(bombs);}
  int i_shield = 0;
  while (shields[i_shield] != NULL){ //potentiellement foireux
    free(shields[i_shield]);
    i_shield ++;
  }
  //free(shields);
  //free(shots);
  //free(shields);
  //free(frames);
  //free(frames_data);
  //free(bombs);
}
