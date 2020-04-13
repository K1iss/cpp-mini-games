#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

static const int defaultLen = 4;            //����Ĭ�ϳ���
static const pair<int, int> originXY(1, 1); //Ĭ�Ͽ�ʼ����
static const int originDir = VK_RIGHT;      //Ĭ��ǰ������
static const int n = 10;                    //��ͼ��С, ��
static const int m = 20;                    //��ͼ��С, ��
static const int interval = 400;            //ִ��ʱ����

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

void setCursor(bool isVisible) //���ع��
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
    int dir; //ǰ������
    int point;
    pair<int, int> food; //ʳ������
    deque<pair<int, int>> body;
    const int shiftX[4] = {0, -1, 0, 1}; //��������
    const int shiftY[4] = {-1, 0, 1, 0};

    bool isFoodInSnake() //�ж�ʳ���Ƿ����ߵ�������
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
        dir = originDir - VK_LEFT; //��������
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
        int lastDir = dir; //��¼��һ�εķ���
        switch (c)
        {
        case 75:
            dir = 0; //��
            break;
        case 72:
            dir = 1; //��
            break;
        case 77:
            dir = 2; //��
            break;
        case 80:
            dir = 3; //��
            break;
        }
        dir = dir;
        if ((dir + lastDir) % 2 == 0)
        {
            dir = lastDir; //��ֹ�û��󴥵�������, ������Ϸ����
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
        //Ӧ����ȥβ�ټ�ͷ, ��Ȼͷ��������һ��βʱ���bug
        printChar("@", 1, body.front().first, body.front().second);
        
    }

    bool isValid() //������Ƿ�Ϸ�
    {
        pair<int, int> head = body.front();
        if (head.first <= 0 || head.first >= n || head.second <= 0 || head.second >= m) //����
        {
            return false;
        }
        for (auto iter = body.begin() + 1; iter < body.end(); iter++)
        {
            if (*iter == head) //�߳Ե��Լ�
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
        printf("������1��ʼ��Ϸ, ����0����\n");
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
                    while (kbhit()) //��ȡ�û����������ַ�
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
                    printf("��������! ���� %d\n", mySnake.getPoint());
                    system("pause");
                    break;
                }
            }
        }
    }
    return 0;
}
