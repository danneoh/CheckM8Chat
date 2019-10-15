#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "GameLogic.h"
#include "Chess.h"
#include "Chat.h"
void Chess(){           
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *button;
    GtkWidget *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);    
    gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT); 
    gtk_container_set_border_width(GTK_CONTAINER(window), BOARD_BORDER); 
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
    gtk_window_set_title(GTK_WINDOW(window), "Check-Mate's Chess"); 
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); 

    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(destroy), window);

    table = gtk_table_new(8, 8, TRUE);
    gtk_container_add(GTK_CONTAINER(window), table);
            
    label = gtk_label_new("Chess");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 3, 5, 1, 2);
    gtk_widget_show(label);
    
    button = gtk_button_new_with_label("Singleplayer");
    g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_hide), NULL);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), (gpointer)window);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(team_color), NULL);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 5, 3, 4);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Button");//creates button
    g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_hide), NULL);//when clicked hides window and goes to next window
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), window);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(Dashboard), NULL);//When the back button is clicked it goes back to main
    gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 0, 1); 
    gtk_widget_show(button);

    gtk_widget_show(table);
    gtk_widget_show(window);
    gtk_main();
}
void team_color(GtkWidget *widget, GAME *game){
    GtkWidget *teamwindow;
    GtkWidget *table;
    GtkWidget *button;
    GtkWidget *label;
    
    static int w[1] = {2};
    static int b[1] = {1};

    teamwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(teamwindow, WINDOW_WIDTH, WINDOW_HEIGHT); 
    gtk_container_set_border_width(GTK_CONTAINER(teamwindow), BOARD_BORDER); 
    gtk_window_set_position(GTK_WINDOW(teamwindow), GTK_WIN_POS_CENTER); 
    gtk_window_set_title(GTK_WINDOW(teamwindow), "Check-Mate's Chess"); 
    gtk_window_set_resizable(GTK_WINDOW(teamwindow), FALSE); 
    
    g_signal_connect(teamwindow, "destroy", G_CALLBACK(Dashboard), NULL);    
    g_signal_connect_swapped(teamwindow, "destroy", G_CALLBACK(Chess), NULL);
  
    table = gtk_table_new(8 ,8, TRUE);
    gtk_container_add(GTK_CONTAINER(teamwindow), table);
            
    label = gtk_label_new("Chose your team.");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 3, 5, 1, 2);
    gtk_widget_show(label);
    
    button = gtk_button_new_with_label("White");
    g_signal_connect(button, "clicked", G_CALLBACK(singleplayer), (gpointer)w);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), (gpointer)button);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), (gpointer)teamwindow);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 5, 3, 4);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Black");
    g_signal_connect(button, "clicked", G_CALLBACK(singleplayer), (gpointer)b);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), (gpointer)button); 
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), (gpointer)teamwindow);
    gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 5, 4, 5);
    gtk_widget_show(button);

    gtk_widget_show(table);
    gtk_widget_show(teamwindow);    
}
void singleplayer(GtkWidget *widget, gpointer data){
    int ai;
    int *teamcolor = (int*)data;
    GAME *game = g_new0(GAME, 1);
    game->board = (int*)malloc(64 * (sizeof(int)));
    game->chose = 0;
    game->hold_x = 0;
    game->hold_y = 0;
    game->side = *teamcolor;
    game->teamcolor = *teamcolor;   
    StartBoard(game->board, &game->teamcolor);
    
    if (game->teamcolor == 2){
        ai = 1;
    } 
    if (game->teamcolor == 1){
        ai = 2;
    }

    game->gamewindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);    
    gtk_widget_set_size_request(game->gamewindow, WINDOW_WIDTH, WINDOW_HEIGHT); 
    gtk_container_set_border_width(GTK_CONTAINER(game->gamewindow), BOARD_BORDER); 
    gtk_window_set_position(GTK_WINDOW(game->gamewindow), GTK_WIN_POS_CENTER); 
    gtk_window_set_title(GTK_WINDOW(game->gamewindow), "Check-Mate's Chess"); 
    gtk_window_set_resizable(GTK_WINDOW(game->gamewindow), FALSE); 

    if (game->side == game->teamcolor){        
        g_signal_connect(game->gamewindow, "destroy", G_CALLBACK(destroy), NULL);    
        g_signal_connect_swapped(game->gamewindow, "destroy", G_CALLBACK(Chess), NULL);
        gtk_widget_set_events(game->gamewindow, GDK_BUTTON_PRESS_MASK);        
        g_signal_connect(GTK_WINDOW(game->gamewindow), "button-press-event", G_CALLBACK(area_click), (gpointer)game);        
         
        game->table = gtk_table_new(9, 9, TRUE); 
        gtk_widget_set_size_request(game->table, 400, 400);        
        DrawBoard(game->table, game->board, &game->teamcolor);

        game->fixed = gtk_fixed_new();
        gtk_fixed_put(GTK_FIXED(game->fixed), game->table, 0, 0);
        gtk_container_add(GTK_CONTAINER(game->gamewindow), game->fixed);
        gtk_widget_show_all(game->gamewindow);
    }
    if (game->side == ai){
        printf("ai's turn\n");
    }
}
gint area_click(GtkWidget *widget, GdkEvent *event, GAME *game){   
    int coord_x, coord_y, grid_x, grid_y;
    int LPM[28];
    GdkModifierType state;
    
    gdk_window_get_pointer(widget->window, &coord_x, &coord_y, &state);

    CoordToGrid(coord_x, coord_y, &grid_x, &grid_y);    
//    printf("coord_x = %d, coord_y = %d, grid_x = %d, grid_y = %d \n", coord_x, coord_y, grid_x, grid_y);
    if (coord_x >= 10 && coord_x <= 399 && coord_y >= 10 && coord_y <= 399){        
        if (game->chose == 1){           
            ChoosePiece(grid_x, grid_y, game->board, game);     
            PossMoves(game->board, (game->hold_y * 8) + game->hold_x, LPM);           
            if (game->chose == 1){
                PrintPossMoves(LPM);   
            }
            if (game->chose == 0){
                printf("%d\n", game->side);
                return TRUE;
            }
        }
        else if (game->chose == 0){
            ChoosePiece(grid_x, grid_y, game->board, game); 
            game->hold_x = grid_x;
            game->hold_y = grid_y;  
        }
    }
    else{
        printf("Invalid Piece.\n");
    }
    return TRUE;
}
void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y){
    *g_x = (c_x - 10) / 50;
    *g_y = (c_y - 10) / 50;
}
void ChoosePiece(int g_x, int g_y, int board[64], GAME *game){
    int LPM[28];
    int i;
    PossMoves(board, (game->hold_y * 8) + game->hold_x, LPM);           
    if (game->chose == 1){
        for (i = 1; i < 28; i++){
            if (g_y * 8 + g_x == LPM[i] && LPM[i] != 69){
                MovePiece(g_x, g_y, LPM, game);
                game->chose = 0;                
            }
        }
        if (game->hold_x == g_x && game->hold_y == g_y){
            game->chose = 0;
            printf("You place the piece back.\n");
        }        
        if (game->chose == 1){
            printf("Piece is already selected.\n"); 
        } 
    }
    else if (game->chose == 0){
        int coord1 = (g_y * 8) + g_x;
        if (board[(g_y * 8) + g_x] == 1){
            printf("Black Rook\n");
        }        
        if (board[(g_y * 8) + g_x] == 2){
            printf("Black Knight\n");
        }        
        if (board[(g_y * 8) + g_x] == 3){
            printf("Black Bishop\n");
        }        
        if (board[(g_y * 8) + g_x] == 4){
            printf("Black Queen\n");
        }        
        if (board[(g_y * 8) + g_x] == 5){
            printf("Black King\n");
        }        
        if (board[(g_y * 8) + g_x] == 6){
            printf("Black Pawn\n");
        }        
        if (board[(g_y * 8) + g_x] == 7){
            printf("Empty\n");
        }        
        if (board[(g_y * 8) + g_x] == 8){
            printf("White Rook\n");
        }        
        if (board[(g_y * 8) + g_x] == 9){
            printf("White Knight\n");
        }        
        if (board[(g_y * 8) + g_x] == 10){
            printf("White Bishop\n");
        }        
        if (board[(g_y * 8) + g_x] == 11){
            printf("White Queen\n");
        }        
        if (board[(g_y * 8) + g_x] == 12){
            printf("White King\n");
        }        
        if (board[(g_y * 8) + g_x] == 13){
            printf("White Pawn\n");
        }        
        PossMoves(board, coord1, LPM);
        PrintPossMoves(LPM);
        if (board[(g_y * 8) + g_x] != 7){
            game->chose = 1;  
        }          
    }
}
void MovePiece(int g_x, int g_y, int LPM[28], GAME *game){
    int tmpboard[64];
    int i;
    GtkWidget *table;

    for (i = 0; i < 64; i++){
        tmpboard[i] = game->board[i];
    } 
    tmpboard[g_y * 8 + g_x] = game->board[LPM[0]];
    tmpboard[LPM[0]] = 7;
    for (i = 0; i < 64; i++){
        game->board[i] = tmpboard[i];
    }
    gtk_container_remove(GTK_CONTAINER(game->gamewindow), game->fixed);
    table = gtk_table_new(9, 9, TRUE); 
    gtk_widget_set_size_request(table, 400, 400);
    DrawBoard(table, game->board, &game->teamcolor);
    
    game->fixed = gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(game->fixed), table, 0, 0);
    gtk_container_add(GTK_CONTAINER(game->gamewindow), game->fixed);

    game->table = table;
    if (game->side == 2){
        game->side = 1;
    }
    else if (game->side == 1){
       game->side = 2;
    }
    gtk_widget_show_all(game->gamewindow);

}
void DrawBoard(GtkWidget *table, int board[64], int *teamcolor){
    int i, j;   
    int a, b; 
    if (*teamcolor == 2){
        a = 0;
        b = 1;
    }
    else{
        a = 1;
        b = 0;
    }
    GtkWidget *chess_icon;
    for (j = 0; j < 9; j++){
        for (i = 0; i < 9; i++){            
            if (i < 8 && j < 8){
                if ((i + j) % 2 == a){                
                    if (board[(j * 8) + i] == 7){
                        chess_icon = gtk_image_new_from_file("./chess_icon/Wsquare.png");
                    } 
                    if (board[(j * 8) + i] == 1){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareBRook.png");
                    }
                    if (board[(j * 8) + i] == 2){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareBKnight.png");
                    }
                    if (board[(j * 8) + i] == 3){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareBBishop.png");
                    }
                    if (board[(j * 8) + i] == 4){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareBQueen.png");
                    }
                    if (board[(j * 8) + i] == 5){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareBKing.png");
                    }
                    if (board[(j * 8) + i] == 6){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareBPawn.png");
                    }
                    if (board[(j * 8) + i] == 8){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareWRook.png");
                    }
                    if (board[(j * 8) + i] == 9){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareWKnight.png");
                    }
                    if (board[(j * 8) + i] == 10){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareWBishop.png");
                    }
                    if (board[(j * 8) + i] == 11){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareWQueen.png");
                    }
                    if (board[(j * 8) + i] == 12){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareWKing.png");
                    }
                    if (board[(j * 8) + i] == 13){
                        chess_icon = gtk_image_new_from_file("./chess_icon/WsquareWPawn.png");
                    }
                }
                if ((i + j) % 2 == b){        
                    if (board[j * 8 + i] == 7){
                        chess_icon = gtk_image_new_from_file("./chess_icon/Bsquare.png");
                    }
                    if (board[(j * 8) + i] == 1){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareBRook.png");
                    }
                    if (board[(j * 8) + i] == 2){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareBKnight.png");
                    }
                    if (board[(j * 8) + i] == 3){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareBBishop.png");
                    }
                    if (board[(j * 8) + i] == 4){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareBQueen.png");
                    }
                    if (board[(j * 8) + i] == 5){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareBKing.png");
                    }
                    if (board[(j * 8) + i] == 6){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareBPawn.png");
                    }
                    if (board[(j * 8) + i] == 8){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareWRook.png");
                    }
                    if (board[(j * 8) + i] == 9){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareWKnight.png");
                    }
                    if (board[(j * 8) + i] == 10){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareWBishop.png");
                    }
                    if (board[(j * 8) + i] == 11){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareWQueen.png");
                    }
                    if (board[(j * 8) + i] == 12){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareWKing.png");
                    }
                    if (board[(j * 8) + i] == 13){
                        chess_icon = gtk_image_new_from_file("./chess_icon/BsquareWPawn.png");
                    }
                }                
                gtk_table_attach(GTK_TABLE(table), chess_icon, i, i + 1, j, j + 1, GTK_FILL, GTK_FILL, 0, 0);
            }
        }
    }

}
