# Interactive-Queue-Management-Program
Author: Rimsha Rizvi

## Overview
This project implements an interactive program for the user to manage a queue of buzzers using an Abstract Data Type (ADT).

## Functions
1. sq_create() - Creates and initializes an empty service queue. Runtime: O(1)
2. sq_free() - Frees all the memory associated with the service queue. Runtime: O(N_t), where N_t is the number of buzzer IDs used during the lifetime of the service queue.
3. sq_display() - Displays the current state of the queue. Runtime: O(N), where N is the current queue length.
4. sq_length() - Returns the length of the queue. Runtime: O(1)
5. sq_give_buzzer() - Assigns a buzzer to the next customer in line. Runtime: O(1) on average.
6. sq_seat() - Seats the next customer in line and returns their buzzer ID. Runtime: O(1)
7. sq_kick_out() - Removes a customer with the given buzzer ID from the queue. Runtime: O(1)
8. sq_take_bribe() - Moves a customer with the given buzzer ID to the front of the queue. Runtime: O(1)
