#include <stdio.h>
#include <string.h>
#include "GameLogic.h"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
void StartBoard(int board[64], int *teamcolor){
    if (teamcolor[0] == 2){
        for (int i = 0; i < 64; i++){      
            if (i == 0 || i == 7){
                board[i] = 1;
            }
            if (i == 1 || i == 6){
                board[i] = 2;
            }
            if (i == 2 || i == 5){
                board[i] = 3;  
            } 
            if (i == 3){
                board[i] = 4;
            }
            if (i == 4){
                board[i] = 5;
            }
            if (i >= 8 && i < 16){
                board[i] = 6;
            }
            if (i >= 16 && i < 48){
                board[i] = 7;
            }
            if (i >= 48 && i < 56){
                board[i] = 13;
            }
            if (i == 56 || i == 63){
                board[i] = 8;
            }
            if (i == 57 || i == 62){
                board[i] = 9;
            }
            if (i == 58 || i == 61){
                board[i] = 10;
            }
            if (i == 59){
                board[i] = 11;
            }
            if (i == 60){
                board[i] = 12;
            }
        }  
    }
    if (teamcolor[0] == 1){
        for (int i = 0; i < 64; i++){      
            if (i == 0 || i == 7){
                board[i] = 8;
            }
            if (i == 1 || i == 6){
                board[i] = 9;
            }
            if (i == 2 || i == 5){
                board[i] = 10;  
            } 
            if (i == 3){
                board[i] = 11;
            }
            if (i == 4){
                board[i] = 12;
            }
            if (i >= 8 && i < 16){
                board[i] = 13;
            }
            if (i >= 16 && i < 48){
                board[i] = 7;
            }
            if (i >= 48 && i < 56){
                board[i] = 6;
            }
            if (i == 56 || i == 63){
                board[i] = 1;
            }
            if (i == 57 || i == 62){
                board[i] = 2;
            }
            if (i == 58 || i == 61){
                board[i] = 3;
            }
            if (i == 59){
                board[i] = 4;
            }
            if (i == 60){
                board[i] = 5;
            }
        }      
    }  
}


int CharToInt(char coord[2]){
    int i = 69;
    if (coord[0] == 'a'){
        i = 0;
    }
    if (coord[0] == 'b'){
        i = 1;
    }
    if (coord[0] == 'c'){
        i = 2;
    }
    if (coord[0] == 'd'){
        i = 3;
    }
    if (coord[0] == 'e'){
        i = 4;
    }
    if (coord[0] == 'f'){
        i = 5;
    }
    if (coord[0] == 'g'){
        i = 6;
    }
    if (coord[0] == 'h'){
        i = 7;
    }
    if (coord[1] == '1'){
        i = i + 56;
    }
    if (coord[1] == '2'){
        i = i + 48;
    }
    if (coord[1] == '3'){
        i = i + 40;
    }
    if (coord[1] == '4'){
        i = i + 32;
    }
    if (coord[1] == '5'){
        i = i + 24;
    }
    if (coord[1] == '6'){
        i = i + 16;
    }
    if (coord[1] == '7'){
        i = i + 8;
    }
    if (coord[1] == '8'){
        i = i;
    }
    if (i > 63){
        printf("Invalid coordinate.\n");
        return 69;
    }
    return i;
}
int PickCoor(){
    char str[10];
    char coord[2];
    scanf(" %s", str);
    if (strcmp(str, "quit") == 0){
        return -1;
    }
    coord[0] = str[0];
    coord[1] = str[1];
    return CharToInt(coord);
}
void PossMoves(int board[64], int PIndex, int LPM[28]){
    int steps       = 0;
    int indexCount  = 0;
    int stepCount   = 0;
    int directCount = 0;
    int tmpInd      = 0;
    tmpInd = PIndex;
    indexCount = 0;
    LPM[indexCount] = PIndex;
    for (int i = 1; i < 28; i++){
        LPM[i] = 69;
    }    
    if (board[PIndex] % 7 == 1){
        steps = 8;
        for (directCount = 1; directCount < 5; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveAlong(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 8;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }    
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 8;
                    }                
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 8;
                    }
                }
            }    
        }    
    } 
    if (board[PIndex] % 7 == 2){
        steps = 2;
        for (directCount = 1; directCount < 9; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveL(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 2;
                } 
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd; 
                    }
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 2;
                    }
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 2;
                    }
                }               
            }
        }
    } 
    if (board[PIndex] % 7 == 3){
        steps = 8;
        for (directCount = 1; directCount < 5; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveDiag(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 8;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 8;
                    }
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 8;                        
                    }
                }
            }
        } 
    } 
    if (board[PIndex] % 7 == 4){
        steps = 8;
        for (directCount = 1; directCount < 5; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveAlong(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 8;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 8;
                    }
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 8;
                    }
                }
            }
        }
        for (directCount = 1; directCount < 5; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveDiag(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 8;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 8;
                    }
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 8;
                    }
                }
            }
        }         
    } 
    if (board[PIndex] % 7 == 5){
        steps = 2;
        for (directCount = 1; directCount < 5; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveAlong(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 2;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 2;
                    }
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 2;
                    }
                }
            }
        }
        for (directCount = 1; directCount < 5; directCount++){
            tmpInd = PIndex;
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveDiag(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 2;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] / 7 == board[PIndex] / 7 && board[tmpInd] != 7){
                        stepCount = 2;
                    }
                    if (board[tmpInd] / 7 != board[PIndex] / 7 && board[tmpInd] != 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                        stepCount = 2;
                    }
                }
            }
        }
    } 
    if (board[PIndex] % 7 == 6){
        steps = 2;
        if (board[PIndex] - 7 < 0){
            directCount = 3;
            if (PIndex / 8 == 1){
                steps++;
            }
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveAlong(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 3;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] != 7){
                        stepCount = 3;
                    }
                }
            }
            directCount = 2;
            tmpInd = MoveDiag(PIndex, directCount);
            if (tmpInd != 69){
                if (board[tmpInd] > 7){
                    indexCount++;
                    LPM[indexCount] = tmpInd;
                }
            }
            directCount = 3;
            tmpInd = MoveDiag(PIndex, directCount);
            if (tmpInd != 69){
                if (board[tmpInd] > 7){
                    indexCount++;
                    LPM[indexCount] = tmpInd;
                }
            }
        }
        if (board[PIndex] - 7 > 0){
            directCount = 1;
            if (PIndex / 8 == 6){
                steps++;
            }
            for (stepCount = 1; stepCount < steps; stepCount++){
                tmpInd = MoveAlong(tmpInd, directCount);
                if (tmpInd == 69){
                    stepCount = 3;
                }
                if (tmpInd != 69){
                    if (board[tmpInd] == 7){
                        indexCount++;
                        LPM[indexCount] = tmpInd;
                    }
                    if (board[tmpInd] != 7){
                        stepCount = 3;
                    }
                }
            }
            directCount = 1;
            tmpInd = MoveDiag(PIndex, directCount);
            if (tmpInd != 69){
                if (board[tmpInd] < 7){
                    indexCount++;
                    LPM[indexCount] = tmpInd;
                }
            }
            directCount = 4;
            tmpInd = MoveDiag(PIndex, directCount);
            if (tmpInd != 69){
                if (board[tmpInd] < 7){
                    indexCount++;
                    LPM[indexCount] = tmpInd;
                }
            }   
        }
    }
}
void PrintPossMoves(int LPM[28]){
    char let;
    int num;
    if (LPM[0] % 8 == 0){
        let = 'a';
    }
    if (LPM[0] % 8 == 1){
        let = 'b';
    }
    if (LPM[0] % 8 == 2){
        let = 'c';
    }
    if (LPM[0] % 8 == 3){
        let = 'd';
    }
    if (LPM[0] % 8 == 4){
        let = 'e';
    }
    if (LPM[0] % 8 == 5){
        let = 'f';
    }
    if (LPM[0] % 8 == 6){
        let = 'g';
    }
    if (LPM[0] % 8 == 7){
        let = 'h';
    }
    if (LPM[0] / 8 == 0){
        num = 8;
    }
    if (LPM[0] / 8 == 1){
        num = 7;
    }
    if (LPM[0] / 8 == 2){
        num = 6;
    }
    if (LPM[0] / 8 == 3){
        num = 5;
    }
    if (LPM[0] / 8 == 4){
        num = 4;
    }
    if (LPM[0] / 8 == 5){
        num = 3;
    }
    if (LPM[0] / 8 == 6){
        num = 2;
    }
    if (LPM[0] / 8 == 7){
        num = 1;
    }
    printf("Possible moves for %c", let);
    printf("%d:\n", num);
    for (int i = 1; i < 28; i++){
        if (LPM[i] == 69){
            continue;
        }
        if (LPM[i] % 8 == 0){
            let = 'a';
        }
        if (LPM[i] % 8 == 1){
            let = 'b';
        }
        if (LPM[i] % 8 == 2){
            let = 'c';
        }
        if (LPM[i] % 8 == 3){
            let = 'd';
        }
        if (LPM[i] % 8 == 4){
            let = 'e';
        }
        if (LPM[i] % 8 == 5){
            let = 'f';
        }
        if (LPM[i] % 8 == 6){
            let = 'g';
        }
        if (LPM[i] % 8 == 7){
            let = 'h';
        }
        if (LPM[i] / 8 == 0){
            num = 8;
        }
        if (LPM[i] / 8 == 1){
            num = 7;
        }
        if (LPM[i] / 8 == 2){
            num = 6;
        }
        if (LPM[i] / 8 == 3){
            num = 5;
        }
        if (LPM[i] / 8 == 4){
            num = 4;
        }
        if (LPM[i] / 8 == 5){
            num = 3;
        }
        if (LPM[i] / 8 == 6){
            num = 2;
        }
        if (LPM[i] / 8 == 7){
            num = 1;
        }    
        printf("%c", let);
        printf("%d ", num);
    }
    printf("\n");
}
int MoveAlong(int orig, int direct){
    int dest;
    if (direct == 1){
        if (orig < 8){
            return 69;
        }
        dest = orig - 8;
    }
    if (direct == 2){
        if (orig % 8 == 7){
            return 69;
        }
        dest = orig + 1;
    }
    if (direct == 3){
        if (orig > 55){
            return 69;
        }
        dest = orig + 8;
    }
    if (direct == 4){
        if (orig % 8 == 0){
            return 69;
        }
        dest = orig - 1;
    }
    return dest;
}
int MoveDiag(int orig, int direct){
    int dest;
    if (direct == 1){
        if (orig < 8 || orig % 8 == 7){
            return 69;
        }
        dest = orig - 7;
    }
    if (direct == 2){
        if (orig > 55 || orig % 8 == 7){
            return 69;
        }
        dest = orig + 9;
    }
    if (direct == 3){
        if (orig > 55 || orig % 8 == 0){
            return 69;
        }
        dest = orig + 7;
    }
    if (direct == 4){
        if (orig < 8 || orig % 8 == 0){
            return 69;
        }
        dest = orig - 9;
    }
    return dest;
}
int MoveL(int orig, int direct){
    int dest;
    int tmpInd;
    if (direct < 3){
        tmpInd = MoveDiag(orig, 1);
        if (tmpInd == 69){
            return tmpInd;
        }
        if (direct == 1){
            dest = MoveAlong(tmpInd, 1);
        }
        if (direct == 2){
            dest = MoveAlong(tmpInd, 2);
        }
    }
    if (direct == 3 || direct == 4){
        tmpInd = MoveDiag(orig, 2);
        if (tmpInd == 69){
            return tmpInd;
        }
        if (direct == 3){
            dest = MoveAlong(tmpInd, 2);          
        }
        if (direct == 4){
            dest = MoveAlong(tmpInd, 3);
        }
    }
    if (direct == 5 || direct == 6){
        tmpInd = MoveDiag(orig, 3);
        if (tmpInd == 69){
            return tmpInd;
        }
        if (direct == 5){
            dest = MoveAlong(tmpInd, 3);          
        }
        if (direct == 6){
            dest = MoveAlong(tmpInd, 4);
        }       
    }
    if (direct > 6){
        tmpInd = MoveDiag(orig, 4);
        if (tmpInd == 69){
            return tmpInd;
        }
        if (direct == 7){
            dest = MoveAlong(tmpInd, 4);          
        }
        if (direct == 8){
            dest = MoveAlong(tmpInd, 1);
        }       
    } 
    return dest;
}
int CheckCheck(int board[64]){
    int whiteKing = 69;
    int blackKing = 69;
    int result = 0;
    int LPM[28];    
    for (int i = 0; i < 64; i++){
        if (board[i] == 5){
            blackKing = i;
        }
        if (board[i] == 12){
            whiteKing = i;
        }
        if (whiteKing != 69 && blackKing != 69){
            i = 69;
        }
    }    
    for (int i = 0; i < 64; i++){
        if (board[i] < 7 && result != 2 && result != 3){
            PossMoves(board, i, LPM);
            for (int k = 1; k < 28 && LPM[k] != 69; k++){
                if (LPM[k] == whiteKing){
                    k = 28;
                    result = result + 2;
                }
            }
        }
        if (board[i] > 7 && result != 1 && result != 3){
            PossMoves(board, i, LPM);
            for (int k = 1; k < 28 && LPM[k] != 69; k++){
                if (LPM[k] == blackKing){
                    k = 28;
                    result = result + 1;
                }
            }
        }
        if (result == 3){
            return result;
        }       
    }
    return result;   
}
int CheckCheckmate(int board[64], int side){
    int LPM[28];
    int tmpboard[64];
    for (int i = 0; i < 64; i++){
        tmpboard[i] = board[i];
    }
    if (side == 1){
        for (int i = 0; i < 64; i++){
            if (board[i] < 7){
                PossMoves(board, i, LPM);
                for (int k = 1; k < 28; k++){
                    tmpboard[LPM[k]] = board[i];
                    tmpboard[i] = 7;
                    int a = CheckCheck(tmpboard);
                    if (a != 1 && a != 3){
                        return 0;
                    }
                    tmpboard[i] = board[i];
                    tmpboard[LPM[k]] = board[LPM[k]];
                }
            }
        }
        return 1;
    }
    if (side == 2){
        for (int i = 0; i < 64; i++){
            if (board[i] > 7){
                PossMoves(board, i, LPM);
                for (int k = 1; k < 28; k++){
                    tmpboard[LPM[k]] = board[i];
                    tmpboard[i] = 7;
                    int a = CheckCheck(tmpboard);
                    if (a != 2 && a != 3){
                        return 0;
                    }
                    tmpboard[i] = board[i];
                    tmpboard[LPM[k]] = board[LPM[k]];
                }
            }
        }
        return 1;
    }
    return 69;
}
int CheckPromote(int board[64], int ai){
    int choice = 0;
    for (int i = 0; i < 8; i++){
        if (board[i] == 13){
            printf("A white pawn is getting promoted!\n");
            printf("1: Queen\n");
            printf("2: Bishop\n");
            printf("3: Knight\n");
            printf("4: Rook\n");
            if (ai != 1){
                scanf("%d", &choice);
                while (choice < 1 && choice > 4){
                    printf("Invalid piece!\n");
                    scanf("%d", &choice);
                }
            }
            if (ai == 1){
                choice = 1;
            }
            board[i] = board[i] - 1 - choice;
            return choice;
        }
    }
    for (int i = 56; i < 64; i++){
        if (board[i] == 6){
            printf("A black pawn is getting promoted!\n");
            printf("1: Queen\n");
            printf("2: Bishop\n");
            printf("3: Knight\n");
            printf("4: Rook\n");
            if (ai != 1){
                scanf("%d", &choice);
                while (choice < 1 && choice > 4){
                    printf("Invalid piece!\n");
                    scanf("%d", &choice);
                }
            }
            if (ai == 1){
                choice = 1;
            }
            board[i] = board[i] - 1 - choice;
            return choice;
        }
    }
    return 0;
}
void CheckEnPass(int board[64], int LPM[28], int EPcode){
    int tmpboard[64];    
    for (int i = 0; i < 64; i++){
        tmpboard[i] = board[i];
    }
    if (LPM[0] / 8 == 3){
        if (EPcode == 2){
            if (tmpboard[LPM[0] - 9] == 7){
                tmpboard[LPM[0] - 9] = tmpboard[LPM[0]];
                tmpboard[LPM[0]] = 7;
                tmpboard[LPM[0] + 1] = 7;
                if (CheckCheck(tmpboard) == 0){
                    for (int j = 1; j < 28; j++){
                        if (LPM[j] == 69){
                            LPM[j] = LPM[0] - 9;
                            break;                       
                        }
                    }
                }
            }               
        }
        if (EPcode == 1){
            if (tmpboard[LPM[0] - 7] == 7){
                tmpboard[LPM[0] - 7] = tmpboard[LPM[0]];
                tmpboard[LPM[0]] = 7;
                tmpboard[LPM[0] - 1] = 7;
                if (CheckCheck(tmpboard) == 0){
                    for (int j = 1; j < 28; j++){
                        if (LPM[j] == 69){
                            LPM[j] = LPM[0] - 7;
                            break;                       
                        }
                    }
                }
            }               
        }
    }
    if (LPM[0] / 8 == 4){
        if (EPcode == 2){
            if (tmpboard[LPM[0] + 7] == 7){
                tmpboard[LPM[0] + 7] = tmpboard[LPM[0]];
                tmpboard[LPM[0]] = 7;
                tmpboard[LPM[0] + 1] = 7;
                if (CheckCheck(tmpboard) == 0){
                    for (int j = 1; j < 28; j++){
                        if (LPM[j] == 69){
                            LPM[j] = LPM[0] + 7;
                            break;                       
                        }
                    }
                }
            }               
        }
        if (EPcode == 1){
            if (tmpboard[LPM[0] + 9] == 7){
                tmpboard[LPM[0] + 9] = tmpboard[LPM[0]];
                tmpboard[LPM[0]] = 7;
                tmpboard[LPM[0] - 1] = 7;
                if (CheckCheck(tmpboard) == 0){
                    for (int j = 1; j < 28; j++){
                        if (LPM[j] == 69){
                            LPM[j] = LPM[0] + 9;
                            break;                       
                        }
                    }
                }
            }               
        }
    
    }
}
void CheckCastle(int board[64], int LPM[28], int CM){
    int tmpboard[64];
    if (CM % 3 != 0){
        if ((board[LPM[0]] == 5 || board[LPM[0]] == 12) && board[LPM[0] - 1] == 7 && board[LPM[0] - 2] == 7 && board[LPM[0] - 3] == 7){
            if (CheckCheck(board) == 0){
                for (int i = 0; i < 64; i++){
                    tmpboard[i] = board[i];
                }
                tmpboard[LPM[0] - 1] = tmpboard[LPM[0]];
                tmpboard[LPM[0]] = 7;
                if (CheckCheck(tmpboard) == 0){
                    tmpboard[LPM[0] - 2] = tmpboard[LPM[0] - 1];
                    tmpboard[LPM[0] - 1] = 7;
                    if (CheckCheck(tmpboard) == 0){
                        for (int k = 1; k < 28; k++){
                            if (LPM[k] == 69){
                                LPM[k] = LPM[0] - 2;
                                k = 28;
                            }
                        }
                    }
                }
            }
        }
    }
    if (CM % 2 != 0){
        if ((board[LPM[0]] == 5 || board[LPM[0]] == 12) && board[LPM[0] + 1] == 7 && board[LPM[0] + 2] == 7){
            if (CheckCheck(board) == 0){
                for (int i = 0; i < 64; i++){
                    tmpboard[i] = board[i];
                }
                tmpboard[LPM[0] + 1] = tmpboard[LPM[0]];
                tmpboard[LPM[0]] = 7;
                if (CheckCheck(tmpboard) == 0){
                    tmpboard[LPM[0] + 2] = tmpboard[LPM[0] + 1];
                    tmpboard[LPM[0] + 1] = 7;
                    if (CheckCheck(tmpboard) == 0){
                        for (int k = 1; k < 28; k++){
                            if (LPM[k] == 69){
                                LPM[k] = LPM[0] + 2;
                                k = 28;
                            }
                        }
                    }
                }
            }
        }
    }
}
void CheckCastleMark(int side, int PossMove, int CM){
    if (side == 1){
        if (PossMove == 4){
            CM = CM * 6;
        }
        if (PossMove == 0){
            CM = CM * 3;
        }
        if (PossMove == 7){
            CM = CM * 2;
        }       
    }
    if (side == 2){
        if (PossMove == 60){
            CM = CM * 6;
        }
        if (PossMove == 56){
            CM = CM * 3;
        }
        if (PossMove == 63){
            CM = CM * 2;
        }        
    }
}
