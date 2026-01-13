/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *\
* All Rights Reserved.                                                        *\
*                                                                             *\
* This document contains proprietary and confidential information.  No        *\
* parts of this document or the computer program it embodies may be in        *\
* any way copied, duplicated, reproduced, translated into a different         *\
* programming language, or or distributed to any person, company, or             *\
* corporation without the prior written consent of Future Entertainment World *\
\*****************************************************************************/

// This file implements the pathfinding logic for the dragon2022-master project.
// It has been refactored to use a more modern C++ class structure (Pathfinder)
// and more efficient data structures (Min-Heap for OPEN list, Hash Table for CLOSED list)
// to improve performance and maintainability compared to the original linked-list based A* implementation.

#include "stdafx.h"
#include "path.h" // Contains declarations for Pathfinder class and related structs
#include "map.h"
#include "Dragon.h"
#include "Hong_Sub.h"
#include "Item.h"
#include "CharDataTable.h"

#include <stdlib.h> // Required for abs() function used in heuristic calculations

// External variable, likely a tool-related flag for pathfinding behavior.
// Its usage is retained as per original code.

// Global collision line data. This remains global as it's not specific to a single
// pathfinding instance and represents static game world data.
int  PathCollisionLineCount;
COLLISIONLINE PathCollisionLine[ MAX_ITEM_COLLISION ];

///////////////////////////////////////////////////////////////////////////////
// MinHeap Implementation - Used for the A* algorithm's OPEN list
// A min-heap efficiently provides the node with the lowest 'f' cost (g + h),
// which is crucial for A* performance.
///////////////////////////////////////////////////////////////////////////////

// Initializes a MinHeap structure.
// Allocates memory for the array of node pointers.
void Pathfinder::HeapInit(MinHeap* heap, int capacity)
{
    heap->nodes = new(std::nothrow) LPSP_NODE[capacity]();

    if (!heap->nodes) {
        heap->size = 0;
        heap->capacity = 0;
        return;
    }

    heap->size = 0;
    heap->capacity = capacity;
}

// Destroys a MinHeap structure.
// Frees the memory allocated for the array of node pointers.
// Note: The actual SP_NODE objects pointed to by 'nodes' are freed by the Pathfinder's destructor,
// not by this HeapDestroy function, to prevent double-free issues.
void Pathfinder::HeapDestroy(MinHeap* heap)
{
    if (heap->nodes)
    {
        delete[] heap->nodes; // 對應 new[] 的釋放
        heap->nodes = NULL;
    }
    heap->size = 0;
    heap->capacity = 0;
}

// Swaps two nodes in the heap array.
void Pathfinder::HeapSwap(MinHeap* heap, int i, int j)
{
    LPSP_NODE temp = heap->nodes[i];
    heap->nodes[i] = heap->nodes[j];
    heap->nodes[j] = temp;
}

// Restores the min-heap property by moving a node up the heap.
// Called after inserting a new node.
void Pathfinder::HeapifyUp(MinHeap* heap, int index)
{
    int parent = (index - 1) / 2;
    // Continue moving up as long as the current node's 'f' cost is less than its parent's
    while (index > 0 && heap->nodes[index]->f < heap->nodes[parent]->f)
    {
        HeapSwap(heap, index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

// Restores the min-heap property by moving a node down the heap.
// Called after extracting the minimum node (root).
void Pathfinder::HeapifyDown(MinHeap* heap, int index)
{
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int smallest = index;

    // Find the smallest among current node, left child, and right child
    if (leftChild < heap->size && heap->nodes[leftChild]->f < heap->nodes[smallest]->f)
    {
        smallest = leftChild;
    }
    if (rightChild < heap->size && heap->nodes[rightChild]->f < heap->nodes[smallest]->f)
    {
        smallest = rightChild;
    }

    // If the smallest is not the current node, swap and continue heapifying down
    if (smallest != index)
    {
        HeapSwap(heap, index, smallest);
        HeapifyDown(heap, smallest);
    }
}

// Inserts a new node into the heap.
// Places it at the end and then calls HeapifyUp to maintain heap property.
void Pathfinder::HeapInsert(MinHeap* heap, LPSP_NODE node)
{
    if (heap->size >= heap->capacity)
    {
        // Error: Heap is full. In a real-world scenario, this might require resizing
        // the heap array or handling the pathfinding failure.
        return;
    }
    heap->nodes[heap->size] = node;
    HeapifyUp(heap, heap->size);
    heap->size++;
}

// Extracts the node with the minimum 'f' cost (which is always the root of the min-heap).
// Replaces the root with the last node and then calls HeapifyDown.
LPSP_NODE Pathfinder::HeapExtractMin(MinHeap* heap)
{
    if (heap->size == 0)
    {
        return NULL; // Heap is empty
    }
    LPSP_NODE minNode = heap->nodes[0]; // The node with the smallest 'f' cost
    heap->size--; // Decrease size
    heap->nodes[0] = heap->nodes[heap->size]; // Move last node to root
    HeapifyDown(heap, 0); // Restore heap property
    return minNode;
}

///////////////////////////////////////////////////////////////////////////////
// HashTable Implementation - Used for the A* algorithm's CLOSED list
// A hash table provides efficient O(1) average-case lookup for visited nodes,
// preventing redundant processing and cycles.
///////////////////////////////////////////////////////////////////////////////

// Initializes a HashTable structure.
// Allocates memory for the array of buckets (linked list heads).
void Pathfinder::HashTableInit(HashTable* table, int capacity)
{
    //table->buckets = (LPSP_NODE*)GlobalAlloc(GPTR, sizeof(LPSP_NODE) * capacity);
    table->buckets = new(std::nothrow) LPSP_NODE[capacity]();

    if (!table->buckets) {
        table->size = 0;
        table->capacity = 0;
        return;
    }

    for (int i = 0; i < capacity; ++i)
    {
        table->buckets[i] = NULL; // Initialize all buckets to NULL
    }
    table->size = 0;
    table->capacity = capacity;
}

// Destroys a HashTable structure.
// Frees the memory allocated for the array of buckets.
// Note: The actual SP_NODE objects are freed by the Pathfinder's destructor.
void Pathfinder::HashTableDestroy(HashTable* table)
{
    if (table->buckets)
    {
        // The SP_NODE objects themselves are freed by the Pathfinder destructor.
        // This function only frees the array of bucket pointers.
        delete [] table->buckets;
        table->buckets = NULL;
    }
    table->size = 0;
    table->capacity = 0;
}

// Simple hash function for a tile number.
// Distributes tile numbers across the hash table buckets.
unsigned int Pathfinder::HashFunction(int tilenum)
{
    return (unsigned int)tilenum % m_closedListHash.capacity;
}

// Inserts a node into the hash table.
// Uses separate chaining to handle collisions.
void Pathfinder::HashTableInsert(HashTable* table, LPSP_NODE node)
{
    unsigned int index = HashFunction(node->number);
    // Check if node already exists in this bucket (should ideally not happen for CLOSED list
    // if A* logic is strictly followed, but good for robustness).
    LPSP_NODE current = table->buckets[index];
    while (current != NULL)
    {
        if (current->number == node->number) return; // Node already in hash table
        current = current->lpNext;
    }
    // Add to the front of the linked list in the bucket
    node->lpNext = table->buckets[index];
    table->buckets[index] = node;
    table->size++;
}

// Finds a node in the hash table by its tile number.
LPSP_NODE Pathfinder::HashTableFind(HashTable* table, int tilenum)
{
    unsigned int index = HashFunction(tilenum);
    LPSP_NODE current = table->buckets[index];
    while (current != NULL)
    {
        if (current->number == tilenum)
        {
            return current; // Node found
        }
        current = current->lpNext;
    }
    return NULL; // Node not found
}

// Removes a node from the hash table by its tile number.
// Necessary when a node is moved from the CLOSED list back to the OPEN list (re-opened).
void Pathfinder::HashTableRemove(HashTable* table, int tilenum)
{
    unsigned int index = HashFunction(tilenum);
    LPSP_NODE current = table->buckets[index];
    LPSP_NODE prev = NULL;

    // Traverse the linked list in the bucket to find the node
    while (current != NULL && current->number != tilenum)
    {
        prev = current;
        current = current->lpNext;
    }

    if (current != NULL) // Node found
    {
        if (prev == NULL) // Node is the head of the list
        {
            table->buckets[index] = current->lpNext;
        }
        else // Node is in the middle or end of the list
        {
            prev->lpNext = current->lpNext;
        }
        table->size--;
        // Important: The SP_NODE object itself is NOT freed here.
        // Its memory is managed by the Pathfinder's destructor.
    }
}

///////////////////////////////////////////////////////////////////////////////
// Pathfinder Class Implementation - Encapsulates the A* pathfinding algorithm
// This class manages the OPEN and CLOSED lists, and performs the path search.
///////////////////////////////////////////////////////////////////////////////

// Pathfinder constructor.
// Initializes the MinHeap for the OPEN list and the HashTable for the CLOSED list.
Pathfinder::Pathfinder(LPCHARACTER character, int mapWidth)
    : m_lpCharacter(character), m_mapWidth(mapWidth)
{
    HeapInit(&m_openListHeap, HEAP_MAX_SIZE);
    HashTableInit(&m_closedListHash, HASH_TABLE_SIZE);
}

// Pathfinder destructor.
// Responsible for freeing all SP_NODE objects that were allocated during pathfinding.
// It iterates through both the OPEN list (heap) and CLOSED list (hash table)
// to ensure all dynamically allocated nodes are deallocated exactly once.
Pathfinder::~Pathfinder()
{
    // Free all nodes that are still in the heap (OPEN list).
    // These are nodes that were generated but not yet processed or part of the final path.
    /*for (int i = 0; i < m_openListHeap.size; ++i)
    {
        GlobalFree(m_openListHeap.nodes[i]);
    }*/
    HeapDestroy(&m_openListHeap); // Free the heap's internal array

    // Free all nodes that are still in the hash table (CLOSED list).
    // These are nodes that have been processed and are not in the OPEN list.
    /*for (int i = 0; i < m_closedListHash.capacity; ++i)
    {
        LPSP_NODE current = m_closedListHash.buckets[i];
        while (current != NULL)
        {
            LPSP_NODE next = current->lpNext;
            GlobalFree(current); // Free the SP_NODE object
            current = next;
        }
    }*/
    HashTableDestroy(&m_closedListHash); // Free the hash table's internal array of buckets
}

// The core A* pathfinding algorithm.
// Finds a path from (sx, sy) to (dx, dy) for the character associated with this Pathfinder instance.
// Returns the destination node (LPSP_NODE) if a path is found, otherwise NULL.
LPSP_NODE Pathfinder::FindPath(int sx, int sy, int dx, int dy, bool isMonster, int maxIterations)
{
    int         nDestTile = dy * m_mapWidth + dx; // Calculate unique tile number for destination
    int         c = 0; // Iteration counter to prevent infinite loops on complex maps

    // Allocate and initialize the starting node
    std::unique_ptr<SP_NODE> startNodePtr(new SP_NODE());
    if (startNodePtr == nullptr) {
        return nullptr; // new 失敗
    }

    // 將所有權轉移到容器
    m_allNodes.push_back(std::move(startNodePtr));
    LPSP_NODE startNode = m_allNodes.back().get(); // 取得原生指標供 Heap 使用

    startNode->g = 0; // Cost from start to current node
    // Heuristic: Manhattan distance (abs(dx) + abs(dy)).
    // This is an admissible heuristic for 8-directional movement with varying costs (straight/diagonal).
    startNode->h = abs(sx - dx) + abs(sy - dy); // Estimated cost from current node to destination
    startNode->f = startNode->g + startNode->h; // Total estimated cost
    startNode->number = sy * m_mapWidth + sx; // Unique identifier for the tile
    startNode->x = sx;
    startNode->y = sy;
    startNode->lpParent = NULL; // Start node has no parent
    for (int i = 0; i < 8; ++i) startNode->lpChild[i] = NULL; // Initialize child pointers
    startNode->lpNext = NULL; // Used for hash table chaining, ensure it's NULL initially

    HeapInsert(&m_openListHeap, startNode); // Add the starting node to the OPEN list (min-heap)

    LPSP_NODE currentNode = NULL;

    // Main A* loop
    while (m_openListHeap.size > 0) // Continue as long as there are nodes to evaluate
    {
        c++; // Increment iteration counter
        if (c > maxIterations) // Check for iteration limit
        {
            // Pathfinding took too long or no path found within limits.
            // Nodes are NOT freed here; the Pathfinder's destructor will handle cleanup.
            while(m_openListHeap.size > 0) HeapExtractMin(&m_openListHeap); // Clear remaining nodes from heap
            return NULL; // Return NULL to indicate no path found
        }

        currentNode = HeapExtractMin(&m_openListHeap); // Get the node with the lowest 'f' cost from OPEN list

        // Check if the current node is the destination
        if (currentNode->number == nDestTile)
        {
            // Path found!
            // Nodes are NOT freed here; the Pathfinder's destructor will handle cleanup.
            while(m_openListHeap.size > 0) HeapExtractMin(&m_openListHeap); // Clear remaining nodes from heap
            return currentNode; // Return the destination node to reconstruct the path
        }

        HashTableInsert(&m_closedListHash, currentNode); // Move current node from OPEN to CLOSED list

        int oldx = currentNode->x;
        int oldy = currentNode->y;

        // Generate successors (neighbors) for the current node (8 directions)
        // dx_offsets and dy_offsets define the relative coordinates for 8-directional movement.
        int dx_offsets[] = {0, -1, 1, 0, 1, 1, -1, -1}; // N, W, E, S, NE, SE, SW, NW
        int dy_offsets[] = {-1, 0, 0, 1, -1, 1, 1, -1};
        // move_costs define the cost to move to a neighbor.
        // Straight moves (N, W, E, S) cost 2. Diagonal moves (NE, SE, SW, NW) cost 3.
        // This matches the original game's cost model.
        int move_costs[] = {2, 2, 2, 2, 3, 3, 3, 3};

        for (int i = 0; i < 8; ++i) // Iterate through all 8 possible neighbors
        {
            int x = oldx + dx_offsets[i];
            int y = oldy + dy_offsets[i];
            int current_dist = move_costs[i]; // Cost to move to this neighbor
            int neighborTileNum = y * m_mapWidth + x; // Unique tile number for the neighbor

            // Check for collision using the external FreeTile function.
            // FreeTile determines if a tile is walkable.
            if (!FreeTile(m_lpCharacter, oldx, oldy, x, y)) continue; // If not free (obstacle), skip this neighbor

            // Check if the neighbor is already in the CLOSED list
            LPSP_NODE neighbor = HashTableFind(&m_closedListHash, neighborTileNum);
            if (neighbor != NULL) // Neighbor is in CLOSED list
            {
                // If a shorter path to this neighbor is found, re-evaluate it.
                if (currentNode->g + current_dist < neighbor->g)
                {
                    // Move the neighbor from CLOSED back to OPEN (re-open it)
                    HashTableRemove(&m_closedListHash, neighborTileNum); // Remove from CLOSED
                    neighbor->lpParent = currentNode; // Update parent
                    neighbor->g = currentNode->g + current_dist; // Update g-cost
                    neighbor->h = abs(x - dx) + abs(y - dy); // Re-calculate heuristic (Manhattan)
                    neighbor->f = neighbor->g + neighbor->h; // Update f-cost
                    HeapInsert(&m_openListHeap, neighbor); // Add back to OPEN
                }
                continue; // Already processed or re-opened, move to next neighbor
            }

            // Check if the neighbor is already in the OPEN list
            // For a simple heap, checking for existence in OPEN is O(N) in worst case.
            // A more advanced A* implementation might use a hash table for OPEN list as well
            // to get O(1) lookup, or store node indices in the heap for O(logN) updates.
            // For this C-style context, we iterate.
            bool foundInOpen = false;
            int openListIndex = -1;
            for(int k = 0; k < m_openListHeap.size; ++k)
            {
                if(m_openListHeap.nodes[k]->number == neighborTileNum)
                {
                    neighbor = m_openListHeap.nodes[k];
                    foundInOpen = true;
                    openListIndex = k; // <--- 🌟 在這裡賦值
                    break;
                }
            }

            if (foundInOpen) // Neighbor is already in OPEN list
            {
                // If a shorter path to this neighbor is found, update its costs.
                if (currentNode->g + current_dist < neighbor->g)
                {
                    neighbor->lpParent = currentNode; // Update parent
                    neighbor->g = currentNode->g + current_dist; // Update g-cost
                    neighbor->h = abs(x - dx) + abs(y - dy); // Re-calculate heuristic (Manhattan)
                    neighbor->f = neighbor->g + neighbor->h; // Update f-cost
                    // Re-inserting creates a duplicate in the heap. The A* algorithm will still work
                    // correctly because the node with the lower 'f' cost will be extracted first.
                    // The duplicate with a higher 'f' cost will be ignored when extracted later
                    // because it will already be in the CLOSED list.
                    //HeapInsert(&m_openListHeap, neighbor); //好像是這裡有時候會崩潰
                    HeapifyUp(&m_openListHeap, openListIndex);
                }
            }
            else // Neighbor is a new node (not in OPEN or CLOSED)
            {
                // Allocate and initialize a new node for the neighbor
                // 2. 分配新節點
                std::unique_ptr<SP_NODE> newNeighborPtr(new SP_NODE());
                if (newNeighborPtr == nullptr) continue;

                m_allNodes.push_back(std::move(newNeighborPtr));
                LPSP_NODE newNeighbor = m_allNodes.back().get(); // 取得原生指標

                newNeighbor->lpParent = currentNode; // Set current node as its parent
                newNeighbor->g = currentNode->g + current_dist; // Calculate g-cost
                newNeighbor->h = abs(x - dx) + abs(y - dy); // Calculate heuristic (Manhattan)
                newNeighbor->f = newNeighbor->g + newNeighbor->h; // Calculate f-cost
                newNeighbor->x = x;
                newNeighbor->y = y;
                newNeighbor->number = neighborTileNum; // Set unique tile number
                for (int j = 0; j < 8; ++j) newNeighbor->lpChild[j] = NULL; // Initialize child pointers
                newNeighbor->lpNext = NULL; // For hash table chaining

                HeapInsert(&m_openListHeap, newNeighbor); // Add the new neighbor to the OPEN list
            }
        }
    }

    // If the loop finishes and the destination was not reached, no path was found.
    // Nodes are NOT freed here; the Pathfinder's destructor will handle cleanup.
    while(m_openListHeap.size > 0) HeapExtractMin(&m_openListHeap); // Clear remaining nodes from heap
    return NULL; // Return NULL to indicate no path found
}

// Original global PathDestroy function is now replaced by Pathfinder's destructor.
// Its functionality is integrated into the class's memory management.
// void PathDestroy( void ) { ... }

// FreeTile and other collision functions remain external.
// These functions depend on global map data (TileMap, g_Map, g_Item, Hero, NPCList, ItemList, Doors)
// and are used by the Pathfinder class for collision detection.
// Their implementations are kept as is from the original file, with minor adjustments for parameters.

// FreeTile: Determines if a given tile (x, y) is free for movement.
// It checks for map boundaries, 'attr_dont' (unwalkable terrain),
// 'occupied' status by other characters (unless 'JoinLocalWar' is true),
// 'attr_inside' (if character cannot enter inside areas), and collisions with items (doors, boxes).
int FreeTile(LPCHARACTER ch, int sx, int sy, int x, int y)
{
    TILE  *pT;
    pT = &TileMap[x][y];
    if (pT->attr_dont)     return 0;

    if (!ch->JoinLocalWar && pT->occupied)
    {
        return 0;
    }

#define MAX_AREA_X_        34 // Defines a maximum area X for character movement checks
#define MAX_AREA_Y_        29 // Defines a maximum area Y for character movement checks

    if (ch)
    {
        int tx, ty;
        tx = ch->x / TILE_SIZE - MAX_AREA_X_;
        ty = ch->y / TILE_SIZE - MAX_AREA_Y_;
        //SDL_Log("%d %d tx=%d,ty=%d", ch->x, ch->y, tx, ty);
        if (x < tx) return 0;
        if (y < ty) return 0;

        if (x > tx + MAX_AREA_X_ + MAX_AREA_X_) return 0;
        if (y > ty + MAX_AREA_Y_ + MAX_AREA_Y_) return 0;

        if (ch->notcomeinside) // Character cannot enter 'inside' areas
        {
            if (pT->attr_inside) return 0;
        }
    }

    // Basic map boundary checks
    if (x < 0) return 0;
    if (y < 0) return 0;
    if (x >= g_Map.file.wWidth)  return 0;
    if (y >= g_Map.file.wHeight) return 0;

    // Special case for Ghost view type (can pass through obstacles)
    if (ch)
        if (Hero) // Assuming 'Hero' is a global pointer to the player character
        {
            if (ch == Hero && Hero->viewtype == VIEWTYPE_GHOST_)
            {
                return 1; // Ghost can move freely
            }
        }

    // Collision check with ground items (doors, boxes)
    LPITEMGROUND  i = g_Item; // Assuming g_Item is a global linked list of ground items
    while (i != NULL)
    {
        switch (i->type)
        {
        case ITEMTYPE_BOX:
            // Original code had a break here, implying no collision check for boxes
            // or it's handled elsewhere. Retaining original behavior.
            break;

        case ITEMTYPE_DOOR:
            if (i->Anitype == ITEMANIMATION_CLOSED) // Only closed doors block movement
            {
                // CheckIntersect is a collision detection function.
                // It checks if the line segment from (sx,sy) to (x,y) intersects with the door.
                if (CheckIntersect(i->ddx[0], i->ddy[0],
                    i->dsx[0], i->dsy[0], (sx << 5) + 16, (sy << 5) + 16, (x << 5) + 16, (y << 5) + 16) > 0)
                {
                    return 0; // Collision with a closed door
                }
            }
        }
        i = i->next;
    }

    return 1; // Tile is free
}

// Tile_Collision: Checks for collision with other characters or 'dont' tiles along a given direction.
// This function seems to be used for direct movement checks rather than pathfinding.
int Tile_Collision(LPCHARACTER ch, DIRECTION dir, int Power)
{
    LPCHARACTER lpCharacter = Hero->lpNext; // Start checking from the character after Hero

    int t_sx = ch->x / TILE_SIZE; // Current tile X of the character
    int t_sy = ch->y / TILE_SIZE; // Current tile Y of the character

    int how_X = 0, how_Y = 0; // Direction offsets

    // Determine X and Y offsets based on the movement direction
    switch (dir)
    {
        case DIRECTION_DOWN:        how_X = 0;    how_Y = 1;    break;
        case DIRECTION_LEFTDOWN:    how_X = -1;    how_Y = 1;    break;
        case DIRECTION_LEFT:        how_X = -1;    how_Y = 0;    break;
        case DIRECTION_LEFTUP:        how_X = -1;    how_Y = -1;    break;
        case DIRECTION_UP:            how_X = 0;    how_Y = -1;    break;
        case DIRECTION_RIGHTUP:        how_X = 1;    how_Y = -1;    break;
        case DIRECTION_RIGHT:        how_X = 1;    how_Y = 0;    break;
        case DIRECTION_RIGHTDOWN:    how_X = 1;    how_Y = 1;    break;
    }

    // Iterate through characters (starting from Hero's next) to check for collisions
    while (lpCharacter != NULL)
    {
        // Check tiles along the movement path up to 'Power' distance
        for (int i = 1; i < Power; i++)
        {
            t_sx += how_X; // Advance tile X
            t_sy += how_Y; // Advance tile Y

            // Check if the tile is an unwalkable 'dont' tile
            if (TileMap[t_sx][t_sy].attr_dont)
            {
                if (i == 1)
                    return -1; // Collision with 'dont' tile at first step
                else
                    return (i - 1); // Collision with 'dont' tile at (i-1) steps
            }

            // Check for collision with other characters
            if (lpCharacter != ch) // Ensure not checking collision with self
                if (ch->x/TILE_SIZE + t_sx == lpCharacter->x/TILE_SIZE && ch->y/TILE_SIZE + t_sy == lpCharacter->y/TILE_SIZE)
                    return(i - 1); // Collision with another character
        }
        lpCharacter = lpCharacter->lpNext; // Move to the next character in the list
    }
    return Power; // No collision found within 'Power' distance
}

// FreeTileLineCheck: Similar to FreeTile but specifically for line-of-sight or line-based collision checks.
// It checks map boundaries, 'attr_dont', 'attr_inside' (if applicable), and closed doors.
int FreeTileLineCheck(LPCHARACTER ch, int sx, int sy, int x, int y)
{
    if (TileMap[x][y].attr_dont)     return 0; // Check for unwalkable terrain

    // Map boundary checks
    if (x < 0) return 0;
    if (y < 0) return 0;
    if (x >= g_Map.file.wWidth)  return 0;
    if (y >= g_Map.file.wHeight) return 0;

    if (ch)
        if (ch->notcomeinside) // Character cannot enter 'inside' areas
        {
            if (TileMap[x][y].attr_inside) return 0;
        }

    // Calculate pixel coordinates for the center of the tile
    int mx = x * TILE_SIZE + 15;
    int my = y * TILE_SIZE + 15;

    // Collision check with ground items (doors)
    LPITEMGROUND  i = g_Item;
    while (i != NULL)
    {
        switch (i->type)
        {
        case ITEMTYPE_DOOR:
            if (i->Anitype == ITEMANIMATION_CLOSED) // Only closed doors block movement
            {
                // CheckIntersect checks if the line segment from (sx,sy) to (x,y) intersects with the door.
                if (CheckIntersect(i->ddx[0], i->ddy[0],
                    i->dsx[0], i->dsy[0], (sx << 5) + 16, (sy << 5) + 16, (x << 5) + 16, (y << 5) + 16) > 0)
                {
                    return 0; // Collision with a closed door
                }
            }
        }
        i = i->next;
    }

    return 1; // Tile is free for line check
}

// FreeTileOther: Checks for collision with other characters or specific item types (boxes).
// This function seems to be for general tile occupancy checks not directly related to pathfinding.
int FreeTileOther(LPCHARACTER ch, int x, int y)
{
    LPCHARACTER lpCharacter = Hero; // Start checking from Hero
    DWORD iid;

    iid = ch->id;
    if (iid >= 10000) iid -= 10000; // Adjust character ID if necessary

    if (TileMap[x][y].attr_dont) return 0; // Check for unwalkable terrain

    // Calculate pixel coordinates for the center of the tile
    int mx = x * TILE_SIZE + 15;
    int my = y * TILE_SIZE + 15;

    // Collision check with ground items (boxes)
    LPITEMGROUND  i = g_Item;
    while (i != NULL)
    {
        switch (i->type)
        {
        case ITEMTYPE_BOX:
            if (i->Anitype == ITEMANIMATION_CLOSED) // Only closed boxes block movement
            {
                // IsInBox checks if a point (mx, my) is inside a given box defined by 4 points.
                if (IsInBox(i->dx[0], i->dy[0], i->dx[1], i->dy[1], i->dx[2], i->dy[2], i->dx[3], i->dy[3], mx, my) > 0)
                    return 0; // Collision with a closed box
            }
        }
        i = i->next;
    }

    return 1; // Tile is free
}

// CheckLineCollision: Implements a Bresenham-like line algorithm to check for collisions
// between a start tile (sx, sy) and an end tile (ex, ey).
// It iterates through each tile on the line and uses FreeTile to check for obstacles.
int CheckLineCollision(LPCHARACTER ch, int sx, int sy, int ex, int ey)
{
    int t, distance;
    int xerr = 0, yerr = 0, delta_x, delta_y;
    int incx, incy;

    // Basic boundary checks for start and end points
    if (sx < 0) return 0;
    if (sy < 0) return 0;
    if (ex >= g_Map.file.wWidth)  return 0;
    if (ey >= g_Map.file.wHeight) return 0;

    delta_x = ex - sx;
    delta_y = ey - sy;

    // Determine increment directions (1, -1, or 0)
    incx = (delta_x > 0) ? 1 : ((delta_x == 0) ? 0 : -1);
    incy = (delta_y > 0) ? 1 : ((delta_y == 0) ? 0 : -1);

    delta_x = abs(delta_x);
    delta_y = abs(delta_y);

    // Determine the longer axis to iterate over
    distance = (delta_x > delta_y) ? delta_x : delta_y;

    bool occupied_flag = false;
    // Temporarily mark current character's tile as unoccupied to allow pathfinding through it
    TILE *pT = &TileMap[ch->position.x][ch->position.y];
    if (pT->occupied)
    {
        pT->occupied = 0;
        occupied_flag = true;
    }

    // Check the starting tile itself
    if( !FreeTile( ch, sx, sy , sx, sy ) ) goto FALSE_;

    // Iterate along the line using a simplified Bresenham's algorithm
    for(t=0; t<=distance; t++)
    {
        xerr += delta_x;
        yerr += delta_y;
        if(xerr >= distance)    {        xerr -= distance;       sx += incx;      }
        if(yerr >= distance)    {        yerr -= distance;       sy += incy;      }
        // Check each tile on the line for freeness
        if( !FreeTile( ch, sx, sy , sx, sy ) ) goto FALSE_;
    }

    // Restore original occupied status of character's tile
    if( occupied_flag )    pT->occupied = 1;

    return 1; // Line is clear

FALSE_:
    // Restore original occupied status if a collision was found
    if( occupied_flag )    pT->occupied = 1;
    return 0; // Line is blocked
}

// PathBuild: The main entry point for pathfinding.
// This function is called externally to find a character for a character.
// It now uses the refactored Pathfinder class internally.
BOOL PathBuild(LPCHARACTER lpCharacter) {
  int sx, sy;
  int dx, dy;

  dx = lpCharacter->destx;
  dy = lpCharacter->desty;

  // If character is in a specific action state (e.g., casting magic), pathfinding might be disabled.
  if (lpCharacter->nCurrentAction == ACTION_MAGIC_BEFORE) return FALSE;

  sx = lpCharacter->x / TILE_SIZE;
  sy = lpCharacter->y / TILE_SIZE;

  bool occupied_flag = false;
  // Get the tile where the character is currently located
  TILE* pT = &TileMap[lpCharacter->position.x][lpCharacter->position.y];

  // Temporarily mark the character's current tile as unoccupied.
  // This prevents the pathfinding algorithm from considering the character's own position as an obstacle.
  if (pT->occupied) {
    pT->occupied = 0;
    occupied_flag = true;
  }

  // Instantiate the Pathfinder class.
  // The Pathfinder object will manage the A* algorithm's state and memory.
  // It takes the character and map width as parameters.
  Pathfinder pathfinder(lpCharacter, g_Map.file.wWidth);
  LPSP_NODE lpspNode = NULL; // Pointer to the destination node returned by pathfinding

  // Determine if the pathfinding is for a monster or a player.
  // This flag can be used by FindPath to adjust behavior (e.g., different movement costs).
  bool isMonster = (lpCharacter->sprno != 0 && lpCharacter->sprno != 1);
  // Call the Pathfinder's FindPath method to perform the A* search.
  lpspNode = pathfinder.FindPath(sx, sy, dx, dy, isMonster);

  // Restore the original occupied status of the character's tile.
  if (occupied_flag) pT->occupied = 1;

  // If lpspNode is NULL, no path was found.
  if (lpspNode == NULL) {
    // The Pathfinder's destructor will automatically handle freeing of allocated nodes.
    // No explicit PathDestroy() call is needed here.
    return FALSE;
  }

  // Path found! Reconstruct the path by traversing parent pointers from the destination node.
  // The path is stored in lpCharacter->path array.
  lpCharacter->path[0][0] = dx; // Destination tile X
  lpCharacter->path[0][1] = dy; // Destination tile Y
  lpCharacter->pathcount = 1; // Initialize path length

  // Traverse up the parent chain until the starting node is reached (lpParent == NULL).
  while (lpspNode->lpParent != NULL) {
    // Check for buffer overflow in lpCharacter->path array.
    if (lpCharacter->pathcount >= MAX_SHORTPATH) {
      break; // Path is too long, truncate
    }

    lpspNode = lpspNode->lpParent; // Move to the parent node
    lpCharacter->path[lpCharacter->pathcount][0] = lpspNode->x; // Store parent's X
    lpCharacter->path[lpCharacter->pathcount][1] = lpspNode->y; // Store parent's Y
    lpCharacter->pathcount++; // Increment path length
  }

  // The Pathfinder object (pathfinder) goes out of scope here,
  // and its destructor (~Pathfinder()) is automatically called,
  // which handles the cleanup of all allocated SP_NODE objects.
  // No explicit PathDestroy() call is needed.

  // Return TRUE if a path of more than one step was found.
  // A pathcount of 1 means the character is already at the destination.
  if (lpCharacter->pathcount > 1) {
    return TRUE;
  }

  return FALSE; // No effective path found (e.g., already at destination)
}