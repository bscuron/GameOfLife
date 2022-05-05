#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

#define INIT_RATE 0.25
#define DELAY 10000

int rows, cols;
int generation = 1;
unsigned long long delay = 100000;

typedef enum CELL_TYPE{
    CELL_DEAD,
    CELL_ALIVE,
} CELL_TYPE;

struct winsize getWinsize(int fd);
void fillBoard(int board[rows][cols], int type);
void showBoard(int board[rows][cols]);
void initBoard(int board[rows][cols]);
void computeNextGeneration(int board[rows][cols]);
int getNeighbors(int board[rows][cols], int row, int col);
void handleKeys(int board[rows][cols]);
void shiftNorth(int board[rows][cols]);
void shiftSouth(int board[rows][cols]);
void shiftEast(int board[rows][cols]);
void shiftWest(int board[rows][cols]);

int main(void){
    srand(time(NULL));

    struct winsize win = getWinsize(STDOUT_FILENO); 
    rows = win.ws_row;
    cols = win.ws_col;
    int board[rows][cols];
    initBoard(board);
    
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    int quit = 0;
    while(!quit){
        generation++;
        showBoard(board);
        computeNextGeneration(board);
        handleKeys(board);
        usleep(DELAY);
    }
    endwin();

    return 0;
}

struct winsize getWinsize(int fd){
    struct winsize win;
    ioctl(fd, TIOCGWINSZ, &win);
    return win;
}

void fillBoard(int board[rows][cols], int type){
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            board[y][x] = type;
        }
    }
}

void showBoard(int board[rows][cols]){
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            switch(board[y][x]){
                case CELL_DEAD:
                    mvprintw(y, x, "%c\n", ' ');
                    break;
                case CELL_ALIVE:
                    mvprintw(y, x, "%c\n", 'X');
                    break;
            }
        }
    }
    mvprintw(0, 0, "Generation: %d\n", generation);
    refresh();
}

void initBoard(int board[rows][cols]){
    fillBoard(board, CELL_DEAD);
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            if(1.0 * rand() / RAND_MAX < INIT_RATE){
                board[y][x] = CELL_ALIVE;
            }
        }
    }
}

void computeNextGeneration(int board[rows][cols]){
    int board2[rows][cols];
    memcpy(board2, board, sizeof(int) * rows * cols);

    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            int neighbors = getNeighbors(board, y, x);
            if(board[y][x] && (neighbors == 2 || neighbors == 3)) continue;
            else if(!board[y][x] && neighbors == 3) board2[y][x] = CELL_ALIVE;
            else board2[y][x] = CELL_DEAD;
        }
    }
    memcpy(board, board2, sizeof(int) * rows * cols);
}

int getNeighbors(int board[rows][cols], int row, int col){
    int neighbors = 0;
    if(row > 1 && board[row - 1][col]) neighbors++;
    if(row > 1 && col < cols - 1 && board[row - 1][col + 1]) neighbors++;
    if(col < cols - 1 && board[row][col + 1]) neighbors++;
    if(row < rows - 1 && col < cols - 1 && board[row + 1][col + 1]) neighbors++;
    if(row < rows - 1 && board[row + 1][col]) neighbors++;
    if(row < rows - 1 && col > 1 && board[row + 1][col - 1]) neighbors++;
    if(col > 1 && board[row][col - 1]) neighbors++;
    if(row > 1 && col > 1 && board[row - 1][col - 1]) neighbors++;
    return neighbors;
}

void handleKeys(int board[rows][cols]){
    wchar_t code = getch();
    switch(code){
        case -1:
            return;
        case KEY_ENTER:
        case '\n':
        case ' ':
            generation = 1;
            initBoard(board);
            break;
    }
}
