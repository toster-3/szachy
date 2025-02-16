#pragma once

#include <array>
#include <string>

enum PieceTag {
	PIECE_NONE,
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

	Piece();
	Piece(PieceTag);
};

typedef std::array<Piece, 64> Board;

struct Chess {
	std::array<Piece, 64> board;

	Chess(std::string);
	Piece drop(int);
};
