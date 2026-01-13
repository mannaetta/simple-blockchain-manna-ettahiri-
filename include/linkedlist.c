#include <stdio.h>
#include <stdlib.h>
#include "../src/linkedlist.h"

Node * createNode(int data){
    Node * head = (Node *)malloc(sizeof(Node));
    head->data = data ;
    head->next = NULL;
    return head ;
}

Node * insertNode(Node * head, int data){
    if(!head){
        return createNode(data);
    }
    Node * current = head;
    while(current->next){
        current = current->next ;
    }
    current->next = createNode(data);
    return head ;
}

Node * deleteNode(Node * head, int data){
    if(!head){
        return NULL ;
    }

    if(head->data == data){
        Node * temp = head ;
        head = head->next ;
        free(temp);
        return head;
    }
    Node* current = head;
    while(current->next && current->next->data != data){
        current = current->next ;
    }
    
    if (current->next) {
        Node * temp = current->next;
        current->next = temp->next;
        free(temp);
    } else {
        printf("Data not found\n"); 
    }
    return head ;
}

void displayList(Node * head){
    if (!head) {
        printf("List is empty\n");
        return;
    }
    while(head){
        printf("%d -> ",head->data);
        head = head->next;
    }
    printf("NULL \n");
}
void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
// Function to free entire linked list
void freeList(Node* head) {
    Node* current = head;
    Node* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
}


