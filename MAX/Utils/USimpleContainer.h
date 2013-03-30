//
//  USimpleContainer.h
//  TerminalExtraction
//
//  Created by Katekov Anton on 10/16/12.
//
//

#ifndef __TerminalExtraction__USimpleContainer__
#define __TerminalExtraction__USimpleContainer__

#include <iostream>

namespace Utils {
    
    using namespace std;
    
    template<class T>
    class USimpleContainer {
        T *_array;
        int         _baseSize;
        int         _count;
        int         _currentSize;
        
    public:
        
        T *GetArrayPointer() const {return _array;};
        
        USimpleContainer();
        USimpleContainer(int baseSize);
        ~USimpleContainer();
        
        void remove(int index);
        bool removeObject(const T& object);
        void addObject(const T& object);
        int indexOf(const T& object);
        void AddObjects(const USimpleContainer<T>* objects);
        void clear();
        T objectAtIndex(int index) const;
        void sort(int (* pointer)(const void *, const void *));
        int GetCount() const { return _count; };
    };

    template <class T>
    USimpleContainer<T>::USimpleContainer():USimpleContainer(100) {
    }
    
    template<class T>
    USimpleContainer<T>::USimpleContainer(int baseSize):_count(0) {
        _baseSize = baseSize;
        _array = new T[baseSize];
        _currentSize = _baseSize;
    }
    
    template <class T>
    USimpleContainer<T>::~USimpleContainer<T>() {
        clear();
        delete [] _array;
    }
    
    template <class T>
    void USimpleContainer<T>::remove(int index) {
        if (index < _count) {
//            T obj  = _array[index];
            
            _array[index] = _array[_count - 1];
            _count--;
        }
    }
    
    template <class T>
    int USimpleContainer<T>::indexOf(const T &object) {
         for (int i = 0; i < _count; i++) {
            if(_array[index] == object)
                return i;
        }
        return -1;
    }
    
    template <class T>
    void USimpleContainer<T>::clear() {
        memset(_array, 0, _currentSize*sizeof(T));
        _count = 0;
    }
    
    template <class T>
    bool USimpleContainer<T>::removeObject(const T& object) {
        for (int i = 0; i < _count; i++) {
            if (_array[i] == object) {
                remove(i);
                return true;
            }
        }
        return false;
    }
    
    template <class T>
    void USimpleContainer<T>::addObject(const T& object) {
        if (_count == _currentSize) {
            T *tmp = _array;
            _currentSize = _currentSize + _baseSize;
            _array = new T[_currentSize];
            for (int i = 0; i < _count; i++)
                _array[i] = tmp[i];
            delete [] tmp;
        }
        _array[_count] = object;
        _count ++;
    }
    
    template <class T>
    T USimpleContainer<T>::objectAtIndex(int index) const {
        T result = _array[index];
        return result;
    }
    
    template <class T>
    void USimpleContainer<T>::sort(int (*pointer)(const void *, const void *)) {
        qsort(_array, _count, sizeof(T), pointer);
    }
    
    template <class T>
    void USimpleContainer<T>::AddObjects(const USimpleContainer<T>* objects)
    {
        int newCount = _count + objects->GetCount();
        if (_currentSize < newCount)
        {
            int need = newCount - _currentSize;
            int times = need/_baseSize;
            if (times * _baseSize < need) 
                times ++;
            int newSize = _currentSize + _baseSize * times;
            
            T *tmp = _array;
            _array = new T[newSize];
            for (int i = 0; i < _count; i++)
                _array[i] = tmp[i];
            
            _currentSize = newSize;
            delete [] tmp;
        }
        memcpy(_array + _count, objects->_array, objects->GetCount() * sizeof(T));
        
        _count = newCount;
    }
}

#endif /* defined(__TerminalExtraction__USimpleContainer__) */
