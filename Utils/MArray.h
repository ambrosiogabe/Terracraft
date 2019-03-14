#pragma once
template<class T>
class MArray
{
public:
	MArray(int maxSize);
	~MArray();
	T operator [] (int i) { return this->arr[i]; };

	void push_back(T e);
	int length() { return this->size; };
	T* get_pointer();

private:
	int size;
	int maxSize;
	T* arr;
};

template<class T>
MArray<T>::~MArray() {
	delete[] arr;
}

template<class T>
MArray<T>::MArray(int maxSize) {
	this->arr = new T[maxSize];
	this->size = 0;
	this->maxSize = maxSize;
}

template<class T>
void MArray<T>::push_back(T e) {
	if (this->size < this->maxSize) {
		this->arr[this->size] = e;
		this->size++;
	}
	else {
		std::cout << "Tried to access index: " << this->size + 1 << " out of size: " << this->maxSize << std::endl;
	}
}

template<class T>
T* MArray<T>::get_pointer() {
	return &this->arr[0];
}