# 仿Explorer树形结构文件管理器


# 项目描述

主要使用链表对树形目录文件结构进行模拟，进行新建文件/文件夹，返回根目录/上级目录，查看下级目录等操作。

# 采用技术

+ 链表
+ 栈

# 实现功能

+ 打开目录文件夹
+ 新建文件/文件夹
+ 删除文件/文件夹
+ 重命名文件/文件夹
+ 排序所有文件
+ 目录文件大小统计
+ 支持返回上级目录及根目录操作

# 演示效果

![效果图1](https://github.com/notfound945/FileExplorer/blob/master/img/effect1.png)

![效果图2](https://github.com/notfound945/FileExplorer/blob/master/img/effect2.png)

![效果图3](https://github.com/notfound945/FileExplorer/blob/master/img/effect3.png)

![效果图4](https://github.com/notfound945/FileExplorer/blob/master/img/effect4.png)

![效果图5](https://github.com/notfound945/FileExplorer/blob/master/img/effect5.png)

![效果图6](https://github.com/notfound945/FileExplorer/blob/master/img/effect6.png)	

![效果图7](https://github.com/notfound945/FileExplorer/blob/master/img/effect7.png)


# 开发语言

+ **C 语言（C 99）**

# 开发环境

+ IDE：**CLion 2019.3.2**
+ CMake：**CMake Version 3.15**
+ GCC：**mingw-w64 6.0**
+ Debugger： **MinGW-w64 GDB Version 8.1**

# 开发日期

**2020/01/01**

# 实现原理

本项目主要采用链表和链栈完成
+ 使用链表来存储文件信息，使用File结构体来存储文件信息，其中File结构体有属性：文件名(name) 文件大小(size) 文件类型(type)。文件链表上每个结点有以下的属性：文件结构体(File \*) 同一目录文件的指针(\* next) 当前结点下一级目录指针(\* downNext)，正是运用这两个指针来表示整个树形结构文件目录。
我也为此制作了对应的图文对照，方便理解。如下所示：

+ 使用链栈来存储每进入的路径名，事实上并不是储存一个文件名那么简单。链栈的属性： 文件链表(link)  指针(\* next)。因此，链栈中存储的是文件链表信息，每进入一个目录，就将当前目录链表(currentDir)压入栈中，正是这样使返回上级目录，回到根目录等功能变得游刃有余！那么路径名是怎么打印的呢？很简单，我只需打印链表头结点中信息就行了。那为什么呢？因为我在创建链表时就这样设计的。


# 源码分析

##  主程序

> main.c

+ File 文件结构体

```
typedef struct File {
    char name[20];
    int size;
    int type;
} File;
```

+ LinkList \* 文件链表
```
typedef struct LinkNode {
    File *file;
    struct LinkNode *next; // 同级目录指针
    struct LinkNode *downNext; // 下级目录指针
} LinkNode, *LinkList;
```

+ LinkStackList \*  文件链表栈
```
typedef struct LinkStackNode {
    LinkList link;
    struct LinkStackNode *next;
} LinkStackNode, *LinkStackList;
```

+ 全局当前目录
LinkList currentDir;

+ 全局根目录
LinkList rootLinkList;

+ 全局链表栈
LinkStackList linkStackList;

+ 全局根目录名
char rootPath[] = "C:";

+ ShowWelcome():void

  欢迎界面，也是帮助界面，打印帮助信息；

+ Dir():void

  列出当前目录所有文件

+ InitLinkList(char name[]):LinkList

  初始化文件链表

+ InitStack():LinkListNode \*

  初始化栈

+ Push(LinkList linkList):void

  压入路径链进入栈

+ Pop():void

  从栈项弹出路径链

+ GetTop():LinkList

  取得栈顶元素

+ CleanStack(LinkStackList stack):void

  清空栈中元素

+ GetConfirm(char name[]):int

  获得请求确认

+ Check(char name[]):int

  检查文件名是否存在

+ UpdateInfo():void

  更新文件夹信息，主要用于新建文件后计算当前文件夹的大小，将数值更新到对应文件夹上

+ MakeFile(File \*file):void

  新建文件/文件夹，根据传入的数值确定是新建文件还是文件夹，并将其加入文件链表中

+ DeleteFile(char name[]):void

  查找并删除指定名称的文件/文件夹

+ RenameFile(char name[]):void

  查找并修改文件名为新名称

+ GetFile(char name[]):File \*

  新建文件信息，输入指定文件名，生成文件信息。如新建文件夹则设置文件大小为0，新建文件则会自动随机生成数值作为当前文件的大小

+ GoHome():void

  返回根目录，不论当前处于何目录，都跳转至根目录

+ GoSuper():void

  返回上级目录，如当前在根目录是无法返回上级目录的

+ Dir():void

  列出当前文件目录下所有文件/文件夹

+ GetFileNumber(LinkList linkList):int

  取得文件数量，主要用于对目录下所有文件排序，将链表数据转换为数组数据

+ ShowSortFileList(File fileList[], int length):void

  打印排序后的文件列表

+ QuickSortFile(File fileList[], int L, int R, int type):void

  快速排序核心代码，使用递归方式进行快速排序

+ SortFile(LinkList linkList, int type):void

  进行文件排序，对当前目录文件做判断，并将链表信息转换成数组

+ GetDir(char name[]):void

  切换目录，切换至指定的目录

+ ExecuteCommand(char commandLine[] []):void

  对输入的字符进行判断，并执行对应命令

+ AutoGenterateFile():void

  向当前空链表自动加入文件链表数据，类似载入数据

+ ShowPath(LinkStackList stack):void

  用于显示当前路径名，主要是当前路径链栈中的各链表头结点的数据

+ ShowCommandLine():void

  获取命令输入内容并进行拆分，传递对应参数

+ main()

  主函数入口，链表，链栈，载入数据都会从此开始
  
## 测试程序

>linklist_test.c

链表测试示例源码

>linkstack_test.c

链栈测试示例源码