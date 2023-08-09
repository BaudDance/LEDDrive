#include "Dino.h"
#include "font.h"
#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define DINO_INIT_X 10      // Dino initial spawn location
#define DINO_INIT_Y 62 - 23 // Dino initial spawn location

#define BASE_LINE_X 0
#define BASE_LINE_Y 63
#define BASE_LINE_X1 127

// Number of pixel dino will jump
#define JUMP_PIXEL 32

/* Bitmap */
// Dino crashed
const uint8_t dino_crashedData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xf1, 0xf5, 0xf1, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7e, 0x7f, 0xfc, 0xf8, 0xe0, 0xe0, 0xf8, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x04, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x81, 0x83, 0x87, 0xff, 0xdf, 0x87, 0x87, 0x87, 0xff, 0xc3, 0xc3, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};
const Image dino_crashedImg = {21, 23, dino_crashedData};
// Dino lifts front leg
const uint8_t dino_front_legData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xbf, 0xbf, 0x3f, 0x3e, 0x3f, 0xfc, 0xf8, 0xf0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x04, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x81, 0x83, 0x87, 0xff, 0xdf, 0x8f, 0x87, 0x87, 0x8f, 0x8b, 0x8b, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};
const Image dino_front_legImg = {21, 23, dino_front_legData};
// Dino lifts back leg
const uint8_t dino_back_legData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xbf, 0xbf, 0x3f, 0x3e, 0x3f, 0xfc, 0xf8, 0xf0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x04, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x81, 0x83, 0x87, 0x8f, 0x9f, 0x97, 0x87, 0x8f, 0xff, 0xc3, 0xc3, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};
const Image dino_back_legImg = {21, 23, dino_back_legData};
// Dino jumps
const uint8_t dino_jumpsData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xbf, 0xbf, 0xbf, 0x3f, 0x3e, 0x3f, 0xfc, 0xf8, 0xf0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x04, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x81, 0x83, 0x87, 0xff, 0xdf, 0x8f, 0x87, 0x8f, 0xff, 0xc3, 0xc3, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
};
const Image dino_jumpsImg = {21, 23, dino_jumpsData};
// Small tree
const uint8_t tree_smallData[] = {
    0xf8, 0x00, 0xff, 0xff, 0xc0, 0x7c, 0xc1, 0xc1, 0xff, 0xff, 0xc0, 0xc0,
};
const Image tree_smallImg = {6, 14, tree_smallData};
// Big tree
const uint8_t tree_bigData[] = {
    0x80, 0xc0, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xe0, 0xe0, 0xe0, 0x3f, 0x7f, 0x7f, 0x60, 0xff, 0xff, 0xff, 0xff, 0x60, 0x7f, 0x3f, 0x1f, 0x80, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80,
};
const Image tree_bigImg = {12, 23, tree_bigData};

/* Jump Time-Distance Table */
const uint8_t time_distance[38] = {0, 4, 7, 11, 14, 17, 20, 23, 25, 28, 30, 31, 33, 34, 35, 36, 37, 37, 38, 38, 37, 37, 36, 35, 34, 33, 31, 30, 28, 25, 23, 20, 17, 14, 11, 7, 4, 0};

/* Higheset Score */
int highest_score = 0;

/**
 * @brief Draw the introduction screen
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void introMessage() {
  OLED_NewFrame();
  OLED_PrintString((128 - 80) / 2, 8, "小恐龙游戏", &font16x16, OLED_COLOR_NORMAL);
  OLED_DrawImage(DINO_INIT_X, DINO_INIT_Y, &dino_jumpsImg, OLED_COLOR_NORMAL);
  OLED_PrintString((128 - 32) / 2, 43, "开始", &font16x16, OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}

void showLine() {
  int line_end = 1;
  OLED_NewFrame();
  OLED_DrawImage(DINO_INIT_X, DINO_INIT_Y, &dino_jumpsImg, OLED_COLOR_NORMAL);
  for (;;) {
    OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, line_end, BASE_LINE_Y, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    line_end += 4;
    if (line_end > BASE_LINE_X1) break;
  }
}

/**
 * @brief Move dino
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void moveDino(int16_t y, int type) {
  if (type == 0)
    OLED_DrawImage(DINO_INIT_X, y, &dino_front_legImg, OLED_COLOR_NORMAL);
  else if (type == 1)
    OLED_DrawImage(DINO_INIT_X, y, &dino_back_legImg, OLED_COLOR_NORMAL);
  else if (type == -1)
    OLED_DrawImage(DINO_INIT_X, y, &dino_jumpsImg, OLED_COLOR_NORMAL);
  else if (type == -2)
    OLED_DrawImage(DINO_INIT_X, y, &dino_crashedImg, OLED_COLOR_NORMAL);
}

/**
 * @brief Move tree
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void moveTree(int16_t x, Image *type) {
  if (x > 127 - type->w) return;
  OLED_DrawImage(x, BASE_LINE_Y - type->h, type, OLED_COLOR_NORMAL);
}

/**
 * @brief Display score while running the game
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void displayScore(int score, int8_t type) {
  static char tmp[15];
  if (type == 0) {
    sprintf(tmp, "%d", score);
    OLED_PrintString(72, 3, "得分：", &font12x12, OLED_COLOR_NORMAL);
    OLED_PrintString(72 + 30, 3, tmp, &font12x12, OLED_COLOR_NORMAL);
  }

  else {
    sprintf(tmp, "%d", highest_score);
    OLED_PrintString(72 - 12, 22, "最高分：", &font12x12, OLED_COLOR_NORMAL);
    OLED_PrintString(72 + 30, 22, tmp, &font12x12, OLED_COLOR_NORMAL);
  }
}

// Main play function
void play() {
  int16_t tree_interval = 252;
  int16_t tree1_interval = 381;
  int16_t tree_x = tree_interval + 100;
  int16_t tree1_x = tree1_interval + 200;
  Image *tree = (HAL_GetTick() % 8) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
  Image *tree1 = (HAL_GetTick() % 5) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
  int16_t dino_right = DINO_INIT_X + dino_front_legImg.w;

  int16_t dino_y = DINO_INIT_Y;
  int jump = 0;

  int score_raw = 0;
  int score = 0;

  int8_t leg = 0;

  static char tmp[30];

  for (;;) {
    OLED_NewFrame();

    if (HAL_GPIO_ReadPin(JUMP_GPIO_Port, JUMP_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin) == GPIO_PIN_RESET) {
      if (jump == 0) jump = 1;
    }

    if (tree_x <= (dino_right - ((jump < 5) ? jump : 5)) && tree_x > (DINO_INIT_X + 1) && (dino_y + dino_back_legImg.h) >= (BASE_LINE_Y - tree->h)) {
      // Collision Happened
      moveDino(dino_y, -2);
      moveTree(tree_x, tree);
      moveTree(tree1_x, tree1);
      OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y, OLED_COLOR_NORMAL);
      displayScore(score, 0);
      if (score > highest_score) {
        highest_score = score;
        OLED_PrintString(75, 22, "新纪录！", &font12x12, OLED_COLOR_NORMAL);
      } else {
        displayScore(score, 1);
      }
      OLED_ShowFrame();
      break;
    }

    if (tree1_x <= (dino_right - ((jump < 5) ? jump : 5)) && tree1_x > (DINO_INIT_X + 1) && (dino_y + dino_back_legImg.h) >= (BASE_LINE_Y - tree1->h)) {
      if (dino_y >= (DINO_INIT_Y - 3)) {
        // Collision Happened
        moveDino(dino_y, -2);
        moveTree(tree_x, tree);
        moveTree(tree1_x, tree1);
        OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y, OLED_COLOR_NORMAL);
        displayScore(score, 0);
        if (score > highest_score) {
          highest_score = score;
          OLED_PrintString(75, 22, "新纪录！", &font12x12, OLED_COLOR_NORMAL);
        } else {
          displayScore(score, 1);
        }
        OLED_ShowFrame();
        break;
      }
    }

    if (jump > 0) {
      dino_y = DINO_INIT_Y - time_distance[jump - 1];
      if (++jump > 38) jump = 0;
    }

    score_raw++;
    score = score_raw / 5;
    displayScore(score, 0);
    if (jump != 0)
      moveDino(dino_y, -1);
    else {
      moveDino(dino_y, leg > 4 ? 1 : 0);
      if (++leg > 9) leg = 0;
    }
    moveTree(tree_x, tree);
    moveTree(tree1_x, tree1);
    OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y, OLED_COLOR_NORMAL);

    tree_x -= 2;
    tree1_x -= 2;
    if (tree_x <= 0) {
      tree_x = tree_interval + HAL_GetTick() / 10 % 20;
      tree = (HAL_GetTick() % 4) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
    }
    if (tree1_x <= 0) {
      tree1_x = tree1_interval + HAL_GetTick() / 10 % 20;
      tree1 = (HAL_GetTick() % 2) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
    }
    static int16_t interval;
    interval = tree_x - tree1_x;
    if (interval > 0 && interval <= 46)
      tree_x += interval;
    else if (interval < 0 && interval >= -46)
      tree1_x += interval;
    OLED_ShowFrame();
  }
}
