#ifndef TETRIS_H
#define TETRISH_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h	>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h> //���� ����� ���� ���̺귯��

#pragma comment(lib,"winmm.lib")

enum {LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32, ARROW = 224, p = 112, P = 80, ESC = 27}; //Ű���� ����Ű �ƽ�Ű�ڵ�
enum {GRAY = 7, BLUE = 9, GREEN = 10, SKY_BLUE = 11, RED = 12, PURPLE = 13, YELLOW = 14, WHITE = 15};

enum {WIDTH = 12, HEIGHT = 22}; // ������ ���� ���� (�� ���� 10 x 20)
#define BOARD_X 4 //������ ����� �� �ణ�� ������ �α� ����
#define BOARD_Y 5
#define BLOCK_MAX 4 //��Ʈ���� �� ����, ���� 4x4 �迭��

#define EMPTY 0
#define PLAY_EMPTY 100 //����� ������ �ʱⰪ
#define BLOCK 1 //��� ���� ����
#define WALL 2 //�� ���� ����
#define FIXED_BLOCK 3 //������ ��� ���� ����
#define CEILING 4 //õ�� ���� ����
#define EFFECT 5 //��� ����� �� ��������� ������� ����Ʈ

//��������/////////////////////////////// 
int Board[WIDTH][HEIGHT] = { 0 }; //���� ������
int PlayingBoard[WIDTH][HEIGHT] = { 0 }; //���� ������ ������
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
}; //��� ��� ����
int bx = 4, by = 0; //����� ��ǥ
int checkCrush = 0, rotation = 0; //�ٴڿ� �ε�������/ȸ�� üũ
int checkNewBlock = 0; //���ο� ��� �����ؾ� �ϴ��� üũ
int checkSpace = 0; //�����̽��� �������� üũ
int currentBlock = 0, nextBlock = 0; //������ ���/������ ���
int score = 0, level = 1, speed = 0, bestScore; //����, ����, �ӵ�, �ְ�����
//�Լ�///////////////////////////////////////
void CursorView(char show); //Ŀ�� ���� �� �Ҹ�
void GotoXY(int x, int y); //�ش� ��ǥ�� �̵��ϴ� �Լ�
void TextColor(int colorNum); //���� �� �ٲ�
void NextBlockColor(int blockColor); //���� ��� �� ����
void PlayBGM(void); //���� bgm ���

void ResetGame(); //���� ��ü �ʱ�ȭ
void InitTitle(void); //�ʱ� ���� ����
void InitBoard(void); //������ �ʱ�ȭ
void InitBlock(void); //�� �ʱ�ȭ
void InitPlayingBoard(void); //����� ������ �ʱ�ȭ
void InitInformation(void); //���۹� ��ũ���� ���
void PauseGame(void); //p�� ������ �� ���� �Ͻ�����
void GameProcess(void); //���� �������

void ProcessBoard(void); //���� ���� �� ����
void CreateScreen(void); //������ �׸���

void CreateNewBlock(void); //�������� ���ο� �� ������
void DropBlock(void); //�����ǿ� �� ����

void MoveKey(void); //����Ű
void MoveBlock(int direction); //����Ű �������� ���� �̵���Ų��.

void RemoveBlockShape(void); //��� �ܻ��� ������
void FixBlockShape(void); //��� ���忡 ������

int CheckCollision(int bx, int by, int rotate); //����� ���� �ε����� �� ������ �ʰ� ����
void CheckLine(void); //�� �� ������ �� �ִ��� üũ�ϴ� �Լ�
int CheckGameOver(void); //������ �������� üũ���ִ� �Լ�
void LevelUpScreen(void); //������ �ϸ� ȭ�� ���
void GameOverScreen(void); //���� ���� �Ǿ��� �� ��µǴ� ��ũ��
#endif