/* Created by Alex Nikol Bourakov on 23/11/2011 as a replacement for tomarray module */

#ifndef XMemoryAllocator_H
#define XMemoryAllocator_H


#define incr_far(z,s) (z+s)


class _XmemoryAllocator { 
	const short _element_size; 
	char **_blocks; 
	size_t _blocks_lim; 

protected: 
	int _elements_lim; 

public: 
	_XmemoryAllocator(size_t element_size): _element_size((short)element_size) { 
		_blocks = new char*[_blocks_lim = 128]; 
		_elements_lim = -1; 
		memset(_blocks, 0, _blocks_lim * sizeof(*_blocks)); 
	} 
	~_XmemoryAllocator() { 
		_Release();  
		delete _blocks; 
	} 
	char& operator[](size_t i) { 
		size_t block_num = i >> 10; 

		if(block_num >= (size_t)_blocks_lim) { 
			size_t new_blocks_lim = block_num << 1; 
			char **new_blocks = new char*[new_blocks_lim]; 
			memset(new_blocks, 0, new_blocks_lim * sizeof(*_blocks)); 
			memcpy(new_blocks, _blocks, _blocks_lim * sizeof(*_blocks)); 
			delete _blocks; 

			_blocks = new_blocks; 
			_blocks_lim = new_blocks_lim; 
		} 

		if(_blocks[block_num] == 0) { 
			_blocks[block_num] = new char[_element_size << 10]; 
			memset(_blocks[block_num], 0, _element_size << 10); 
		} 
		if(_elements_lim < (int)i) { 
			_elements_lim = i; 
		} 
		return _blocks[block_num][_element_size * (i&0x3FF)];
	}

	size_t Size() const { 
		if(_elements_lim < 0) { 
			return 0;
		} 
		return (size_t)(_elements_lim + 1); 
	} 

	void Resize(size_t lim){ 
		_elements_lim = ((int)lim) - 1; 
	} 

	void _Release() {
		for(size_t j = 0; j < (size_t)_blocks_lim; ++j) { 
			if(_blocks[j]) { 
				delete _blocks[j]; 
			} 
			_blocks[j] = 0; 
		} 
	}

	void fmemcpy(const char*Src, long Cnt/*in bytes*/) { 
		long block_size = _element_size * 1024; 
		for(size_t j = 0; Cnt >= 0; ++j, Cnt -= block_size, Src += block_size) { 
			memcpy(&this->operator[](j * 1024), Src, Cnt < block_size? Cnt: block_size); 
		} 
	} 
}; 


template<typename T> class XmemoryAllocator: public _XmemoryAllocator { 
public: 
	XmemoryAllocator(): _XmemoryAllocator(sizeof(T)) { 
	} 
	inline T& operator[](size_t i) { 
		return (T&)_XmemoryAllocator::operator[](i); 
	} 
	XmemoryAllocator<T>& operator = (const XmemoryAllocator<T>& other) { 
		size_t lim = other.Size(); 
		for(size_t j = 0; j < lim; j += 1024) { 
			size_t dif = lim - j; 
			memcpy(&operator[](j), &(const_cast<XmemoryAllocator<T>&>(other))[j], sizeof(T) * (dif < 1024? dif: 1024)); 
		} 
		_elements_lim = ((int)lim) - 1; 
		return *this; 
	} 
}; 


struct _zAvoid: public XmemoryAllocator<void*>
{
};
struct _sAlong: public XmemoryAllocator<long>
{
	void incr(long first, long last, double val);
	void incr(long first, long last, long val);
	long fetch_max_min(long G_lim, long&XM, long&xm, char return_min);
};
class _uAshort: public XmemoryAllocator<unsigned short>
{
};
class _uAchar: public XmemoryAllocator<unsigned char>
{
};
class _sAchar: public XmemoryAllocator<char>
{
};



class XMemoryFile { 
	_XmemoryAllocator _content; 
	char _name[32]; 

	fpos_t _pos; 
	fpos_t _length; 

	bool _initialized; 
	bool _eof; 

	void Reset() { 
		_pos = 0; 
		_eof = 0; 
	} 

	void Init() { 
		_length = 0; 
		_initialized = 0; 
		_name[0] = 0; 
		Reset(); 
	} 

public: 
	XMemoryFile(): _content(1) { 
		Init(); 
	} 
	bool isinitialized() const { 
		return _initialized; 
	} 
	bool operator==(const char *name) const { 
		return strcmp(_name, name) == 0; 
	} 
	void open(const char *name) { 
		strncpy(_name, name, sizeof(_name)); 
		_initialized = 1; 
		Reset(); 
	} 
	void close() { 
		Reset(); 
	} 
	void unlink() { 
		_content._Release(); 
		Init(); 
	} 
	void rename(const char *name) { 
		strncpy(_name, name, sizeof(_name)); 
	} 
	size_t read(char*desc, size_t elemsize, size_t count) { 
		size_t nread = 0; 
		while(_pos < _length && nread < count) { 
			size_t nbytes = 0; 
			while(nbytes++ < elemsize && _pos < _length) { 
				*desc++ = _content[(size_t)_pos++]; 
			} 
			if(--nbytes == elemsize) { 
				++nread; 
			} 
		} 
		if(nread < count) { 
			_eof = true; 
		} 
		return nread; 
	} 
	size_t write(const char*src, size_t elemsize, size_t count) { 
		size_t nwritten = 0; 
		while(nwritten < count) { 
			size_t nbytes = 0; 
			while(nbytes++ < elemsize) { 
				_content[(size_t)_pos++] = *src++; 
			} 
			++nwritten; 
		} 
		if(_pos > _length) { 
			_length = _pos; 
		} 
		_eof = _pos == _length; 
		return nwritten; 
	} 
	fpos_t getpos() { 
		return _pos; 
	} 
	int chsize(size_t newsize) { 
		_length = newsize; 
		return 0; 
	} 
	int setpos(const fpos_t *pos) { 
		_pos = pos? *pos: 0; 
		if(_pos > _length) { 
			_length = _pos; 
		} 
		_eof = false; 
		return 0; 
	} 
	int setpos_end() { 
		return setpos(&_length); 
	} 
	bool eof() { 
		return _eof; 
	} 
}; 


#endif 