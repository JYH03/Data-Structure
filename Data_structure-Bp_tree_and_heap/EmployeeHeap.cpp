#include "EmployeeHeap.h"

void EmployeeHeap::Insert(EmployeeData *data) // Heap insertion function
{
    if (this->size >= this->maxCapacity) // If size has reached max capacity before insertion
        ResizeArray();                   // Change capacity
    this->size++;                        // Increase space for insertion
    this->heapArr[this->size] = data;    // Insert data
    UpHeap(this->size);                  // Sort from the inserted position
}

EmployeeData *EmployeeHeap::Top() // Return top data
{
    if (IsEmpty())
        return NULL;
    return this->heapArr[1]; // Return top data
}

void EmployeeHeap::Delete()
{
    if (IsEmpty()) // If empty, do not proceed with deletion
        return;
    heapArr[1] = heapArr[this->size]; // Delete top data and move last data to top
    heapArr[this->size] = NULL;       // Delete last data
    this->size--;                     // Decrease size after deletion
    DownHeap(1);                      // After deleting top, move new top down to find proper position
}

bool EmployeeHeap::IsEmpty() // Function to check if heap is empty
{
    if (this->size == 0)
        return true;
    return false;
}

void EmployeeHeap::UpHeap(int index) // Compare from given index up to top (1), used for insert
{
    for (int i = index; i > 1;)
    {
        int parent = i / 2;
        if (this->heapArr[parent]->getIncome() >= this->heapArr[i]->getIncome())
            break;
        swap(this->heapArr[i], this->heapArr[parent]);
        i = parent;
    }
}

void EmployeeHeap::DownHeap(int index) // Move the data at index down to its proper position
{
    int current = index;
    while (current * 2 <= this->size)
    {
        int child = current * 2;
        if (child + 1 <= this->size &&
            this->heapArr[child]->getIncome() < this->heapArr[child + 1]->getIncome())
            ++child;

        if (this->heapArr[current]->getIncome() >= this->heapArr[child]->getIncome())
            break;

        swap(this->heapArr[current], this->heapArr[child]);
        current = child;
    }
}

void EmployeeHeap::ResizeArray()
{
    this->maxCapacity *= 2;                                  // Double the max capacity
    EmployeeData **newArr = new EmployeeData *[maxCapacity + 1](); // Index 0 is unused

    for (int i = 1; i <= this->size; ++i)
    {
        newArr[i] = this->heapArr[i]; // Copy existing data
    }

    delete[] this->heapArr; // Delete previously allocated memory
    this->heapArr = newArr; // Assign new memory with doubled capacity
}

int EmployeeHeap::getSize()
{
    return this->size; // Return size
}

EmployeeData *EmployeeHeap::getData(int index)
{
    if (index < 1 || index > this->size)
        return NULL;
    return this->heapArr[index];
}

void EmployeeHeap::Rebuild()
{
    for (int i = this->size / 2; i >= 1; --i)
        DownHeap(i);
}
