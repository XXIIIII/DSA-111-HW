#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define STRING_MAX_LENGTH 1000002
#define MAX_HASH_ARRAY_SIZE 10030000

/**
 * Converts uppercase letters A-Z to numbers 1-26
 * Used for polynomial rolling hash calculation
 */
int find_number(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 1;
    }
    return 0; // Handle invalid input
}

/**
 * Rabin-Karp algorithm to compute hash values for pattern and all its rotations
 * Stores hash values in all_pattern array for quick lookup
 */
void rabin_karp(long long base, long long highest_power, char *pattern, int *all_pattern, int pattern_length, long long prime) {
    long long pattern_hash = 0;
    
    // Calculate hash for the original pattern
    for (int i = 0; i < pattern_length; i++) {
        pattern_hash = (base * pattern_hash + find_number(pattern[i])) % prime;
    }
    all_pattern[pattern_hash] = 1;
    
    // Calculate hashes for all rotations of the pattern
    for (int i = 1; i < pattern_length; i++) {
        // Rolling hash: remove first character and add it to the end
        pattern_hash = (base * (pattern_hash - (find_number(pattern[i-1]) * highest_power)) + find_number(pattern[i-1])) % prime;
        if (pattern_hash < 0) {
            pattern_hash += prime;
        }
        all_pattern[pattern_hash] = 1;
    }
}

int main() {
    int text_length, pattern_length;
    
    // Large prime numbers for multiple hash functions to reduce collisions
    long long prime1 = 10013387;
    long long prime2 = 10013537;
    long long prime3 = 10013659;
    long long prime4 = 10013807;
    
    int match_count = 0;
    long long base = 26; // Base for polynomial rolling hash (26 letters)
    
    // Hash values for current window in text
    long long text_hash1 = 0, text_hash2 = 0, text_hash3 = 0, text_hash4 = 0;
    
    // Precomputed powers for rolling hash
    long long highest_power1 = 1, highest_power2 = 1, highest_power3 = 1, highest_power4 = 1;
    
    char text[STRING_MAX_LENGTH];
    char pattern[STRING_MAX_LENGTH];
    
    scanf("%d%d", &text_length, &pattern_length);
    
    // Precompute base^(pattern_length-1) mod prime for each hash function
    for (int i = 1; i < pattern_length; i++) {
        highest_power1 = (highest_power1 * base) % prime1;
    }
    for (int i = 1; i < pattern_length; i++) {
        highest_power2 = (highest_power2 * base) % prime2;
    }
    for (int i = 1; i < pattern_length; i++) {
        highest_power3 = (highest_power3 * base) % prime3;
    }
    for (int i = 1; i < pattern_length; i++) {
        highest_power4 = (highest_power4 * base) % prime4;
    }
    
    // Hash tables to store all pattern rotation hashes
    int pattern_hashes1[MAX_HASH_ARRAY_SIZE] = {0};
    int pattern_hashes2[MAX_HASH_ARRAY_SIZE] = {0};
    int pattern_hashes3[MAX_HASH_ARRAY_SIZE] = {0};
    int pattern_hashes4[MAX_HASH_ARRAY_SIZE] = {0};
    
    scanf("%s", text);
    scanf("%s", pattern);
    
    // Generate hash values for pattern and all its rotations using 4 different hash functions
    rabin_karp(base, highest_power1, pattern, pattern_hashes1, pattern_length, prime1);
    rabin_karp(base, highest_power2, pattern, pattern_hashes2, pattern_length, prime2);
    rabin_karp(base, highest_power3, pattern, pattern_hashes3, pattern_length, prime3);
    rabin_karp(base, highest_power4, pattern, pattern_hashes4, pattern_length, prime4);
    
    // Calculate initial hash values for first window in text
    for (int i = 0; i < pattern_length; i++) {
        text_hash1 = (base * text_hash1 + find_number(text[i])) % prime1;
    }
    for (int i = 0; i < pattern_length; i++) {
        text_hash2 = (base * text_hash2 + find_number(text[i])) % prime2;
    }
    for (int i = 0; i < pattern_length; i++) {
        text_hash3 = (base * text_hash3 + find_number(text[i])) % prime3;
    }
    for (int i = 0; i < pattern_length; i++) {
        text_hash4 = (base * text_hash4 + find_number(text[i])) % prime4;
    }
    
    // Slide window across text and check for matches
    for (int i = 0; i <= text_length - pattern_length; i++) {
        // Check if current window matches any rotation using all 4 hash functions
        if (pattern_hashes1[text_hash1] == 1 && 
            pattern_hashes2[text_hash2] == 1 && 
            pattern_hashes3[text_hash3] == 1 && 
            pattern_hashes4[text_hash4] == 1) {
            match_count++;
        }
        
        // Roll the window to next position (if not at end)
        if (i < text_length - pattern_length) {
            // Update hash1: remove leftmost character, add new rightmost character
            text_hash1 = (base * (text_hash1 - find_number(text[i]) * highest_power1) + find_number(text[i + pattern_length])) % prime1;
            if (text_hash1 < 0) {
                text_hash1 += prime1;
            }
            
            // Update hash2
            text_hash2 = (base * (text_hash2 - find_number(text[i]) * highest_power2) + find_number(text[i + pattern_length])) % prime2;
            if (text_hash2 < 0) {
                text_hash2 += prime2;
            }
            
            // Update hash3
            text_hash3 = (base * (text_hash3 - find_number(text[i]) * highest_power3) + find_number(text[i + pattern_length])) % prime3;
            if (text_hash3 < 0) {
                text_hash3 += prime3;
            }
            
            // Update hash4
            text_hash4 = (base * (text_hash4 - find_number(text[i]) * highest_power4) + find_number(text[i + pattern_length])) % prime4;
            if (text_hash4 < 0) {
                text_hash4 += prime4;
            }
        }
    }
    
    printf("%d\n", match_count);
    return 0;
}
