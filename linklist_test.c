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
    while(linkList->next) {
        linkList = linkList->next;
    }
    LinkNode *newNode = (LinkNode *) malloc(sizeof(LinkNode));
    strcpy(newNode->name, name);
    newNode->next = linkList->next;
    newNode->downNext = NULL;
    linkList->next = newNode;
}

void GetDir(char name[20]) {
    printf("GetDir()\n");
    LinkList temp = currentDir->next;
    if (temp != NULL) {
        while (temp) {
            if (!strcmp(temp->name, name)) {
                printf("%s ", temp->name);
                temp->downNext = Init(name);
                currentDir = temp->downNext;
            }
            temp = temp->next;
        }
        printf("\n");
    } else {
        printf("<没有任何内容>\n");
    }
}

int main() {
    LinkList linkList = Init("Root");
    linkList->downNext = Init("head");
    currentDir = linkList->downNext;
    rootDir = linkList->downNext;
    Display(currentDir);
    CreateNext(currentDir, "phl");
    CreateNext(currentDir, "fyl");
    printf("create\n");
    Display(currentDir);
    GetDir("phl");
    CreateNext(currentDir, "demo.exe");
    CreateNext(currentDir, "main.exe");
    printf("show()\n");
    Display(currentDir);
    printf("root\n");
    Display(rootDir);
    return 0;
}