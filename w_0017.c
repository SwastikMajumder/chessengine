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

//print the board
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

//Upload fen on board
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

//rank and file range from 0 to 7
// b is the change in rank/file how much to travel along a file or rank
//a is the given rank/file of the piece
int sc(int a, int b){
	if (b == 0){
		return 1;
	} else if (b > 0){
		return ((a + b) < 8); //rank/file + change should not be going out of the board in the right side
	} else {
		return ((a + b) >= 0); //If we are going towards black we should not fall left of the board
	}
}

//Both rank and file should be like that
int outside(int rank, int file, int r, int f){
	return sc(rank, r) && sc(file, f);
}

//Generate moves of non sliding piece like knights or king
//Simple move generation
void playundo(int *board, int rank, int file, int r, int f, int color){
	printf("\n");
	if (outside(rank, file, r, f)){
		int a = rank * 8 + file;
		int value = board[a + (r * 8) + f];
		if (value == EMPTY || (value & 1) == !color){
			board[a + (r * 8) + f]  = board[a];
			board[a] = EMPTY;
			print_board(board);
			board[a] = board[a + (r * 8) + f];
			board[a + (r * 8) + f] = value;
		}
	}
	printf("\n");
}

//Generate moves of sliding pieces
void pu2(int *board, int rank, int file, int r, int f, int color){
	int x = r;
	int y = f;
	int a = rank * 8 + file;
	while (outside(rank, file, x, y) && board[a + (x * 8) + y] == EMPTY){ //if pieces doesnot fall and no other pieces came in between extend the attack of sliding
		board[a + (x * 8) + y] = board[a]; 
		board[a] = EMPTY;
		print_board(board);
		board[a] = board[a + (x * 8) + y];
		board[a + (x * 8) + y] = EMPTY;
		x += r; // increase range
		y += f;
		printf("\n");
	}
	int value = board[a + (x * 8) + y];
   // If the final piece is of opposite color try to eat it
	if (outside(rank, file, x, y) && (value & 1) == !color){
		board[a + (x * 8) + y] = board[a];
		board[a] = EMPTY;
		print_board(board);
		board[a] = board[a + (x * 8) + y];
		board[a + (x * 8) + y] = value;
	}
	printf("\n");
}

void legalgen(int *board, int color){
	int rank, file;
	int value;
	for (rank = 0; rank < 8; ++rank){
		for (file = 0; file < 8; ++file){
			if (board[rank * 8 + file] == EMPTY) continue;
			int pieceType = board[rank * 8 + file] >> 1;
			if (pieceType == PAWN && (board[rank * 8 + file] & 1) == color){ // generate pawn moves of our color
				int a = rank * 8 + file;
             // black and white pawn go in different directions
				if (color == BLACK){ 
					if (board[a + 8] == EMPTY){ //single forward
						board[a + 8] = board[a];
						board[a] = EMPTY;
						print_board(board);
						board[a] = board[a + 8];
						board[a + 8] = EMPTY;
						if (board[a + 16] == EMPTY && rank == 1){ //double forward
							board[a + 16] = board[a];
							board[a] = EMPTY;
							print_board(board);
							board[a] = board[a + 16];
							board[a + 16] = EMPTY;
						}
					}
					if ((board[a + 9] & 1) == !color){ //eat
						value = board[a + 9];
						board[a + 9] = board[a];
						board[a] = EMPTY;
						print_board(board);
						board[a] =  board[a + 9];
						board[a + 9] = value;
					}
					if ((board[a + 7] & 1) == !color){ //eat in the other side
                                                value = board[a + 7];
                                                board[a + 7] = board[a];
                                                board[a] = EMPTY;
                                                print_board(board);
                                                board[a] =  board[a + 7];
                                                board[a + 7] = value;
                                        }
				} else { //same comments for white
					if (board[a - 8] == EMPTY){
                                                board[a - 8] = board[a];
                                                board[a] = EMPTY;
                                                print_board(board);
                                                board[a] = board[a - 8];
                                                board[a - 8] = EMPTY;
                                                if (board[a - 16] == EMPTY && rank == 6){
                                                        board[a - 16] = board[a];
                                                        board[a] = EMPTY;
                                                        print_board(board);
                                                        board[a] = board[a - 16];
                                                        board[a - 16] = EMPTY;
                                                }
                                        }
                                        if ((board[a - 9] & 1) == !color){
                                                value = board[a - 9];
                                                board[a - 9] = board[a];
                                                board[a] = EMPTY;
                                                print_board(board);
                                                board[a] =  board[a - 9];
                                                board[a - 9] = value;
                                        }
                                        if ((board[a - 7] & 1) == !color){
                                                value = board[a - 7];
                                                board[a - 7] = board[a];
                                                board[a] = EMPTY;
                                                print_board(board);
                                                board[a] =  board[a - 7];
                                                board[a - 7] = value;
                                        }
				}
					
			}
			else if (pieceType == KNIGHT && (board[rank * 8 + file] & 1) == color){
				playundo(board, rank, file, +1, +2, color); //non sliding moves generate the r and f is the change in index
				playundo(board, rank, file, +1, -2, color);
				playundo(board, rank, file, -1, +2, color);
				playundo(board, rank, file, -1, -2, color);
				playundo(board, rank, file, +2, +1, color);
				playundo(board, rank, file, +2, -1, color);
				playundo(board, rank, file, -2, +1, color);
				playundo(board, rank, file, -2, -1, color);
			}
          // sliding pieces
			else if (pieceType == ROOK && (board[rank * 8 + file] & 1) == color){
				pu2(board, rank, file, +1, 0, color);
				pu2(board, rank, file, -1, 0, color);
				pu2(board, rank, file, 0, +1, color);
				pu2(board, rank, file, 0, -1, color);
			}
			else if (pieceType == BISHOP &&(board[rank * 8 + file] & 1) == color){
				pu2(board, rank, file, +1, -1, color);
				pu2(board, rank, file, +1, +1, color);
				pu2(board, rank, file, -1, -1, color);
				pu2(board, rank, file, -1, +1, color);
			}
			else if (pieceType == QUEEN && (board[rank * 8 + file] & 1) == color){
				pu2(board, rank, file, +1, 0, color);
                                pu2(board, rank, file, -1, 0, color);
                                pu2(board, rank, file, 0, +1, color);
                                pu2(board, rank, file, 0, -1, color);
				pu2(board, rank, file, +1, -1, color);
                                pu2(board, rank, file, +1, +1, color);
                                pu2(board, rank, file, -1, -1, color);
                                pu2(board, rank, file, -1, +1, color);
			}
			else if (pieceType == KING && (board[rank * 8 + file] & 1) == color){
				playundo(board, rank, file, +1, 0, color);
                                playundo(board, rank, file, -1, 0, color);
                                playundo(board, rank, file, 0, +1, color);
                                playundo(board, rank, file, 0, -1, color);
                                playundo(board, rank, file, +1, +1, color);
                                playundo(board, rank, file, +1, -1, color);
                                playundo(board, rank, file, -1, +1, color);
                                playundo(board, rank, file, -1, -1, color);
			}
		}
	}
}


int main (){
	int board[64] = {0};
	int i;
	for (i=0; i<64; ++i) board[i] = EMPTY;
	put_fen(board, "r3kbnr/8/pppppppp/b1K3N1/RP2P2R/4nq2/P1PP1PPP/2BQ1BN1 ");
	print_board(board);
	legalgen(board, WHITE);
	return 0;
}
				
