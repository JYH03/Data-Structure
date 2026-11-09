# 자료구조 프로젝트

자료구조 수업에서 진행한 C++ 프로젝트를 모아둔 저장소입니다. 각 프로젝트는 독립적인 프로그램이며, 별도의 폴더에서 빌드하고 실행합니다.

## 프로젝트 목록

| 프로젝트             | 주요 내용                                 | 폴더                                           |
| ---------------- | ------------------------------------- | -------------------------------------------- |
| Playlist         | 음악 정보를 큐와 이진 탐색 트리로 관리하고 플레이리스트를 구성   | [프로젝트 보기](./Data_structure-Playlist)         |
| B+ Tree and Heap | B+ 트리, 힙, 선택 트리를 이용한 직원 정보 관리         | [프로젝트 보기](./Data_structure-Bp_tree_and_heap) |
| Graph Traversal  | 그래프 탐색, 최단 경로, 최소 신장 트리 등 그래프 알고리즘 구현 | [프로젝트 보기](./Data_structure-Graph_Traversal)  |

## 폴더 구조

```text
Data-Structure/
├── README.md
├── Data_structure-Playlist/
├── Data_structure-Bp_tree_and_heap/
└── Data_structure-Graph_Traversal/
```

각 폴더에는 해당 프로젝트의 소스 코드와 헤더, 기존 저장소에 포함된 데이터 파일이 들어 있습니다.

## 프로젝트별 내용

### Playlist

음악 정보를 큐에 저장하고, 가수와 곡 제목을 기준으로 이진 탐색 트리에서 검색합니다. 검색한 곡을 이용해 플레이리스트를 구성하고 출력합니다.

- 주요 자료구조: 큐, 이진 탐색 트리, 연결 리스트
- 주요 명령: `LOAD`, `ADD`, `QPOP`, `SEARCH`, `MAKEPL`, `PRINT`, `DELETE`, `EXIT`
- 데이터 파일: `Music_List.txt`

### B+ Tree and Heap

직원 정보를 B+ 트리에 저장하고, 힙과 선택 트리를 이용해 직원을 관리합니다.

- 주요 자료구조: B+ 트리, 힙, 선택 트리
- 주요 명령: `LOAD`, `PRINT_BP`, `ADD_ST`, `PRINT_ST`, `DELETE`, `EXIT`
- 데이터 파일: `employee.txt`

### Graph Traversal

인접 리스트 또는 인접 행렬로 그래프를 구성하고 탐색 및 경로 계산을 수행합니다.

- 주요 자료구조: 인접 리스트, 인접 행렬
- 주요 알고리즘: BFS, DFS, Kruskal, Dijkstra, Bellman-Ford, Floyd
- 데이터 파일: `graph_L.txt`, `graph_M.txt`

## 빌드 및 실행

각 폴더에는 별도의 `main.cpp`가 있습니다. 실행하려는 프로젝트 폴더의 `.cpp` 파일들을 함께 빌드하고, 다른 프로젝트의 소스 파일은 함께 빌드하지 않습니다.

세 프로그램은 실행 시 현재 작업 폴더에서 `command.txt`를 읽습니다. 기존 저장소에는 이 파일이 포함되어 있지 않으므로, 실행 전에 사용할 명령을 `command.txt`에 작성해야 합니다.

프로젝트 별 데이터 파일도 실행 시 현재 작업 폴더에서 읽습니다. 실행 결과는 각 프로그램이 사용하는 `log.txt`에서 확인할 수 있습니다.
