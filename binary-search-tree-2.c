/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */

Node* searchIterative(Node* head, int key);
void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
	if(ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224s
 */
void iterativeInorder(Node* node)
{
	top=-1;	//top 위치 초기화
	for(;;){	//stack 이 공백일 때 까지 반복
		for(;node;node = node->left)	push(node);	//stack에 push

		node=pop();	//temp에 pop

		if(!node) break;	//temp가 NULL 이면 반복 종료

		printf(" [%d] ", node->key);
		node=node->right;	//temp->right 로 이동(출력된 노드의 오른쪽으로 이동)
	}

}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	front=rear=-1;
	if(!ptr) return;	//공백트리이면 return;
	enQueue(ptr);		//ptr 을 queue 에 add

	for(;;){
		ptr=deQueue();	//ptr을 dequeue 

		if(ptr){	//ptr이 존재하면 출력 및 자식 노드를 enqueue
			printf(" [%d] ",ptr->key);
			if(ptr->left)	enQueue(ptr->left);
			if(ptr->right)	enQueue(ptr->right);
		}
		else break;	//queue에 더이상 값이 없으면 반복문 종료
	}
	
}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
}


void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	if(top==-1)	return NULL;
	else return stack[top--]; 
}

void push(Node* aNode)
{	if(top==MAX_STACK_SIZE-1) printf("stack is full\n");
	else stack[++top]=aNode;
}



Node* deQueue()
{
	if(front==rear)	return 0;
	else return queue[++front];
}

void enQueue(Node* aNode)
{
	if(rear==MAX_QUEUE_SIZE-1)	printf("queue is full\n");
	else queue[++rear]=aNode;
}

void printStack(){

}


Node* searchIterative(Node* head, int key)
{
	Node* p=head->left;
	while(p){//p가 NULL이 될 때 까지 반복 -> 원소를 끝까지 추적하였을 때
		if(key==p->key) return p;	//key와 일치하는 원소를 찾으면 해당되는 주소를 바환
		if(key<p->key)	//key 값이 원소의 값보다 작으면
			p=p->left;	//왼쪽 서브트리로 이동
		else
			p=p->right;	//key가 원소의 값보다 크면 오른쪽 서브트리로 이동
	}
	return NULL; //반복문 안에서 해당되는 값을 찾지 못 했으므로 NULL 반환
}