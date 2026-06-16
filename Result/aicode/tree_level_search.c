#include <stdio.h>
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 큐를 활용하여 트리를 레벨 단위로 탐색하는 함수
void level_order(TreeNode* root) 
{
    if (root == NULL) return;

    // 트리 노드 포인터를 담는 큐 선언 (기존 큐 함수 활용)
    TreeNode* queue[100];
    int front = 0, rear = 0;

    queue[rear++] = root; // 루트 노드 enqueue

    while (front < rear) {
        TreeNode* curr = queue[front++]; // dequeue
        printf("%d ", curr->data);       // 현재 노드 처리

        // 자식 노드들을 순서대로 큐에 삽입
        if (curr->left != NULL)  queue[rear++] = curr->left;
        if (curr->right != NULL) queue[rear++] = curr->right;
    }
}



// 트리의 전체 노드 개수를 구하는 함수
int get_node_count(TreeNode* node) {
    if (node == NULL) return 0;
    return 1 + get_node_count(node->left) + get_node_count(node->right);
}

// 트리의 최대 높이(깊이)를 구하는 함수
int get_height(TreeNode* node) {
    if (node == NULL) return 0;
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}
//=============================================================================
//일괄 정렬
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// 전역 또는 매개변수로 쓸 배열과 인덱스
int sorted_arr[100];
int arr_idx = 0;

// Step 1: 중위 순회를 돌며 기존 트리의 데이터를 오름차순으로 배열에 수집
void collect_data(Node* node) {
    if (node == NULL) return;

    collect_data(node->left);
    sorted_arr[arr_idx++] = node->data; // 배열에 순서대로 저장
    collect_data(node->right);
}

// Step 2: 정렬된 배열을 가지고 완벽한 균형 트리로 재조립 (이진 탐색 알고리즘 활용)
Node* build_balanced_tree(int start, int end) {
    if (start > end) return NULL;

    // 항상 가운데 원소를 루트로 선택 (균형을 맞추는 핵심)
    int mid = (start + end) / 2;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = sorted_arr[mid];

    // 좌우 서브트리를 재귀적으로 빌드
    new_node->left = build_balanced_tree(start, mid - 1);
    new_node->right = build_balanced_tree(mid + 1, end);

    return new_node;
}

// 기존 불균형 트리의 메모리를 해제하는 함수 (메모리 누수 방지용 - 교수님 가산점 포인트)
void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

//"루트만 넣으면 싹 정렬해주는 마스터 함수"
Node* balance_entire_tree(Node* root) {
    if (root == NULL) return NULL;

    arr_idx = 0; // 인덱스 초기화

    // 1. 기존 데이터 싹 모으기
    collect_data(root);

    // 2. 기존의 망가진 트리 메모리 해제
    free_tree(root);

    // 3. 모은 데이터를 바탕으로 완벽한 균형 트리 새로 짜기 (0번부터 arr_idx - 1번까지)
    return build_balanced_tree(0, arr_idx - 1);
}
//===================================================================================================
//일렬로 나열된건 할수없는 기존코드의 단점 극복

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// 가짜 루트(Dummy Grandparent)를 활용한 좌회전
Node* rotate_left(Node* grand, Node* parent, Node* child) {
    if (grand != NULL) {
        grand->right = child;
    }
    parent->right = child->left;
    child->left = parent;
    return child;
}

// 가짜 루트를 활용한 우회전
Node* rotate_right(Node* grand, Node* parent, Node* child) {
    if (grand != NULL) {
        grand->right = child; // 편의상 그레이바인은 다 오른쪽으로 연결되므로 right를 수정
    }
    else {
        grand = child; // 만약 grand가 없으면 전체 루트가 바뀜
    }
    parent->left = child->right;
    child->right = parent;
    return child;
}

// Step 1: 트리를 한 줄로 길게 펴는 함수 (오른쪽 자식만 갖도록)
int tree_to_vine(Node* grand) {
    int count = 0;
    Node* tmp = grand->right;

    while (tmp != NULL) {
        if (tmp->left != NULL) {
            Node* child = tmp->left;
            rotate_right(grand, tmp, child);
            tmp = child; // 우회전 후 새로 올라온 노드로 이동
        }
        else {
            count++;
            grand = tmp;
            tmp = tmp->right; // 이미 오른쪽만 있다면 다음 노드로 이동
        }
    }
    return count; // 총 노드의 개수 반환
}

// Step 2: 일렬로 선 노드들을 원하는 횟수만큼 좌회전시켜 압축하는 함수
void compress(Node* grand, int m) {
    Node* tmp = grand->right;
    for (int i = 0; i < m; i++) {
        Node* child = tmp->right;
        rotate_left(grand, tmp, child);
        grand = child;
        tmp = grand->right;
    }
}

//  한계를 극복한 마스터 함수: 루트만 넣으면 포인터 회전으로 일괄 정렬
Node* balance_dsw(Node* root) {
    if (root == NULL) return NULL;

    // 회전을 매끄럽게 하기 위해 '가짜 부모 노드(Dummy)'를 생성
    Node dummy;
    dummy.right = root;
    dummy.left = NULL;

    // 1단계: 트리를 한 줄로 편다 (노드 개수 파악)
    int count = tree_to_vine(&dummy);

    // 2단계: 완벽한 이진 트리를 만들기 위한 수학적 계산
    // count보다 작거나 같은 2의 거듭제곱 중 가장 큰 수 (예: count=5면, h=4)
    int h = 1;
    while (h <= count + 1) {
        h <<= 1;
    }
    h >>= 1;
    h -= 1;

    // 나머지 잎 노드(Leaves)들을 먼저 압축 처리
    compress(&dummy, count - h);

    // 남은 노드들을 반씩 줄여나가며 정상적인 트리 층을 쌓아 올림
    while (h > 1) 
    {
        h >>= 1;
        compress(&dummy, h);
    }

    return dummy.right; // 가짜 부모의 오른쪽 자식이 새로운 균형 트리의 루트가 됨
}