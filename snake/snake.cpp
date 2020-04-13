#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

static const int defaultLen = 4;            //蛇身默认长度
static const pair<int, int> originXY(1, 1); //默认开始坐标
static const int originDir = VK_RIGHT;      //默认前进方向
static const int n = 10;                    //地图大小, 行
static const int m = 20;                    //地图大小, 列
static const int interval = 400;            //执行时间间隔

void printChar(char *ch, UINT count, UINT x, UINT y)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    ULONG unuse;
    pos.X = y;
    pos.Y = x;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(h, &bInfo);
    WriteConsoleOutputCharacterA(h, ch, count, pos, &unuse);
}

void setCursor(bool isVisible) //隐藏光标
{
    HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cinfo;
    cinfo.bVisible = isVisible;
    cinfo.dwSize = 1;
    SetConsoleCursorInfo(fd, &cinfo);
}

class Snake
{
private:
    int dir; //前进方向
    int point;
    pair<int, int> food; //食物坐标
    deque<pair<int, int>> body;
    const int shiftX[4] = {0, -1, 0, 1}; //左上右下
    const int shiftY[4] = {-1, 0, 1, 0};

    bool isFoodInSnake() //判断食物是否在蛇的身体内
    {
        for (auto iter = body.begin(); iter < body.end(); iter++)
        {
            if (*iter == food)
            {
                return true;
            }
        }
        return false;
    }

    void setFood()
    {
        do
        {
            int tmpx = rand() % (n - 1) + 1;
            int tmpy = rand() % (m - 1) + 1;
            food = make_pair(tmpx, tmpy);
        } while (isFoodInSnake());
        printChar("#", 1, food.first, food.second);
    }

public:
    Snake()
    {
        dir = originDir - VK_LEFT; //方向向右
        point = 0;
        for (int i = 0; i < defaultLen; i++)
        {
            body.push_front(make_pair(originXY.first + shiftX[dir] * i,
                                      originXY.second + shiftY[dir] * i));
            printChar("@", 1, body.front().first, body.front().second);
        }
        for (int i = 0; i <= n; i++)
        {
            printChar("+", 1, i, 0);
            printChar("+", 1, i, m);
        }
        for (int j = 0; j <= m; j++)
        {
            printChar("+", 1, 0, j);
            printChar("+", 1, n, j);
        }
        setFood();
    }

    void changeDir(char c)
    {
        int lastDir = dir; //记录上一次的方向
        switch (c)
        {
        case 75:
            dir = 0; //左
            break;
        case 72:
            dir = 1; //上
            break;
        case 77:
            dir = 2; //右
            break;
        case 80:
            dir = 3; //下
            break;
        }
        dir = dir;
        if ((dir + lastDir) % 2 == 0)
        {
            dir = lastDir; //防止用户误触导致死亡, 提升游戏体验
        }
    }

    int getPoint()
    {
        return point;
    }

    void move()
    {
        pair<int, int> head = body.front();
        body.push_front(make_pair(head.first + shiftX[dir],
                                  head.second + shiftY[dir]));
        if (body.front() == food)
        {
            point++;
            setFood();
        }
        else
        {
            printChar(" ", 1, body.back().first, body.back().second);
            body.pop_back();
        }
        //应该先去尾再加头, 不然头所在是上一个尾时会出bug
        printChar("@", 1, body.front().first, body.front().second);
        
    }

    bool isValid() //检测蛇是否合法
    {
        pair<int, int> head = body.front();
        if (head.first <= 0 || head.first >= n || head.second <= 0 || head.second >= m) //碰壁
        {
            return false;
        }
        for (auto iter = body.begin() + 1; iter < body.end(); iter++)
        {
            if (*iter == head) //蛇吃到自己
            {
                return false;
            }
        }
        return true;
    }
};

int main()
{
    srand((unsigned)time(NULL));
    while (true)
    {
        system("cls");
        string inputStr;
        printf("请输入1开始游戏, 输入0结束\n");
        cin >> inputStr;
        if (inputStr == "0")
        {
            break;
        }
        else if (inputStr == "1")
        {
            system("cls");
            setCursor(false);
            Snake mySnake;
            char key;
            while (true)
            {
                Sleep(interval);
                if (kbhit())
                {
                    while (kbhit()) //获取用户最后输入的字符
                    {
                        key = getch();
                    }
                    mySnake.changeDir(key);
                }
                mySnake.move();
                if (!mySnake.isValid())
                {
                    for (int i = 0; i <= n; i++)
                    {
                        printf("\n");
                    }
                    setCursor(true);
                    printf("您已死亡! 分数 %d\n", mySnake.getPoint());
                    system("pause");
                    break;
                }
            }
        }
    }
    return 0;
}
