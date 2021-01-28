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
			if (piece == PAWN){
				if (color == BLACK && file == 1){
					piece |= (VIRGIN >> 1);
				}
				else if (color == WHITE && file == 6){
					piece |= (VIRGIN >> 1);
				}
			}
			piece <<= 1;
			piece |= color;
			board[rank * 8 + file] = piece;
		}
		++i;
	}
}

int main (){
	int board[64] = {0};
	int i;
	for (i=0; i<64; ++i) board[i] = EMPTY;
	put_fen(board, "r3kbnr/8/pppppppp/b1K3N1/RP2P2R/4nq2/P1PP1PPP/2BQ1BN1 ");
	print_board(board);
	return 0;
}
				
