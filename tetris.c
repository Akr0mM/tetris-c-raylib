#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 30

typedef enum {
  SHAPE_I,
  SHAPE_O,
  SHAPE_T,
  SHAPE_L,
  SHAPE_J,
  SHAPE_Z,
  SHAPE_S
} ShapeType;

typedef struct Tetris {
  ShapeType shape;
  int x;
  int y;
  int (*tetris)[4][4];
  int rotation;
} Tetris;

typedef struct Block {
  int x;
  int y;
  Color c;
  bool isFilled;
} Block;

void getInput();
void drawTetris(Tetris tetris);
void newCurrent();
void dropCurrent();
void moveCurrent(int dir);
void undoMove();
void fixCurrent();
void rotateCurrent();
void freezeCurrent();
void drawFreezedTetris();
void checkLines();
void instantDrop();

const int bs = 30;
int shapes[7][4][4][4] = {
    {{{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
    },
    {
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
        },
    },
    {
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 1},
            {0, 1, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
        },
    },
    {
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0},
        },
    },
    {
        {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
        },
    },
    {
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
        },
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
        },
    }};

Tetris current;
Tetris currentLastPos;
Block freezedBlocks[HEIGHT][WIDTH];

Color colors[7] = {SKYBLUE, YELLOW, DARKPURPLE, ORANGE, DARKBLUE, RED, GREEN};

int main(void) {
  srand(time(NULL));

  InitWindow(WIDTH * bs, HEIGHT * bs, "Tetris wow");

  SetTargetFPS(11);

  newCurrent(&current);

  double lastTime = GetTime();

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    getInput();
    if (GetTime() - lastTime >= 0.9) {
      dropCurrent();
      lastTime = GetTime();
    }

    // Draw
    BeginDrawing();

    ClearBackground(BLACK);
    drawTetris(current);
    drawFreezedTetris();

    EndDrawing();
  }

  CloseWindow(); // Close window and OpenGL context

  return 0;
}

void checkLines() {
  for (int y = HEIGHT - 1; y >= 0; y--) {
    bool lineIsFilled = true;
    for (int x = 0; x < WIDTH; x++) {
      if (!freezedBlocks[y][x].isFilled) {
        lineIsFilled = false;
        break;
      }
    }

    if (lineIsFilled) {
      // Décaler toutes les lignes au-dessus vers le bas
      for (int i = y; i > 0; i--) {
        for (int x = 0; x < WIDTH; x++) {
          freezedBlocks[i][x] = freezedBlocks[i - 1][x];
          freezedBlocks[i][x].y = i;
          freezedBlocks[i][x].x = x;
        }
      }

      // Vider la première ligne (tout en haut)
      for (int x = 0; x < WIDTH; x++) {
        freezedBlocks[0][x].isFilled = false;
        freezedBlocks[0][x].x = -1; // ou 0 si tu ne testes pas x
      }

      // Re-vérifie cette ligne (car elle contient maintenant la ligne du
      // dessus)
      y++;
    }
  }
}

void fixCurrent() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (current.tetris[current.rotation][i][j]) {
        if (current.x + j < 0) {
          current.x += 1;
          fixCurrent();
        } else if (current.x + j >= WIDTH) {
          current.x -= 1;
          fixCurrent();
        }

        if (current.y + i > HEIGHT - 1 ||
            freezedBlocks[current.y + i][current.x + j].isFilled) {
          if (current.y != currentLastPos.y) {
            undoMove();
            freezeCurrent();
          } else {
            undoMove();
          }
        }
      }
    }
  }
}

void freezeCurrent() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (current.tetris[current.rotation][i][j]) {
        freezedBlocks[current.y + i][current.x + j] =
            (Block){current.x + j, current.y + i, colors[current.shape], true};
      }
    }
  }

  newCurrent();
}

void drawFreezedTetris() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      Block b = freezedBlocks[i][j];
      if (b.isFilled) {
        DrawRectangle(b.x * bs, b.y * bs, bs, bs, b.c);
      }
    }
  }
}

void instantDrop() {
  while (1) {
    dropCurrent();
    if (current.y == currentLastPos.y) {
      break;
    }
  }
}

void rotateCurrent() {
  current.rotation += 1;
  current.rotation %= 4;
  fixCurrent();
}

void undoMove() { current = currentLastPos; };

void getInput() {
  if (IsKeyDown(KEY_LEFT)) {
    moveCurrent(-1);
  }
  if (IsKeyDown(KEY_RIGHT)) {
    moveCurrent(1);
  }

  if (IsKeyPressed(KEY_UP)) {
    rotateCurrent();
  }

  if (IsKeyDown(KEY_DOWN)) {
    dropCurrent();
  }

  if (IsKeyPressed(KEY_SPACE)) {
    instantDrop();
  }
}

void dropCurrent() {
  currentLastPos = current;
  current.y += 1;
  fixCurrent();
}

void drawTetris(Tetris tetris) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris.tetris[tetris.rotation][i][j] == 1) {
        DrawRectangle((tetris.x + j) * bs, (tetris.y + i) * bs, bs, bs,
                      colors[tetris.shape]);
      }
    }
  }
}

void newCurrent() {
  int shape = rand() % 7;
  current = (Tetris){shape, (WIDTH - 4) / 2, -1, shapes[shape], rand() % 4};
  currentLastPos = current;
  fixCurrent();
  checkLines();
}

void moveCurrent(int dir) {
  currentLastPos = current;
  current.x += dir;
  fixCurrent();
}
