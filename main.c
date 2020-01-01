#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 文件结构体
typedef struct File {
    char name[20];
    int size;
    int type;
} File;

// 文件链表
typedef struct LinkNode {
    File *file;
    struct LinkNode *next; // 同级目录指针
    struct LinkNode *downNext; // 下级目录指针
} LinkNode, *LinkList;

// 路径栈
typedef struct LinkStackNode {
    LinkList link;
    struct LinkStackNode *next;
} LinkStackNode, *LinkStackList;

// 全局当前目录
LinkList currentDir;
// 全局根目录
LinkList rootLinkList;
// 全局链表栈
LinkStackList linkStackList;
// 根目录名
char rootPath[] = "C:";

// 帮助页
void ShowWelcome();

void Dir();

// 初始化文件链表
LinkList InitLinkList(char name[20]) {
    LinkNode *head = (LinkNode *) malloc(sizeof(LinkNode));
    LinkNode *downHead = (LinkNode *) malloc(sizeof(LinkNode));
    File *file = (File *) malloc(sizeof(File));
    strcpy(file->name, name);
    head->file = file;
    downHead->file = file;
    downHead->next = NULL;
    downHead->downNext = NULL;
    head->next = NULL;
    head->downNext = downHead;
}

// 初始化链表栈
LinkStackNode *InitStack() {
    LinkStackNode *head = (LinkStackNode *) malloc(sizeof(LinkStackNode));
    head->link = NULL;
    head->next = NULL;
    return head;
}

// 入栈
void Push(LinkList linkList) {
    LinkStackList temp = linkStackList;
    while (temp->next) {
        temp = temp->next;
    }
    LinkStackNode *newLink = (LinkStackNode *) malloc(sizeof(LinkStackNode));
    newLink->link = linkList;
    newLink->next = temp->next;
    temp->next = newLink;
}

// 出栈
void Pop() {
    LinkStackList del;
    LinkStackList temp = linkStackList;
    if (temp->next) {
        while (temp->next->next) {
            temp = temp->next;
        }
        del = temp->next;
        temp->next = NULL;
        free(del);
        return;
    } else {
        printf("栈为空");
        return;
    }
}

// 取栈顶元素
LinkList GetTop() {
    LinkStackList temp = linkStackList->next;
    if (temp) {
        while (temp->next) {
            temp = temp->next;
        }
        return  temp->link;
    } else {
        printf("栈已经为空\n");
        return NULL;
    }
}

// 清空栈
void CleanStack(LinkStackList stack) {
    free(stack);
    stack = InitStack();
}

// 操作确认
int GetConfirm(char name[20]) {
    char answer[5];
    memset(answer, 0, sizeof answer);
    printf("\n===========================\n");
    printf(" 是否删除 %s ? 此操作不可逆！ \n", name);
    printf("===========================\n");
    printf("确认？ [Y/N]：");
    gets(answer);
    if (strcmp(answer, "y") == 0 || strcmp(answer, "Y") == 0) {
        return 1;
    } else {
        return 0;
    }
}

// 文件名检查
int Check(char name[20]) {
    int pass = 0;
    LinkList temp = InitLinkList("");
    temp = currentDir->downNext;
    while (temp->next) {
        if (!strcmp(temp->next->file->name, name)) {
            pass = 0;
            return pass;
        }
        temp = temp->next;
    }
    pass = 1;
    return pass;
}

// 更新文件大小信息
void UpdateInfo() {
    int size = 0;
    LinkList temp = InitLinkList("");
    temp->next = currentDir->downNext->next;
    while (temp) {
        size += temp->file->size;
        temp = temp->next;
    }
    printf("计算 %s 目录大小为 %d\n", currentDir->file->name, size);
    currentDir->file->size = size;
}

// 新建文件
void MakeFile(File *file) {
    if (file != NULL) {
        LinkNode *newNode = (LinkNode *) malloc(sizeof(LinkNode));
        newNode->file = file;
        newNode->next = currentDir->downNext->next;
        if (newNode->file->type == 0) {
            newNode->downNext = InitLinkList(file->name);
        } else {
            newNode->downNext = InitLinkList("<File>");
        }
        currentDir->downNext->next = newNode;
        UpdateInfo();
    } else {
        printf("文件为空，没有新建文件\n");
        return;
    }
}

// 删除文件
void DeleteFile(char name[20]) {
    LinkList temp = InitLinkList("");
    LinkList delete = InitLinkList("");
    temp = currentDir->downNext;
    while (temp->next) {
        if (!strcmp(temp->next->file->name, name)) {
            printf("找到 %s \n", name);
            if (GetConfirm(name)) {
                delete = temp->next;
                temp->next = delete->next;
                free(delete);
                printf("删除完成\n");
                return;
            } else {
                printf("文件已保留\n");
                return;
            }
        }
        temp = temp->next;
    }
    printf("未找到 %s\n", name);
}

// 重命名文件
void RenameFile(char name[20]) {
    char newName[20];
    LinkList temp = InitLinkList("");
    temp = currentDir->downNext;
    while (temp->next) {
        if (!strcmp(temp->next->file->name, name)) {
            printf("找到 %s \n", name);
            printf("请输入新的名称：");
            scanf("%s", newName);
            if (Check(newName) == 1) {
                strcpy(temp->next->file->name, newName);
                printf("%s 命名成功.\n", newName);
                return;
            } else {
                printf("新文件与其它文件名冲突，重命名失败!\n");
                return;
            }
        }
        temp = temp->next;
    }
    printf("未找到 %s\n", name);
    return;
}

// 生成文件信息
File *GetFile(char name[20], int type) {
    if (Check(name) == 1) {
        int size = 0;
        if (type == 1) {
            // 随机生成文件大小
            size = rand() % 200 * (rand() % 100 + rand() % 10 + rand() % 4);
        } else if (type == 0) {
            size = 0;
        }
        File *newFile = (File *) malloc(sizeof(File));
        strcpy(newFile->name, name);
        newFile->size = size;
        newFile->type = type;
        printf("取得文件信息 %s %d %d\n", newFile->name, newFile->size, newFile->type);
        return newFile;
    } else {
        printf("名为 %s 的文件已存在.\n", name);
        return NULL;
    }
}

// 返回根目录
void GoHome() {
    currentDir = rootLinkList;
    // 清空路径栈
    CleanStack(linkStackList);
    Push(currentDir);
}

// 返回上级目录
void GoSuper() {
    if (linkStackList->next->next) {
        Pop();
        // 取链表顶部元素
        currentDir = GetTop();
    } else {
        printf("已是根目录，不能再返回上级.\n");
    }
}

// 列出当前目录文件
void Dir() {
    UpdateInfo();
    LinkList temp = currentDir->downNext->next;
    printf("名称 \t\t 大小 \t\t 类型 \n");
    if (temp == NULL) {
        printf("\t\t<没有任何文件>\n");
    } else {
        while (temp) {
            if (temp->file->type == 1) {
                printf("%10s \t %6d \t <File>\n", temp->file->name, temp->file->size);
            } else if (temp->file->type == 0) {
                printf("%10s \t %6d \t <Dir>\n", temp->file->name, temp->file->size);
            } else {
                printf("未知文件\n");
            }
            temp = temp->next;
        }
    }
}

// 取得列表文件数量
int GetFileNumber(LinkList linkList) {
    int fileNum = 0;
    LinkList temp = linkList->downNext->next;
    if (temp == NULL) {
        printf("\t\t<没有任何文件>\n");
    } else {
        while (temp) {
            fileNum++;
            temp = temp->next;
        }
    }
    printf("文件数 %d.\n", fileNum);
    return fileNum;
}

// 打印排序后文件
void ShowSortFileList(File fileList[], int length) {
    int i = 0;
    printf("\n排序后\n");
    printf("名称 \t\t 大小 \t\t 类型 \n");
    for (i = 0; i < length; i++) {
        if (fileList[i].type == 1) {
            printf("%10s \t %6d \t <File>\n", fileList[i].name, fileList[i].size);
        } else {
            printf("%10s \t %6d \t <Dir>\n", fileList[i].name, fileList[i].size);
        }
    }
}

// 快速排序 1为升序 0为降序
void QuickSortFile(File fileList[], int L, int R, int type) {
    int i = L;
    int j = R;
    //支点
    int pivot = fileList[(L + R) / 2].size;
    //左右两端进行扫描，只要两端还没有交替，就一直扫描
    while (i <= j) {
        if (type == 1) {
            //寻找直到比支点大的数
            while (pivot > fileList[i].size)
                i++;
            //寻找直到比支点小的数
            while (pivot < fileList[j].size)
                j--;
        } else if (type == 0) {
            //寻找直到比支点小的数
            while (pivot < fileList[i].size)
                i++;
            //寻找直到比支点大的数
            while (pivot > fileList[j].size)
                j--;
        }
        //此时已经分别找到了比支点小的数(右边)、比支点大的数(左边)，它们进行交换
        if (i <= j) {
            File temp = fileList[i];
            fileList[i] = fileList[j];
            fileList[j] = temp;
            i++;
            j--;
        }
    }
    //上面一个while保证了第一趟排序支点的左边比支点小，支点的右边比支点大了。
    //“左边”再做排序，直到左边剩下一个数(递归出口)
    if (L < j)
        QuickSortFile(fileList, L, j, type);
    //“右边”再做排序，直到右边剩下一个数(递归出口)
    if (i < R)
        QuickSortFile(fileList, i, R, type);
}

// 文件大小排序
void SortFile(LinkList linkList, int type) {
    int i = 0;
    int length = GetFileNumber(linkList);
    File fileList[length];
    if (length == 0) {
        printf("\t\t<没有任何文件>\n");
        return;
    } else {
        LinkList temp = linkList->downNext->next;
        while (temp) {
            fileList[i] = *temp->file;
            temp = temp->next;
            i++;
        }
    }
    QuickSortFile(fileList, 0, length - 1, type);
    ShowSortFileList(fileList, length);
}

// 取得目录
void GetDir(char name[20]) {
    LinkList temp = currentDir->downNext->next;
    while (temp) {
        if (!strcmp(temp->file->name, name)) {
            printf("找到 %s \n", name);
            if (temp->file->type == 0) {
                Push(temp);
                currentDir = temp;
                return;
            } else if (temp->file->type == 1) {
                printf("不过 %s 好像是文件，不能以目录形式打开.\n");
                return;
            } else {
                printf("文件类型错误.\n");
                return;
            }
        }
        temp = temp->next;
    }
    printf("未找到 %s\n", name);
    return;
}

// 命令识别
void ExecuteCommand(char commandLine[10][20]) {
    if (!strcmp(commandLine[0], "exit")) {
        printf("退出程序.\n");
        exit(0);
    } else if (!strcmp(commandLine[0], "help")) {
        printf("显示帮助菜单.\n");
        ShowWelcome();
    } else if (!strcmp(commandLine[0], "cls")) {
        printf("清理屏幕.\n");
        system("cls");
    } else if (!strcmp(commandLine[0], "dir")) {
        printf("列出当前目录所有文件.\n");
        Dir();
    } else if (!strcmp(commandLine[0], "mk")) {
        printf("新建文件.\n");
        if (commandLine[1] != NULL) {
            if (strlen(commandLine[1]) == 0) {
                printf("参数不正确，如果你不知道用法请键入 help 了解。\n");
                return;
            } else {
                MakeFile(GetFile(commandLine[1], 1));
            }
        } else {
            return;
        }
    } else if (!strcmp(commandLine[0], "mkdir")) {
        printf("新建文件夹.\n");
        if (commandLine[1] != NULL) {
            if (strlen(commandLine[1]) == 0) {
                printf("参数不正确，如果你不知道用法请键入 help 了解。\n");
                return;
            } else {
                MakeFile(GetFile(commandLine[1], 0));
            }
        } else {
            return;
        }
    } else if (!strcmp(commandLine[0], "cd")) {
        printf("目录操作.\n");
        if (commandLine[1] != NULL) {
            if (strlen(commandLine[1]) == 0) {
                printf("参数不正确，如果你不知道用法请键入 help 了解。\n");
                return;
            } else if (!strcmp(commandLine[1], "..")) {
                printf("返回上层目录.\n");
                GoSuper();
            } else if (!strcmp(commandLine[1], "/")) {
                printf("返回根目录.\n");
                GoHome();
            } else {
                printf("打开 %s \n", commandLine[1]);
                GetDir(commandLine[1]);
            }
        } else {
            return;
        }
    } else if (!strcmp(commandLine[0], "del")) {
        printf("删除文件操作.\n");
        if (commandLine[1] != NULL) {
            if (strlen(commandLine[1]) == 0) {
                printf("参数不正确，如果你不知道用法请键入 help 了解。\n");
                return;
            } else {
                printf("删除 %s \n", commandLine[1]);
                DeleteFile(commandLine[1]);
            }
        } else {
            return;
        }
    } else if (!strcmp(commandLine[0], "rename")) {
        printf("重命名文件操作.\n");
        if (commandLine[1] != NULL) {
            if (strlen(commandLine[1]) == 0) {
                printf("参数不正确，如果你不知道用法请键入 help 了解。\n");
                return;
            } else {
                printf("重命名 %s \n", commandLine[1]);
                RenameFile(commandLine[1]);
            }
        } else {
            return;
        }
    } else if (!strcmp(commandLine[0], "desc")) {
        printf("降序排列\n");
        SortFile(currentDir, 0);
    } else if (!strcmp(commandLine[0], "asc")) {
        printf("升序排列\n");
        SortFile(currentDir, 1);
    } else {
        printf("参数不正确，如果你不知道用法请键入 help 了解。\n");
    }
}

// 自动生成目录文件
void AutoGenterateFile() {
    MakeFile(GetFile("Music", 0));
    MakeFile(GetFile("eclipse.exe", 1));
    MakeFile(GetFile("User", 0));
    MakeFile(GetFile("main.exe", 1));
    MakeFile(GetFile("regedit.exe", 1));
    MakeFile(GetFile("EFI", 0));
    MakeFile(GetFile("setupact.log", 1));
    MakeFile(GetFile("Windows", 0));
    GetDir("Windows");
    MakeFile(GetFile("winhlp32.exe", 1));
    MakeFile(GetFile("ProgramData", 0));
    MakeFile(GetFile("whois.exe", 1));
    MakeFile(GetFile("system.ini", 1));
    MakeFile(GetFile("Document", 0));
    GoSuper();
    GetDir("User");
    MakeFile(GetFile("john", 0));
    MakeFile(GetFile("config.con", 1));
    MakeFile(GetFile("phl", 0));
    MakeFile(GetFile("fyl", 0));
    GoHome();
}

// 显示路径
void ShowPath(LinkStackList stack) {
    while (stack->next) {
        stack = stack->next;
        printf("%s/", stack->link->file->name);
    }
    printf(" > ");
}

// 显示帮助菜单
void ShowWelcome() {
    system("cls");
    printf("Simulate Explorer [版本 V1.10]\n");
    printf("(c) 2019 Personal Development。保留所有权利。\n");
    printf("\n[用法：]\n");
    printf("cd \t\t 打开目录(/表示根目录 | ..表示上层目录)\n");
    printf("dir \t\t 列出当前目录文件\n");
    printf("mk \t\t 新建文件\n");
    printf("mkdir \t\t 新建文件夹\n");
    printf("del \t\t 删除文件\n");
    printf("rename \t\t 重命名文件\n");
    printf("desc \t\t 降序排列文件\n");
    printf("asc \t\t 升序排列文件\n");
    printf("cls \t\t 清理屏幕信息\n");
    printf("exit \t\t 退出程序\n");
    printf("help \t\t 显示此帮助菜单\n\n");
}

// 命令行输入
void ShowCommandLine() {
    char commandLine[30];
    char parameter[10][20];
    char spilt[] = " ";
    ShowWelcome();
    while (1) {
        int i = 0;
        // 打印当前路径
        ShowPath(linkStackList);
        // 等待用户输入
        gets(commandLine);
        // 命令字符串分割
        char *result = NULL;
        result = strtok(commandLine, spilt);
        while (result != NULL) {
            strcpy(parameter[i], result);
            result = strtok(NULL, spilt);
            i++;
        }
        // 执行命令
        ExecuteCommand(parameter);
        // 清除数组内容
        memset(parameter, 0, sizeof parameter);
        memset(commandLine, 0, sizeof commandLine);
        LinkStackList watch = linkStackList;
        printf("\n");
    }
}

// 主函数
int main() {
    // 初始化文件链表
    LinkList linkList = InitLinkList(rootPath);
    // 初始化链表栈 压入根目录链表进栈
    linkStackList = InitStack();
    Push(linkList);
    // 初始化全局变量
    rootLinkList = linkList;
    currentDir = linkList;
    // 自动生成文件信息
    AutoGenterateFile();
    // 输出命令行界面
    ShowCommandLine();
    return 0;
}
