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

int sc(int a, int b){
	if (b == 0){
		return 1;
	} else if (b > 0){
		return ((a + b) < 8);
	} else {
		return ((a + b) >= 0);
	}
}

int outside(int rank, int file, int r, int f){
	return sc(rank, r) && sc(file, f);
}

#define playundo(r, f) \
	if (outside(rank, file, r, f)){ \
		t1 = rank * 8 + file; \
		int t2 = board[t1 + (r * 8) + f]; \
		if (value == EMPTY || (value & 1) == !color){ \
			board[t1 + (r * 8) + f]  = board[t1]; \
			board[t1] = EMPTY; \
			SEARCH();
			board[t1] = board[t1 + (r * 8) + f]; \
			board[t1 + (r * 8) + f] = t2; \
		} \
	} \

#define pu2(r, f) \
	x = r; \
	y = f; \
	t1 = rank * 8 + file; \
	while (outside(rank, file, x, y) && board[t1 + (x * 8) + y] == EMPTY){ \
		board[t1 + (x * 8) + y] = board[t1]; \
		board[t1] = EMPTY; \
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
		SEARCH(); \
		board[t1] = board[t1 + (x * 8) + y]; \
		board[t1 + (x * 8) + y] = t2; \
	}

#define INFINITY 1000

#define SEARCH() tmp = -negamax(board, depth - 1, !color); \
		 if (tmp > score) {score = tmp;}

int negamax(int *board, int depth, int color){
	int t1, t2, x, y;
	if (depth == 0){
		return (color == WHITE ? 1 : -1)  * scoreval(board);
	}
	
	int score = -INFINITY;
	int tmp;

	int rank, file;
	int value;
	for (rank = 0; rank < 8; ++rank){
		for (file = 0; file < 8; ++file){
			if (board[rank * 8 + file] == EMPTY) continue;
			int pieceType = board[rank * 8 + file] >> 1;
			if (pieceType == PAWN && (board[rank * 8 + file] & 1) == color){
				int a = rank * 8 + file;
				if (color == BLACK){
					if (board[a + 8] == EMPTY){
						board[a + 8] = board[a];
						board[a] = EMPTY;
						SEARCH();
						board[a] = board[a + 8];
						board[a + 8] = EMPTY;
						if (board[a + 16] == EMPTY && rank == 1){
							board[a + 16] = board[a];
							board[a] = EMPTY;
							SEARCH();
							board[a] = board[a + 16];
							board[a + 16] = EMPTY;
						}
					}
					if ((board[a + 9] & 1) == !color){
						value = board[a + 9];
						board[a + 9] = board[a];
						board[a] = EMPTY;
						SEARCH();
						board[a] =  board[a + 9];
						board[a + 9] = value;
					}
					if ((board[a + 7] & 1) == !color){
                                                value = board[a + 7];
                                                board[a + 7] = board[a];
                                                board[a] = EMPTY;
                                                SEARCH();
                                                board[a] =  board[a + 7];
                                                board[a + 7] = value;
                                        }
				} else {
					if (board[a - 8] == EMPTY){
                                                board[a - 8] = board[a];
                                                board[a] = EMPTY;
                                                SEARCH();
                                                board[a] = board[a - 8];
                                                board[a - 8] = EMPTY;
                                                if (board[a - 16] == EMPTY && rank == 6){
                                                        board[a - 16] = board[a];
                                                        board[a] = EMPTY;
                                                       	SEARCH();
                                                        board[a] = board[a - 16];
                                                        board[a - 16] = EMPTY;
                                                }
                                        }
                                        if ((board[a - 9] & 1) == !color){
                                                value = board[a - 9];
                                                board[a - 9] = board[a];
                                                board[a] = EMPTY;
                                                SEARCH();
                                                board[a] =  board[a - 9];
                                                board[a - 9] = value;
                                        }
                                        if ((board[a - 7] & 1) == !color){
                                                value = board[a - 7];
                                                board[a - 7] = board[a];
                                                board[a] = EMPTY;
                                                SEARCH();
                                                board[a] =  board[a - 7];
                                                board[a - 7] = value;
                                        }
				}
					
			}
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
	
	return score;
}


int main (){
	int board[64] = {0};
	int i;
	for (i=0; i<64; ++i) board[i] = EMPTY;
	put_fen(board, "r3kbnr/8/pppppppp/b1K3N1/RP2P2R/4nq2/P1PP1PPP/2BQ1BN1 ");
	print_board(board);
	int score = negamax(board, 2, WHITE);
	printf("\n%d", score);
	return 0;
}
				
