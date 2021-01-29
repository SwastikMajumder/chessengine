#include <stdio.h>
#include <string.h>

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

#define BLACK 0
#define WHITE 1

#define VIRGIN 16

#define EMPTY -1

int piecescore[6] = { 100, 280,  320, 479, 929,  100000 };
int pqst[6][64] = {
     	 {   0,   0,   0,   0,   0,   0,   0,   0,
            78,  83,  86,  73, 102,  82,  85,  90,
             7,  29,  21,  44,  40,  31,  44,   7,
           -17,  16,  -2,  15,  14,   0,  15, -13,
           -26,   3,  10,   9,   6,   1,   0, -23,
           -22,   9,   5, -11, -10,  -2,   3, -19,
           -31,   8,  -7, -37, -36, -14,   3, -31,
             0,   0,   0,   0,   0,   0,   0,   0},
         { -66, -53, -75, -75, -10, -55, -58, -70,
            -3,  -6, 100, -36,   4,  62,  -4, -14,
            10,  67,   1,  74,  73,  27,  62,  -2,
            24,  24,  45,  37,  33,  41,  25,  17,
            -1,   5,  31,  21,  22,  35,   2,   0,
           -18,  10,  13,  22,  18,  15,  11, -14,
           -23, -15,   2,   0,   2,   0, -23, -20,
           -74, -23, -26, -24, -19, -35, -22, -69},
         { -59, -78, -82, -76, -23,-107, -37, -50,
           -11,  20,  35, -42, -39,  31,   2, -22,
            -9,  39, -32,  41,  52, -10,  28, -14,
            25,  17,  20,  34,  26,  25,  15,  10,
            13,  10,  17,  23,  17,  16,   0,   7,
            14,  25,  24,  15,   8,  25,  20,  15,
            19,  20,  11,   6,   7,   6,  20,  16,
            -7,   2, -15, -12, -14, -15, -10, -10},
         {  35,  29,  33,   4,  37,  33,  56,  50,
            55,  29,  56,  67,  55,  62,  34,  60,
            19,  35,  28,  33,  45,  27,  25,  15,
             0,   5,  16,  13,  18,  -4,  -9,  -6,
           -28, -35, -16, -21, -13, -29, -46, -30,
           -42, -28, -42, -25, -25, -35, -26, -46,
           -53, -38, -31, -26, -29, -43, -44, -53,
           -30, -24, -18,   5,  -2, -18, -31, -32},
	 {   6,   1,  -8,-104,  69,  24,  88,  26,
            14,  32,  60, -10,  20,  76,  57,  24,
            -2,  43,  32,  60,  72,  63,  43,   2,
             1, -16,  22,  17,  25,  20, -13,  -6,
           -14, -15,  -2,  -5,  -1, -10, -20, -22,
           -30,  -6, -13, -11, -16, -11, -16, -27,
           -36, -18,   0, -19, -15, -15, -21, -38,
           -39, -30, -31, -13, -31, -36, -34, -42},
 	 {   4,  54,  47, -99, -99,  60,  83, -62,
           -32,  10,  55,  56,  56,  55,  10,   3,
           -62,  12, -57,  44, -67,  28,  37, -31,
           -55,  50,  11,  -4, -19,  13,   0, -49,
           -55, -43, -52, -28, -51, -47,  -8, -50,
           -47, -42, -43, -79, -64, -32, -29, -32,
            -4,   3, -14, -50, -57, -18,  13,   4,
            17,  30,  -3, -14,   6,  -1,  40,  18}
};

//Below three functions for printing the board

void piece2character(int code){
	char *character_string = "pnbrqk";
	if (code == EMPTY){
		putchar(' ');
		return;
	}
	int type = (code >> 1) & 7;
	putchar(character_string[type] + ((code & 1) ? ('A' - 'a') : 0));
}

void print_line(){
	int i;
	printf("  ");
	for (i=0; i<8; ++i){
		printf("+---");
	}
	printf("+\n");
}

void print_board(int *board){
	int i;
	for (i=0; i<8; ++i){
		print_line();
		int j;
		printf("%d ", 8-i);
		for (j=0; j<8; ++j){
			printf("+ ");
			piece2character(board[i*8 + j]);
			printf(" ");
		}
		printf("+");
		printf("\n");
	}
	print_line();
	printf("    a   b   c   d   e   f   g   h\n");
}

// Uploading fen to board

void put_fen(int *board, char *fen){
	int i;
	i=0;
	while (1){
		if (fen[i] == ' '){
			break;
		}
		if 
		(
			('a' < fen[i] && fen[i] < 'z') ||
			('A' < fen[i] && fen [i] <'Z')
		){
			int index=i;
			int rank = 0;
			while (1){
				if (index == 0) break;
				--index;
				if (fen[index] == '/'){
					++rank;
				}
			}
			int file = 0;
			index = i;
			while (1){
				if (index == 0) break;
				--index;
				if (fen[index] == '/') break;
				if 
				(
					('a' < fen[index] && fen[index] < 'z') ||
					('A' < fen[index] && fen[index] < 'Z')
				){
					++file;
				}
				else if ('0' < fen[index] && fen[index] < '9'){
					file += fen[index] - '0';
				}
			}
			int piece;
			int type = fen[i];
			int color = BLACK;
			if ('A' < type && type < 'Z'){ type += ('a' - 'A'); color = WHITE; }
			switch (type){
				case 'p': piece = PAWN; break;
				case 'r': piece = ROOK; break;
				case 'n': piece = KNIGHT; break;
				case 'b': piece = BISHOP; break;
				case 'q': piece = QUEEN; break;
				case 'k': piece = KING; break;
			}
			int pieceType = piece;
			piece <<= 1;
			piece |= color;
			if (pieceType == KING || pieceType == ROOK){
				piece |= VIRGIN;
			}
			board[rank * 8 + file] = piece;
		}
		++i;
	}
}

//Socring function

int scoreval(int *board){
    int value=0;
    int rank, file;
    for (rank=0; rank < 8; ++rank){
	    for (file =0; file < 8; ++file){
		    int i;
		    int piece = board[rank * 8 + file];
		    if (piece == EMPTY) continue;
		    if ((piece & 1) == BLACK){
			    value -= piecescore[(piece >> 1) & 7];
			    value -= pqst[(piece >> 1) & 7][(7 - rank) * 8 + file];
		    } else {
			    value += piecescore[(piece >> 1) & 7];
			    value += pqst[(piece >> 1) & 7][rank * 8 + file];
		    }
	    }
    }
    /*if (((board[62] >> 1) & 7) == KING){
	    value += 1;
    }
    for (rank=0; rank <8 ; ++rank){
        for (file = 0; file < 8; ++file){
            int piece = board[rank * 8 + file];
            int pieceType = (piece >> 1) & 7;
            int color;
            if ((piece & 1) == WHITE){
                color = 1;
            } else {
                color = -1;
            }
            switch (pieceType){
                case PAWN:
                    value += color;
                    break;
                case KNIGHT:
                    value += 30 * color;
                    break;
                case BISHOP:
                    value += 30 * color;
                    break;
                case ROOK:
                    value += 50 * color;
                    break;
                case QUEEN:
                    value += 90 * color;
                    break;
		case KING:
		    value += 1000 * color;
                default: break;
            }
        }
    } */
    return value;
}

//Checking if a one piece gets moved outside the board
// a is the position of the piece and b is the number of ranks/files moves

int sc(int a, int b){
	if (b == 0){
		return 1;
	} else if (b > 0){
		return ((a + b) < 8);
	} else {
		return ((a + b) >= 0);
	}
}

//If the piece doesnot fall off and both rank and file are in range it is ok

int outside(int rank, int file, int r, int f){
	return sc(rank, r) && sc(file, f);
}

//Direct move generation for sliding pieces

#define playundo(r, f) \
	if (outside(rank, file, r, f)){ \
		t1 = rank * 8 + file; \
		t3 = board[t1]; \
		if (((board[t1] >> 1) & 7) == KING){ \
			board[t1] &= ~VIRGIN; \
		} \
		int t2 = board[t1 + (r * 8) + f]; \
		if (t2 != EMPTY && (t2 & 1) == !color){ \
			extend = 1; \
		} \
		if (t2 == EMPTY || (t2 != EMPTY && (t2 & 1) == !color)){ \
			board[t1 + (r * 8) + f]  = board[t1]; \
			board[t1] = EMPTY; \
			start = t1; \
			end = t1 + (r * 8) + f; \
			SEARCH(); \
			extend = 0; \
			board[t1] = t3; \
			board[t1 + (r * 8) + f] = t2; \
			ABC(); \
		} \
	}

//Move generation by increasing ranges slowly one by one for sliding pieces

//The while loop does the moves for the sliding pieces
//The below if conditions checks if a piece can be eaten and an extra range is there or not

#define pu2(r, f) \
	x = r; \
	y = f; \
	t1 = rank * 8 + file; \
	while (outside(rank, file, x, y) && board[t1 + (x * 8) + y] == EMPTY){ \
		t3 = board[t1]; \
		if (((board[t1] >> 1) & 7) == ROOK){ \
                        board[t1] &= ~VIRGIN; \
                } \
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
		start = t1; \
        	end = t1 + (x * 8) + y; \
		SEARCH(); \
		board[t1] = t3; \
		board[t1 + (x * 8) + y] = EMPTY; \
		ABC(); \
		x += r; \
		y += f; \
	} \
	t2 = board[t1 + (x * 8) + y]; \
	if (outside(rank, file, x, y) && t2 != EMPTY && (t2 & 1) == !color){ \
		extend = 1; \
		t3 = board[t1]; \
		if (((board[t1] >> 1) & 7) == ROOK){ \
                        board[t1] &= ~VIRGIN; \
                } \
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
		start = t1; \
                end = t1 + (x * 8) + y; \
		SEARCH(); \
		extend = 0; \
		board[t1] = t3; \
		board[t1 + (x * 8) + y] = t2; \
		ABC(); \
	}

#define INFINITY 1000000

struct LINE {
	int cmove;
	int argmove[100];
};

//Our negamax recursion

#define SEARCH() tmp = -negamax(board, extend && depth == 1 ? 1 : depth - 1, -beta, -alpha, !color, &line)
//		 if (tmp > score) {pv[depth] = (start << 6) | end; score = tmp;}

#define ABC() if (tmp >= beta) { return beta; } \
	      if (tmp > alpha){ alpha = tmp; pline -> argmove[0] = (start << 6) | end; \
	      memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(int)); \
	      pline -> cmove = line.cmove + 1;}

//The function for moves generation

int negamax(int *board, int depth, int alpha, int beta, int color, struct LINE *pline){
	int t1, t2, t3, t4, x, y;
	struct LINE line;
	if (depth == 0){
		pline ->cmove = 0;
		return (color == WHITE ? 1 : -1)  * scoreval(board);
	}
	
	int tmp;

	int extend=0;
	int rank, file;
	int value;
	int start, end;
	//Iterate through the whole board
	if (color == BLACK){
		if (board[4] == (VIRGIN | (KING << 1) | BLACK) &&
		    board[7] == (VIRGIN | (ROOK << 1) | BLACK) &&
		    board[5] == EMPTY && board[6] == EMPTY){
			board[6] = (KING << 1) | BLACK;
			board[5] = (ROOK << 1) | BLACK;
			board[4] = board[7] = EMPTY;
			start = 4;
			end = 6;
			SEARCH();
			board[4] = VIRGIN | (KING << 1) | BLACK;
			board[7] = VIRGIN | (ROOK << 1) | BLACK;
			board[5] = board[6] = EMPTY;
			ABC();
		}
		if (board[4] == (VIRGIN | (KING << 1) | BLACK) &&
                    board[0] == (VIRGIN | (ROOK << 1) | BLACK) &&
                    board[1] == EMPTY && board[2] == EMPTY && board[3] == EMPTY){
                        board[2] = (KING << 1) | BLACK;
                        board[3] = (ROOK << 1) | BLACK;
                        board[0] = board[4] = EMPTY;
			start = 4;
			end = 2;
                        SEARCH();
                        board[4] = VIRGIN | (KING << 1) | BLACK;
                        board[0] = VIRGIN | (ROOK << 1) | BLACK;
                        board[3] = board[2] = EMPTY;
                        ABC();
                }
	} else {
		if (board[60] == (VIRGIN | (KING << 1) | WHITE) &&
                    board[63] == (VIRGIN | (ROOK << 1) | WHITE) &&
                    board[61] == EMPTY && board[62] == EMPTY){
                        board[62] = (KING << 1) | WHITE;
                        board[61] = (ROOK << 1) | WHITE;
                        board[60] = board[63] = EMPTY;
			start = 60;
			end = 62;
                        SEARCH();
                        board[60] = VIRGIN | (KING << 1) | WHITE;
                        board[63] = VIRGIN | (ROOK << 1) | WHITE;
                        board[61] = board[62] = EMPTY;
                        ABC();
                }
		if (board[60] == (VIRGIN | (KING << 1) | BLACK) &&
                    board[56] == (VIRGIN | (ROOK << 1) | BLACK) &&
                    board[57] == EMPTY && board[58] == EMPTY && board[59] == EMPTY){
                        board[58] = (KING << 1) | BLACK;
                        board[59] = (ROOK << 1) | BLACK;
                        board[60] = board[56] = EMPTY;
			start = 60;
			end = 58;
                        SEARCH();
                        board[60] = VIRGIN | (KING << 1) | BLACK;
                        board[56] = VIRGIN | (ROOK << 1) | WHITE;
                        board[58] = board[59] = EMPTY;
                        ABC();
                }
	}
	for (rank = 0; rank < 8; ++rank){
		for (file = 0; file < 8; ++file){
			if (board[rank * 8 + file] == EMPTY) continue;
			int pieceType = (board[rank * 8 + file] >> 1) & 7;
			//Generate pawn moves
			if (pieceType == PAWN && (board[rank * 8 + file] & 1) == color){
				int a = rank * 8 + file;
				start = a;
				//Check colors of the pawns because they are different directions
				if (color == BLACK){
					if (board[a + 8] == EMPTY){
						//Single move forward
						board[a + 8] = board[a];
						board[a] = EMPTY;
						end = a + 8;
						SEARCH();
						board[a] = board[a + 8];
						board[a + 8] = EMPTY;
						ABC();
						//Double move forward
						if (board[a + 16] == EMPTY && rank == 1){
							board[a + 16] = board[a];
							board[a] = EMPTY;
							end = a + 16;
							SEARCH();
							board[a] = board[a + 16];
							board[a + 16] = EMPTY;
							ABC();
						}
					}
					//Eating the pieces
					if (outside(rank, file, 1, 1) \
					    && board[a + 9] != EMPTY && (board[a + 9] & 1) == !color){
						extend = 1;
						value = board[a + 9];
						board[a + 9] = board[a];
						board[a] = EMPTY;
						end = a + 9;
						SEARCH();
						extend = 0;
						board[a] =  board[a + 9];
						board[a + 9] = value;
						ABC();
					}
					//Eating the pieces on the other side
					if (outside(rank, file, 1, -1) \
					    && board[a + 7] != EMPTY && (board[a + 7] & 1) == !color){
						extend = 1;
                                                value = board[a + 7];
                                                board[a + 7] = board[a];
                                                board[a] = EMPTY;
						end = a + 7;
                                                SEARCH();
						extend = 0;
                                                board[a] =  board[a + 7];
                                                board[a + 7] = value;
						ABC();
                                        }
				} else {
					//Same stuff in the other direction in the other color
					if (board[a - 8] == EMPTY){
                                                board[a - 8] = board[a];
                                                board[a] = EMPTY;
						end = a - 8;
                                                SEARCH();
                                                board[a] = board[a - 8];
                                                board[a - 8] = EMPTY;
						ABC();
                                                if (board[a - 16] == EMPTY && rank == 6){
                                                        board[a - 16] = board[a];
                                                        board[a] = EMPTY;
							end = a - 16;
                                                       	SEARCH();
                                                        board[a] = board[a - 16];
                                                        board[a - 16] = EMPTY;
							ABC();
                                                }
                                        }
                                        if (outside(rank, file, -1, -1) \
					    && board[a - 9] != EMPTY && (board[a - 9] & 1) == !color){
						extend = 1;
                                                value = board[a - 9];
                                                board[a - 9] = board[a];
                                                board[a] = EMPTY;
						end = a - 9;
                                                SEARCH();
						extend = 0;
                                                board[a] =  board[a - 9];
                                                board[a - 9] = value;
						ABC();
                                        }
                                        if (outside(rank, file, -1, 1) \
					    && board[a - 7] != EMPTY && (board[a - 7] & 1) == !color){
						extend = 1;
                                                value = board[a - 7];
                                                board[a - 7] = board[a];
                                                board[a] = EMPTY;
						end = a - 7;
                                                SEARCH();
						extend = 0;
                                                board[a] =  board[a - 7];
                                                board[a - 7] = value;
						ABC();
                                        }
				}
					
			}
			//Generate according to the given direction
			else if (pieceType == KNIGHT && (board[rank * 8 + file] & 1) == color){
				playundo(+1, +2);
				playundo(+1, -2);
				playundo(-1, +2);
				playundo(-1, -2);
				playundo(+2, +1);
				playundo(+2, -1);
				playundo(-2, +1);
				playundo(-2, -1);
			}
			else if (pieceType == ROOK && (board[rank * 8 + file] & 1) == color){
				pu2(+1, 0);
				pu2(-1, 0);
				pu2(0, +1);
				pu2(0, -1);
			}
			else if (pieceType == BISHOP &&(board[rank * 8 + file] & 1) == color){
				pu2(+1, -1);
				pu2(+1, +1);
				pu2(-1, -1);
				pu2(-1, +1);
			}
			else if (pieceType == QUEEN && (board[rank * 8 + file] & 1) == color){
				pu2(+1, 0);
                                pu2(-1, 0);
                                pu2(0, +1);
                                pu2(0, -1);
				pu2(+1, -1);
                                pu2(+1, +1);
                                pu2(-1, -1);
                                pu2(-1, +1);
			}
			else if (pieceType == KING && (board[rank * 8 + file] & 1) == color){
				playundo(+1, 0);
                                playundo(-1, 0);
                                playundo(0, +1);
                                playundo(0, -1);
                                playundo(+1, +1);
                                playundo(+1, -1);
                                playundo(-1, +1);
                                playundo(-1, -1);
			}
		}
	}
	
	return alpha; // Negamax stuff return the best possible node
}


int main (){
	int board[64] = {0};
	struct LINE line;
	int i;
	line.cmove = 0;
	for (i=0; i<100; ++i) line.argmove[i] = 0;
	for (i=0; i<64; ++i) board[i] = EMPTY;
	put_fen(board, "B7/K1B1p1Q1/5r2/7p/1P1kp1bR/3P3R/1P1NP3/2n5 ");
	print_board(board);
	int score = negamax(board, 5, -INFINITY, +INFINITY, WHITE, &line);
	for (i=0; i<line.cmove; ++i){
		int a = line.argmove[i] >> 6;
		int b = line.argmove[i] & ((1 << 6) - 1);
		printf("%c%c%c%c ", (a & 7) + 'a', 8 - (a >> 3) + '0', (b & 7) + 'a', 8 - (b >> 3) + '0');
	}
	printf("\n%d", score);
	return 0;
}
				
