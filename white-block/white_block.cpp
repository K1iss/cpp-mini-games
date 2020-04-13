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

static const int row = 5; //块显示的行数
static const int col = 5; //可点击块的个数
static const int fps = 50; //帧率

class Block
{
private:
    deque<int> q; //存储块
    int point;
    clock_t start;

    double getTime() //获取游戏运行的时间
    {
        return (double)(clock() - start) / CLOCKS_PER_SEC;
    }

public:
    Block()
    {
        for (int i = 0; i < row; i++)
        {
            q.push_back(rand() % col); //放入n个块
        }
        point = 0;
        start = clock();
    }

    void draw() //画出游戏画面
    {
        system("cls");
        printf("时间: %.2lf\t分数: %d\n", getTime(), point);
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
    while (true) //主循环
    {
        system("cls");
        printf("输入1开始, 输入0结束\n");
        string input;
        cin >> input;
        if (input == "0")
        {
            break;
        }
        else if (input == "1") //进入游戏循环
        {
            Block myBlock;
            Timer t;
            t.start(1000/fps, bind(&Block::draw, &myBlock));
            char key = '0'; //记录按键
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
                    printf("游戏结束! 您的最终分数为: %d\n", myBlock.getPoint());
                    system("pause");
                    break;
                }
            }
        }
    }
    return 0;
}
