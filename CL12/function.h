#pragma once
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <Windows.h>//Sleep
#include "stdafx.h"
#include "Setting.h"


#define BUFFSIZE 1024
#define WALL_R (COLS - 20) //�E���̕�
#define WALL_L 2           //�����̕�
#define TOP 0              //��̕�
#define UNDER (LINES - 5)  //���̕�

// �u���b�N�̍\����
struct BLOCK {
    double X;
    double Y;      //�u���b�N�̍��W
    int life;      //���񓖂���Ή��邩
    struct BLOCK* next;  //���̃u���b�N�ւ̃|�C���^
};

// �{�[���̍\����
struct Ball {
    double X;
    double Y;      // ���W
    double Dx;
    double Dy;     // ���ꂼ��̑��x�x�N�g��
    int waitCount; // ���x
};

// �o�[�̍\����
struct Bar {
    double X;     // �o�[�̈ʒu
    double Y;
    int width;    // �o�[�̒���
    int shoot;    // �{�[���𔭎˂��邩(1)���Ȃ���(0)
    char addBar[12];   // �o�[�̕`�敔��
    char eraseBar[12]; // �o�[�̏�������
};

// ���R�[�h�̍\����
struct Record {
    int level;      //���x��
    double score;   //���X�R�A
    double score1;  //�{�[����ڂ̃X�R�A
    double score2;  //�{�[����ڂ̃X�R�A
    double score3;  //�{�[���O�ڂ̃X�R�A
    int time;       //���v�����^�C��
    int time1;      //�{�[����ڂ̐����^�C��
    int time2;      //�{�[����ڂ̐����^�C��
    int time3;      //�{�[���O�ڂ̐����^�C��
};

void freeBlocks(struct BLOCK* block);
void makeBlock(struct BLOCK** block, double x, double y);
void makeBlocks(struct BLOCK** block);
void showBlocks(struct BLOCK* block);
int breakBlock(struct BLOCK** block, double x, double y, double* Dx, double* Dy, int* count);
int CollisionDetection(struct Ball* ball, struct Bar* bar);
void moveBar(struct Bar* bar, struct Ball* ball, int ch);
void Level(struct Bar* bar, int level);
void printRecord(struct Record *rec);
void printScore(int time, int level, double score, int life);
void printGameOver(struct Record *rec);
void printGameName();
void printWall();
void printName();
int selectName(int i,int ch);

