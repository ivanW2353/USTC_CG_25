#include <iostream>
#include <cassert>

#include "TemplateDArray.h"

// default constructor
template<typename T>
TemplateDArray<T>::TemplateDArray() {
	Init();
}

// set an array with default values
template<typename T>
TemplateDArray<T>::TemplateDArray(int nSize, const T& dValue)
	: m_pData(new T[nSize]), m_nSize(nSize), m_nMax(nSize)
{
	for (int i = 0; i < nSize; i++)
		m_pData[i] = dValue;
}

template<typename T>
TemplateDArray<T>::TemplateDArray(const TemplateDArray& arr)
	: m_pData(new T[arr.m_nSize]), m_nSize(arr.m_nSize), m_nMax(arr.m_nSize)
{
	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr.m_pData[i];
}

// deconstructor
template<typename T>
TemplateDArray<T>::~TemplateDArray()
{
	Free();
}

// display the elements of the array
template<typename T>
void TemplateDArray<T>::Print() const {
	std::cout << "size= " << m_nSize << ":";
	for (int i = 0; i < m_nSize; i++)
		std::cout << " " << GetAt(i);
	std::cout << std::endl;
}

// initilize the array
template<typename T>
void TemplateDArray<T>::Init() {
	m_pData = nullptr;
	m_nSize = 0;
	m_nMax = 0;
}

// free the array
template<typename T>
void TemplateDArray<T>::Free() {
	delete[] m_pData;
	m_pData = nullptr;
	m_nSize = 0;
	m_nMax = 0;
}

// allocate enough memory
template<typename T>
void TemplateDArray<T>::Reserve(int nSize) {
	if (m_nMax >= nSize)
		return;
	while (m_nMax < nSize)
		m_nMax = (m_nMax == 0) ? 1 : 2 * m_nMax;
	T* pData = new T[m_nMax];
	for (int i = 0; i < m_nSize; i++)
		pData[i] = m_pData[i];
	delete[] m_pData;
	m_pData = pData;
}

// get the size of the array
template<typename T>
int TemplateDArray<T>::GetSize() const {
	return m_nSize;
}

// set the size of the array
template<typename T>
void TemplateDArray<T>::SetSize(int nSize) {
	if (m_nSize == nSize)
		return;
	Reserve(nSize);
	for (int i = m_nSize; i < nSize; i++)
		m_pData[i] = static_cast<T>(0);
	m_nSize = nSize;
}

// get an element at an index
template<typename T>
const T& TemplateDArray<T>::GetAt(int nIndex) const {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// set the value of an element
template<typename T>
void TemplateDArray<T>::SetAt(int nIndex, const T& dValue) {
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

// overload operator '[]'
template<typename T>
T& TemplateDArray<T>::operator[](int nIndex) {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// overload operator '[]'
template<typename T>
const T& TemplateDArray<T>::operator[](int nIndex) const {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// add a new element at the end of the array
template<typename T>
void TemplateDArray<T>::PushBack(const T& dValue) {
	Reserve(m_nSize + 1);
	m_pData[m_nSize++] = dValue;
}

// delete an element at some index
template<typename T>
void TemplateDArray<T>::DeleteAt(int nIndex) {
	assert(nIndex >= 0 && nIndex < m_nSize);
	for (int i = nIndex; i < m_nSize - 1; i++)
		m_pData[i] = m_pData[i + 1];
	m_nSize--;
}

// insert a new element at some index
template<typename T>
void TemplateDArray<T>::InsertAt(int nIndex, const T& dValue) {
	assert(nIndex >= 0 && nIndex <= m_nSize);
	Reserve(m_nSize + 1);
	for (int i = m_nSize; i > nIndex; i--)
		m_pData[i] = m_pData[i - 1];
	m_pData[nIndex] = dValue;
	m_nSize++;
}

// overload operator '='
template<typename T>
TemplateDArray<T>& TemplateDArray<T>::operator = (const TemplateDArray& arr) {
	if (this == &arr)
		return *this;
	delete[] m_pData;
	m_pData = new T[arr.m_nSize];
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;
	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr.m_pData[i];
	return *this;
}

// explicit instantiation
template class TemplateDArray<double>;
template class TemplateDArray<int>;
template class TemplateDArray<char>;
