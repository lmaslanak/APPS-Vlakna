//============================================================================
// Name        : du1.cpp
// Author      : Josef Holis
// Version     :
// Copyright   : Your copyright notice
// Description : Homework 1 for APPS in C++, Ansi-style
//============================================================================

// 1. type of number: int
// 2. sorting metod: BubbleSort (the Design And Analysis Of Algorithms page. 128)
// 2. sorting metod: merge (the Design And Analysis Of Algorithms page. 200)

#include <iostream>
#include <sys/time.h>
#include <pthread.h>

using namespace std;

struct thread_argument
{
    int id;         // user identification
    int from, to;   // data range
    int *data;      // array
    int max;        // result
};

// Merge two sorted arrays into one sorted array.
// Input: Arrays B[0..p - 1] and C[0..q - 1] both sorted.
// Output: Sorted array A[0..p + q - 1] of the elements of B and C.
void merge(int B[], int p, int C[], int q, int q1, int A[], int r)
{
    int i = 0;
    int j = q;
    int k = 0;

    while ((i < p) && (j < q1))
    {
        if (B[i] <= C[j])
        {
            A[k] = B[i];
            i++;
        }
        else
        {
            A[k] = C[j];
            j++;
        }
        k++;
    }


    if (i == p)
    {
        for (int l = k; l <= p + q -1; l++)
        {
            A[l] = C[j];
            j++;
        }
    }
    else
    {
        for (int l = k; l <= p + q -1; l++)
        {
            A[l] = B[i];
            i++;
        }
    }
}

// Sort a given array by bubble sort.
// Input: An array A[0..n - 1] of orderable elements.
// Outout: Array A[0..n - 1] sorted in nondecreasing order.
void bubbleSort(int A[], int from, int to)
{
    for (int i = 0; i <= (to - 2); i++)
        for (int j = 0 + from; j <= (to - 2) - i; j++)
            if (A[j + 1] < A [j])
            {
                int swapp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = swapp;
            }
}

int time_to_ms(timeval *before, timeval *after)
{
    timeval res;
    timersub(after, before, &res);
    return 1000 * res.tv_sec + res.tv_usec / 1000;
}

void *my_thread(void *void_arg)
{
    thread_argument *ptr_data = (thread_argument *) void_arg;

    cout << "Thread " << ptr_data->id << " started from " << ptr_data->from <<
            " width length " << ptr_data->to << "..." << endl;

    bubbleSort(ptr_data->data, ptr_data->from, ptr_data->to);

    return NULL;
}

#define LENGTH_LIMIT 10000000

int main()
{
    // constants
    const int SMALL_ARRAY_LENGTH = 10;
    const int LARGE_ARRAY_LENGTH = 10000;

    cout << "Small array: " << SMALL_ARRAY_LENGTH << endl;
    cout << "Large array: " << LARGE_ARRAY_LENGTH << endl;

    // variables
    // Measurement time in ms.
    timeval time_before, time_after;

    // Array location.
    int *one_thread_small_array = new int[SMALL_ARRAY_LENGTH];
    int *one_thread_large_array = new int[LARGE_ARRAY_LENGTH];

    int *two_thread_small_array = new int[SMALL_ARRAY_LENGTH];
    int *two_thread_small_array_1 = new int[SMALL_ARRAY_LENGTH];

    int *two_thread_large_array = new int[LARGE_ARRAY_LENGTH];
    int *two_thread_large_array_1 = new int[LARGE_ARRAY_LENGTH];

    if (!one_thread_small_array ||
            !one_thread_large_array ||
            !two_thread_small_array ||
            !two_thread_large_array ||
            !two_thread_small_array_1)
    {
        cout << "Not enought memory for array!" << endl;
        return 1;
    }

    // Initialization of random number generator and load to array.
    srand((int) time(NULL));

    // small array
    for (int i = 0; i < SMALL_ARRAY_LENGTH; i++)
    {
        int r = rand() % (SMALL_ARRAY_LENGTH * 10);

        one_thread_small_array[i] = r;
        two_thread_small_array[i] = r;
    }
    // large array
    for (int i = 0; i < LARGE_ARRAY_LENGTH; i++)
    {
        int r = rand() % (LARGE_ARRAY_LENGTH * 10);

        one_thread_large_array[i] = r;
        two_thread_large_array[i] = r;
    }

    // Sort the field with one thread.
    // small array
    cout << endl << "Small array(unsorted): ";
    for (int i = 0; i < SMALL_ARRAY_LENGTH; i++)
        cout << one_thread_small_array[i] << " ";
    cout << endl;

    gettimeofday(&time_before, NULL);
    bubbleSort(one_thread_small_array, 0, SMALL_ARRAY_LENGTH);
    gettimeofday(&time_after, NULL);

    cout << "Small array(sorted): ";
    for (int j = 0; j < SMALL_ARRAY_LENGTH; j++)
        cout << one_thread_small_array[j] << " ";
    cout << endl;
    cout << endl << "The sort time (small array): " << time_to_ms(&time_before, &time_after) << " [ms]" << endl;

    // large array
    gettimeofday(&time_before, NULL);
    bubbleSort(one_thread_large_array, 0,LARGE_ARRAY_LENGTH);
    gettimeofday(&time_after, NULL);
    cout << "The sort time (large array): " << time_to_ms(&time_before, &time_after) << " [ms]" << endl;


    cout << endl << "Time to thread." << endl;

    // Sort the field with two thread.
    // small array
    pthread_t pt1, pt2;
    thread_argument ta1, ta2;

    ta1.id = 1;
    ta1.from = 0;
    ta1.to = SMALL_ARRAY_LENGTH / 2;
    ta1.data = two_thread_small_array;

    ta2.id = 2;
    ta2.from = ta1.to;
    ta2.to = SMALL_ARRAY_LENGTH;
    ta2.data = two_thread_small_array;

    // Check correct data in thread.
    for (int i = 0; i< SMALL_ARRAY_LENGTH; i++)
    {
        if (i == 0)
            cout << endl << "Thread id " << ta1.id << ": ";
        if (i < 5)
            cout << ta1.data[i] << " ";
        if (i == 5)
            cout << endl << "Thread id " << ta2.id << ": ";
        if (i > 4)
            cout << ta2.data[i] << " ";
    }
    cout << endl << endl;

    // Start measurement time.
    gettimeofday(&time_before, NULL);

    pthread_create(&pt1, NULL, my_thread, &ta1);
    pthread_create(&pt2, NULL, my_thread, &ta2);

    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);

    merge(ta1.data, ta1.to, ta2.data, ta2.from, ta2.to, two_thread_small_array_1, SMALL_ARRAY_LENGTH);

    // End measurement time.
    gettimeofday(&time_after, NULL);
    cout << endl << "The sort time (small array, thread): " << time_to_ms(&time_before, &time_after) << " [ms]" << endl;

    cout << endl << "Merge array: ";
    for (int i = 0; i < SMALL_ARRAY_LENGTH; i++)
        cout << two_thread_small_array_1[i] << " ";
    cout << endl << endl;

    // large array
    ta1.id = 1;
    ta1.from = 0;
    ta1.to = LARGE_ARRAY_LENGTH / 2;
    ta1.data = two_thread_large_array;

    ta2.id = 2;
    ta2.from = ta1.to;
    ta2.to = LARGE_ARRAY_LENGTH;
    ta2.data = two_thread_large_array;

    // Start measurement time.
    gettimeofday(&time_before, NULL);

    pthread_create(&pt1, NULL, my_thread, &ta1);
    pthread_create(&pt2, NULL, my_thread, &ta2);

    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);

    merge(ta1.data, ta1.to, ta1.data, ta2.from, ta2.to, two_thread_large_array_1, SMALL_ARRAY_LENGTH);

    // End measurement time.
    gettimeofday(&time_after, NULL);
    cout << endl << "The sort time (large array, thread): " << time_to_ms(&time_before, &time_after) << " [ms]" << endl;

    // Clean up after your self.
    delete[] one_thread_small_array;
    delete[] one_thread_large_array;
    delete[] two_thread_small_array;
    delete[] two_thread_small_array_1;
    delete[] two_thread_large_array;

    pthread_exit(NULL);

    return 0;
}
