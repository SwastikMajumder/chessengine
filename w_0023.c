#include <stdio.h>

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
		for (j=0; j<8; ++j){
			printf("+ ");
			piece2character(board[i*8 + j]);
			printf(" ");
		}
		printf("+");
		printf("\n");
	}
	print_line();
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
			piece <<= 1;
			piece |= color;
			board[rank * 8 + file] = piece;
		}
		++i;
	}
}

//Socring function

int scoreval(int *board){
    int value=0;
    int rank, file;
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
                    value += 3 * color;
                    break;
                case BISHOP:
                    value += 3 * color;
                    break;
                case ROOK:
                    value += 5 * color;
                    break;
                case QUEEN:
                    value += 9 * color;
                    break;
                default: break;
            }
        }
    }
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
		int t2 = board[t1 + (r * 8) + f]; \
		if (value == EMPTY || (value & 1) == !color){ \
			board[t1 + (r * 8) + f]  = board[t1]; \
			board[t1] = EMPTY; \
			start = t1; \
			end = t1 + (r * 8) + f; \
			SEARCH(); \
			board[t1] = board[t1 + (r * 8) + f]; \
			board[t1 + (r * 8) + f] = t2; \
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
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
		start = t1; \
        	end = t1 + (x * 8) + y; \
		SEARCH(); \
		board[t1] = board[t1 + (x * 8) + y]; \
		board[t1 + (x * 8) + y] = EMPTY; \
		x += r; \
		y += f; \
	} \
	t2 = board[t1 + (x * 8) + y]; \
	if (outside(rank, file, x, y) && (t2 & 1) == !color){ \
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
		start = t1; \
                end = t1 + (x * 8) + y; \
		SEARCH(); \
		board[t1] = board[t1 + (x * 8) + y]; \
		board[t1 + (x * 8) + y] = t2; \
	}

#define INFINITY 1000

//Our negamax recursion

#define SEARCH()  tmp = -negamax(board, depth - 1, !color, pv); \
		 if (tmp > score) {pv[depth] = (start << 6) | end; score = tmp;}

//The function for moves generation

int negamax(int *board, int depth, int color, int *pv){
	int t1, t2, x, y;
	if (depth == 0){
		return (color == WHITE ? 1 : -1)  * scoreval(board);
	}
	
	int score = -INFINITY;
	int tmp;

	int rank, file;
	int value;
	int start, end;
	//Iterate through the whole board
	for (rank = 0; rank < 8; ++rank){
		for (file = 0; file < 8; ++file){
			if (board[rank * 8 + file] == EMPTY) continue;
			int pieceType = board[rank * 8 + file] >> 1;
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
						//Double move forward
						if (board[a + 16] == EMPTY && rank == 1){
							board[a + 16] = board[a];
							board[a] = EMPTY;
							end = a + 16;
							SEARCH();
							board[a] = board[a + 16];
							board[a + 16] = EMPTY;
						}
					}
					//Eating the pieces
					if (outside(rank, file, 1, 1) \
					    && board[a + 9] != EMPTY && (board[a + 9] & 1) == !color){
						value = board[a + 9];
						board[a + 9] = board[a];
						board[a] = EMPTY;
						end = a + 9;
						SEARCH();
						board[a] =  board[a + 9];
						board[a + 9] = value;
					}
					//Eating the pieces on the other side
					if (outside(rank, file, 1, -1) \
					    && board[a + 7] != EMPTY && (board[a + 7] & 1) == !color){
                                                value = board[a + 7];
                                                board[a + 7] = board[a];
                                                board[a] = EMPTY;
						end = a + 7;
                                                SEARCH();
                                                board[a] =  board[a + 7];
                                                board[a + 7] = value;
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
                                                if (board[a - 16] == EMPTY && rank == 6){
                                                        board[a - 16] = board[a];
                                                        board[a] = EMPTY;
							end = a - 16;
                                                       	SEARCH();
                                                        board[a] = board[a - 16];
                                                        board[a - 16] = EMPTY;
                                                }
                                        }
                                        if (outside(rank, file, -1, -1) \
					    && board[a - 9] != EMPTY && (board[a - 9] & 1) == !color){
                                                value = board[a - 9];
                                                board[a - 9] = board[a];
                                                board[a] = EMPTY;
						end = a - 9;
                                                SEARCH();
                                                board[a] =  board[a - 9];
                                                board[a - 9] = value;
                                        }
                                        if (outside(rank, file, -1, 1) \
					    && board[a - 7] != EMPTY && (board[a - 7] & 1) == !color){
                                                value = board[a - 7];
                                                board[a - 7] = board[a];
                                                board[a] = EMPTY;
						end = a - 7;
                                                SEARCH();
                                                board[a] =  board[a - 7];
                                                board[a - 7] = value;
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
	
	return score; // Negamax stuff return the best possible node
}


int main (){
	int board[64] = {0};
	int pv[6] = {0};
	int i;
	for (i=0; i<64; ++i) board[i] = EMPTY;
	put_fen(board, "7R/7R/p7/2r1r3/8/3P4/8/8 ");
	print_board(board);
	int score = negamax(board, 5, WHITE, pv);
	for (i=0; i<6; ++i){
		printf("%d %d ", pv[i] >> 6, pv[i] & ((1 << 6) - 1));
		printf("\n");
	}
	printf("\n%d", score);
	return 0;
}
				
