#ifndef ASHE_MANAGEDBINARY_H_
#define ASHE_MANAGEDBINARY_H_

#include <cstring>

namespace ashe
{

template <class T>
class ManagedBinary
{
public:
	typedef ManagedBinary<T> thisClass;

	T *data;
	size_t size, realSize;

protected:
	void __construct(const thisClass *src) throw()
	{
		if(src->data && src->size)
		{
			delete[] this->data;
			this->data = 0;
			this->realSize = this->size = 0;

			this->realSize = this->size = src->size;
			this->data = new T[this->size];
			memcpy(this->data, src->data, this->size * sizeof(T));
		}
		else
		{
			memset(this, 0, sizeof(thisClass));
		}
	};

public:
	ManagedBinary(const size_t size = 0) throw()
		: data(0)
		, size(0)
		, realSize(0)
	{
		if(size)
			this->setSize(size);
	}
	ManagedBinary(const thisClass *src) throw()
		: data(0)
		, size(0)
		, realSize(0)
	{
		this->__construct(src);
	}
	ManagedBinary(const thisClass &src) throw()
		: data(0)
		, size(0)
		, realSize(0)
	{
		this->__construct(&src);
	}
	virtual ~ManagedBinary() throw()
	{
		delete[] this->data;
		this->data = 0;
		this->realSize = this->size = 0;
	}

	virtual thisClass &operator =(const thisClass &src) throw()
	{
		this->__construct(&src);
		return *this;
	}
	virtual thisClass *operator =(const thisClass *src) throw()
	{
		this->__construct(src);
		return this;
	}

	virtual void setSize(const size_t size) throw()
	{
		if(this->size != size)
		{
			delete[] this->data;
			this->data = 0;
			this->size = this->realSize = size;
			if(this->realSize)
				this->data = new T[this->realSize];
		}
		memset(this->data, 0, this->realSize * sizeof(T));
	}
	virtual void resize(const size_t newSize, const bool forced = true) throw()
	{
		if(forced)
		{
			if(! newSize)
			{
				delete[] this->data;
				this->data = 0;
				this->realSize = this->size = 0;
			}
			else
			{
				T *newData = new T[newSize];
				memset(newData, 0, newSize  * sizeof(T));
				memcpy(newData, this->data, newSize * sizeof(T));
				delete[] this->data;
				this->data = 0;
				this->realSize = this->size = 0;
				this->realSize = this->size = newSize;
				this->data = newData;
			}
		}
		else
		{
			if(this->size < newSize)
			{
				T *newData = new T[newSize];
				memset(newData, 0, newSize  * sizeof(T));
				memcpy(newData, this->data, newSize * sizeof(T));
				delete[] this->data;
				this->data = 0;
				this->realSize = this->size = 0;
				this->realSize = this->size = newSize;
				this->data = newData;
			}
			else
				this->size = size;
		}
	}
	virtual void fillZeros() throw()
	{
		memset(this->data, 0, this->size * sizeof(T));
	}
	virtual void empty() throw()
	{
		delete[] this->data;
		this->data = 0;
		this->realSize = this->size = 0;		
	}
	virtual void swap(thisClass &with) throw()
	{
		T *data = this->data;
		const size_t size = this->size, realSize = this->realSize;

		this->data = with.data;
		this->size = with.size;
		this->realSize = with.realSize;
		with.data = data;
		with.size = size;
		with.realSize = realSize;
	}
};

} // namespace 'ashe'

#endif // ASHE_MANAGEDBINARY_H_
