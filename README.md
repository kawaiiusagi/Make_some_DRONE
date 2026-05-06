# DRONE SIMULATOR

자율 드론의 경로 계획과 배터리 소모를 시뮬레이션하는 C언어 프로그램 입니다!

## 프로젝트 구조

```c
├── drone.h     # 공통 헤더 (구조체 정의, 함수 선언, 상수)
├── main.c      # 진입점 — 각 단계를 순서대로 호출
├── 01.c        # Step 1 : 좌표 생성
├── 02.c        # Step 2 : 연결 리스트 구축
├── 03.c        # Step 3 : 거리 및 배터리 계산
└── 04.c        # Step 4 : 비행 시뮬레이션
```

---

## 전체 실행 흐름

```
main()
 │
 ├─► start_UI()       [01.c]  비밀번호 인증 + 좌표 입력 방식 선택
 │       │
 │       ├─ do_random()       랜덤 좌표 자동 생성
 │       └─ do_direct()       사용자가 직접 좌표 입력
 │                └──────────────────────► 01.txt 저장
 │
 ├─► read_list()      [02.c]  01.txt 읽기 → 이중 연결 리스트 구축 (x 기준 정렬)
 │                └──────────────────────► 02.txt 저장
 │
 ├─► read_node()      [03.c]  02.txt 읽기 → 구간별 거리·K·배터리 계산
 │                └──────────────────────► 03.txt 저장
 │
 └─► make_final()     [04.c]  03.txt 읽기 → 시뮬레이션 실행
                   └──────────────────────► 04.txt 저장
```

> **핵심 설계 원칙** : `main.c`가 각 단계를 순서대로 호출하고,  
> 각 단계는 이전 단계의 `.txt` 파일을 읽어 처리한 뒤 다음 단계의 `.txt` 파일을 생성합니다.  
> 파일이 단계 간 유일한 데이터 통로 역할을 합니다.

---

## 단계별 설명

### Step 1 — 좌표 생성 (`01.c` → `01.txt`)

드론이 순서대로 방문할 **웨이포인트 좌표**를 생성합니다.

- 랜덤 생성 / 직접 입력 중 선택
- 시작점 `(0, 0)` 은 항상 자동 포함
- 최대 10개 좌표, 중복 좌표 자동 방지
- 결과를 `01.txt`에 저장

### Step 2 — 연결 리스트 구축 (`02.c` → `02.txt`)

`01.txt`의 좌표를 읽어 **이중 연결 리스트**로 구성합니다.

- x 좌표 오름차순으로 정렬 삽입 (`sort_nodes`)
- 각 노드에 순서 인덱스(`link_pos`) 부여
- 결과를 `02.txt`에 저장

### Step 3 — 배터리 계산 (`03.c` → `03.txt`)

`02.txt`를 읽어 구간마다 **거리와 배터리 소모량**을 계산합니다.

| 항목 | 내용 |
|------|------|
| 거리 | 유클리드 거리 |
| 상수 K | 거리 > 50 이면 `1.7`, 이하면 `1.5` |
| 배터리 소모 | `K × 거리` |

구간명은 `AB → BC → CD → ...` 순으로 자동 부여되며, 전체 합산(TOTAL)과 함께 `03.txt`에 저장됩니다.

### Step 4 — 비행 시뮬레이션 (`04.c` → `04.txt`)

`03.txt`를 읽어 드론의 실제 비행을 시뮬레이션합니다.

**최대 배터리 결정**
```
MaxBattery = total_battery + rand_offset
             (rand_offset : 0 ~ total_battery × 1.5 사이 랜덤)
```

**구간마다 상태 판정 (우선순위 순)**

| 우선순위 | 조건 | 상태 |
|----------|------|------|
| 1 | 이전 구간이 ERROR 또는 FAILURE | `RECOVERY` |
| 2 | 10% 랜덤 확률 (`FAILURE_PROB = 10`) | `FAILURE` |
| 3 | 잔여 배터리 < 최대 배터리 / 2 | `ERROR` |
| 4 | 해당 없음 | `NORMAL` |

- `ERROR` 발생 시 : 스택에 쌓인 경로를 역순으로 출력하며 복귀
- `FAILURE` 발생 시 : 즉시 임무 중단
- 결과를 `04.txt`에 저장

---

## 주요 자료구조

```c
// 웨이포인트 노드 (이중 연결 리스트)
typedef struct Node {
    int x, y;
    int link_pos;
    struct Node *rlink, *llink;
} Node;

// 구간 정보 노드 (단방향 연결 리스트)
typedef struct Dist_node {
    char* stage;
    double distance, k, battery_use;
    struct Dist_node* next;
} Dist_node;

// 시뮬레이션 결과 노드 (단방향 연결 리스트)
typedef struct report_Node {
    char* stage;
    double dist, K, battery_use;
    int state;
    struct report_Node* rlink;
} report_Node;

// 복귀 경로 스택
typedef struct reportstack_Node {
    char* stage;
    double used_battery;
    struct reportstack_Node* rlink;
} reportstack_Node;
```

---

## 상수 설정 (`drone.h`)

```c
#define Map_size     100   // 좌표 최대 범위
#define FAILURE_PROB  10   // FAILURE 발생 확률 (%)
```

---

> Windows 환경 기준으로 작성되었습니다 (`windows.h`, `Sleep()` 사용).
