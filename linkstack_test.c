#include <stdio.h>
#include <stdlib.h>

#define ElemType int

typedef struct StackNode {
   ElemType data;
   struct StackNode *next;
}StackNode, *StackList;

StackNode * Init() {
    StackNode *head = (StackNode *) malloc(sizeof(StackNode));
    head->data = 0;
    head->next = NULL;
    return head;
}

void Push(StackList stackList, ElemType e) {
    while (stackList->next) {
        stackList = stackList->next;
    }
    StackNode *newNode = (StackNode *) malloc(sizeof(StackNode));
    newNode->data = e;
    newNode->next = stackList->next;
    stackList->next = newNode;
}

void Pop(StackList stackList, ElemType e) {
    StackNode *del;
    StackList temp = stackList;
    if (temp->next) {
        while (temp->next->next) {
            temp = temp->next;
        }
        e = temp->data;
        del = temp->next;
        temp->next = NULL;
        free(del);
    } else {
        printf("NULL");
    }
}

void Clean(StackList stackList) {
   free(stackList);
   stackList = Init();
}

void GetTop(StackList stackList) {
    StackList temp = stackList->next;
    if (temp) {
        while (temp->next) {
            temp = temp->next;
        }
        printf("\n top %d \n", temp->data);
    } else {
        printf("Õ»ÒÑ¾­Îª¿Õ");
    }
}

void Show(StackList stackList) {
    StackList temp = stackList->next;
    while(temp) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int  main() {
    ElemType e;
    StackList stackList = Init();
    Push(stackList, 1);
    Show(stackList);
    GetTop(stackList);
    Push(stackList, 2);
    Show(stackList);
    GetTop(stackList);
    Push(stackList, 3);
    GetTop(stackList);
    Show(stackList);
    Pop(stackList, e);
    GetTop(stackList);
    Show(stackList);
    Pop(stackList, e);
    Show(stackList);
    Pop(stackList, e);
    Show(stackList);
    Pop(stackList, e);
    printf("NULL get");
    GetTop(stackList);
    return 0;
}

