//
//  cList.h
//  MAX
//
//  Created by  Developer on 12.02.13.
//  Copyright (c) 2013 AntonKatekov. All rights reserved.
//

#ifndef MAX_cList_h
#define MAX_cList_h


#include <algorithm>
#include <cstdlib>
#include <assert.h>


#define MALLOCN(type, count)  static_cast<type*>(malloc(sizeof(type) * (count)))


template<typename T> class cList
{
public:
	cList() : v_(), capacity_(), size_() {}
    
	~cList() { Clear(); }
    
	cList (const cList& list);
	cList& operator= (const cList& list);
    
	size_t Size() const { return size_; }
    
	T&       Back()       { return v_[size_ - 1]; }
	T const& Back() const { return v_[size_ - 1]; }
    
	T&       operator [] (size_t const idx)       { assert (idx < size_); return v_[idx]; }
	T const& operator [] (size_t const idx) const { assert (idx < size_); return v_[idx]; }
    
	void Add (T const& elem);
    
	void Insert (size_t const i, T const& e);
    
	void Delete (size_t const idx);
    
	void Clear() { Reserve (0); }
    
	void PopBack();
    
	void Reserve (size_t const n);
    
	bool Contains (const T& e) const;
    
	void RemoveDuplicates();
    
private:
	T*     v_;
	size_t capacity_;
	size_t size_;
};

template<typename T>
cList<T>::cList (const cList& list) :
capacity_ (list.capacity_),
size_ (list.size_)
{
	v_ = new T[list.capacity_];
	for (size_t i = 0; i < size_; i++)
		new (&v_[i]) T (list.v_[i]);
}

template<typename T>
cList<T>& cList<T>::operator= (const cList& list)
{
	for (size_t i = 0; i < size_; i++)
		v_[i].~T();
	delete[] v_;
    
	capacity_ = list.capacity_;
	size_ = list.size_;
    
	v_ = new T[capacity_];
	for (size_t i = 0; i < size_; i++)
		new (&v_[i]) T (list.v_[i]);
    
	return *this;
}


template<typename T> void cList<T>::Add (T const& e)
{
	if (size_ >= capacity_) Reserve (std::max ( (size_t) 1U, size_ * 2));
	new (&v_[size_]) T (e);
	++size_;
}

template<typename T> void cList<T>::Insert (size_t const i, T const& e)
{
	if (i > size_) throw;
	if (size_ >= capacity_) Reserve (std::max ( (size_t) 1U, size_ * 2));
    
	for (size_t n = size_; n > i; --n) v_[n] = v_[n - 1];
	new (&v_[i]) T (e);
	++size_;
}

template<typename T> void cList<T>::Delete (size_t const idx)
{
	if (idx >= size_)
		return; // XXX should throw exception
    
	for (size_t i = idx; i < size_ - 1; ++i) v_[i] = v_[i + 1];
	PopBack();
}


template<typename T> void cList<T>::PopBack()
{
	--size_;
	v_[size_].~T();
}


template<typename T> void cList<T>::Reserve (size_t const n)
{
	T* const new_v = n == 0 ? 0 : MALLOCN (T, n);
    
	size_t       i;
	T*     const old_v    = v_;
	size_t const old_size = size_;
	size_t const new_size = std::min (old_size, n);
	try
	{
		for (i = 0; i < new_size; ++i) new (&new_v[i]) T (old_v[i]);
	}
	catch (...)
	{
		for (size_t k = i; k != 0;) new_v[--i].~T();
		free (new_v);
		throw;
	}
    
	v_        = new_v;
	capacity_ = n;
	size_     = new_size;
    
	for (size_t k = old_size; k != 0;) old_v[--k].~T();
	free (old_v);
}

template<typename T> bool cList<T>::Contains (const T& e) const
{
	for (size_t idx = 0; idx < size_; idx++)
	{
		if (v_[idx] == e)
			return true;
	}
	return false;
}

template<typename T> void cList<T>::RemoveDuplicates()
{
	for (unsigned int i = 0; i < size_; i++)
	{
		for (unsigned int k = i + 1; k < size_; k++)
		{
			if (v_[i] == v_[k])
			{
				Delete (i);
				i--;
				break;
			}
		}
	}
}

#endif
