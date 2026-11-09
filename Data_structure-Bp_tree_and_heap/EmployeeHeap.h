#pragma once

#include "EmployeeData.h"

class EmployeeHeap
{
private:
    int datanum;
    EmployeeData **heapArr;
    int maxCapacity = 10;
    int size = 0;

public:
    EmployeeHeap()
    {
        this->size = 0;
        this->maxCapacity = 10;
        this->heapArr = new EmployeeData *[this->maxCapacity + 1]();
    }
    ~EmployeeHeap()
    {
        delete[] heapArr;
    }

    void Insert(EmployeeData *data);
    EmployeeData *Top();
    void Delete();

    bool IsEmpty();

    void UpHeap(int index);
    void DownHeap(int index);
    void ResizeArray();
    void Rebuild();
    int getSize();
    EmployeeData *getData(int index);
};
