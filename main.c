#include "tetris.h"

int main(void) {
    ResetGame(); //���� �ʱ�ȭ
    GameProcess(); //���� �������
    return 0;
}

void ResetGame(void) {
    srand((unsigned int)time(NULL)); //�ʱ�ȭ
    //PlayBGM(); //���� bgm ��� ���� (��Ʈ������ ���� ������ �־�� ��)
    CursorView(FALSE); //Ŀ�� �����
    InitTitle(); //���� ���� �� �ʱ�ȭ��

    FILE* file = fopen("bestScore.txt", "rt"); //��Ʈ������ ���� ������ �� �̸��� �ؽ�Ʈ ������ �־�� ��
    if (file == 0) //���Ͽ� �ƹ��͵� ������
        bestScore = 0; //�ְ������� 0
    else {
        fscanf(file, "%d", &bestScore);
        fclose(file);
    }

    InitBoard(); //���� ������ �ʱ�ȭ
    CreateScreen();
    InitInformation(); //���� ���ھ� �� ���۹� �ҷ�����
    nextBlock = rand() % 7; //�������� �������� ������ ��� ����
    CreateNewBlock(); //�������� ��� �ϳ� ����

    //���� �������� �ʱ�ȭ
    checkCrush = 0, rotation = 0; //�ٴڿ� �ε�������/ȸ�� üũ
    checkNewBlock = 0; //���ο� ��� �����ؾ� �ϴ��� üũ
    checkSpace = 0; //�����̽��� �������� üũ
    score = 0, level = 1, speed = 0;
}

void GameProcess(void) {
    while (1) {
        for (int i = 0; i < 5; i++) { //��� ������ �� 5���� �̵���ȸ�� �ο���
            MoveKey();
            if (checkCrush > 0 && !CheckCollision(bx, by, rotation))
                Sleep(100); //��� �浹 ���� �� �߰��� �̵��� �ð� Ȯ����
            if (checkSpace == 1) {  //�����̽��� ��������
                checkSpace = 0; // �ٽ� �����̽��� �������� Ȯ�� ���� �ʱ�ȭ
                break; //while�� �������´�(��� ������ �ʿ� ���� ����)
            }
        }

        DropBlock(); //�� ����߸���
        CreateScreen(); //������ ���
        if (checkNewBlock == 1) { //�� ��� ���� ���� Ȱ��ȭ �Ǿ����� �� �� ��� �����ϱ�
            CreateNewBlock(); //�ٽ� ���ο� ��� ����
        }
    }
}

void PlayBGM(void) {
    PlaySound(TEXT("Tetris - Bradinsky.wav"), NULL, SND_ASYNC || SND_LOOP);
}

void InitTitle(void) {
    int n = 0;
    int x = 3, y = 4;
    GotoXY(x, y + 0); printf("============================================\n"); printf("\n"); TextColor(RED);
    GotoXY(x, y + 1); printf("����  ����  ����  �����  ��  ����\n"); Sleep(100); TextColor(YELLOW);
    GotoXY(x, y + 2); printf("  ��    ��        ��    ��    ��  ��  ��    \n"); Sleep(100); TextColor(GREEN);
    GotoXY(x, y + 3); printf("  ��    ����    ��    �����  ��    ���\n"); Sleep(100); TextColor(SKY_BLUE);
    GotoXY(x, y + 4); printf("  ��    ��        ��    ��  ��    ��      ��\n"); Sleep(100); TextColor(BLUE);
    GotoXY(x, y + 5); printf("  ��    ����    ��    ��  ���  ��  ���  \n"); Sleep(100); TextColor(PURPLE);
    GotoXY(x, y + 6); printf("\n"); TextColor(WHITE);
    GotoXY(x, y + 7); printf("============================================\n");
    printf("\n");
    GotoXY(x, y + 8); printf("                      ��Made by Eun Chae Cho\n");
    printf("\n");
    while (n < 3) {
        GotoXY(x, y + 12);
        printf("          ��Press Any Key to Start��          \n");
        Sleep(300);
        GotoXY(x, y + 12);
        printf("                                               ");
        Sleep(300);
        n++;
    }
    GotoXY(x, y + 12); printf("          ��Press Any Key to Start��          \n");
    GotoXY(x + 10, y + 14); printf("   ��      Rotate Block\n"); Sleep(100);
    GotoXY(x + 10, y + 15); printf("��    ��   Move Block\n"); Sleep(100);
    GotoXY(x + 10, y + 16); printf("   ��      Soft Drop\n"); Sleep(100);
    GotoXY(x + 10, y + 17); printf("SPACE      Hard Drop\n"); Sleep(100);
    GotoXY(x + 10, y + 18); printf("p / P      Pause Game\n"); Sleep(100);
    GotoXY(x + 10, y + 19); printf("ESC        Quit Game\n"); Sleep(100);
    _getch();
    system("cls");
    GotoXY(0, 0);
    Sleep(500);
}

void CreateNewBlock(void) {
    bx = 4, by = 0; //����� ��ǥ
    currentBlock = nextBlock;
    nextBlock = rand() % 7;
    rotation = 0;
    checkNewBlock = 0; // �� ��� ���� ���� ��Ȱ��ȭ

    //���� ��� ������ ���� ���
    for (int i = 0; i < BLOCK_MAX; i++) {
        for (int j = 0; j < BLOCK_MAX; j++) {
            if (Block[currentBlock][rotation][i][j] == BLOCK)
                Board[bx + i][by + j] = BLOCK;
        }
    }

    //next block ���
    for (int i = 0; i < BLOCK_MAX; i++) {
        for (int j = 0; j < BLOCK_MAX; j++) {
            GotoXY(40 + 2 * i, 8 + j);
            if (Block[nextBlock][rotation][i][j] == BLOCK) {
                NextBlockColor(nextBlock);
                printf("��");
            }
            else
                printf("  ");
        }
        printf("\n");
    }
}

void CreateScreen(void) {
    int i, j;   // �̵��� �Ÿ�
    //�� ����
    for (int i = 0; i < HEIGHT; i++) {
        Board[0][i] = WALL;
    }

    for (int i = 0; i < HEIGHT; i++) {
        Board[WIDTH - 1][i] = WALL;
    }

    for (int i = 0; i < WIDTH; i++) {
        Board[i][HEIGHT - 1] = WALL;
    }

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            GotoXY(BOARD_X + i * 2, BOARD_Y + j);
            if (Board[i][j] == WALL) {
                TextColor(WHITE);
                printf("��");
            }
            else if (Board[i][j] == BLOCK) {
                TextColor(WHITE);
                printf("��");
            }
            else if (Board[i][j] == FIXED_BLOCK) {
                TextColor(WHITE);
                printf("��");
            }
            else if (Board[i][j] == EFFECT) {
                TextColor(WHITE);
                printf("��");
            }
            else {
                printf("  ");
                TextColor(WHITE);
            }
        }
    }
}

void MoveKey(void) {
    if (_kbhit()) {
        int nkey = _getch();
        if (nkey == SPACEBAR) {
            checkSpace = 1;
            while (!checkCrush)
                DropBlock();
            checkSpace = 0;
        }
        else if (nkey == ESC) {
            GameOverScreen();
        }
        else if (nkey == p || nkey == P) { //p�� PŰ�� ������ �� ���� �Ͻ�������
            PauseGame();
        }
        else if (nkey == ARROW) {
            nkey = _getch();

            switch (nkey) {
            case UP:
                if (CheckCollision(bx, by, (rotation + 1) % 4))
                    MoveBlock(UP);
                break;
            case LEFT:
                if (CheckCollision(bx - 1, by, rotation))
                    MoveBlock(LEFT);
                break;
            case RIGHT:
                if (CheckCollision(bx + 1, by, rotation))
                    MoveBlock(RIGHT);
                break;
            case DOWN:
                if (checkCrush == 0 && CheckCollision(bx, by + 1, rotation))
                    MoveBlock(DOWN);
                break;
            }
        }
    }
}

void MoveBlock(int direction) {
    int i, j;
    switch (direction) {
    case LEFT:
        RemoveBlockShape();
        for (i = 0; i < BLOCK_MAX; i++) {
            for (j = 0; j < BLOCK_MAX; j++) {
                if (Block[currentBlock][rotation][i][j] == BLOCK)
                    Board[bx + i - 1][by + j] = BLOCK;
            }
        }
        bx--;
        break;

    case RIGHT:
        RemoveBlockShape();
        for (i = 0; i < BLOCK_MAX; i++) {
            for (j = 0; j < BLOCK_MAX; j++) {
                if (Block[currentBlock][rotation][i][j] == BLOCK)
                    Board[bx + i + 1][by + j] = BLOCK;
            }
        }
        bx++;
        break;

    case DOWN:
        if (!CheckCollision(bx, by, rotation) && checkCrush == 1) {//��, �ٴڿ� ��Ҵ°�
            FixBlockShape();
            Sleep(100);
        }
        else if (!CheckCollision(bx, by, rotation) && checkCrush == 0) { //���� ��Ҵ°�
            FixBlockShape();
            Sleep(100);
        }
        else { //���̳� ��Ͽ� �� ����� ��
            RemoveBlockShape();
            for (i = 0; i < BLOCK_MAX; i++) {
                for (j = 0; j < BLOCK_MAX; j++) {
                    if (Block[currentBlock][rotation][i][j] == BLOCK)
                        Board[bx + i][by + j + 1] = BLOCK;
                }
            }
            by++;
            if (checkSpace == 0) //�����̽� �� ��������(�̷��� �� �ϸ� �ϵ��� �ϴ� �� ���� �ɸ�)
                Sleep(400 - speed * 50); //�ʱ� �ӵ�: 350 �ִ� ���� 7����
        }

        break;

    case UP: //��� ���� ȸ��
        RemoveBlockShape();
        rotation = (rotation + 1) % 4;
        for (i = 0; i < BLOCK_MAX; i++) {
            for (j = 0; j < BLOCK_MAX; j++) {
                if (Block[currentBlock][rotation][i][j] == BLOCK)
                    Board[bx + i][by + j] = BLOCK;
            }
        }
        break;
    }
}

void InitBlock(void) {
    for (int i = 0; i < BLOCK_MAX; i++) {
        for (int j = 0; j < BLOCK_MAX; j++) {
            Block[currentBlock][rotation][i][j] = EMPTY;
        }
    }
}

void InitBoard() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            Board[i][j] = EMPTY;
            PlayingBoard[i][j] = PLAY_EMPTY;
        }
    }

    //�� ����
    for (int i = 0; i < HEIGHT; i++) { //���� ��
        Board[0][i] = WALL;
    }

    for (int i = 0; i < HEIGHT; i++) { //������ ��
        Board[WIDTH - 1][i] = WALL;
    }

    for (int i = 0; i < WIDTH; i++) { //�ٴ�
        Board[i][HEIGHT - 1] = WALL;
    }

    GotoXY(BOARD_X, BOARD_Y);
}

void InitPlayingBoard(void) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            PlayingBoard[i][j] = PLAY_EMPTY;
        }
    }
}

void InitInformation(void) {
    TextColor(GREEN);
    GotoXY(4, 2); printf("- - - - - - - - - - - - ");
    GotoXY(4, 3); printf("��T E T R I S  G A M E��");
    GotoXY(4, 4); printf("- - - - - - - - - - - - ");
    TextColor(WHITE);
    GotoXY(34, 5); printf("�� ==Next Block== ��\n");
    GotoXY(34, 7); printf("                   \n");
    GotoXY(34, 9); printf("                   \n");
    GotoXY(34, 11); printf("                   \n");
    GotoXY(34, 13); printf("__________________\n");
    GotoXY(34, 16); printf("Your Score  : %d\n", score);
    GotoXY(34, 17); printf("Best Score  : %d\n", bestScore);
    GotoXY(34, 18); printf("Level  : %d\n", level);
    GotoXY(34, 23); printf("    ��        Shift            SPACE : Hard Drop\n");
    GotoXY(34, 25); printf("��  ��  ��    Left / Right     p / P : Pause\n");
    GotoXY(34, 27); printf("    ��        Soft Drop        ESC : End Game\n");
    printf("\n");
    GotoXY(34, 29); printf("https://github.com/eccho03\n");
}

void RemoveBlockShape(void) {
    int i, j;
    for (i = 0; i < BLOCK_MAX; i++) {
        for (j = 0; j < BLOCK_MAX; j++) {
            if (Block[currentBlock][rotation][i][j] == BLOCK)
                Board[bx + i][by + j] = EMPTY;
        }
    }
}

void FixBlockShape(void) {
    int i, j;
    for (i = 0; i < BLOCK_MAX; i++) {
        for (j = 0; j < BLOCK_MAX; j++) {
            if (Block[currentBlock][rotation][i][j] == BLOCK)
                Board[bx + i][by + j] = FIXED_BLOCK;
        }
    }
}

void ProcessBoard(void) {
    int i, j;
    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            if (Board[i][j] != PlayingBoard[i][j]) { //�ٸ� ���� ī��(�̰� ������ �ӵ� ����, ȭ�� ��¦��)
                GotoXY(BOARD_X + 2 * i, BOARD_Y + j);
                switch (Board[i][j]) {
                case EMPTY:
                    printf("  ");
                    break;
                case WALL:
                    printf("��");
                    break;
                case BLOCK:
                    printf("��");
                    break;
                case FIXED_BLOCK:
                    printf("��");
                    break;
                case EFFECT:
                    printf("��");
                    break;
                }
            }
        }
    }

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            PlayingBoard[i][j] = Board[i][j];
        }
    }
}

void GotoXY(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char show) {
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

void DropBlock(void) {
    if (checkCrush > 0) {//����̳� �ٴڿ� �ε����� ��
        FixBlockShape(); //��� �ٴڿ� ����
        checkCrush = 0; //�÷��׸� �ٽ� �浹 ������ �ʱ�ȭ
        CheckLine(); //�� �� ���� á���� üũ
        if (CheckGameOver()) { //���� �����Ǿ����� üũ
            GameOverScreen();
        }
        GotoXY(34, 16); printf("Your Score  : %d\n", score); //���� ������Ʈ
        checkNewBlock = 1; //�� ��� �÷��� ON
        return;
    }

    if (checkCrush == 0 && CheckCollision(bx, by + 1, rotation)) //�ٴڿ� �� �ε����µ� �� �� �� ������
        MoveBlock(DOWN); //��� ����߸���
    else
        checkCrush++;
}

int CheckCollision(int bx, int by, int rotate) {
    for (int i = 0; i < BLOCK_MAX; i++) {
        for (int j = 0; j < BLOCK_MAX; j++) {
            if (Block[currentBlock][rotate][i][j] == BLOCK && (((Board[bx + i][by + j] == WALL)) || (Board[bx + i][by + j] == FIXED_BLOCK)))
                return FALSE;
        }
    }
    return TRUE;
}

void TextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void CheckLine(void) {
    int BlockAmount; //�� ���� ��� ����
    int i, j, k = 0, l = 0;
    int combo = 0;
    for (j = HEIGHT - 2; j > 3;) {
        BlockAmount = 0; // �� �� ��� ���� ���� ���� �ʱ�ȭ
        for (i = 1; i < WIDTH - 1; i++) {
            if (Board[i][j] == FIXED_BLOCK) {
                BlockAmount++;
                if (BlockAmount == WIDTH - 2) {
                    for (int i = 1; i <= WIDTH - 1; i++) {
                        ProcessBoard();
                        Board[i][j] = EFFECT;
                    }
                }
            }
        }
        if (BlockAmount == WIDTH - 2) {
            Sleep(300);
            combo++;
            for (k = j; k > 1; k--) {
                for (l = 1; l < WIDTH - 1; l++) {
                    if (Board[l][k - 1] != CEILING) {
                        Board[l][k] = Board[l][k - 1];
                    }
                    if (Board[l][k - 1] == CEILING)
                        Board[l][k] = EMPTY;
                }
            }
        }
        else
            j--;
    }

    if (combo > 0) {
        GotoXY(34, 19); printf("--------------");
        GotoXY(34, 20); printf("%d C O M B O !", combo);
        GotoXY(34, 21); printf("--------------");
        Sleep(100);
        GotoXY(34, 19); printf("              ");
        GotoXY(34, 20); printf("              ");
        GotoXY(34, 21); printf("              ");
        Sleep(100);

        GotoXY(34, 19); printf("--------------");
        GotoXY(34, 20); printf("%d C O M B O !", combo);
        GotoXY(34, 21); printf("--------------");
        Sleep(100);
        GotoXY(34, 19); printf("              ");
        GotoXY(34, 20); printf("              ");
        GotoXY(34, 21); printf("              ");
        Sleep(100);

        GotoXY(34, 19); printf("--------------");
        GotoXY(34, 20); printf("%d C O M B O !", combo);
        GotoXY(34, 21); printf("--------------");
        Sleep(100);
        GotoXY(34, 19); printf("              ");
        GotoXY(34, 20); printf("              ");
        GotoXY(34, 21); printf("              ");
        score += 100 * combo; 
        if (score >= 300 && score % 300 == 0) {//score 300�� �̻��̸� 300�� ���� ������
            level++;
            if (level >= 7)
                level = 7; //������ 7�� ������ 7�� ����(�ӵ� ������ �ִ� 7������ ����)
            speed++;
            LevelUpScreen();
        }
    }
}

void PauseGame(void) {
    int i = 3, j = 10, k = 12, l = 12;
    TextColor(YELLOW);
    GotoXY(i, j + 0); printf("������������������������\n");
    GotoXY(i, j + 1); printf("��                     ��\n");
    GotoXY(i, j + 2); printf("��      /(*�ơ��*)/   ��\n");
    GotoXY(i, j + 3); printf("��      P A U S E      ��\n");
    GotoXY(i, j + 4); printf("�� - - - - - - - - - - ��\n");
    GotoXY(i, j + 5); printf("��  Press Any Key      ��\n");
    GotoXY(i, j + 6); printf("��      to resume! ! ! ��\n");
    GotoXY(i, j + 7); printf("������������������������\n");
    _getch();
    system("cls");
    InitPlayingBoard();
    CreateScreen();
    InitInformation();
    Sleep(100);
    TextColor(YELLOW);
    GotoXY(k, l + 0); printf("��- - -��\n");
    GotoXY(k, l + 1); printf("��  3  ��\n");
    GotoXY(k, l + 2); printf("��- - -��\n");
    Sleep(200);
    GotoXY(k, l + 0); printf("         \n");
    GotoXY(k, l + 1); printf("         \n");
    GotoXY(k, l + 2); printf("         \n");
    Sleep(200);

    GotoXY(k, l + 0); printf("��- - -��\n");
    GotoXY(k, l + 1); printf("��  2  ��\n");
    GotoXY(k, l + 2); printf("��- - -��\n");
    Sleep(200);
    GotoXY(k, l + 0); printf("         \n");
    GotoXY(k, l + 1); printf("         \n");
    GotoXY(k, l + 2); printf("         \n");
    Sleep(200);

    GotoXY(k, l + 0); printf("��- - -��\n");
    GotoXY(k, l + 1); printf("��  1  ��\n");
    GotoXY(k, l + 2); printf("��- - -��\n");
    Sleep(200);
    GotoXY(k, l + 0); printf("         \n");
    GotoXY(k, l + 1); printf("         \n");
    GotoXY(k, l + 2); printf("         \n");
    Sleep(200);

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            if (Block[currentBlock][rotation][i][j] == BLOCK) {
                GotoXY(BOARD_X + i, BOARD_Y + j);
                printf("��");
                TextColor(SKY_BLUE);
            }
            else
                printf("  ");
        }
    }
    TextColor(WHITE);
    GotoXY(34, 5); printf("�� ==Next Block== ��\n");

}

int CheckGameOver(void) {
    for (int i = 0; i < WIDTH; i++) { //õ�� ����
        Board[i][3] = CEILING;
    }
    for (int i = 0; i < BLOCK_MAX; i++) {
        for (int j = 0; j < BLOCK_MAX; j++) {
            if (Board[bx + i][by + j] == FIXED_BLOCK && Board[bx + i][by + j + 1] == CEILING) //�ٷ� �� ����� ����, �� ���� õ���̸� ���� ����
                return TRUE;
        }
    }
    return FALSE;
}

void GameOverScreen(void) {
    PlaySound(NULL, 0, 0); //bgm ����
    system("cls");
    GotoXY(7, 0); printf("�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١�\n");
    GotoXY(7, 1); printf("                          \n");
    GotoXY(7, 2); printf("      G A M E   O V E R   \n");
    GotoXY(7, 3); printf("                       \n");
    GotoXY(7, 4); printf("      Score :        %d      \n", score);
    GotoXY(7, 5); printf("                          \n");
    GotoXY(7, 6); printf("�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١�\n");
    if (score > bestScore) { //�ְ��� ����
        FILE* file = fopen("bestScore.txt", "wt");
        fprintf(file, "%d", score);
        GotoXY(7, 5); printf("       �� Best Score ��   \n");
    }   
    Sleep(1000);
    exit(1);
}

void LevelUpScreen(void) {
    TextColor(YELLOW);
    GotoXY(4, 10); 
    if (score == 2400)
        printf("��You Reached MAX Level��\n");
    else if (score < 2400)
        printf("��   L E V E L  U P   ��\n");
    Sleep(500);
    system("cls");
    InitPlayingBoard();
    CreateScreen();
    InitInformation();
}

void NextBlockColor(int blockColor) {
    switch (blockColor) {
    case 0:
        TextColor(BLUE);
        printf("��");
        TextColor(WHITE);
        break;
    case 1:
        TextColor(GREEN);
        printf("��");
        TextColor(WHITE);
        break;
    case 2:
        TextColor(SKY_BLUE);
        printf("��");
        TextColor(WHITE);
        break;
    case 3:
        TextColor(RED);
        printf("��");
        TextColor(WHITE);
        break;
    case 4:
        TextColor(PURPLE);
        printf("��");
        TextColor(WHITE);
        break;
    case 5:
        TextColor(YELLOW);
        printf("��");
        TextColor(WHITE);
        break;
    case 6:
        TextColor(GRAY);
        printf("��");
        TextColor(WHITE);
        break;

    }
}