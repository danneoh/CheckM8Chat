#ifndef GAMELOGIC_H
#define GAMELOGIC_H

void StartBoard(int board[64], int *teamcolor);
int CharToInt(char coord[2]);
int PickCoor();
void PossMoves(int board[64], int PIndex, int LPM[28]);
void PrintPossMoves(int LPM[28]);
int MoveAlong(int orig, int direct);
int MoveDiag(int orig, int direct);
int MoveL(int orig, int direct);
int CheckCheck(int board[64]);
int CheckCheckmate(int board[64], int side);
int CheckPromote(int board[64], int ai);
void CheckEnPass(int board[64],int LPM[28], int EPcode);
void CheckCastle(int board[64], int LPM[28], int CM);
void CheckCastleMark(int side, int PossMove, int CM);
//int CheckStalemate(int board[64], int side, int EPcode);
#endif

