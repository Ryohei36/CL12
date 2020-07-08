#include "function.h"

// 全てのブロックのメモリを解放
void freeBlocks(struct BLOCK* block)
{
    struct BLOCK* p = NULL;
    while (p != NULL) {
        p = block->next;
        free(block);
        block = p;
    }
}

// ブロック生成
void makeBlock(struct BLOCK** block, double x, double y)
{
    struct BLOCK* ptr, * pb;

    pb = (struct BLOCK*)malloc(sizeof(struct BLOCK));
    if (*block == NULL) {
        *block = pb;
    }
    else {
        ptr = *block;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = pb;
    }

    pb->X = x;
    pb->Y = y;
    pb->life = 1;
    pb->next = NULL;
}

void makeBlocks(struct BLOCK** block)
{
    double x, y ;

    for (y = 2 * LINES / 15; y < 5 * LINES / 15; y += 5) {

        for (x = (double)((WALL_R - 8) % 9) / 2 + 6; x <= (double)(WALL_R - 10); x += 11) {
            makeBlock(block, x, y);
        }
    }
}

// ブロックを表示
void showBlocks(struct BLOCK* block)
{
    while (block != NULL) {
        mvaddstr((int)block->Y, (int)block->X, "%%%%%");
        block = block->next;
    }
}

//ブロックに当たったら壊す
int breakBlock(struct BLOCK** block, double x, double y, double* Dx, double* Dy, int* count)
{
    struct BLOCK* ptr, * tmp;
    ptr = *block;
    tmp = ptr;

    if (ptr == NULL) {
        return 1;
    }

    while (ptr != NULL) {
        if (ptr->Y == y) {
            //もしブロックの真ん中あたりに当たったら
            if ((ptr->X < x) && (x < ptr->X + 4)) {
                *Dy *= -1;
                break;
            }

            //もしブロックの左端にあたったら
            if (ptr->X == x) {
                //左側から来たボールが左端に当たった場合
                if (*Dx == 1) {
                    *Dx *= -1;
                    break;
                    //右側から来たボールが左端に当たった場合
                }
                else {
                    *Dy *= -1;
                    break;
                }
            }

            //もしブロックの右端に当たったら
            if (x == ptr->X + 4) {
                //左側から来たボールが右端に当たった場合
                if (*Dx == 1) {
                    *Dy *= -1;
                    break;
                    //右側から来たボールが左端に当たった場合
                }
                else {
                    *Dx *= -1;
                    break;
                }
            }
        }
        ptr = ptr->next;
    }
    /* while終了 */

    /* 当たっていなければptrはNULLのはず。当たっていたら当たったところでbreakしているはず */
    if (ptr != NULL) {

        if (ptr->life == 1) {
            *count += 1;

            //一番初めのブロックだった場合
            if (ptr == *block) {
                *block = ptr->next;
                mvaddstr((int)ptr->Y,(int)ptr->X, "     ");
                free(ptr);

                //一番初めブロック以外だった場合
            }
            else {
                //ptrの手前まで移動
                while (tmp->next != ptr) tmp = tmp->next;

                mvaddstr((int)ptr->Y, (int)ptr->X, "     ");
                tmp->next = ptr->next;
                free(ptr);
            }
            clear();
        }
    }
    return 0;
}

// ボールとバーの当たり判定(下に落ちた時だけ1を返す)
int CollisionDetection(struct Ball* ball, struct Bar* bar)
{
    //ボールが下に落ちたら1を返す
    if ((*ball).Y > LINES - 1) return 1;

    //左にぶつかったら
    if ((*ball).X < WALL_L + 2) {
        (*ball).X = WALL_L + 2;
        (*ball).Dx = 1;
    }

    //右にぶつかったら
    else if ((*ball).X >= WALL_R - 1) {
        (*ball).X = WALL_R - 2;
        (*ball).Dx = -1;
    }

    //上にぶつかったら
    else if ((*ball).Y < TOP) {
        (*ball).Y = 1;
        (*ball).Dy = 1;
    }

    //バーにボールが当たった時
    else if (((*ball).Y == (*bar).Y) && ((*ball).X >= (*bar).X) && ((*ball).X <= (*bar).X + ((*bar).width - 1))) {
        (*ball).Y = (*bar).Y - 1;
        (*ball).Dy = -1;
    }
    return 0;
}

//バーを動かす
void moveBar(struct Bar* bar, struct Ball* ball, int ch)
{
    mvaddstr((int)(*bar).Y, (int)(*bar).X, (*bar).eraseBar);
    switch (ch) {
    case KEY_LEFT:
        if ((*bar).X <= WALL_L + 3) (*bar).X = WALL_L + 3;
        (*bar).X -= 1;
        break;

    case KEY_RIGHT:
        if ((*bar).X >= WALL_R - ((*bar).width + 1)) (*bar).X = WALL_R - ((*bar).width + 1);
        (*bar).X += 1;
        break;

    case KEY_UP:
        //ボールのスピードアップ
        if ((*ball).waitCount > 500) (*ball).waitCount -= 500;
        break;

    case KEY_DOWN:
        //ボールのスピードダウン
        if ((*ball).waitCount < 9000) (*ball).waitCount += 500;
        break;

    default:
        break;
    }
    mvaddstr((int)(*bar).Y, (int)(*bar).X, (*bar).addBar);
}

//LEVEL別にバーの長さを変更
void Level(struct Bar* bar, int level)
{
    if (level == 1) {
        strcpy_s((*bar).addBar, "========");
        strcpy_s((*bar).eraseBar, "        ");
        (*bar).width = 8;
    }
    else if (level == 2) {
        strcpy_s((*bar).addBar, "=======");
        strcpy_s((*bar).eraseBar, "       ");
        (*bar).width = 7;
    }
    else if (level == 3) {
        strcpy_s((*bar).addBar, "======");
        strcpy_s((*bar).eraseBar, "      ");
        (*bar).width = 6;
    }
    else if (level == 4) {
        strcpy_s((*bar).addBar, "=====");
        strcpy_s((*bar).eraseBar, "     ");
        (*bar).width = 5;
    }
    else {
        strcpy_s((*bar).addBar, "====");
        strcpy_s((*bar).eraseBar, "    ");
        (*bar).width = 4;
    }
}

// レコードを描画
void printRecord(struct Record *rp)
{
    clear();
    mvprintw(LINES / 2 - 3, 2 * COLS / 7, "1st ball survived for  %d[s]", rp->time1);
    mvprintw(LINES / 2 - 2, 2 * COLS / 7, "2st ball survived for  %d[s]", rp->time2);
    mvprintw(LINES / 2 - 1, 2 * COLS / 7, "3st ball survived for  %d[s]", rp->time3);
    mvprintw(LINES / 2 - 3, 4 * COLS / 7, "Summary, You survived for  %d[s]", rp->time);
    mvprintw(LINES / 2, 2 * COLS / 7, "Your 1st Score  %6.1f", rp->score1);
    mvprintw(LINES / 2 + 1, 2 * COLS / 7, "Your 2st Score  %6.1f", rp->score2);
    mvprintw(LINES / 2 + 2, 2 * COLS / 7, "Your 3st Score  %6.1f", rp->score3);
    mvprintw(LINES / 2, 4 * COLS / 7, "Summary, Your total Score  %6.1f", rp->score);
    refresh();
    Sleep(5);
}

// Scoreを表示
void printScore(int timer, int level, double score, int life)
{
    mvprintw(5, WALL_R + 1, "Time %d[s]", timer);
    mvprintw(6, WALL_R + 1, "Level %d", level);
    mvprintw(7, WALL_R + 1, "SCORE %5.1f", score);
    mvprintw(8, WALL_R + 1, "Ball %d", life);
}

// GAMEOVERを表示
void printGameOver(struct Record *rp)
{   
    /* 端末の幅によって表示方法変更 */
    if (COLS > 150) {
        mvprintw(LINES / 2 - 4, (COLS - 70) / 2, " GGGG    AAA   M     M  EEEEE        OOOO   V     V  EEEEE  RRRR       ");
        mvprintw(LINES / 2 - 3, (COLS - 70) / 2, "G    G  A   A  MM   MM  E           O    O  V     V  E      R   R      ");
        mvprintw(LINES / 2 - 2, (COLS - 70) / 2, "G       A   A  M M M M  E           O    O  V     V  E      R    R     ");
        mvprintw(LINES / 2 - 1, (COLS - 70) / 2, "G       AAAAA  M  M  M  EEEEE       O    O  V     V  EEEEE  RRRRR      ");
        mvprintw(LINES / 2, (COLS - 70) / 2, "G       A   A  M     M  E           O    O  V     V  E      R  R       ");
        mvprintw(LINES / 2 + 1, (COLS - 70) / 2, "G   GGG A   A  M     M  E           O    O   V   V   E      R   R      ");
        mvprintw(LINES / 2 + 2, (COLS - 70) / 2, "G    G  A   A  M     M  E           O    O    V V    E      R    R     ");
        mvprintw(LINES / 2 + 3, (COLS - 70) / 2, " GGGG   A   A  M     M  EEEEE        OOOO      V     EEEEE  R     R    ");
    }
    else {
        mvprintw(LINES / 2, (WALL_R - 10) / 2, "GAME OVER");
    }
    refresh();
    Sleep(50);
    clear();

    /* レコード表示 */
    printRecord(rp);
    /* THANK YOU FOR PLAYING */
    clear();
    printRecord(rp);
   /* mvprintw(LINES / 2, (COLS - 20) / 2, "Thank you for playing");
    mvprintw(LINES / 2 + 2, (COLS - 10) / 2, "[r]: restart");
    mvprintw(LINES / 2 + 4, (COLS - 10) / 2, "[q]: quit");*/
}

// BreakOutを表示
void printGameName()
{
    clear();
    mvprintw(LINES / 2 - 4, (COLS - 72) / 2, "BBBBBB                               D");
    mvprintw(LINES / 2 - 3, (COLS - 72) / 2, "B    B                               D");
    mvprintw(LINES / 2 - 2, (COLS - 72) / 2, "B    B                               D");
    mvprintw(LINES / 2 - 1, (COLS - 72) / 2, "BBBBB    OOO   U    U  N NNNN    DDDDD");
    mvprintw(LINES / 2, (COLS - 72) / 2, "B  B    O   O  U    U  NN    N  D    D");
    mvprintw(LINES / 2 + 1, (COLS - 72) / 2, "B   B   O   O  U    U  N     N  D    D");
    mvprintw(LINES / 2 + 2, (COLS - 72) / 2, "B    B  O   O  U    U  N     N  D    D");
    mvprintw(LINES / 2 + 3, (COLS - 72) / 2, "BBBBBB   OOO    UUUU   N     N   DDDDD");
}

// 両側の壁を生成
void printWall()
{
    int i = 0;
    //枠の作成
    for (i = 0; i < LINES; i++) {
        mvaddch(i, WALL_L, ' ');
        mvaddch(i, WALL_R, ' ');
        mvaddch(i, WALL_L, '|');
        mvaddch(i, WALL_R, '|');
    }
}

//名前の読み込み，表示する
void printName() {

    //iniファイル読み込み
    char currentDirectory[BUFFSIZE];
    getCurrentDirectory(currentDirectory);
    char section[BUFFSIZE];
    char keyWord[BUFFSIZE];
    char settingFile[BUFFSIZE];


    //名前の読み込み
    sprintf_s(section, "default");
    sprintf_s(keyWord, "name1");
    sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);
    char inName1[BUFFSIZE];
    readChar(section, keyWord, "none", inName1, settingFile);

    sprintf_s(section, "default");
    sprintf_s(keyWord, "name2");
    sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);
    char inName2[BUFFSIZE];
    readChar(section, keyWord, "none", inName2, settingFile);

    sprintf_s(section, "default");
    sprintf_s(keyWord, "name3");
    sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);
    char inName3[BUFFSIZE];
    readChar(section, keyWord, "none", inName3, settingFile);

    clear();
    mvaddstr(LINES / 2 - 1, (COLS - 10) / 2, "Choosed Name");

    mvaddstr(LINES / 2 + 1, (COLS - 10) / 2,inName1);
    mvaddstr(LINES / 2 + 2, (COLS - 10) / 2,inName2);
    mvaddstr(LINES / 2 + 3, (COLS - 10) / 2,inName3);

}
int selectName(int i,int ch) {

    mvaddstr(LINES / 2 + 1, (COLS - 10) / 2 - 1, "*");
    switch (ch) {
    case KEY_UP:
        mvaddstr(LINES / 2 + 1 + i, (COLS - 10) / 2 - 1, " ");
        if (i <= 0) {

        }
        else i--;
        mvaddstr(LINES / 2 + 1 + i, (COLS - 10) / 2 - 1, "*");
    case KEY_DOWN:
        mvaddstr(LINES / 2 + 1 + i, (COLS - 10) / 2 - 1, " ");
        if (i > 3) {

        }
        else i++;
        mvaddstr(LINES / 2 + 1 + i, (COLS - 10) / 2 - 1, "*");

    }
    return i;
  
}