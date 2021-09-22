#ifndef SHARED_PTR
#define SHARED_PTR

namespace ft {

template<class T>
class shared_ptr
{
public:
	shared_ptr(): _ptr(NULL), _refCount(0)				{ _refCount = new size_t(0);}
	shared_ptr(T* pointer): _ptr(pointer), _refCount(0)	{ _refCount = new size_t(1);}
	shared_ptr(const shared_ptr & other)
	{
		_ptr = other._ptr;
		_refCount = other._refCount;
		if (_ptr != NULL)
			++(*_refCount);
	}
	~shared_ptr() { _clear();}

	shared_ptr& operator=(const shared_ptr& other)
	{
		_clear();
		_ptr = other._ptr;
		_refCount = other._refCount;
		if (_ptr != NULL)
			++(*_refCount);
		return *this;
	}
	T* operator->() const				{ return _ptr;}
	T& operator*() const				{ return *_ptr;}
	T* 		get() const					{ return _ptr;}
	size_t	getReferenceCount() const	{ return *_refCount; }

private:
	void _clear()
	{
		if (*_refCount)
			--(*_refCount);
		if (*_refCount == 0)
		{
			delete _ptr;
			delete _refCount;
		}
	}

	T*	 	_ptr;
	size_t* _refCount;
}; /* class shared_ptr */

} /* namespace ft */

#endif /* SHARED_PTR */