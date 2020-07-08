#include "main.h"

void run();

int main(int argc, char** argv)
{
    initscr();           //curses
    noecho();            //curses
    cbreak();            //curses
    keypad(stdscr, TRUE); //curses
    run();               //���s
    endwin();            //curses
    return 0;
}

void run()
{
    struct Bar bar;                //�o�[�̈ʒu���
    struct Ball ball;              //�{�[���̈ʒu���
    struct Record rec;             //����̃Q�[�����L�^�ƈꎞ�I�ȕێ��L�^
    struct BLOCK* block = NULL;    //�u���b�N

    int ch;                //�L�[�{�[�h���͗p
    int i = 0;
    int count = 0;         //�u���b�N���ǂꂾ���j�󂳂ꂽ��
    int loop = 0;          //����ɂ���ă{�[���̎��Ԃ𐧌�
    time_t t1, t2, t3;       //����
    int life = 3;          //�����ʂ�LIFE
    double score = 0;          //�X�R�A my�@0 �ŏ���������
    double level_score = 0;  //�ꎞ�I�ȃX�R�A���L�^
    double tmp_score = 0;

    t1, t2, t3 = time(NULL);

    // �L�^�̐ݒ�
    rec.level = 1;         //���x��

    // �o�[�̐ݒ�
    bar.Y = LINES - 5;     //�o�[�̏����ʒu
    bar.X = WALL_R / 2;
    bar.width = 7;         //�o�[�̒����̏����ݒ�
    bar.shoot = 0;         //�{�[���𔭎˂��邩(1) or ���Ȃ���(0)
    strcpy_s(bar.addBar, "=======");
    strcpy_s(bar.eraseBar, "       ");

    // �{�[���̐ݒ�
    ball.X = WALL_R / 2;   //�{�[���̏����ʒu
    ball.Y = LINES - 5;
    ball.Dx = 1;           //�{�[���̕����x�N�g���̏����ݒ�
    ball.Dy = -1;          //�΂߉E��45�x�ɐi��
    ball.waitCount = 3000; //b�|�[���̑���

    timeout(0);


INTRO:
    // �Q�[���̖��O��\��
    printGameName();
    mvprintw(LINES / 2 + 5, (COLS - 10) / 2, "[s]: start");
    mvprintw(LINES / 2 + 7, (COLS - 10) / 2, "[q]: quit");

    /* �L�[���́@[s]���������疼�O�I���Ɉڍs�@[q]����������I��*/
    while (1) {
        ch = getch();
        if (ch == 's') {
            clear();
            goto SELECT;
        }
        else if (ch == 'q') return;
    }
SELECT:
    //���[�U�[�̖��O��I��
    printName();
    //�Z���N�g�̃|�C���^
    mvaddstr(LINES / 2 + 1, (COLS - 10) / 2 - 1, "*");

    int n = 0;
    /* �L�[���� [s]����������X�^�[�g�@[q]����������I��*/
    while (1) {
        ch = getch();
        n = selectName(n,ch);
        if (ch == 's') {
            clear();
            goto START;
        }
        else if (ch == 'q') return;
    }

START:
    clear();
    makeBlocks(&block);   // �u���b�N����

INIT:
    while ((ch = getch()) != 'q') {
        showBlocks(block);
        mvaddch((int)ball.Y, (int)ball.X, ' ');
        //shoot��0�̎��̓o�[�ƈꏏ�Ɉړ�
        if (bar.shoot == 0) {
            ball.X = bar.X + 1;
            ball.Y = bar.Y - 1;
            //�X�y�[�X�����͂��ꂽ�甭��(shoot=1)
            if (ch == ' ') bar.shoot = 1;
        }
        else if (bar.shoot == 1) {
            t1 = time(NULL);
            goto GAME;
        }
        printWall();
        printScore(0, rec.level, score, life);
        moveBar(&bar, &ball, ch);
        mvaddch((int)ball.Y, (int)ball.X, '@');
    }
    goto END;

GAME:
    while ((ch = getch()) != 'q') {
        //�{�[���̎��Ԃ�i�߂�B
        loop++;
        showBlocks(block);
        mvaddch((int)ball.Y, (int)ball.X, ' ');
        printWall();

        moveBar(&bar, &ball, ch);

        t2 = time(NULL);
        score = tmp_score + (count * 1000) * (rec.level / 2.0) + (100000.0 / ball.waitCount) * (int)(t2 - t1);


        // score�������𖞂������烌�x���A�b�v
        if ((score - level_score) > (8000 * rec.level)) {
            tmp_score = score;
            rec.level++;
            count = 0;
            mvprintw(LINES / 2, WALL_R / 2, "LEVEL UP!!");
            t3 = time(NULL);
            if (ball.waitCount > 500) ball.waitCount -= 500;
            level_score = score;
            Level(&bar, rec.level);
        }

        // 3�b�o������ALEVEL UP�������B
        if ((int)(t2 - t3) == 3) mvprintw(LINES / 2, WALL_R / 2, "          ");
        // �E���ɃX�R�A�\��
        printScore((int)(t2 - t1), rec.level, score, life);

        if (loop % ball.waitCount == 0) {
            ball.X += ball.Dx;
            ball.Y += ball.Dy;

            if (CollisionDetection(&ball, &bar) == 1) {
                //�{�[�����o�[�̈ʒu�ɁB�����Ȃǂ������l�ɖ߂���life�����炷�B
                bar.shoot = 0;
                ball.X = bar.X;        //�{�[���̈ʒu��������
                ball.Y = bar.Y;
                ball.Dx = 1;           //�{�[���̑��x�x�N�g����������
                ball.Dy = -1;
                loop = 0;             //���Ԃ�������
                life--;                //���C�t�����炷

                if (life == 2) {
                    rec.time1 = (int)(t2 - t1);
                    rec.score1 = score;
                    tmp_score = score;
                    count = 0;
                    goto INIT;
                }
                else if (life == 1) {
                    rec.time2 = (int)(t2 - t1);
                    rec.score2 = score - rec.score1;
                    tmp_score = score;
                    count = 0;
                    goto INIT;
                }
                else if (life == 0) {
                    rec.time3 = (int)(t2 - t1);
                    rec.score3 = score - rec.score2 - rec.score1;
                    rec.time = rec.time1 + rec.time2 + rec.time3;
                    rec.score = score;
                    /* �Q�[���I�[�o�[������3�b�ҋ@���ċL�^��\�� */
                    printGameOver(&rec);
                    goto RESTART;
                }
            }
        }

        //��ꂽ�u���b�N���Ȃ����̔���B�����Ă������
        if (breakBlock(&block, ball.X, ball.Y, &ball.Dx, &ball.Dy, &count) == 1) {
            clear();
            mvprintw(LINES / 2, COLS / 2, "GAME CLEAR");
            Sleep(5);
            return;
        }

        mvaddch((int)ball.Y, (int)ball.X, '@' | COLOR_PAIR(2));
    }/* ������while�����Ă��� */
    goto END;


RESTART:
    /* �f�[�^�̏����� */
    life = 3;
    tmp_score = 0;
    count = 0;
    ball.waitCount = 3000;
    score = 0;
    rec.level = 1;
    freeBlocks(block);  //�O��̃u���b�N�����ׂĊJ�����Ă���B
    block = NULL;
    bar.X = WALL_R / 2;
    bar.Y = LINES - 5;
    while (1) {
        ch = getch();
        if (ch == 'r') goto START;
        if (ch == 'q') goto END;
    }

END:
    clear();
    return;
}
