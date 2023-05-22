#pragma once

#include <string>
#include <vector>
#include <map>

template <typename T>
class Registry
{
private:
	std::vector<T>* id_container;
	std::map<std::string, T>* key_container;

public:
	Registry();
	~Registry();

	size_t Insert(const std::string& key, T item);

	T operator[](int index) const;
	T operator[](const std::string& key) const;
};

template<typename T>
inline Registry<T>::Registry()
{
	id_container = new std::vector<T>();
	key_container = new std::map<std::string, T>();
}

template<typename T>
inline Registry<T>::~Registry()
{
	delete id_container;
	delete key_container;
}

template<typename T>
inline size_t Registry<T>::Insert(const std::string& key, T item)
{
	id_container->push_back(item);
	key_container->insert(make_pair(key, item));

	return id_container->size() - 1;
}

template<typename T>
inline T Registry<T>::operator[](int index) const
{
	return id_container->at(index);
}

template<typename T>
inline T Registry<T>::operator[](const std::string& key) const
{
	return key_container->at(key);
}
