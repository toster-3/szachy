#include <cctype>
#include <string>

#include "chess.h"

bool isPiece(unsigned char c)
{
	switch (c) {
	case WPAWN:
	case WKNIGHT:
	case WBISHOP:
	case WROOK:
	case WQUEEN:
	case WKING:
	case BPAWN:
	case BKNIGHT:
	case BBISHOP:
	case BROOK:
	case BQUEEN:
	case BKING:
		return true;

	default:
		return false;
	}
}

Board Chess::getBoard()
{
	return this->board;
}

// todo: transition to bitboards at some point
Chess::Chess(std::string s)
{
	int idx = 0;
	for (unsigned char c : s) {
		if (std::isspace(c) || idx >= 64)
			break;

		if (isPiece(c)) {
			board[idx].tag = static_cast<enum PieceTag>(c);
			idx++;
		} else if (c == '/') {
			if (idx % 8 != 0) {
				idx = idx - (idx % 8) + 8;
			}
		} else if (std::isdigit(c)) {
			idx += (c - '0');
		}
	}
}
