//
//  UContainer.h
//  TerminalExtraction
//
//  Created by Yarik Smirnov on 10/16/12.
//
//

#ifndef __TerminalExtraction__UContainer__
#define __TerminalExtraction__UContainer__

#include <iostream>

namespace Utils {
    
    using namespace std;
    
    #define ELEMENT_SIZE sizeof(shared_ptr<T>) 
    
    template<class T>
    class UContainer {
        //#pragma DATA_ALIGN ( 16 );
//        __attribute__((packed))
        shared_ptr<T> *_array __attribute__ ((aligned (8)));
        int         _baseSize;
        int         _count;
        int         _currentSize;
        
    public:
        
        UContainer();
        UContainer(int baseSize);
        ~UContainer();
        
        void remove(int index);
        bool removeObject(const shared_ptr<T>& object);
        void addObject(const shared_ptr<T>& object);
        int indexOf(const shared_ptr<T>& object);
        void AddObjects(const UContainer<T>* objects);
        void clear();
        shared_ptr<T> objectAtIndex(int index) const;
        void sort(int (* pointer)(const void *, const void *), size_t size);
        int GetCount() const { return _count; }
    };

    template <class T>
    UContainer<T>::UContainer():UContainer(100) {
    }
    
    template<class T>
    UContainer<T>::UContainer(int baseSize):_count(0) {
        _baseSize = baseSize;
        _array = new shared_ptr<T>[baseSize];//(shared_ptr<T>*)malloc(ELEMENT_SIZE*_baseSize);
        _currentSize = _baseSize;
    }
    
    template <class T>
    UContainer<T>::~UContainer<T>() {
        clear();
        delete [] _array;
    }
    
    template <class T>
    void UContainer<T>::remove(int index) {
        if (index < _count) {
            shared_ptr<T> obj  = _array[index];
            obj.reset();
            
            _array[index] = _array[_count - 1];
            _array[_count - 1] = nullptr;
            _count--;
        }
    }
    
    template <class T>
    int UContainer<T>::indexOf(const shared_ptr<T> &object) {
         for (int i = 0; i < _count; i++) {
            if(_array[index] == object)
                return i;
        }
        return -1;
    }
    
    template <class T>
    void UContainer<T>::clear() {
        for (int i = 0; i < _count; i++) {
            _array[i].reset();
        }
        _count = 0;
    }
    
    template <class T>
    bool UContainer<T>::removeObject(const shared_ptr<T>& object) {
        for (int i = 0; i < _count; i++) {
            if (_array[i] == object) {
                remove(i);
                return true;
            }
        }
        return false;
    }
    
    template <class T>
    void UContainer<T>::addObject(const shared_ptr<T>& object) {
        if (_count == _currentSize) {
            shared_ptr<T> *tmp = _array;
            
            _array = new shared_ptr<T>[_currentSize + _baseSize];// (shared_ptr<T>*)malloc(ELEMENT_SIZE*(_currentSize + _baseSize));

            memcpy(_array, tmp, ELEMENT_SIZE*_currentSize);
            _currentSize += _baseSize;
            delete [] tmp;
        }
        _array[_count] = object;
        _count ++;
    }
    
    template <class T>
    shared_ptr<T> UContainer<T>::objectAtIndex(int index) const {
        shared_ptr<T> result = _array[index];
        return result;
    }
    
    template <class T>
    void UContainer<T>::sort(int (*pointer)(const void *, const void *), size_t size) {
        qsort(_array, _count, size, pointer);
    }
    
    template <class T>
    void UContainer<T>::AddObjects(const UContainer<T>* objects)
    {
        if (_currentSize < objects->GetCount() + _count)
        {
            shared_ptr<T> *tmp = _array;
            _array = new shared_ptr<T>[_count + _baseSize];// (shared_ptr<T>*)malloc(ELEMENT_SIZE*(objects.GetCount() + _count + _baseSize));
            memcpy(_array, tmp, ELEMENT_SIZE*_count);
            _currentSize = objects->GetCount() + _count + _baseSize;
            delete [] tmp;
           // free(tmp);
        }
        memcpy(_array + ELEMENT_SIZE * _count, &objects, ELEMENT_SIZE * objects->GetCount());
        _currentSize += objects->GetCount();
    }
}

#endif /* defined(__TerminalExtraction__UContainer__) */
