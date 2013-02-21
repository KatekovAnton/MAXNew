//
//  UContainer.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/16/12.
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
        shared_ptr<T> *_array;
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
        int GetCount() const { return _count; };
    };

    template <class T>
    UContainer<T>::UContainer():UContainer(100) {
    }
    
    template<class T>
    UContainer<T>::UContainer(int baseSize):_count(0) {
        _baseSize = baseSize;
        _array = new shared_ptr<T>[baseSize];
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
            _array = new shared_ptr<T>[_currentSize + _baseSize];
            for (int i = 0; i < _count; i++)
                _array[i] = tmp[i];
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
            _array = new shared_ptr<T>[_count + _baseSize];
            for (int i = 0; i < _count; i++)
                _array[i] = tmp[i];
            
            _currentSize = objects->GetCount() + _count + _baseSize;
            delete [] tmp;
        }
        for (int i = 0; i < objects->GetCount(); i++)
            _array[_count + i] = objects->objectAtIndex(i);
        
        _count += objects->GetCount();
    }
}

#endif /* defined(__TerminalExtraction__UContainer__) */
