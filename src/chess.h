#pragma once

#include <array>
#include <string>

enum PieceTag {
	WPAWN = 'P',
	WKNIGHT = 'N',
	WBISHOP = 'B',
	WROOK = 'R',
	WQUEEN = 'Q',
	WKING = 'K',
	BPAWN = 'p',
	BKNIGHT = 'n',
	BBISHOP = 'b',
	BROOK = 'r',
	BQUEEN = 'q',
	BKING = 'k',
};

struct Piece {
	PieceTag tag;
};

typedef std::array<Piece, 64> Board;

class Chess {
	std::array<Piece, 64> board;

public:
	Chess(std::string);
	Board getBoard();
};
