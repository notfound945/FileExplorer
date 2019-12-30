#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 文件链表
typedef struct LinkNode {
    char name[20];
    struct LinkNode *next; // 同级目录指针
    struct LinkNode *downNext; // 下级目录指针
} LinkNode, *LinkList;

LinkList currentDir;
LinkList rootDir;

LinkList Init() {
    LinkNode *head = (LinkNode *) malloc(sizeof(LinkNode));
    strcpy(head->name, "root");
    head->next = NULL;
    head->downNext = NULL;
    return head;
}

void Display(LinkList linkList) {
    printf("\n");
    if (linkList != NULL) {
        while (linkList) {
            printf("%s ", linkList->name);
            linkList = linkList->next;
        }
    } else {
        printf("<没有任何内容>");
    }
    printf("\n");
}

void CreateNext(LinkList linkList, char name[20]) {
    while(linkList->next) {
        linkList = linkList->next;
    }
    LinkNode *newNode = (LinkNode *) malloc(sizeof(LinkNode));
    strcpy(newNode->name, name);
    newNode->next = linkList->next;
    newNode->downNext = NULL;
    linkList->next = newNode;
}

void CreateDownNext(LinkList linkList, char name[20]) {
    LinkNode *head = (LinkNode *) malloc(sizeof(LinkNode));
    LinkNode *temp = (LinkNode *) malloc(sizeof(LinkNode));
    strcpy(head->name, "head");
    head->downNext = NULL;
    head->next = NULL;

    strcpy(temp->name, name);
    temp->next = head;
    temp->downNext = linkList->downNext;
    linkList->downNext = temp;
}

void GetDir(char name[20]) {
    LinkList temp = Init();
    temp->downNext = Init();
    temp->next = currentDir;
    printf("\nGetDir(%s)\n", name);
    Display(temp);
    while (temp->next) {
        if (!strcmp(temp->next->name, name)) {
            printf("\n找到 %s 文件\n", name);
            printf(" -%s- \n ", temp->name);
            currentDir = temp->downNext;
            return;
        }
        temp = temp->next;
    }
    printf("\n未找到\n");
    return;
}

int main() {
    LinkList linkList = Init();
    currentDir = linkList;
    rootDir = linkList;
    printf("创建下级目录\n");
    CreateDownNext(currentDir, "main");
//    Display(currentDir->downNext);
    printf("切换当前目录 downNext\n");
    currentDir = rootDir->downNext;
    printf("创建文件\n");
    CreateNext(currentDir, "sub");
    CreateNext(currentDir, "testsub");
    CreateNext(currentDir, "666666");
    printf("打印当前目录\n");
    Display(currentDir->next);
    GetDir("666666");
    printf("switch 66666\n");
//    CreateNext(currentDir, "666");
    Display(currentDir);
    return 0;
}