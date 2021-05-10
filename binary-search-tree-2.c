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

Node* searchIterative(Node* head, int key);		//해당되는 key를 가진 노드의 주소를 반환하는 함수
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
	Node* delete=searchIterative(head,key);	//삭제하려는 값의 위치 확인
	
/*i 삭제하려는 값이 tree에 없을 때*/
	if(!delete){	
		printf("key is not in tree\n");
		return 0;
	}
	Node* p=head->left;		
	
	// /*tree에 노드가 하나밖에 없다면 헤더노드의 left를 NULL로 하고 삭제할 노드의 메모리 해제 해준다.*/
	// if(p==delete){head->left=NULL; free(delete); return 0;}	

/*ii 삭제하고자 하는 노드가 단말노드 일 때*/
	if(delete->left == NULL&&delete->right==NULL){//leaf 노드 일 경우
		
		/*tree에 노드가 하나밖에 없다면 헤더노드의 left를 NULL로 하고 삭제할 노드의 메모리 해제 해준다.*/
		if(p==delete){head->left=NULL; free(delete); return 0;}	
		
		while(1){
		/*부모노드의 link를 NULL로 해주고, 삭제할 노드의 메모리를 해제한다.*/
		if(p->left==delete){	
			p->left=NULL; free(delete); return 0;}
		if(p->right==delete){
			p->right=NULL; free(delete); return 0;}

		if(key<p->key)	//key 값이 원소의 값보다 작으면
			p=p->left;	//왼쪽 서브트리로 이동
		else
			p=p->right;	//key가 원소의 값보다 크면 오른쪽 서브트리로 이동
		}
	}

/*iii 삭제하고자 하는 노드가 하나의 자식만을 가질 때*/
	if(delete->left==NULL){	//오른쪽 자식노드만을 가질 때
		if(p==delete){head->left=delete->right; free(delete); return 0;}//첫 번째 노드가 삭제할 대상일 때 헤더노드가 삭제할 노드의 자식노드를 가리킨다.

		while(1){	//삭제할 때 까지 반복
			if(p->left==delete){	//p->left 에 삭제할 항목이 있다면
				p->left=delete->right;	//p 는 삭제할 항목의 right를 자식노드로 가지게 된다.
				free(delete);
				return 0;
			}
			if(p->right==delete){	//p->right에 삭제할 항목이 있다면
				p->right=delete->right;	//p는 삭제할 항목의 right를 자식노드로 가지게 된다.
				free(delete);
				return 0;
			}/*트리 이동*/
			if(key<p->key)	p=p->left;
			else	p=p->right;
		}
	}
	/*왼쪽 자식노드만을 가질 때. 앞선 오른쪽 자식노드만을 가질 때와 삭제할 항목의 left를 자식노드로
	가지는 것 외에 동일하다.*/
	if(delete->right==NULL){	//왼쪽 자식노드만을 가질 때
		if(p==delete){head->left=delete->left; free(delete); return 0;}	//첫 번째 노드가 삭제할 대상일 때 헤더노드가 삭제할 노드의 자식노드를 가리킨다.

		while(1){
			if(p->left==delete){
				p->left=delete->left;
				free(delete);
				return 0;
			}
			if(p->right==delete){
				p->right=delete->left;
				free(delete);
				return 0;
			}
			if(key<p->key)	p=p->left;
			else	p=p->right;
		}
	}
	
/*iv 삭제하고자 하는 노드가 두 개의 자식을 가질 때 (오른쪽 서브트리에서 가장 작은 값으로 대체)*/
	Node* ptr=NULL;	//삭제될 노드를 대체할 노드를 가리킬 포인터
	p=head;
	while(1){
		
		if(p->left==delete){//p -> left 가 삭제할 항목인 경우
			
			ptr=delete->right; //삭제할 항목의 오른쪽 서브트리로 이동
			
			if(ptr->left==NULL){	//삭제할 항목의 오른쪽 노드가 가장 작은 값이라면
				
				ptr->left=delete->left;
				p->left=ptr;
				free(delete);
				return 0;
			}

			ptr=ptr->left;	
			delete=delete->right;	//ptr의 이전 노드에 위치
			
			for(;;){	//삭제할 항목의 오른쪽 서브트리 중 가장 작은 값 찾기
				if(ptr->left==NULL) break;

				ptr=ptr->left;
				delete=delete->left;
			}
			/*ptr: 가장 작은 노드 (삭제할 항목의 대체 노드)
			, delete: 가장 작은 노드의 부모 노드, p: 삭제할 항목의 부모노드*/
			ptr->left=p->left->left;	//ptr을 삭제될 공간의 left 노드와 연결
			delete->left=ptr->right;	//ptr의 부모노드의 left 를 ptr->right 와 연결
			ptr->right=p->left->right;	//ptr을 삭제될 공간의 right 노드와 연결
			
			free(p->left);		// 노드 삭제
			
			p->left=ptr;				//삭제된 공간의 부모노드와 ptr 연결
			return 0;
		}
		if(p->right==delete){//p -> right 가 삭제할 항목인 경우
			
			ptr=delete->right; //삭제할 항목의 오른쪽 서브트리로 이동
			
			if(ptr->left==NULL){	//삭제할 항목의 오른쪽 노드가 가장 작은 값이라면
				
				ptr->left=delete->left;
				p->right=ptr;
				free(delete);
				return 0;
			}

			ptr=ptr->left;	
			delete=delete->right;	//ptr의 이전 노드에 위치
			
			for(;;){	//삭제할 항목의 오른쪽 서브트리 중 가장 작은 값 찾기
				if(ptr->left==NULL) break;

				ptr=ptr->left;
				delete=delete->left;
			}

			/*ptr: 가장 작은 노드 (삭제할 항목의 대체 노드)
			, delete: 가장 작은 노드의 부모 노드, p: 삭제할 항목의 부모노드*/
			ptr->left=p->right->left;	//ptr을 삭제될 공간의 left 노드와 연결
			delete->left=ptr->right;	//ptr의 부모노드의 left 를 ptr->right 와 연결
			ptr->right=p->right->right;	//ptr을 삭제될 공간의 right 노드와 연결
			
			free(p->right);		// 노드 삭제
			
			p->right=ptr;				//삭제된 공간의 부모노드와 ptr 연결
			return 0;
		}

		/*삭제할 노드를 찾아 이동*/
		if(p->key==-9999) p=p->left;	//p의 위치가 헤더노드일 경우 다음 노드인 p->left로 이동
		else if(key<p->key)	p=p->left;
		else	p=p->right;
	}


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



Node* pop()	//stack pop
{
	if(top==-1)	return NULL;	//stack 이 비어있다면 NULL 값 return 
	else return stack[top--]; 	//stack 의 value를 리턴하고 top-=1
}

void push(Node* aNode)	//stack push 
{	if(top==MAX_STACK_SIZE-1) printf("stack is full\n");	//stack이 꽉 차 있다면 다음을 출력
	else stack[++top]=aNode;	//stack의 top을 +1 하고,  Node의 주소를 저장한다.
}



Node* deQueue()	//deQueue
{
	if(front==rear)	return 0;	//Queue가 비어있다면 return 0
	else return queue[++front];	//front+=1, front가 가리키는 값 return 
}

void enQueue(Node* aNode)	//enQueue
{
	if(rear==MAX_QUEUE_SIZE-1)	printf("queue is full\n");	//Queue가 꽉 차있다면 다음을 출력
	else queue[++rear]=aNode;	// rear+=1, aNode의 주소를 Queue에 저장
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