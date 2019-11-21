#include<stdio.h>
#include<stdlib.h>
#include "list.h"

//Creates a new Node and returns pointer to it. 
struct Node* list_new_node(int x) {
		struct Node* newNode
				= (struct Node*)malloc(sizeof(struct Node));
		newNode->data = x;
		newNode->prev = NULL;
		newNode->next = NULL;
		return newNode;
}

struct Node* list_remove_tail(struct Node *head)
{
		if(head == NULL) return NULL;
		
		struct Node *ret = head;
		
		while(ret->next != NULL) ret = ret->next; // Go To last Node
		ret->prev->next = NULL;
		return head;
}

struct Node* list_remove_head(struct Node *head)
{
		if(head == NULL) return NULL;
		
		struct Node *ret = head;
		head = head->next;
		head->prev = NULL;
		
		return head;
}

struct Node *list_remove(struct Node *head, int data)
{
		if(head == NULL) return NULL;

		struct Node *ret = head;

		while(ret != NULL) {
				if(data == ret->data) {
						if(ret == head) return list_remove_head(head);
						if(ret->next == NULL) return list_remove_tail(head);
						if(ret->prev) ret->prev->next = ret->next;
						if(ret->next) ret->next->prev = ret->prev;
						return head;
				}
				ret = ret->next;
		}
		return head;
}

//Inserts a Node at head of doubly linked list
struct Node *list_insert_head(struct Node *head, int x) {
		struct Node* newNode = list_new_node(x);
		if(head == NULL) {
				head = newNode;
				return head;
		}
		head->prev = newNode;
		newNode->next = head; 
		head = newNode;

		return head;
}

//Inserts a Node at tail of Doubly linked list
struct Node *list_insert_tail(struct Node *head, int x) {
		struct Node* temp = head;
		struct Node* newNode = list_new_node(x);
		if(head == NULL) {
				head = newNode;
				return head;
		}
		while(temp->next != NULL) temp = temp->next; // Go To last Node
		temp->next = newNode;
		newNode->prev = temp;

		return head;
}

//Prints all the elements in linked list in forward traversal order
void list_print(struct Node *head) {
		struct Node* temp = head;
		printf("Forward: ");
		while(temp != NULL) {
				printf("%d ",temp->data);
				temp = temp->next;
		}
		printf("\n");
}

//Prints all elements in linked list in reverse traversal order. 
void list_reverse_print(struct Node *head) {
		struct Node* temp = head;
		if(temp == NULL) return; // empty list, exit
		// Going to last Node
		while(temp->next != NULL) {
				temp = temp->next;
		}
		// Traversing backward using prev pointer
		printf("Reverse: ");
		while(temp != NULL) {
				printf("%d ",temp->data);
				temp = temp->prev;
		}
		printf("\n");
}

