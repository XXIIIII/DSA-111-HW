#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MAX_KNIGHTS 200001

// Global array to store attack counts for each knight
int attack_counts[MAX_KNIGHTS];

// Structure representing a node in the min-heap (priority queue)
typedef struct heap_node {
    long long health;
    int index;
} heap_node;

// Structure representing a knight with Union-Find and heap functionality
typedef struct knight {
    // Union-Find properties
    int parent;
    int height;
    int size;
    bool is_dead;
    
    // Health heap (min-heap for managing health values)
    heap_node *health_heap;
    int heap_size;
    int heap_array_size;
    
    // Combat properties
    long long health;
    long long attack;
    long long total_attack;
    
    // Set management
    struct knight **set_elements;  // Array of pointers to knights in this set
    int set_array_size;
    int set_size;
    
    // Attack counting
    int attack_count;
    int total_attack_count;
    int damage_threshold;
} knight;

// Global array of all knights
knight knight_array[MAX_KNIGHTS];

/**
 * Creates a new heap node with given health and index
 * @param health - health value for the node
 * @param index - knight index
 * @param initial_size - initial size for heap allocation
 * @return pointer to allocated heap node
 */
heap_node *create_heap_node(long long health, int index, int initial_size) {
    heap_node *node = (heap_node *)malloc(initial_size * sizeof(heap_node));
    assert(node != NULL);
    node[0].health = health;
    node[0].index = index;
    return node;
}

/**
 * Union-Find: Find operation with path compression
 * @param x - knight index to find root of
 * @return root of the set containing x
 */
int find_set_root(int x) {
    if (x != knight_array[x].parent) {
        knight_array[x].parent = find_set_root(knight_array[x].parent);
    }
    return knight_array[x].parent;
}

/**
 * Maintains min-heap property by moving element down
 * @param heap - heap array
 * @param heap_size - current heap size
 * @param cursor - index to start heapifying from
 */
void heapify_down(heap_node *heap, int heap_size, int cursor) {
    int smallest = cursor;
    int left, right;
    
    while (true) {
        left = 2 * cursor + 1;
        right = 2 * cursor + 2;
        
        if (left >= heap_size) break;
        
        // Find smallest among cursor, left child, and right child
        if (left < heap_size && heap[left].health < heap[cursor].health) {
            smallest = left;
        }
        if (right < heap_size && heap[right].health < heap[smallest].health) {
            smallest = right;
        }
        
        if (smallest == cursor) break;
        
        // Swap with smallest child
        long long temp_health = heap[cursor].health;
        heap[cursor].health = heap[smallest].health;
        heap[smallest].health = temp_health;
        
        int temp_index = heap[cursor].index;
        heap[cursor].index = heap[smallest].index;
        heap[smallest].index = temp_index;
        
        cursor = smallest;
    }
}

/**
 * Removes the minimum element from heap
 * @param heap - heap array
 * @param knight_set - index of knight set
 */
void remove_min_from_heap(heap_node *heap, int knight_set) {
    int heap_size = knight_array[knight_set].heap_size;
    
    // Move last element to root
    heap[0].health = heap[heap_size - 1].health;
    heap[0].index = heap[heap_size - 1].index;
    
    knight_array[knight_set].heap_size--;
    heapify_down(heap, knight_array[knight_set].heap_size, 0);
}

/**
 * Processes damage to a set of knights, removing dead ones
 * @param damage - damage to apply
 * @param heap - health heap of the defending set
 * @param defending_set - index of defending knight set
 * @param attacking_set - index of attacking knight set
 */
void apply_damage_and_remove_dead(long long damage, heap_node *heap, int defending_set, int attacking_set) {
    heap[0].health -= damage;
    
    // Remove all knights that die from this attack
    while (heap[0].health <= knight_array[defending_set].damage_threshold) {
        int dying_knight = heap[0].index;
        knight_array[dying_knight].is_dead = true;
        
        // Record number of attacks this knight survived
        attack_counts[dying_knight] = knight_array[find_set_root(dying_knight)].total_attack_count - 
                                     knight_array[dying_knight].attack_count;
        
        // Remove attack power of dead knight
        knight_array[defending_set].total_attack -= knight_array[dying_knight].attack;
        
        // Remove from heap
        if (knight_array[defending_set].heap_size == 1) {
            knight_array[defending_set].heap_size--;
        } else {
            remove_min_from_heap(heap, defending_set);
        }
        
        if (knight_array[defending_set].heap_size == 0) break;
        
        heap[0].health -= damage;
    }
    
    // Restore health after damage calculation
    if (knight_array[defending_set].heap_size != 0) {
        heap[0].health += damage;
    }
    
    // Adjust health values based on set sizes and damage thresholds
    if (knight_array[attacking_set].heap_size > knight_array[defending_set].heap_size) {
        // Large set attacks small set
        for (int i = 0; i < knight_array[defending_set].heap_size; i++) {
            heap[i].health -= (damage - knight_array[attacking_set].damage_threshold + 
                              knight_array[defending_set].damage_threshold);
        }
        if (knight_array[attacking_set].size <= knight_array[defending_set].size) {
            knight_array[defending_set].damage_threshold = knight_array[attacking_set].damage_threshold;
        }
    } else {
        // Small set attacks large set
        for (int i = 0; i < knight_array[attacking_set].heap_size; i++) {
            knight_array[attacking_set].health_heap[i].health += 
                (damage - knight_array[attacking_set].damage_threshold + 
                 knight_array[defending_set].damage_threshold);
        }
        
        if (knight_array[attacking_set].size > knight_array[defending_set].size) {
            knight_array[attacking_set].damage_threshold = damage + knight_array[defending_set].damage_threshold;
        } else {
            knight_array[defending_set].damage_threshold = damage + knight_array[defending_set].damage_threshold;
        }
    }
}

/**
 * Maintains min-heap property by moving element up
 * @param heap - heap array
 * @param heap_size - current heap size
 * @param index - index to start heapifying from
 */
void heapify_up(heap_node *heap, int heap_size, int index) {
    if (index <= 0) return;
    
    int parent = (index - 1) / 2;
    
    if (parent >= 0 && heap[parent].health > heap[index].health) {
        // Swap with parent
        long long temp_health = heap[parent].health;
        heap[parent].health = heap[index].health;
        heap[index].health = temp_health;
        
        int temp_index = heap[parent].index;
        heap[parent].index = heap[index].index;
        heap[index].index = temp_index;
        
        heapify_up(heap, heap_size, parent);
    }
}

/**
 * Inserts a new node into the heap
 * @param target_set - set to insert into
 * @param new_node - node to insert
 */
void insert_into_heap(int target_set, heap_node *new_node) {
    knight_array[target_set].heap_size++;
    int heap_size = knight_array[target_set].heap_size;
    
    // Add new node at the end
    knight_array[target_set].health_heap[heap_size - 1].health = new_node->health;
    knight_array[target_set].health_heap[heap_size - 1].index = new_node->index;
    
    // Maintain heap property
    heapify_up(knight_array[target_set].health_heap, heap_size, heap_size - 1);
}

/**
 * Merges two health heaps
 * @param set_a - first set index
 * @param set_b - second set index
 */
void merge_health_heaps(int set_a, int set_b) {
    if (knight_array[set_a].heap_size > knight_array[set_b].heap_size) {
        // Merge set_b into set_a
        if (knight_array[set_a].heap_array_size < knight_array[set_a].heap_size + knight_array[set_b].heap_size) {
            knight_array[set_a].health_heap = (heap_node *)realloc(
                knight_array[set_a].health_heap, 
                sizeof(heap_node) * ((knight_array[set_a].heap_size + knight_array[set_b].heap_size) * 2)
            );
            knight_array[set_a].heap_array_size = (knight_array[set_a].heap_size + knight_array[set_b].heap_size) * 2;
        }
        
        for (int i = 0; i < knight_array[set_b].heap_size; i++) {
            insert_into_heap(set_a, &knight_array[set_b].health_heap[i]);
        }
        free(knight_array[set_b].health_heap);
        
        if (knight_array[set_a].size < knight_array[set_b].size) {
            knight_array[set_b].health_heap = knight_array[set_a].health_heap;
            knight_array[set_b].heap_size = knight_array[set_a].heap_size;
            knight_array[set_b].heap_array_size = knight_array[set_a].heap_array_size;
        }
    } else {
        // Merge set_a into set_b
        if (knight_array[set_b].heap_array_size < knight_array[set_a].heap_size + knight_array[set_b].heap_size) {
            knight_array[set_b].health_heap = (heap_node *)realloc(
                knight_array[set_b].health_heap, 
                sizeof(heap_node) * ((knight_array[set_a].heap_size + knight_array[set_b].heap_size) * 2)
            );
            knight_array[set_b].heap_array_size = (knight_array[set_a].heap_size + knight_array[set_b].heap_size) * 2;
        }
        
        for (int i = 0; i < knight_array[set_a].heap_size; i++) {
            insert_into_heap(set_b, &knight_array[set_a].health_heap[i]);
        }
        free(knight_array[set_a].health_heap);
        
        if (knight_array[set_a].size > knight_array[set_b].size) {
            knight_array[set_a].health_heap = knight_array[set_b].health_heap;
            knight_array[set_a].heap_size = knight_array[set_b].heap_size;
            knight_array[set_a].heap_array_size = knight_array[set_b].heap_array_size;
        }
    }
}

/**
 * Union-Find: Link operation that merges two sets
 * @param set_a - first set root
 * @param set_b - second set root
 */
void link_sets(int set_a, int set_b) {
    if (set_a == set_b) return;
    
    // Apply damage from set_a to set_b
    apply_damage_and_remove_dead(knight_array[set_a].total_attack, knight_array[set_b].health_heap, set_b, set_a);
    
    if (knight_array[set_a].size > knight_array[set_b].size) {
        // set_a absorbs set_b
        knight_array[set_a].total_attack_count++;
        
        // Update attack counts for absorbed knights
        for (int i = 0; i < knight_array[set_b].set_size; i++) {
            knight_array[set_b].set_elements[i]->attack_count = 
                knight_array[set_a].total_attack_count - knight_array[set_b].total_attack_count + 
                knight_array[set_b].set_elements[i]->attack_count;
        }
        
        // Expand set_elements array if needed
        if (knight_array[set_a].set_array_size < knight_array[set_a].set_size + knight_array[set_b].set_size) {
            knight_array[set_a].set_elements = (knight **)realloc(
                knight_array[set_a].set_elements, 
                sizeof(knight*) * ((knight_array[set_a].set_size + knight_array[set_b].set_size) * 2)
            );
            knight_array[set_a].set_array_size = (knight_array[set_a].set_size + knight_array[set_b].set_size) * 2;
        }
        
        // Move knights from set_b to set_a
        for (int i = 0; i < knight_array[set_b].set_size; i++) {
            knight_array[set_a].set_elements[knight_array[set_a].set_size + i] = knight_array[set_b].set_elements[i];
        }
        free(knight_array[set_b].set_elements);
        
        knight_array[set_a].set_size += knight_array[set_b].set_size;
        knight_array[set_b].parent = set_a;
        knight_array[set_a].size += knight_array[set_b].size;
        knight_array[set_a].total_attack += knight_array[set_b].total_attack;
    } else {
        // set_b absorbs set_a
        for (int i = 0; i < knight_array[set_a].set_size; i++) {
            knight_array[set_a].set_elements[i]->attack_count = 
                knight_array[set_a].set_elements[i]->attack_count - 1 + 
                knight_array[set_b].total_attack_count - knight_array[set_a].total_attack_count;
        }
        
        // Expand set_elements array if needed
        if (knight_array[set_b].set_array_size < knight_array[set_a].set_size + knight_array[set_b].set_size) {
            knight_array[set_b].set_elements = (knight **)realloc(
                knight_array[set_b].set_elements, 
                sizeof(knight*) * ((knight_array[set_a].set_size + knight_array[set_b].set_size) * 2)
            );
            knight_array[set_b].set_array_size = (knight_array[set_a].set_size + knight_array[set_b].set_size) * 2;
        }
        
        // Move knights from set_a to set_b
        for (int i = 0; i < knight_array[set_a].set_size; i++) {
            knight_array[set_b].set_elements[knight_array[set_b].set_size + i] = knight_array[set_a].set_elements[i];
        }
        free(knight_array[set_a].set_elements);
        
        knight_array[set_b].set_size += knight_array[set_a].set_size;
        knight_array[set_a].parent = set_b;
        knight_array[set_b].size += knight_array[set_a].size;
        knight_array[set_b].total_attack += knight_array[set_a].total_attack;
        
        if (knight_array[set_a].height == knight_array[set_b].height) {
            knight_array[set_b].height++;
        }
    }
    
    // Merge the health heaps
    merge_health_heaps(set_a, set_b);
}

/**
 * Union-Find: Union operation
 * @param knight_a - first knight index
 * @param knight_b - second knight index
 */
void union_sets(int knight_a, int knight_b) {
    link_sets(find_set_root(knight_a), find_set_root(knight_b));
}

/**
 * Main function - processes knight battles
 */
int main() {
    int num_knights, num_battles;
    scanf("%d%d", &num_knights, &num_battles);
    
    // Initialize knights with health values
    for (int i = 1; i <= num_knights; i++) {
        scanf("%lld", &knight_array[i].health);
        
        heap_node *initial_heap = create_heap_node(knight_array[i].health, i, 2);
        knight_array[i].health_heap = initial_heap;
        knight_array[i].parent = i;
        knight_array[i].height = 0;
        knight_array[i].size = 1;
        knight_array[i].is_dead = false;
        knight_array[i].heap_size = 1;
        knight_array[i].heap_array_size = 2;
        knight_array[i].attack_count = 0;
        knight_array[i].total_attack_count = 0;
        knight_array[i].damage_threshold = 0;
        
        // Initialize set with self
        knight_array[i].set_elements = (knight **)malloc(sizeof(knight*) * 2);
        knight_array[i].set_elements[0] = &knight_array[i];
        knight_array[i].set_array_size = 2;
        knight_array[i].set_size = 1;
    }
    
    // Read attack values
    for (int i = 1; i <= num_knights; i++) {
        scanf("%lld", &knight_array[i].attack);
        knight_array[i].total_attack = knight_array[i].attack;
    }
    
    // Process battles
    for (int battle = 0; battle < num_battles; battle++) {
        int attacker, defender;
        scanf("%d%d", &attacker, &defender);
        
        if (!knight_array[attacker].is_dead && !knight_array[defender].is_dead) {
            union_sets(attacker, defender);
        }
    }
    
    // Calculate final attack counts for living knights
    for (int i = 1; i <= num_knights; i++) {
        if (!knight_array[i].is_dead) {
            attack_counts[i] = knight_array[find_set_root(i)].total_attack_count - knight_array[i].attack_count;
        }
    }
    
    // Output results
    for (int i = 1; i <= num_knights; i++) {
        printf("%d ", attack_counts[i]);
    }
    
    return 0;
}
