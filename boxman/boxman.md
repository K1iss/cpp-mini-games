# 推箱子

本来以为会比较复杂，但是其实推箱子的逻辑很简单，只有人移动的判定，和胜利的判定。我判定胜利的方法是利用两个`set`容器分别存储当前各个箱子的坐标和目标箱子的坐标，然后比较两个set容器是否相等。显然，在游戏过程当中，箱子的坐标会不断变化，如果仅仅用数组来存储，很可能和目标箱子的顺序不同，因此我采用了具有顺序的`set`容器。

关于地图，我是用`txt`文件来存储的，在`maps`文件夹里，开头两个数字表示行和列，`@ #`分别表示人、箱子的初始位置，`.`表示箱子的目标位置，行末最好不要有多余的空格。如果要添加地图，除了在`maps`里放入对应文件外，还需在`boxman.cpp`在开头定义的地图文件名称变量加入对应的名称：

```c++
static const set<string> mapName = {"1", "2"};
```

