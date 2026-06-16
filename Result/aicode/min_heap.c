#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} MinHeap;

void init(MinHeap* h) {
    h->size = 0;
}

int isEmpty(MinHeap* h) {
    return h->size == 0;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* 삽입 후 부모와 비교하며 위로 올림 */
void siftUp(MinHeap* h, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (h->data[index] < h->data[parent]) {
            swap(&h->data[index], &h->data[parent]);
            index = parent;
        }
        else {
            break;
        }
    }
}

/* 삭제 후 자식과 비교하며 아래로 내림 */
void siftDown(MinHeap* h, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < h->size && h->data[left] < h->data[smallest])
            smallest = left;
        if (right < h->size && h->data[right] < h->data[smallest])
            smallest = right;

        if (smallest == index)
            break;

        swap(&h->data[index], &h->data[smallest]);
        index = smallest;
    }
}

void push(MinHeap* h, int value) {
    if (h->size >= MAX_SIZE) {
        printf("힙이 가득 찼습니다.\n");
        return;
    }
    h->data[h->size] = value;
    h->size++;
    siftUp(h, h->size - 1);
}

int peek(MinHeap* h) {
    if (isEmpty(h)) {
        printf("힙이 비어 있습니다.\n");
        exit(1);
    }
    return h->data[0];
}

int pop(MinHeap* h) {
    if (isEmpty(h)) {
        printf("힙이 비어 있습니다.\n");
        exit(1);
    }

    int minValue = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    siftDown(h, 0);

    return minValue;
}

void printHeap(MinHeap* h) {
    printf("힙 내부 배열: ");
    for (int i = 0; i < h->size; i++) {
        printf("%d ", h->data[i]);
    }
    printf("\n");
}

int main(void) {
    MinHeap heap;
    init(&heap);

    int values[] = { 5, 3, 8, 1, 9, 2, 7 };
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        push(&heap, values[i]);
    }

    printHeap(&heap);

    printf("오름차순으로 꺼낸 결과: ");
    while (!isEmpty(&heap)) {
        printf("%d ", pop(&heap));
    }
    printf("\n");

    return 0;
}