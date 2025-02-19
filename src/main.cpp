#include <cstdio>
#include <iostream>
#include <stdexcept>

#include <raylib.h>
#include <raymath.h>
#include <stdexcept>

#include "chess.h"

// 0.75 ekranu

#define LIGHT_SQUARE_COLOR Color{0xF0, 0xD9, 0xB5, 0xFF}
#define DARK_SQUARE_COLOR Color{0xB5, 0x88, 0x63, 0xFF}
// #define BG_COLOR Color{0x16, 0x15, 0x12, 0xFF}
#define BG_COLOR RAYWHITE

std::array<Texture, 12> getPieceTextures(int size)
{
	std::array<Texture, 12> ret;
	std::array<Image, 12> images;

	images[0] = LoadImage("wP.png");
	images[1] = LoadImage("wN.png");
	images[2] = LoadImage("wB.png");
	images[3] = LoadImage("wR.png");
	images[4] = LoadImage("wQ.png");
	images[5] = LoadImage("wK.png");
	images[6] = LoadImage("bP.png");
	images[7] = LoadImage("bN.png");
	images[8] = LoadImage("bB.png");
	images[9] = LoadImage("bR.png");
	images[10] = LoadImage("bQ.png");
	images[11] = LoadImage("bK.png");

	int cnt = 0;
	for (Image i : images) {
		ImageResize(&i, size, size);
		ret[cnt] = LoadTextureFromImage(i);
		UnloadImage(i);
		cnt++;
	}

	return ret;
}

Texture pieceTexture(Piece piece, std::array<Texture, 12> textures)
{
	switch (piece.tag) {
	case WPAWN:
		return textures[0];
	case WKNIGHT:
		return textures[1];
	case WBISHOP:
		return textures[2];
	case WROOK:
		return textures[3];
	case WQUEEN:
		return textures[4];
	case WKING:
		return textures[5];
	case BPAWN:
		return textures[6];
	case BKNIGHT:
		return textures[7];
	case BBISHOP:
		return textures[8];
	case BROOK:
		return textures[9];
	case BQUEEN:
		return textures[10];
	case BKING:
		return textures[11];
	default:
		throw std::invalid_argument("a");
	}
}

void ungetPieceTextures(std::array<Texture, 12> textures)
{
	for (auto i : textures) {
		UnloadTexture(i);
	}
}

int whereInBoardIsPos(Vector2 pos, Rectangle boardRec, float cellSize)
{
	if (!CheckCollisionPointRec(pos, boardRec)) {
		return -1;
	}

	Rectangle rec = boardRec;
	rec.height = cellSize;
	rec.width = cellSize;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (CheckCollisionPointRec(pos, rec)) {
				return i * 8 + j;
			}
			rec.x += cellSize;
		}
		rec.x = boardRec.x;
		rec.y += cellSize;
	}

	// this shouldnt happen :)
	return -1;
}

int main(void)
{
	Chess chess("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	for (auto i : chess.board) {
		std::cout.put(i.tag);
		std::cout << " ";
	}

	ChangeDirectory("res");

	const int screenWidth = 1080;
	const int screenHeight = 720;
	InitWindow(screenWidth, screenHeight, "stuff");
	SetTargetFPS(60);

	int display = GetCurrentMonitor();
	SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
	SetWindowState(FLAG_FULLSCREEN_MODE);

	float scrWidthFloat = static_cast<float>(GetScreenWidth());
	float scrHeightFloat = static_cast<float>(GetScreenHeight());
	Vector2 orig = {
	    (scrWidthFloat - 0.75f * scrHeightFloat) / 2.0f,
	    scrHeightFloat / 8.0f,
	};
	float boardSize = 0.75f * scrHeightFloat;
	float cellSize = boardSize / 8;
	Rectangle boardRec = Rectangle{orig.x, orig.y, boardSize, boardSize};
	std::array<Texture, 12> pieceTextures = getPieceTextures(cellSize);
	Piece heldPiece(PIECE_NONE);
	bool holding = false;
	int origCell = 0;

	while (!WindowShouldClose()) {

		Vector2 mousePos = GetMousePosition();

		int mouseCell = whereInBoardIsPos(mousePos, boardRec, cellSize);

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouseCell > -1) {
			if (holding == false && chess.board[mouseCell].tag != PIECE_NONE) {
				holding = true;
				origCell = mouseCell;
				heldPiece = chess.drop(mouseCell);
			}
		} else if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON) && holding) {
			holding = false;
			int cell = (mouseCell > -1) ? mouseCell : origCell;
			chess.board[cell] = heldPiece;
			heldPiece.tag = PIECE_NONE;
		}

		BeginDrawing();
		ClearBackground(BG_COLOR);

		DrawText(TextFormat("held: %x\nholding: %d", &heldPiece, holding), 0, 0, 20, GRAY);

		DrawRectangle(orig.x, orig.y, boardSize, boardSize, LIGHT_SQUARE_COLOR);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int currentSquare = j * 8 + i;

				if (i % 2 != j % 2) {
					DrawRectangle(i * cellSize + orig.x, j * cellSize + orig.y, cellSize, cellSize, DARK_SQUARE_COLOR);
				}

				if (mouseCell == currentSquare) {
					DrawRectangle(i * cellSize + orig.x, j * cellSize + orig.y, cellSize, cellSize, LIME);
				}

				try {
					DrawTexture(pieceTexture(chess.board.at(currentSquare), pieceTextures), i * cellSize + orig.x,
					            j * cellSize + orig.y, WHITE);
				} catch (std::invalid_argument &e) {
					// dont draw
				}
			}
		}

		try {
			DrawTexture(pieceTexture(heldPiece, pieceTextures), mousePos.x, mousePos.y, WHITE);
		} catch (std::invalid_argument &e) {
		}

		EndDrawing();
	}

	ungetPieceTextures(pieceTextures);
	CloseWindow();

	return 0;
}
