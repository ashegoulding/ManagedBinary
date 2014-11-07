#ifndef ASHE_MANAGEDBINARY_H_
#define ASHE_MANAGEDBINARY_H_

#include <cstring>
#include <string>
#include <sstream>
#include <typeinfo>

namespace ashe
{

template <class T>
class ManagedBinary
{
public:
	typedef ManagedBinary<T> thisClass;

	T *data;
	size_t size, realSize;
	bool clearUpOnDelete;

protected:
	void __construct(const thisClass *src) throw();

	std::string __dummyString;

public:
	ManagedBinary(const size_t size = 0) throw();
	ManagedBinary(const T *arr, const size_t size) throw();
	ManagedBinary(const thisClass *src) throw();
	ManagedBinary(const thisClass &src) throw();
	virtual ~ManagedBinary() throw();

	virtual thisClass &operator =(const thisClass &src) throw();
	virtual thisClass *operator =(const thisClass *src) throw();

	virtual void setSize(const size_t size) throw();
	virtual void resize(const size_t newSize, const bool forced = true) throw();
	inline virtual void fillZeros() throw();
	inline virtual void fill(const T x) throw();
	inline virtual void fill(const unsigned char x) throw();
	inline virtual void empty() throw();
	inline virtual void swap(thisClass &with) throw();

	virtual std::string toString(const char **y = NULL) throw();
};

template<class T>
ManagedBinary<T>::ManagedBinary(const size_t size) throw ()
	: data(NULL)
	, size(0)
	, realSize(0)
	, clearUpOnDelete(true)
{
	if(size)
		this->setSize(size);
}

template<class T>
ManagedBinary<T>::ManagedBinary(const T* arr, const size_t size) throw ()
	: data(NULL)
	, size(0)
	, realSize(0)
	, clearUpOnDelete(true)
{
	if(arr)
	{
		size_t copySize;
		if(size)
			copySize = size*sizeof(T);
		else
			copySize = strlen(arr);
		this->setSize(copySize);
		memcpy(this->data, arr, copySize);
	}
}

template<class T>
ManagedBinary<T>::ManagedBinary(const thisClass* src) throw ()
	: data(NULL)
	, size(0)
	, realSize(0)
	, clearUpOnDelete(true)
{
	this->__construct(src);
}

template<class T>
ManagedBinary<T>::ManagedBinary(const thisClass& src) throw ()
	: data(NULL)
	, size(0)
	, realSize(0)
	, clearUpOnDelete(true)
{
	this->__construct(&src);
}

template<class T>
ManagedBinary<T>::~ManagedBinary() throw ()
{
	if(this->clearUpOnDelete)
		this->empty();
}

template<class T>
ManagedBinary<T>& ManagedBinary<T>::operator =(const thisClass& src) throw ()
{
	this->__construct(&src);
	return *this;
}

template<class T>
ManagedBinary<T>* ManagedBinary<T>::operator =(const thisClass* src) throw ()
{
	this->__construct(src);
	return this;
}

template<class T>
void ManagedBinary<T>::__construct(const thisClass* src) throw ()
{
	this->setSize(src->size);
	if(src->data)
		memcpy(this->data, src->data, sizeof(T)*this->size);
	this->clearUpOnDelete = src->clearUpOnDelete;
}

template<class T>
void ManagedBinary<T>::setSize(const size_t size) throw ()
{
	if(this->realSize == size)
	{
		// Case: No change
		this->size = size;
		this->fillZeros();
		return;
	}
	this->empty();
	if(size)
	{
		this->realSize = this->size = size;
		this->data = new T[this->realSize];
		this->fillZeros();
	}
}

template<class T>
void ManagedBinary<T>::resize(const size_t newSize, const bool forced) throw ()
{
	if(forced)
	{
		// Case: Alias of thisClass::setSize()
		this->setSize(newSize);
		return;
	}
	else if(this->realSize >= newSize)
	{
		// Case: Not necessary to grow
		this->size = newSize;
		return;
	}
	// Case: Need to grow
	// TODO: Grow as requested.
	thisClass newOne(newSize);
	if(this->size)
		memcpy(newOne.data, this->data, sizeof(T)*this->size);
	this->clearUpOnDelete = true;
	this->swap(newOne);
}

template<class T>
inline void ManagedBinary<T>::fillZeros() throw ()
{
	memset(this->data, 0, sizeof(T)*this->size);
}

template<class T>
inline void ManagedBinary<T>::empty() throw ()
{
	if(this->data || this->realSize)
	{
		delete[] this->data;
		this->data = NULL;
		this->realSize = this->size = 0;
	}
}

template<class T>
inline void ManagedBinary<T>::swap(thisClass& with) throw ()
{
	thisClass dummyOne;
	memcpy(&dummyOne, &with, sizeof(thisClass));
	memcpy(&with, this, sizeof(thisClass));
	memcpy(this, &dummyOne, sizeof(thisClass));
	dummyOne.clearUpOnDelete = false;
}

template<class T>
inline void ManagedBinary<T>::fill(const T x) throw ()
{
	T *p = this->data;
	const T *end = p + this->size;
	while(p < end)
		*(p++) = x;
}

template<class T>
inline void ManagedBinary<T>::fill(const unsigned char x) throw ()
{
	memset(this->data, x, sizeof(T)*this->size);
}

template<class T>
std::string ManagedBinary<T>::toString(const char** y) throw ()
{
	std::stringstream sb;
	sb << "[ManagedBinary<" << typeid(T).name() << "> @ " << std::hex << this << std::dec << "]\n";
	sb << "size: " << this->size << ", " << this->size*sizeof(T) << (this->size > 1? "bytes" : "byte");
	if(this->size == this->realSize)
		sb << "( == realSize)";
	else
		sb << "\nrealSize: " << this->realSize << ", " << this->realSize*sizeof(T) << (this->realSize > 1? "bytes" : "byte");
	if(! this->clearUpOnDelete)
		sb << "\nWill not clear up on delete.";

	if(y)
	{
		this->__dummyString = sb.str();
		*y = this->__dummyString.c_str();
	}
	return sb.str();
}

} // namespace 'ashe'

#endif // ASHE_MANAGEDBINARY_H_
