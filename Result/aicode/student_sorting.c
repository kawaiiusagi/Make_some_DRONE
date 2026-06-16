#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 20

// ===================== 구조체 정의 =====================
typedef struct {
    char name[MAX_NAME];
    int score;
} Student;

// 연결 리스트 기반 큐 노드
typedef struct QNode {
    Student data;
    struct QNode* next;
} QNode;

// 큐 (front, rear 포인터만 관리)
typedef struct {
    QNode* front;
    QNode* rear;
    int count;
} Queue;

// 이진 탐색 트리 (점수 기준)
typedef struct TreeNode {
    Student student;
    struct TreeNode* left, * right;
} TreeNode;

// ===================== 큐 함수 (연결 리스트) =====================
void initQueue(Queue* q) //연결 자료구조로 큐 생성
{
    q->front = q->rear = NULL;
    q->count = 0;
}

int isEmpty(Queue* q) //큐가 비었나
{
    return q->front == NULL;
}

// 삽입 (rear 뒤에 노드 추가)
void enqueue(Queue* q, Student s) {
    QNode* newNode = (QNode*)malloc(sizeof(QNode));
    if (newNode == NULL) {
        printf("메모리 할당 실패\n");
        return;
    }
    newNode->data = s;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->count++;
}

// 삭제 (front에서 제거 후 반환)
Student dequeue(Queue* q) {
    Student empty = { "", -1 };
    if (isEmpty(q)) {
        printf("큐가 비었습니다.\n");
        return empty;
    }

    QNode* temp = q->front;
    Student s = temp->data;

    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;  // 마지막 노드 삭제 시 rear도 NULL

    free(temp);
    q->count--;
    return s;
}

// 큐 전체 메모리 해제 (혹시 남아있을 경우 대비)
void freeQueue(Queue* q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
}

// ===================== 선택 정렬 =====================
// 점수 내림차순 정렬 (1등이 맨 앞)
void selectionSort(Student arr[], int n) 
{
    for (int i = 0; i < n - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].score > arr[maxIdx].score) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            Student temp = arr[i];
            arr[i] = arr[maxIdx];
            arr[maxIdx] = temp;
        }
    }
}

// ===================== BST 함수 =====================
TreeNode* createNode(Student s) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->student = s;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 점수를 기준으로 삽입
TreeNode* insert(TreeNode* root, Student s) {
    if (root == NULL) return createNode(s);

    if (s.score < root->student.score)
        root->left = insert(root->left, s);
    else
        root->right = insert(root->right, s);

    return root;
}

// 중위 순회 (점수 오름차순 출력)
void inorder(TreeNode* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("이름: %-10s 점수: %d\n", root->student.name, root->student.score);
    inorder(root->right);
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// ===================== 데이터 입력 함수 =====================

// 방법 1: 표준 입력(stdin)으로부터 직접 입력받기
// 사용자가 학생 수를 먼저 입력하고, 이름/점수를 반복 입력
int loadFromStdin(Queue* q) {
    int n;
    printf("학생 수를 입력하세요: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("잘못된 입력입니다.\n");
        return 0;
    }

    Student s;
    for (int i = 0; i < n; i++) {
        printf("학생 %d - 이름 점수 입력: ", i + 1);
        scanf("%s %d", s.name, &s.score);
        enqueue(q, s);
    }
    return n;
}

// 방법 2: 파일 이름을 입력받아 파일에서 읽어오기
// 파일 형식: 이름 점수 (한 줄에 한 명씩)
// 예) Kim 85
//     Lee 92
int loadFromFile(Queue* q) {
    char filename[100];
    printf("읽어올 파일 이름을 입력하세요: ");
    scanf("%s", filename);

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return 0;
    }

    Student s;
    int count = 0;
    while (fscanf(fp, "%s %d", s.name, &s.score) == 2) {
        enqueue(q, s);
        count++;
    }

    fclose(fp);
    printf("%d명의 학생 정보를 '%s' 파일에서 불러왔습니다.\n", count, filename);
    return count;
}

// 정렬된 결과를 파일에 저장
void saveToFile(const char* filename, Student arr[], int n) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("파일을 생성할 수 없습니다.\n");
        return;
    }

    fprintf(fp, "===== 성적 순위 (높은 점수순) =====\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d위: %-10s %d점\n", i + 1, arr[i].name, arr[i].score);
    }

    fclose(fp);
    printf("결과가 '%s' 파일에 저장되었습니다.\n", filename);
}

// ===================== 메인 함수 =====================
int main() {
    Queue q;
    initQueue(&q);

    int n = 0;
    int choice;

    printf("===== 데이터 입력 방식 선택 =====\n");
    printf("1. 표준 입력(직접 입력)\n");
    printf("2. 파일에서 읽기\n");
    printf("선택: ");
    scanf("%d", &choice);

    if (choice == 1) {
        n = loadFromStdin(&q);
    }
    else if (choice == 2) {
        n = loadFromFile(&q);
    }
    else {
        printf("잘못된 선택입니다.\n");
        return 1;
    }

    if (n <= 0) {
        printf("입력된 데이터가 없습니다. 프로그램을 종료합니다.\n");
        return 1;
    }

    // 큐에서 꺼내서 배열로 변환 (정렬은 배열에서 수행)
    Student* students = (Student*)malloc(sizeof(Student) * n);
    for (int i = 0; i < n; i++) 
    {
        students[i] = dequeue(&q);
    }

    // 선택 정렬로 점수 내림차순 정렬
    selectionSort(students, n);

    printf("\n===== 정렬 결과 =====\n");
    for (int i = 0; i < n; i++) {
        printf("%d위: %-10s %d점\n", i + 1, students[i].name, students[i].score);
    }

    // BST에 삽입 (점수 기준)
    TreeNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, students[i]);
    }

    printf("\n===== BST 중위순회 (오름차순) =====\n");
    inorder(root);

    // 결과를 파일로 저장
    saveToFile("result.txt", students, n);

    // 메모리 해제
    freeTree(root);
    freeQueue(&q);   // 혹시 남아있는 큐 노드 정리 (보통 이미 비어있음)
    free(students);

    return 0;
}