#ifndef LIST_H
#define LIST_H

#include <cstdlib>
#include <string>

// список на основе массива, с сортировкой как чисел (целых или вещественных), так и строк (в формате char*)
// используется быстрая сортировка QuickSort
template <class T>
class List
{
public:
    List();
    ~List();

    void Add(T val);
    void RemoveAt(int ind);
    void Clear();

    void Sort();

    int Size() { return size; }

    T& operator[] (const int index) { return arr[index]; };

private:
    char CharStrCmp(void* a, void* b);
    char IntCmp(void* a, void* b);

    void QuickSort(char (List::*cmp)(void*, void*), T*, int);

    T* arr;
    int size;
};

template<class T>
List<T>::List()
{
    arr = NULL;
    size = 0;
}

template<class T>
void List<T>::Add(T val){
    T* newArr = new T[size+1];
    for(int i = 0; i < size; i++){
        newArr[i] = arr[i];
    }
    newArr[size] = val;
    delete[] arr;
    arr = newArr;
    size++;
}

template<class T>
void List<T>::RemoveAt(int ind){
    if (ind < 0 || ind >= size) return;
    T* newArr = new T[size-1];
    int j = 0;
    for(int i = 0; i < size; i++){
        if (i == ind) continue;
        newArr[j++] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    size--;
}

template<class T>
void List<T>::Clear(){
    if (arr){
        delete[]arr;
        size = 0;
        arr = NULL;
    }
}

template<class T>
List<T>::~List()
{
    if (arr) {
        delete[]arr;
        arr = NULL;
        size = 0;
    }
}

template<class T>
void List<T>::Sort()
{
    if (!size) return;
    char (List::*cmp) (void*, void*) = nullptr;

    if (typeid(T) == typeid(char*)) cmp = &List::CharStrCmp;
    else if (typeid(T) == typeid(int)) cmp = &List::IntCmp;

    if (!cmp) return;

    QuickSort(cmp, arr, size);
}

template<class T>
void List<T>::QuickSort(char (List::*cmp)(void*, void*), T* arr, int size)
{
    T x, w;
    int i, j;

    x = arr[size/2];
    i=0; j=size-1;
    do
    {
        while ((this->*cmp)(&arr[i],&x) < 0) i++;
        while ((this->*cmp)(&x,&arr[j]) < 0) j--;

        if (i<=j)
        {
            w = arr[i]; arr[i] = arr[j]; arr[j] = w;
            i++; j--;
        }
    }
    while (i<j);
    if (j>0)
        QuickSort (cmp, arr, j+1);
    if (i<size-1)
        QuickSort (cmp, arr+i, size-i);
}

template<class T>
char List<T>::CharStrCmp(void* a, void* b)
{
    int sa = strlen(*(char**)a), sb = strlen(*(char**)b);
    if (sa > sb) return 1;
    else if (sa < sb) return -1;

    char* str1 = *(char**)a;
    char* str2 = *(char**)b;
    return strcmp(str1, str2);
}

template<class T>
char List<T>::IntCmp(void* a, void* b)
{
    if (*(int*)a == *(int*)b) return 0;
    return *(int*)a > *(int*)b ? 1 : -1;
}

#endif // LIST_H
