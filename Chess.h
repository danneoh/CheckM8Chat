#ifndef CHESS_H
#define CHESS_H
// Constants intiated //
#define MAX_MSGLEN  100 
#define SQUARE_SIZE 50  
#define WINDOW_BORDER 10
#define BOARD_BORDER 10
#define BOARD_WIDTH  (8*SQUARE_SIZE)
#define BOARD_HEIGHT (8*SQUARE_SIZE)
#define WINDOW_WIDTH  (16*SQUARE_SIZE)
#define WINDOW_HEIGHT (12*SQUARE_SIZE)
// Struct to keep the game data //
typedef struct{
        int *board;
        int chose;
        int hold_x;
        int hold_y;
        int side;
        int teamcolor;
        GtkWidget *gamewindow;
        GtkWidget *fixed;
        GtkWidget *table;
} GAME;
// Function definitions //
void Chess();
void DrawBoard(GtkWidget *table, int board[64], int *teamcolor);
void team_color(GtkWidget *widget, GAME *game);
void singleplayer(GtkWidget *widget, gpointer data);
void destroy(GtkWidget *widget, gpointer data);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y);
void ChoosePiece(int g_x, int g_y, int board[64], GAME *game);
void MovePiece(int g_x, int g_y, int LPM[28], GAME *game);
gint area_click(GtkWidget *widget, GdkEvent *event, GAME *game); 

#endif
