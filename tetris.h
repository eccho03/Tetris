#ifndef TETRIS_H
#define TETRISH_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h	>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h> //음악 재생을 위한 라이브러리

#pragma comment(lib,"winmm.lib")

enum {LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32, ARROW = 224, p = 112, P = 80, ESC = 27}; //키보드 방향키 아스키코드
enum {GRAY = 7, BLUE = 9, GREEN = 10, SKY_BLUE = 11, RED = 12, PURPLE = 13, YELLOW = 14, WHITE = 15};

enum {WIDTH = 12, HEIGHT = 22}; // 보드판 가로 세로 (벽 제외 10 x 20)
#define BOARD_X 4 //보드판 출력할 때 약간의 여백을 두기 위함
#define BOARD_Y 5
#define BLOCK_MAX 4 //테트리스 블럭 가로, 세로 4x4 배열임

#define EMPTY 0
#define PLAY_EMPTY 100 //복사된 보드판 초기값
#define BLOCK 1 //블록 숫자 지정
#define WALL 2 //벽 숫자 지정
#define FIXED_BLOCK 3 //고정된 블록 숫자 지정
#define CEILING 4 //천장 숫자 지정
#define EFFECT 5 //블록 사라질 때 별모양으로 사라지는 이펙트

//전역변수/////////////////////////////// 
int Board[WIDTH][HEIGHT] = { 0 }; //게임 보드판
int PlayingBoard[WIDTH][HEIGHT] = { 0 }; //게임 진행중 보드판
int Block[7][4][BLOCK_MAX][BLOCK_MAX] = {
	{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
	 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
	 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
	{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
	 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
	{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
	 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
	{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
	 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
	{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
	 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
	{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
	 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록 모양 저장
int bx = 4, by = 0; //블록의 좌표
int checkCrush = 0, rotation = 0; //바닥에 부딪혔는지/회전 체크
int checkNewBlock = 0; //새로운 블록 생성해야 하는지 체크
int checkSpace = 0; //스페이스바 눌렀는지 체크
int currentBlock = 0, nextBlock = 0; //생성될 블록/생성한 블록
int score = 0, level = 1, speed = 0, bestScore; //점수, 레벨, 속도, 최고점수
//함수///////////////////////////////////////
void CursorView(char show); //커서 생성 및 소멸
void GotoXY(int x, int y); //해당 좌표로 이동하는 함수
void TextColor(int colorNum); //글자 색 바꿈
void NextBlockColor(int blockColor); //다음 블록 색 지정
void PlayBGM(void); //게임 bgm 재생

void ResetGame(); //게임 전체 초기화
void InitTitle(void); //초기 게임 상태
void InitBoard(void); //보드판 초기화
void InitBlock(void); //블럭 초기화
void InitPlayingBoard(void); //복사된 보드판 초기화
void InitInformation(void); //조작법 스크린에 띄움
void PauseGame(void); //p를 눌렀을 때 게임 일시정지
void GameProcess(void); //게임 진행과정

void ProcessBoard(void); //게임 진행 중 상태
void CreateScreen(void); //보드판 그리기

void CreateNewBlock(void); //랜덤으로 새로운 블럭 생성함
void DropBlock(void); //보드판에 블럭 놓기

void MoveKey(void); //방향키
void MoveBlock(int direction); //방향키 방향으로 블럭을 이동시킨다.

void RemoveBlockShape(void); //블록 잔상을 없애줌
void FixBlockShape(void); //블록 보드에 고정함

int CheckCollision(int bx, int by, int rotate); //블록이 벽에 부딪혔을 때 나가지 않게 해줌
void CheckLine(void); //한 줄 없어질 수 있는지 체크하는 함수
int CheckGameOver(void); //게임이 끝났는지 체크해주는 함수
void LevelUpScreen(void); //레벨업 하면 화면 출력
void GameOverScreen(void); //게임 오버 되었을 때 출력되는 스크린
#endif