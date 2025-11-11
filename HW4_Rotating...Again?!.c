#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define STRING_LENGTH 1000002
#define MAX_ARRAY 10030000
#define BASE 26

// Global variable to track total collision count
long long total_collisions = 0;

// Hash table node for collision detection
typedef struct HashNode {
    long long secondary_hash;  // Hash value using second prime
    long long count;           // Number of occurrences
    struct HashNode *next;     // Next node in collision chain
} HashNode;

/**
 * Creates a new hash table node
 * @param value: The secondary hash value to store
 * @return: Pointer to the newly created node
 */
HashNode *createHashNode(long long value) {
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    assert(new_node != NULL);
    
    new_node->secondary_hash = value;
    new_node->count = 1;
    new_node->next = NULL;
    
    return new_node;
}

/**
 * Removes a string pattern from the hash table and updates collision count
 * @param pattern: String pattern to remove
 * @param prime1: First prime number for hashing
 * @param prime2: Second prime number for hashing
 * @param base: Base value for polynomial hashing
 * @param hash_table: Hash table array
 * @param power_mod1: Precomputed power for first prime
 * @param power_mod2: Precomputed power for second prime
 * @param pattern_length: Length of the pattern
 */
void removeStringPattern(char *pattern, long long prime1, long long prime2, long long base, 
                        HashNode *hash_table, long long power_mod1, long long power_mod2, int pattern_length) {
    
    long long hash_value = 0;
    long long min_hash1 = 0;
    long long min_hash2 = 0;
    
    // Calculate initial hash with first prime
    for (int i = 0; i < pattern_length; i++) {
        hash_value = (base * hash_value + (pattern[i] - 'a' + 1)) % prime1;
    }
    
    min_hash1 = hash_value;
    
    // Find minimum hash among all rotations (first prime)
    for (int i = 1; i < pattern_length; i++) {
        hash_value = (base * (hash_value - ((pattern[i-1] - 'a' + 1) * power_mod1)) + (pattern[i-1] - 'a' + 1)) % prime1;
        if (hash_value < 0) {
            hash_value = hash_value + prime1;
        }
        if (hash_value < min_hash1) {
            min_hash1 = hash_value;
        }
    }

    // Calculate hash with second prime
    hash_value = 0;
    for (int i = 0; i < pattern_length; i++) {
        hash_value = (base * hash_value + (pattern[i] - 'a' + 1)) % prime2;
    }
    
    min_hash2 = hash_value;
    
    // Find minimum hash among all rotations (second prime)
    for (int i = 1; i < pattern_length; i++) {
        hash_value = (base * (hash_value - ((pattern[i-1] - 'a' + 1) * power_mod2)) + (pattern[i-1] - 'a' + 1)) % prime2;
        if (hash_value < 0) {
            hash_value = hash_value + prime2;
        }
        if (hash_value < min_hash2) {
            min_hash2 = hash_value;
        }
    }
    
    // Find and remove the pattern from hash table
    HashNode *current = &hash_table[min_hash1];
    HashNode *previous = NULL;
    
    while (true) {
        if (current->secondary_hash == min_hash2) {
            // Found the pattern, update collision count
            total_collisions -= (current->count - 1);
            current->count -= 1;
            
            // Remove node if count reaches zero
            if (current->count == 0) {
                if (current != &hash_table[min_hash1]) {
                    previous->next = current->next;
                    free(current);
                }
            }
            break;
        } else {
            previous = current;
            current = current->next;
        }
    }
}

/**
 * Adds a string pattern to the hash table using Rabin-Karp algorithm
 * @param pattern: String pattern to add
 * @param prime1: First prime number for hashing
 * @param prime2: Second prime number for hashing
 * @param base: Base value for polynomial hashing
 * @param hash_table: Hash table array
 * @param power_mod1: Precomputed power for first prime
 * @param power_mod2: Precomputed power for second prime
 * @param pattern_length: Length of the pattern
 */
void addStringPattern(char *pattern, long long prime1, long long prime2, long long base, 
                     HashNode *hash_table, long long power_mod1, long long power_mod2, int pattern_length) {
    
    long long hash_value = 0;
    long long min_hash1 = 0;
    long long min_hash2 = 0;
    
    // Calculate initial hash with first prime
    for (int i = 0; i < pattern_length; i++) {
        hash_value = (base * hash_value + (pattern[i] - 'a' + 1)) % prime1;
    }
    
    min_hash1 = hash_value;
    
    // Find minimum hash among all rotations (first prime)
    for (int i = 1; i < pattern_length; i++) {
        hash_value = (base * (hash_value - ((pattern[i-1] - 'a' + 1) * power_mod1)) + (pattern[i-1] - 'a' + 1)) % prime1;
        if (hash_value < 0) {
            hash_value = hash_value + prime1;
        }
        if (hash_value < min_hash1) {
            min_hash1 = hash_value;
        }
    }

    // Calculate hash with second prime
    hash_value = 0;
    for (int i = 0; i < pattern_length; i++) {
        hash_value = (base * hash_value + (pattern[i] - 'a' + 1)) % prime2;
    }
    
    min_hash2 = hash_value;
    
    // Find minimum hash among all rotations (second prime)
    for (int i = 1; i < pattern_length; i++) {
        hash_value = (base * (hash_value - ((pattern[i-1] - 'a' + 1) * power_mod2)) + (pattern[i-1] - 'a' + 1)) % prime2;
        if (hash_value < 0) {
            hash_value = hash_value + prime2;
        }
        if (hash_value < min_hash2) {
            min_hash2 = hash_value;
        }
    }

    // Insert or update pattern in hash table
    HashNode *current = NULL;
    
    // Check if this is the first entry at this hash position
    if (hash_table[min_hash1].secondary_hash == 0 && 
        hash_table[min_hash1].count == 0 && 
        hash_table[min_hash1].next == NULL) {
        
        hash_table[min_hash1].secondary_hash = min_hash2;
        hash_table[min_hash1].count += 1;
    } else {
        current = &hash_table[min_hash1];
        
        while (true) {
            if (current->secondary_hash == min_hash2) {
                // Found existing pattern, update collision count
                total_collisions += current->count;
                current->count += 1;
                break;
            } else {
                if (current->next == NULL) {
                    // Pattern not found, create new node
                    current->next = createHashNode(min_hash2);
                    break;
                } else {
                    current = current->next;
                }
            }
        }
    }
}

int main() {
    int num_strings, num_queries;
    long long prime1 = 10013387;
    long long prime2 = 10013537;
    long long base = BASE;
    long long power_mod1 = 1;  // base^(pattern_length-1) mod prime1
    long long power_mod2 = 1;  // base^(pattern_length-1) mod prime2
    
    // Initialize hash table
    HashNode hash_table[MAX_ARRAY] = {0, 0, NULL};
    
    scanf("%d%d", &num_strings, &num_queries);
    
    char input_string[STRING_LENGTH];
    scanf("%s", input_string);
    int pattern_length = strlen(input_string);
    
    // Precompute powers for efficient hash calculation
    for (int i = 1; i < pattern_length; i++) {
        power_mod1 = power_mod1 * BASE % prime1;
    }
    
    for (int i = 1; i < pattern_length; i++) {
        power_mod2 = power_mod2 * BASE % prime2;
    }

    // Process initial strings
    addStringPattern(input_string, prime1, prime2, base, hash_table, power_mod1, power_mod2, pattern_length);
    
    for (int i = 0; i < num_strings - 1; i++) {
        scanf("%s", input_string);
        addStringPattern(input_string, prime1, prime2, base, hash_table, power_mod1, power_mod2, pattern_length);
    }
    
    printf("%lld\n", total_collisions);
    
    // Process queries
    int command;
    for (int i = 0; i < num_queries; i++) {
        scanf("%d", &command);
        scanf("%s", input_string);
        
        if (command == 1) {
            // Add string
            addStringPattern(input_string, prime1, prime2, base, hash_table, power_mod1, power_mod2, pattern_length);
        } else {
            // Remove string
            removeStringPattern(input_string, prime1, prime2, base, hash_table, power_mod1, power_mod2, pattern_length);
        }
        
        printf("%lld\n", total_collisions);
    }
    
    return 0;
}
