#include <iostream>
#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <deque>
#include "Timer.hpp"

using namespace std;

typedef deque<int>::iterator iter;

static const int row = 5; //����ʾ������
static const int col = 5; //�ɵ����ĸ���
static const int fps = 50; //֡��

class Block
{
private:
    deque<int> q; //�洢��
    int point;
    clock_t start;

    double getTime() //��ȡ��Ϸ���е�ʱ��
    {
        return (double)(clock() - start) / CLOCKS_PER_SEC;
    }

public:
    Block()
    {
        for (int i = 0; i < row; i++)
        {
            q.push_back(rand() % col); //����n����
        }
        point = 0;
        start = clock();
    }

    void draw() //������Ϸ����
    {
        system("cls");
        printf("ʱ��: %.2lf\t����: %d\n", getTime(), point);
        for (iter p = q.end() - 1; p >= q.begin(); p--)
        {
            for (int i = 0; i < *p; i++)
            {
                printf(" ");
            }
            printf("#\n");
        }
        for (int i = 1; i <= col; i++)
        {
            printf("%c", (char)(i + '0'));
        }
        printf("\n");
    }

    int getPoint()
    {
        return point;
    }

    bool check(char c)
    {
        if (c - '1' == q.front())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void renew()
    {
        q.pop_front();
        q.push_back(rand() % col);
        point++;
    }
};

int main()
{
    srand((unsigned)time(NULL));
    while (true) //��ѭ��
    {
        system("cls");
        printf("����1��ʼ, ����0����\n");
        string input;
        cin >> input;
        if (input == "0")
        {
            break;
        }
        else if (input == "1") //������Ϸѭ��
        {
            Block myBlock;
            Timer t;
            t.start(1000/fps, bind(&Block::draw, &myBlock));
            char key = '0'; //��¼����
            while (true)
            {
                fflush(stdin);
                key = getch();
                if (myBlock.check(key))
                {
                    myBlock.renew();
                }
                else
                {
                    t.stop();
                    printf("��Ϸ����! �������շ���Ϊ: %d\n", myBlock.getPoint());
                    system("pause");
                    break;
                }
            }
        }
    }
    return 0;
}
