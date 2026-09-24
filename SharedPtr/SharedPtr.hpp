#pragma once

struct Header
{
	size_t RefCounter;
};

template<typename T>
class SharedPtr
{
public:
	SharedPtr();

	SharedPtr(const SharedPtr& other);

	SharedPtr(SharedPtr&& other);

	~SharedPtr();

	SharedPtr& operator=(const SharedPtr& other);

	SharedPtr& operator=(SharedPtr&& other);

	T& operator*();
	T* operator->();
	bool operator==(const SharedPtr<T>& other) const;



	void MakeShared(T value);
	size_t UseCount() const;
	void Swap(SharedPtr<T>& other);

	void Reset();
	void Reset(T* new_ptr);

private:
	Header* header;
	T* ptr;
};

template<typename T>
SharedPtr<T>::SharedPtr()
{
	header = new Header();
	ptr = new T();
	header->RefCounter++;
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
	if (!header)
		return;

	header->RefCounter--;

	if (header->RefCounter <= 0)
	{
		delete header;
		delete ptr;
		this->ptr = nullptr;
		this->header = nullptr;
	}
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
{
	this->ptr = other.ptr;
	this->header = other.header;

	header->RefCounter = 1;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other)
{
	this->ptr = other.ptr;
	header = other.header;

	other.header = nullptr;
	other.ptr = nullptr;
}

template<typename T>
void SharedPtr<T>::MakeShared(T value)
{
	if (this->ptr != nullptr)
	{
		header->RefCounter--;
		if (header->RefCounter <= 0) {
			delete header;
			delete ptr;
		}
	}
		
	this->ptr = new T(value);
	header->RefCounter++;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other)
{
	if (this == &other)
		return *this;

	this->header->RefCounter--;

	if (this->header->RefCounter <= 0)
	{
		delete this->ptr;
		delete this->header;
	}

	this->ptr = other.ptr;
	this->header = other.header;

	this->header->RefCounter++;

	return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other)
{
	this->header->RefCounter--;

	if (this->header->RefCounter <= 0)
	{
		delete this->ptr;
		delete this->header;
	}

	this->header = other.header;
	this->ptr = other.ptr;

	other.ptr = nullptr;
	other.header = nullptr;

	return *this;
}

template<typename T>
T& SharedPtr<T>::operator*() {
	return *this->ptr;
}

template<typename T>
T* SharedPtr<T>::operator->() {
	return this->ptr;
}

template<typename T>
size_t SharedPtr<T>::UseCount() const {
	return this->header->RefCounter;
}

template<typename T>
void SharedPtr<T>::Swap(SharedPtr<T>& other) {
	std::swap(this->ptr, other.ptr);
	std::swap(this->header, other.header);
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const {
	if (this->ptr && other.ptr) {
		if (this->ptr == other.ptr && this->header->RefCounter == other.header->RefCounter) {
			return true;
		}

		return false;
	}

	return false;
}

template<typename T>
void SharedPtr<T>::Reset() {
	if (!header)
		return;

	header->RefCounter--;

	if (header->RefCounter <= 0)
	{
		delete header;
		delete ptr;
	}

	this->ptr = nullptr;
	this->header = nullptr;
}


template<typename T>
void SharedPtr<T>::Reset(T* new_ptr) {
	if (!header)
		return;

	header->RefCounter--;

	if (header->RefCounter <= 0)
	{
		delete header;
		delete ptr;
	}

	this->ptr = new_ptr;
	this->header = new Header();
	this->header->RefCounter = 1;
}