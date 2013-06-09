#include <new>
#include <iostream>


class IndexOutofBounds {
public:
	IndexOutofBounds(int v) : m_badValue(v) { }
	virtual ~IndexOutofBounds() { }
	void diagnostic(); 
private:
	int m_badValue;
};

template <typename T> 
class Vector {
public:
	Vector();		// create an empty vector
	Vector(int size);	// create a vector of size elements
	Vector(const Vector& r);	// copy ctor
	virtual ~Vector();
	T& operator[](int index) throw(IndexOutofBounds);
		// return the specified element, throws exception when index <0 or >=m_nSize
	int size();		// return the size of the vector
	int inflate(int addSize);	// expand the storage to m_nSize+addSize, return the new size
private:
	T *m_pElements;
	int m_nSize;
};

template <typename T>
Vector<T>::Vector() {
	m_nSize = 0;
};

template <typename T>
Vector<T>::Vector (int size) : m_nSize(size) {
	T *m_pElements = new T[size];	
};

template <typename T>
Vector<T>::Vector (const Vector& r) {
	m_nSize = r.size();
	try {
		*m_pElements = new T[m_nSize];
	}
	catch (std::bad_alloc ba_loc) {
		std::cerr << "Memory allocation failed" << ba_loc.what() << char(10);
	}
	for (int i = 0; i < m_nSize; i++) {
		*(m_pElements + i) = r[i];	
	}
};

template <typename T>
T& Vector<T>::operator[](int index) throw(IndexOutofBounds) {
	try {
		if (index < 0 || index > m_nSize)
		throw IndexOutofBounds(index);
		else {
			return *(m_pElements + index);
		}
	}
	catch (IndexOutofBounds &ex) {
		ex.diagnostic();
	}
	//catch (NotAlloced) {
	//}
}

template <typename T>
int Vector<T>::inflate(int addSize) {
	T* dele = m_pElements;
	try {
		m_pElements = new T[addSize + m_nSize];
	}
	catch (std::bad_alloc ba_loc) {
		std::cerr << "Memory allocation failed" << ba_loc.what() << char(10);
	}
	for (int i = 0; i < m_nSize; i++) {
		*(m_pElements + i) = *(dele + i);
	}
	delete[]  dele;
	m_nSize += addSize;
}

template <typename T>
int Vector<T>::size() {
	return m_nSize;
}
