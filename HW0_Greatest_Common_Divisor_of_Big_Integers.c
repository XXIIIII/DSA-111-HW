#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DIGITS 260

/**
 * Converts a string representation of a number to an integer array
 * The array is right-aligned with leading zeros
 * @param input_string: Input number as string
 * @param digit_array: Output array to store digits
 */
void string_to_digit_array(char input_string[MAX_DIGITS], int digit_array[MAX_DIGITS]) {
    int string_length = strlen(input_string);
    int start_index = MAX_DIGITS - string_length;
    
    // Fill array with leading zeros
    for (int i = 0; i < start_index; i++) {
        digit_array[i] = 0;
    }
    
    // Convert characters to integers
    for (int i = 0; i < string_length; i++) {
        digit_array[start_index + i] = input_string[i] - '0';
    }
}

/**
 * Counts the number of significant digits (excluding leading zeros)
 * @param digit_array: Array of digits
 * @return: Number of significant digits
 */
int count_significant_digits(int digit_array[MAX_DIGITS]) {
    int leading_zeros = 0;
    while (leading_zeros < MAX_DIGITS && digit_array[leading_zeros] == 0) {
        leading_zeros++;
    }
    return MAX_DIGITS - leading_zeros;
}

/**
 * Compares two big integers and returns pointer to the larger one
 * @param num_a: First number
 * @param num_b: Second number
 * @return: Pointer to larger number, NULL if equal
 */
int* compare_big_integers(int* num_a, int* num_b) {
    int length_a = count_significant_digits(num_a);
    int length_b = count_significant_digits(num_b);
    
    // Compare by length first
    if (length_a > length_b) {
        return num_a;
    } else if (length_b > length_a) {
        return num_b;
    }
    
    // Same length - compare digit by digit
    int start_index = MAX_DIGITS - length_a;
    for (int i = start_index; i < MAX_DIGITS; i++) {
        if (num_a[i] > num_b[i]) {
            return num_a;
        }
        if (num_b[i] > num_a[i]) {
            return num_b;
        }
    }
    
    return NULL; // Numbers are equal
}

/**
 * Checks if a big integer is not zero
 * @param number: Big integer to check
 * @return: true if not zero, false if zero
 */
bool is_not_zero(int* number) {
    for (int i = 0; i < MAX_DIGITS; i++) {
        if (number[i] != 0) {
            return true;
        }
    }
    return false;
}

/**
 * Divides a big integer by 2 (right shift operation)
 * @param number: Big integer to divide by 2
 */
void divide_by_two(int* number) {
    int result[MAX_DIGITS] = {0};
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        if (number[i] >= 2 || number[i] == 0) {
            if (number[i] % 2 == 0) {
                result[i] = number[i] / 2;
            } else {
                result[i] = number[i] / 2;
                if (i + 1 < MAX_DIGITS) {
                    number[i + 1] += 10;
                }
            }
        } else {
            // Handle case where digit is 1
            if (i + 1 < MAX_DIGITS) {
                number[i + 1] += 10;
            }
            result[i] = 0;
        }
    }
    
    // Copy result back to original array
    for (int i = 0; i < MAX_DIGITS; i++) {
        number[i] = result[i];
    }
}

/**
 * Subtracts smaller number from larger number (minuend - subtrahend)
 * @param minuend: Number to subtract from (will be modified)
 * @param subtrahend: Number to subtract
 */
void subtract_big_integers(int minuend[], int subtrahend[]) {
    int result[MAX_DIGITS] = {0};
    
    for (int i = MAX_DIGITS - 1; i >= 0; i--) {
        if (minuend[i] < subtrahend[i]) {
            // Need to borrow from higher digits
            minuend[i] += 10;
            int borrow_index = 1;
            
            // Find next non-zero digit to borrow from
            while (i - borrow_index >= 0 && minuend[i - borrow_index] == 0) {
                borrow_index++;
            }
            
            // Perform borrowing
            if (i - borrow_index >= 0) {
                minuend[i - borrow_index] -= 1;
                for (int j = 1; j < borrow_index; j++) {
                    minuend[i - j] = 9;
                }
            }
        }
        result[i] = minuend[i] - subtrahend[i];
    }
    
    // Copy result back to minuend
    for (int i = 0; i < MAX_DIGITS; i++) {
        minuend[i] = result[i];
    }
}

/**
 * Multiplies a big integer by 2^k (left shift by k positions)
 * @param number: Big integer to multiply
 * @param k: Power of 2 to multiply by
 */
void multiply_by_power_of_two(int number[], int k) {
    if (k == 0) return;
    
    int* current = number;
    int temp_array[MAX_DIGITS] = {0};
    int* next = temp_array;
    
    for (int shift = 0; shift < k; shift++) {
        bool carry = false;
        
        // Multiply by 2
        for (int i = MAX_DIGITS - 1; i >= 0; i--) {
            next[i] = 2 * current[i];
            
            if (carry) {
                next[i] += 1;
                carry = false;
            }
            
            if (next[i] >= 10) {
                carry = true;
                next[i] %= 10;
            }
        }
        
        // Swap arrays
        int* swap = current;
        current = next;
        next = swap;
        
        // Clear the next array for next iteration
        for (int i = 0; i < MAX_DIGITS; i++) {
            next[i] = 0;
        }
    }
    
    // Copy final result back to original array
    for (int i = 0; i < MAX_DIGITS; i++) {
        number[i] = current[i];
    }
}

/**
 * Prints a big integer (removes leading zeros)
 * @param number: Big integer to print
 */
void print_big_integer(int number[]) {
    int significant_digits = count_significant_digits(number);
    int start_index = MAX_DIGITS - significant_digits;
    
    if (significant_digits == 0) {
        printf("0");
        return;
    }
    
    for (int i = start_index; i < MAX_DIGITS; i++) {
        printf("%d", number[i]);
    }
}

/**
 * Binary GCD Algorithm for big integers
 * Implements the efficient binary algorithm for computing GCD
 */
int main() {
    int number_a[MAX_DIGITS] = {0};
    int number_b[MAX_DIGITS] = {0};
    char input_string1[MAX_DIGITS];
    char input_string2[MAX_DIGITS];
    int power_of_two = 0;
    
    // Read input
    scanf("%s", input_string1);
    scanf("%s", input_string2);
    
    // Convert strings to digit arrays
    string_to_digit_array(input_string1, number_a);
    string_to_digit_array(input_string2, number_b);
    
    // Find the larger number
    int* larger = compare_big_integers(number_a, number_b);
    
    if (larger == NULL) {
        // Numbers are equal - GCD is the number itself
        printf("%s", input_string1);
        return 0;
    }
    
    // Set up larger and smaller numbers
    int* smaller;
    if (larger == number_a) {
        smaller = number_b;
    } else {
        smaller = number_a;
    }
    
    // Binary GCD Algorithm
    while (is_not_zero(larger) && is_not_zero(smaller)) {
        // Check if both numbers are even
        if (larger[MAX_DIGITS - 1] % 2 == 0 && smaller[MAX_DIGITS - 1] % 2 == 0) {
            power_of_two++;
            divide_by_two(larger);
            divide_by_two(smaller);
        } else if (smaller[MAX_DIGITS - 1] % 2 == 0) {
            // Only smaller is even
            divide_by_two(smaller);
        } else if (larger[MAX_DIGITS - 1] % 2 == 0) {
            // Only larger is even
            divide_by_two(larger);
        }
        
        // Ensure larger >= smaller
        if (smaller == compare_big_integers(larger, smaller)) {
            int* temp = smaller;
            smaller = larger;
            larger = temp;
        }
        
        // Subtract smaller from larger
        subtract_big_integers(larger, smaller);
    }
    
    // Multiply result by 2^power_of_two
    multiply_by_power_of_two(smaller, power_of_two);
    
    // Print the result
    print_big_integer(smaller);
    
    return 0;
}
