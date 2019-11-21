struct Node  {
		int data;
		struct Node* next;
		struct Node* prev;
};

struct Node* list_new_node(int x);
struct Node* list_insert_head(struct Node *head,int x);
struct Node* list_insert_tail(struct Node *head,int x);
struct Node* list_remove_head(struct Node *head);
struct Node* list_remove_tail(struct Node *head);
struct Node *list_remove(struct Node *head, int data);
void list_print(struct Node *head);
void list_reverse_print(struct Node *head);
