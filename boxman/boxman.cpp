#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

static const set<string> mapName = {"1", "2"}; //合法的地图名称

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

class Game
{
private:
    vector<vector<char>> map;   //存储原始地图, 不包含人和箱子
    vector<vector<char>> image; //上一帧的游戏画面
    pair<int, int> manXY;       //人的坐标
    set<pair<int, int>> box;    //箱子坐标
    set<pair<int, int>> target; //目标坐标
    int n, m;                   //地图的大小
    int step;
    const int shiftX[4] = {0, -1, 0, 1};
    const int shiftY[4] = {-1, 0, 1, 0};

public:
    Game() : step(0){};

    void initMap(string input) //初始化地图
    {
        input = "maps/" + input + ".txt";
        FILE *fp = fopen(input.c_str(), "r");
        char c;
        fscanf(fp, "%d %d", &n, &m);
        for (int i = 0; i < n; i++)
        {
            map.push_back({});
            image.push_back({});
            cout << endl;
            for (int j = 0; j < m; j++)
            {
                do
                {
                    fscanf(fp, "%c", &c);
                } while (c == '\n');
                map[i].push_back(c);
                image[i].push_back(' ');
                if (map[i][j] == '@')
                {
                    manXY = make_pair(i, j);
                    map[i][j] = ' ';
                }
                else if (map[i][j] == '#')
                {
                    box.insert(make_pair(i, j));
                    map[i][j] = ' ';
                }
                else if (map[i][j] == '.')
                {
                    target.insert(make_pair(i, j));
                }
            }
        }
        fclose(fp);
    }

    bool move(char c)
    {
        if (c == VK_ESCAPE)
        {
            return false;
        }
        int dir = -1;
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
        if (dir == -1)
        {
            return true;
        }

        pair<int, int> nowXY(manXY.first + shiftX[dir], manXY.second + shiftY[dir]);
        if (image[nowXY.first][nowXY.second] == ' ' || image[nowXY.first][nowXY.second] == '.')
        {
            manXY = nowXY;
            step++;
        }
        else if (image[nowXY.first][nowXY.second] == '#')
        {
            pair<int, int> nowBoxXY(nowXY.first + shiftX[dir], nowXY.second + shiftY[dir]);
            if (image[nowBoxXY.first][nowBoxXY.second] == ' ' || image[nowBoxXY.first][nowBoxXY.second] == '.')
            {
                manXY = nowXY;
                box.erase(nowXY);
                box.insert(nowBoxXY);
                step++;
            }
        }
        return true;
    }

    void draw()
    {
        vector<vector<char>> tmp = map;
        auto iter = box.begin();
        while (iter != box.end())
        {
            tmp[iter->first][iter->second] = '#';
            iter++;
        }
        tmp[manXY.first][manXY.second] = '@';
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (tmp[i][j] != image[i][j])
                {
                    printChar(&tmp[i][j], 1, i, j);
                }
            }
        }
        image = tmp;
    }

    bool check()
    {
        if (box == target)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int getStep()
    {
        return step;
    }
};

int main()
{
    while (true)
    {
        system("cls");
        string input;
        cout << "请输入以下地图的名称进入关卡, 输入exit结束" << endl;
        auto iter = mapName.begin();
        while (iter != mapName.end())
        {
            cout << *iter << endl;
            iter++;
        }
        cout << endl << "您的选择为: ";
        cin >> input;
        if (input == "exit")
        {
            break;
        }
        else
        {
            if (mapName.find(input) == mapName.end())
            {
                cout << "您选择的关卡不存在! 请重新输入!" << endl;
                system("pause");
                continue;
            }
            else
            {
                system("cls");
                setCursor(false);
                Game myGame;
                char key;
                myGame.initMap(input);
                while (true)
                {
                    myGame.draw();
                    fflush(stdin);
                    key = getch();
                    if (key == 0 || key == 224)
                    {
                        key = getch();
                    }
                    if (!myGame.move(key)) //用户退出
                    {
                        setCursor(true);
                        cout << "您已结束游戏!" << endl;
                        system("pause");
                        break;
                    }
                    if (myGame.check())
                    {
                        myGame.draw();
                        setCursor(true);
                        cout << "您已成功! 步数: " << myGame.getStep() << endl;
                        system("pause");
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
