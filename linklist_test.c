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
char root[] = "C:";

LinkList Init(char name[]) {
    LinkNode *head = (LinkNode *) malloc(sizeof(LinkNode));
    strcpy(head->name, name);
    head->next = NULL;
    head->downNext = NULL;
    return head;
}

void Display(LinkList linkList) {
    LinkList temp = linkList->next;
    if (temp != NULL) {
        while (temp) {
            printf("%s ", temp->name);
            temp = temp->next;
        }
        printf("\n");
    } else {
        printf("<没有任何内容>\n");
    }
}

void CreateNext(LinkList linkList, char name[20]) {
    LinkList temp = linkList;
    while(linkList->next) {
        linkList = linkList->next;
    }
    LinkNode *newNode = (LinkNode *) malloc(sizeof(LinkNode));
    strcpy(newNode->name, name);
    newNode->next = linkList->next;
    newNode->downNext = NULL;
    linkList->next = newNode;
    rootDir = temp;
}

void GetDir(char name[20]) {
    printf("GetDir()\n");
    LinkList temp = currentDir->next;
    if (temp != NULL) {
        while (temp) {
            if (!strcmp(temp->name, name)) {
                printf("找到 %s \n", temp->name);
                temp->downNext = Init(name);
                currentDir = temp->downNext;
                return;
            }
            temp = temp->next;
        }
        printf("<未找到此文件 %s >\n", name);
        return;
    } else {
        printf("<查找 %s 时没有任何内容>\n", name);
        return;
    }
}

void GoHome() {
    currentDir = rootDir;
}

int main() {
    LinkList linkList = Init(root);
    linkList->downNext = Init(root);
    currentDir = linkList->downNext;
    rootDir = linkList;
    Display(currentDir);
    CreateNext(currentDir, "phl");
    CreateNext(currentDir, "fyl");
    printf("create\n");
    Display(currentDir);
    GetDir("phl");
    CreateNext(currentDir, "demo.exe");
    CreateNext(currentDir, "main.exe");
    CreateNext(currentDir, "Music");
    GetDir("Music");
    CreateNext(currentDir, "EXO");
    CreateNext(currentDir, "CXK");
    printf("go back root\n");
    GoHome();
    Display(currentDir);
    printf("Try again\n");
    GetDir("dsf");
    GetDir("phl");
    GetDir("Music");
    Display(currentDir);
    printf("root Dir\n");
    Display(rootDir);
    printf("linklist\n");
    Display(linkList->downNext);
    return 0;
}