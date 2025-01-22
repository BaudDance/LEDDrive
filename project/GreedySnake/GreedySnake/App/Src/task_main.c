#include "task_main.h"
#include <stdlib.h>
#include <string.h>
#include "oled.h"

#define SNAKE_MAX_LENGTH 100							// 蛇的最大长度
#define SCREEN_WIDTH 128								// 屏幕宽度
#define SCREEN_HEIGHT 64								// 屏幕高度
#define INFOBAR_HEIGHT 17								// 信息栏高度
#define GRID_SIZE 4 									// 4x4像素一个格子
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE) 			// 横向格子数
#define GRID_HEIGHT ((SCREEN_HEIGHT - INFOBAR_HEIGHT) / GRID_SIZE)  // 纵向格子数 预留顶部显示分数

typedef enum {
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT
} Direction;  // 蛇的移动方向

typedef enum {
    GAME_START,
    GAME_RUNNING,
    GAME_OVER
} GameState;  // 游戏状态

typedef struct {
    int x;
    int y;
} Point; 	// 蛇的位置

// 游戏状态变量
static GameState gameState = GAME_START;
static Point snake[SNAKE_MAX_LENGTH];			// 蛇的位置
static int snakeLength = 3;						// 当前蛇的长度
static Direction direction = DIRECTION_RIGHT;	// 蛇的移动方向
static Point food;								// 食物的位置
static int score = 0;							// 当前分数

// 初始化蛇的位置
void initSnake() {
    snakeLength = 3;
    snake[0].x = GRID_WIDTH / 2;
    snake[0].y = GRID_HEIGHT / 2;
    snake[1].x = snake[0].x - 1;
    snake[1].y = snake[0].y;
    snake[2].x = snake[1].x - 1;
    snake[2].y = snake[1].y;
    direction = DIRECTION_RIGHT;
    score = 0;
}

// 生成新的食物
void generateFood() {
    int valid;
    do {
        valid = 1;
        food.x = rand() % (GRID_WIDTH - 1);  // 减1避免靠近边框
        food.y = rand() % (GRID_HEIGHT - 1);  // 减1避免靠近边框
        
        for (int i = 0; i < snakeLength; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

// 检查是否发生碰撞
int checkCollision() {
    // 检查是否撞墙
    if (snake[0].x <= 0 || snake[0].x >= GRID_WIDTH - 1 ||
        snake[0].y <= 0 || snake[0].y >= GRID_HEIGHT - 1) {
        return 1;
    }
    
    // 检查是否撞到自己
    for (int i = 1; i < snakeLength; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;
        }
    }
    return 0;
}

// 移动蛇
void moveSnake() {
    // 保存蛇头的位置
    Point newHead = snake[0];
    
    // 根据方向移动蛇头
    switch (direction) {
        case DIRECTION_UP:
            newHead.y--;
            break;
        case DIRECTION_RIGHT:
            newHead.x++;
            break;
        case DIRECTION_DOWN:
            newHead.y++;
            break;
        case DIRECTION_LEFT:
            newHead.x--;
            break;
    }
    
    // 移动蛇身
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;
    
    // 检查是否吃到食物
    if (snake[0].x == food.x && snake[0].y == food.y) {
        score += 10;
        snakeLength++;
        generateFood();
    }
}

// 旋钮回调函数
void onKnobForward() {
    if (gameState == GAME_START) {
        gameState = GAME_RUNNING;
        initSnake();
        generateFood();
        return;
    }
    
    if (gameState != GAME_RUNNING) return;
    
    // 根据当前方向确定下一个方向
    switch (direction) {
        case DIRECTION_UP:
            direction = DIRECTION_RIGHT;
            break;
        case DIRECTION_RIGHT:
            direction = DIRECTION_DOWN;
            break;
        case DIRECTION_DOWN:
            direction = DIRECTION_LEFT;
            break;
        case DIRECTION_LEFT:
            direction = DIRECTION_UP;
            break;
    }
}

void onKnobBackward() {
    if (gameState == GAME_START) {
        gameState = GAME_RUNNING;
        initSnake();
        generateFood();
        return;
    }
    
    if (gameState != GAME_RUNNING) return;
    
    // 根据当前方向确定下一个方向
    switch (direction) {
        case DIRECTION_UP:
            direction = DIRECTION_LEFT;
            break;
        case DIRECTION_RIGHT:
            direction = DIRECTION_UP;
            break;
        case DIRECTION_DOWN:
            direction = DIRECTION_RIGHT;
            break;
        case DIRECTION_LEFT:
            direction = DIRECTION_DOWN;
            break;
    }
}

void onKnobPressed() {
    if (gameState == GAME_OVER) {
        gameState = GAME_START;
        initSnake();  // 重置蛇的位置和长度
        score = 0;    // 重置分数
    }else if (gameState == GAME_START) {
        gameState = GAME_RUNNING;
        initSnake();
        generateFood();
    }
}

void MainTaskInit() {
    HAL_Delay(20);
    OLED_Init();
    Knob_Init();
    Knob_SetForwardCallback(onKnobForward);
    Knob_SetBackwardCallback(onKnobBackward);
    Knob_SetPressedCallback(onKnobPressed);
    srand(HAL_GetTick());
}

void drawStartScreen() {
    // 标题使用最大字体
    OLED_PrintString(10, 15, "~~贪吃蛇~~", &font20x20, OLED_COLOR_NORMAL);
    // 提示信息使用小字体
    OLED_PrintString(10, 40, "按下旋钮开始游戏~", &font13x13, OLED_COLOR_NORMAL);
}

void drawGameScreen() {
    char scoreStr[20];
    sprintf(scoreStr, "得分: %d", score);
    // 分数使用小字体，节省空间
    OLED_PrintString(2, 4, scoreStr, &font13x13, OLED_COLOR_NORMAL);
    
    // 绘制游戏区域边框
    OLED_DrawRectangle(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - INFOBAR_HEIGHT - (GRID_SIZE + 1), OLED_COLOR_NORMAL);
    
    // 绘制蛇
    for (int i = 0; i < snakeLength; i++) {
        OLED_DrawFilledRectangle(snake[i].x * GRID_SIZE + 1, 
                                snake[i].y * GRID_SIZE + INFOBAR_HEIGHT,  // 避开顶部分数区域
                                GRID_SIZE - 1, 
                                GRID_SIZE - 1, 
                                OLED_COLOR_NORMAL);
    }
    
    // 绘制食物
    OLED_DrawFilledRectangle(food.x * GRID_SIZE + 1, 
                            food.y * GRID_SIZE + INFOBAR_HEIGHT,  // 避开顶部分数区域
                            GRID_SIZE - 1, 
                            GRID_SIZE - 1, 
                            OLED_COLOR_NORMAL);
}

void drawGameOverScreen() {
    // Game Over使用大字体
	OLED_PrintString(10, 4, "!游戏结束!", &font20x20, OLED_COLOR_NORMAL);
    
    char scoreStr[20];
    sprintf(scoreStr, "最终得分: %d", score);
    // 分数使用中等字体
    OLED_PrintString(10, 29, scoreStr, &font13x13, OLED_COLOR_NORMAL);
    // 重启提示使用小字体
    OLED_PrintString(10, 46, "按下旋钮重新游戏!", &font13x13, OLED_COLOR_NORMAL);
}

void MainTask() {
    static uint32_t lastMoveTime = 0;
    const uint32_t moveInterval = 300; // 移动间隔（毫秒）
    
    Knob_Loop();
    OLED_NewFrame();
    
    switch (gameState) {
        case GAME_START:
            drawStartScreen();
            break;
            
        case GAME_RUNNING:
            if (HAL_GetTick() - lastMoveTime >= moveInterval) {
                moveSnake();
                if (checkCollision()) {
                    gameState = GAME_OVER;
                }
                lastMoveTime = HAL_GetTick();
            }
            drawGameScreen();
            break;
            
        case GAME_OVER:
            drawGameOverScreen();
            break;
    }
    
    OLED_ShowFrame();
}
