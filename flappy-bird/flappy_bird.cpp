#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>

using namespace std;

static const int N = 40;
static const int M = 100; //��Ϸ�����С
//static const int FPS = 30;                           //֡��
static const int PILLAR_NUM = 8;                       //���ӵ�����
static const int WIDTH = 10;                           //���ӵĿ��
static const int HEIGHT = 10;                          //ȱ�ڵĸ߶�
static const float PILLAR_SPEED = 10;                  //���������ƶ����ٶ�, ��λ ��/��
static const float BIRD_SPEED = -20;                   //�������ٶ�
static const float ACCELERATION = 30;                  //��ļ��ٶ�
static const int SPACE = 15;                           //���ӵļ��
static const float PILLAR_ORIGIN_XY = 50;              //Ĭ����������
static const pair<float, float> BIRD_ORIGIN_XY(1, 10); //Ĭ��������
static char image[N + 1][M + 1];                       //��Ϸ����

inline void printChar(char *ch, UINT count, UINT x, UINT y)
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

void setCursor(bool isVisible) //false���ع��
{
    HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cinfo;
    cinfo.bVisible = isVisible;
    cinfo.dwSize = 1;
    SetConsoleCursorInfo(fd, &cinfo);
}


class Bird
{
private:
    pair<float, float> XY;
    float speed; //����ٶ�, ��Ϊ������
    float a;     //��ļ��ٶ�

public:
    Bird(pair<float, float> co) : XY(co), speed(0), a(ACCELERATION){};

    void move(float dt)
    {
        XY.first += dt * speed;
        speed += dt * a;
    }

    void setSpeed(float x)
    {
        speed = x;
    }

    friend class Game;
};

class Pillar
{
private:
    deque<pair<float, int>> pillars; //���ӵ�����(��)��ȱ�ڵ�����(��)
    float speed;                     //�����ƶ����ٶ�, ��Ϊ������

public:
    Pillar() : speed(PILLAR_SPEED)
    {
        for (int i = 0; i < PILLAR_NUM; i++)
        {
            pillars.push_back(make_pair(PILLAR_ORIGIN_XY + i * (WIDTH + SPACE), rand() % (N - HEIGHT) + 1));
        }
    }

    void renew()
    {
        if (pillars.front().first + WIDTH < 0) //��������
        {
            pillars.pop_front();
            pillars.push_back(make_pair(pillars.back().first + WIDTH + SPACE, rand() % (N - HEIGHT) + 1));
        }
    }

    void move(float dt)
    {
        for (auto iter = pillars.begin(); iter < pillars.end(); iter++)
        {
            iter->first -= speed * dt;
        }
    }

    friend class Game;
};

class Game
{
private:
    Bird myBird;
    Pillar myPillar;
    time_t lastTime; //�ϴ�ִ�г����ʱ��

public:
    Game() : myBird(BIRD_ORIGIN_XY), lastTime(clock()){};

    void renew()
    {
        float dt = (float)(clock() - lastTime) / CLOCKS_PER_SEC; //�仯ʱ��
        lastTime = clock();
        myBird.move(dt);
        myPillar.move(dt);
        myPillar.renew();
    }

    bool isBirdAlive()
    {
        for (auto iter = myPillar.pillars.begin(); iter < myPillar.pillars.end(); iter++)
        {
            if (myBird.XY.second > iter->first && myBird.XY.second < iter->first + WIDTH)
            {
                if (myBird.XY.first < iter->second || myBird.XY.first > iter->second + HEIGHT)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void draw()
    {
        //system("cls");
        char tmp[N + 1][M + 1];
        memset(tmp, 0, sizeof(tmp));
        for (int i = 0; i <= N; i++)
        {
            tmp[i][0] = tmp[i][M] = '+';
        }
        for (int j = 0; j <= M; j++)
        {
            tmp[0][j] = tmp[N][j] = '+';
        }
        for (auto iter = myPillar.pillars.begin(); iter < myPillar.pillars.end(); iter++)
        {
            for (int i = 1; i < N; i++)
            {
                for (int j = iter->first; j < iter->first + WIDTH; j++)
                {
                    if (j <= 0 || j >= M)
                    {
                        continue;
                    }
                    if (i >= iter->second && i < iter->second + HEIGHT)
                    {
                        continue;
                    }
                    tmp[i][j] = '#';
                }
            }
        }
        if ((int)(myBird.XY.first + 0.5) > 0 && (int)(myBird.XY.first + 0.5) < N && (int)(myBird.XY.second + 0.5) > 0 && (int)(myBird.XY.second + 0.5) < M)
        {
            tmp[(int)(myBird.XY.first + 0.5)][(int)(myBird.XY.second + 0.5)] = '@';
        }
        for (int i = 0; i <= N; i++)
        {
            for (int j = 0; j <= M; j++)
            {
                if (tmp[i][j] == 0)
                {
                    tmp[i][j] = ' ';
                }
                if (tmp[i][j] != image[i][j])
                {
                    printChar(*(tmp + i) + j, 1, i, j);
                    image[i][j] = tmp[i][j];
                }
            }
        }
    }

    void changeSpeed()
    {
        myBird.setSpeed(BIRD_SPEED);
    }
};

int main()
{
    srand((unsigned)time(NULL));
    while (true)
    {
        system("cls");
        string inputStr;
        cout << "���� start ��ʼ��Ϸ, ���� exit �˳�" << endl;
        cin >> inputStr;
        if (inputStr == "start")
        {
            memset(image, 0, sizeof(image));
            system("cls");
            Game myGame;
            char key;
            while (true)
            {
                //Sleep(1000/FPS);
                setCursor(false);
                if (kbhit())
                {
                    while (kbhit())
                    {
                        key = getch();
                    }
                    myGame.changeSpeed();
                }
                myGame.renew(); //������Ϸ״̬
                myGame.draw();  //������Ϸ����
                if (!myGame.isBirdAlive())
                {
                    setCursor(true);
                    for (int i = 0; i <= N; i++)
                    {
                        printf("\n");
                    }
                    printf("��������!");
                    system("pause");
                    break;
                }
            }
        }
        else if (inputStr == "exit")
        {
            break;
        }
    }
    return 0;
}
