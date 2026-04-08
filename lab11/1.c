/*
Write a C/C++ Program to simulate the following algorithms find the total no. of cylinder movements for various input requests:
- FCFS
- SSTF
- SCAN
- C-SCAN
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define MAX 100

int calculateMovements(int sequence[], int size);
void fcfs(int requests[], int n, int head);
void sstf(int requests[], int n, int head);
int compare(const void *a, const void *b);
int compareDesc(const void *a, const void *b);
void scan(int requests[], int n, int head, int maxCylinder);
void cscan(int requests[], int n, int head, int maxCylinder);

int main()
{
	int n, head, maxCylinder;
	int requests[MAX];

	printf("=== Disk Scheduling Algorithms Simulator ===\n\n");

	printf("Enter the number of disk requests: ");
	scanf("%d", &n);

	printf("Enter the disk requests (space-separated):\n");
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &requests[i]);
	}

	printf("Enter the current head position: ");
	scanf("%d", &head);

	printf("Enter the maximum cylinder number: ");
	scanf("%d", &maxCylinder);

	printf("\n========================================\n");

	fcfs(requests, n, head);
	sstf(requests, n, head);
	scan(requests, n, head, maxCylinder);
	cscan(requests, n, head, maxCylinder);

	printf("\n========================================\n");
	printf("Simulation Complete!\n");

	return 0;
}

int calculateMovements(int sequence[], int size)
{
	int total = 0;
	for (int i = 1; i < size; i++)
	{
		total += abs(sequence[i] - sequence[i - 1]);
	}
	return total;
}

void fcfs(int requests[], int n, int head)
{
	int sequence[MAX];
	sequence[0] = head;

	for (int i = 0; i < n; i++)
	{
		sequence[i + 1] = requests[i];
	}

	int total = calculateMovements(sequence, n + 1);

	printf("\n=== FCFS (First Come First Served) ===\n");
	printf("Sequence: ");
	for (int i = 0; i <= n; i++)
	{
		printf("%d ", sequence[i]);
	}
	printf("\nTotal Cylinder Movements: %d\n", total);
}

void sstf(int requests[], int n, int head)
{
	int sequence[MAX];
	int visited[MAX] = {0};
	int current = head;
	sequence[0] = head;

	for (int i = 0; i < n; i++)
	{
		int nearest = -1;
		int minDist = INT_MAX;

		// Find the nearest unvisited request
		for (int j = 0; j < n; j++)
		{
			if (!visited[j])
			{
				int dist = abs(requests[j] - current);
				if (dist < minDist)
				{
					minDist = dist;
					nearest = j;
				}
			}
		}

		visited[nearest] = 1;
		current = requests[nearest];
		sequence[i + 1] = current;
	}

	int total = calculateMovements(sequence, n + 1);

	printf("\n=== SSTF (Shortest Seek Time First) ===\n");
	printf("Sequence: ");
	for (int i = 0; i <= n; i++)
	{
		printf("%d ", sequence[i]);
	}
	printf("\nTotal Cylinder Movements: %d\n", total);
}

int compare(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int compareDesc(const void *a, const void *b)
{
	return (*(int *)b - *(int *)a);
}

void scan(int requests[], int n, int head, int maxCylinder)
{
	int sequence[MAX];
	int left[MAX], right[MAX];
	int leftCount = 0, rightCount = 0;

	sequence[0] = head;

	for (int i = 0; i < n; i++)
	{
		if (requests[i] < head)
		{
			left[leftCount++] = requests[i];
		}
		else if (requests[i] > head)
		{
			right[rightCount++] = requests[i];
		}
	}

	qsort(left, leftCount, sizeof(int), compare);
	qsort(right, rightCount, sizeof(int), compareDesc);

	int idx = 1;

	if (leftCount > 0)
	{
		for (int i = 0; i < leftCount; i++)
		{
			sequence[idx++] = left[i];
		}
	}

	if (rightCount > 0)
	{
		qsort(right, rightCount, sizeof(int), compare);
		for (int i = 0; i < rightCount; i++)
		{
			sequence[idx++] = right[i];
		}
	}

	int total = calculateMovements(sequence, idx);

	printf("\n=== SCAN (Elevator Algorithm) ===\n");
	printf("Sequence: ");
	for (int i = 0; i < idx; i++)
	{
		printf("%d ", sequence[i]);
	}
	printf("\nTotal Cylinder Movements: %d\n", total);
}

void cscan(int requests[], int n, int head, int maxCylinder)
{
	int sequence[MAX];
	int left[MAX], right[MAX];
	int leftCount = 0, rightCount = 0;

	sequence[0] = head;

	for (int i = 0; i < n; i++)
	{
		if (requests[i] < head)
		{
			left[leftCount++] = requests[i];
		}
		else if (requests[i] > head)
		{
			right[rightCount++] = requests[i];
		}
	}

	qsort(left, leftCount, sizeof(int), compareDesc);
	qsort(right, rightCount, sizeof(int), compare);

	int idx = 1;

	for (int i = 0; i < rightCount; i++)
	{
		sequence[idx++] = right[i];
	}

	sequence[idx++] = maxCylinder;

	sequence[idx++] = 0;

	for (int i = 0; i < leftCount; i++)
	{
		sequence[idx++] = left[i];
	}

	int total = calculateMovements(sequence, idx);

	printf("\n=== C-SCAN (Circular SCAN) ===\n");
	printf("Sequence: ");
	for (int i = 0; i < idx; i++)
	{
		printf("%d ", sequence[i]);
	}
	printf("\nTotal Cylinder Movements: %d\n", total);
}
