/*

*/


#define MAX_ITEM_COLLISION		100


#ifndef	__PATH_H__
#define	__PATH_H__

#include "dragon.h"
#include "char.h"
#include <vector> // for the ownership container
#include <memory> // for std::unique_ptr

// SP_NODE and SP_STACK definitions remain global for now,
// but their instances will be managed by Pathfinder class.
typedef struct	tagSPNODE
{
	int					f, h, g;
	int					x, y;
	int					number;
	struct tagSPNODE	*lpParent;
	struct tagSPNODE	*lpChild[ 8 ];
	struct tagSPNODE	*lpNext;
} SP_NODE, *LPSP_NODE;

// SP_STACK is no longer needed with priority queue and hash map
// typedef struct	tagSPSTACK
// {
//	struct tagSPNODE	*lpNode;
//	struct tagSPSTACK	*lpNext;
// } SP_STACK, *LPSP_STACK;


typedef struct  tagColisionLine
{
	int sx, sy, dx, dy;
}COLLISIONLINE, *LPCOLLISIONLINE;


#ifdef __cplusplus
extern "C"
{
#endif

	extern int  PathCollisionLineCount;
	extern COLLISIONLINE PathCollisionLine[ MAX_ITEM_COLLISION ];

	extern int			FreeTile( LPCHARACTER lpcharacter, int sx, int sy, int x, int y );
	extern int			FreeTileOther( LPCHARACTER ch, int sx, int sy );
	extern int CheckLineCollision( LPCHARACTER ch, int sx, int sy, int ex, int ey );
	extern int			Tile_Collision(LPCHARACTER ch, DIRECTION dir, int Power);

#ifdef __cplusplus
}
#endif

// Pathfinder class definition
#ifdef __cplusplus

// Simple Min-Heap for Priority Queue
#define HEAP_MAX_SIZE 10000 // Max nodes in heap
typedef struct tagMinHeap
{
    LPSP_NODE* nodes; // Array of pointers to SP_NODE
    int size;
    int capacity;
} MinHeap;

// Simple Hash Table for Closed List
#define HASH_TABLE_SIZE 10007 // A prime number for hash table size
typedef struct tagHashTable
{
    LPSP_NODE* buckets; // Array of linked list heads
    int size;
    int capacity;
} HashTable;

class Pathfinder
{
private:
    MinHeap     m_openListHeap;   // Priority queue for OPEN list
    HashTable   m_closedListHash; // Hash table for CLOSED list
    LPCHARACTER m_lpCharacter;    // Character for whom path is being found
    int         m_mapWidth;       // Map width for tile number calculation

    std::vector<std::unique_ptr<SP_NODE>> m_allNodes;

    // Helper functions for MinHeap
    void HeapInit(MinHeap* heap, int capacity);
    void HeapDestroy(MinHeap* heap);
    void HeapInsert(MinHeap* heap, LPSP_NODE node);
    LPSP_NODE HeapExtractMin(MinHeap* heap);
    void HeapifyUp(MinHeap* heap, int index);
    void HeapifyDown(MinHeap* heap, int index);
    void HeapSwap(MinHeap* heap, int i, int j);

    // Helper functions for HashTable
    void HashTableInit(HashTable* table, int capacity);
    void HashTableDestroy(HashTable* table);
    unsigned int HashFunction(int tilenum);
    void HashTableInsert(HashTable* table, LPSP_NODE node);
    LPSP_NODE HashTableFind(HashTable* table, int tilenum);
    void HashTableRemove(HashTable* table, int tilenum);
    // LPSP_NODE HashTableRemove(HashTable* table, int tilenum); // Not strictly needed for A* but good to have

public:
    Pathfinder(LPCHARACTER character, int mapWidth);
    ~Pathfinder(); // Destructor for cleanup

    // Merged PathFind function
    LPSP_NODE   FindPath( int sx, int sy, int dx, int dy, bool isMonster = false, int maxIterations = 1000 );
};

// PathBuild will now use the Pathfinder class
extern BOOL PathBuild( LPCHARACTER lpCharacter );

#endif // __cplusplus

#endif	// __PATH_H__
