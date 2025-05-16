#include <stdio.h>
#include <stdbool.h>
#include "linkedlist/intlist.h"

/** 
 * @file addtwonumbers.c
 * @brief Sums two linked lists representing numbers
 * 
 * The following program solves a classic problem called "Add Two Numbers".
 * A good illustration of using linked lists for numerical manipulation, 
 * especially when dealing with numbers that might exceed the capacity of standard integer types. 
 * Two non-empty linked lists * are given, each representing a non-negative integer with its digits stored in reverse order. 
 * For instance, the number 342 would be * represented as a linked list with nodes containing 2 -> 4 -> 3. 
 */

int main() {
    int num1_arr[] = {2, 4, 3}; // Represents the number 342
    IntList num1 = intlist_from_array(num1_arr, 3); 
    if (!num1) {
        fprintf(stderr, "Error: cannot create the first number list.");
        return 1;
    }

    int num2_arr[] = {5, 6, 4}; // Represents the number 465
    IntList num2 = intlist_from_array(num2_arr, 3); 
    if (!num2) {
        fprintf(stderr, "Error: cannot create the second number list.");
        return 1;
    }
    
    IntList sum = intlist_new();
    if (!sum) {
        fprintf(stderr, "Error: cannot create the sum list.");
        return 1;
    }

    bool status1, status2;
    int digit1 = 0,  digit2 = 0, carry = 0; // Current digits from the lists
    size_t i = 0;
    while (true) {
        digit1 = digit2 = 0;
        // Check if we really get an element from the sources lists 
        status1 = intlist_get_at(num1, i, &digit1);
        status2 = intlist_get_at(num2, i, &digit2);

        if (!status1 && !status2 && carry == 0) {
            break; // Exit loop if both lists are exhausted and no carry
        }
        
        // Set digit to 0 if already reach its end
        if (!status1) {
            digit1 = 0;
        }
        
        if (!status2) {
            digit2 = 0;
        }
        
        int total = digit1 + digit2 + carry;
        carry = total / 10;
        total %= 10;
        
        if (!intlist_push(sum, total)) {
            fprintf(stderr, "Error: cannot add the new element.\n");
            return 1;
        }

        i++;
    }

    printf("\nFirst number:\t");
    intlist_print(num1);

    printf("\nSecond number:\t");
    intlist_print(num2);

    printf("\nSum result:\t");
    intlist_print(sum); // Output: 7 -> 0 -> 8 which is 807 (342 + 465) stored in reverse order

    return 0;
}