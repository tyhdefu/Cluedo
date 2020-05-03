#pragma once
#include <ostream>
#include <vector>
#include <Windows.h>

template<class T, int size>
class mytable
{
public:
	mytable();
	~mytable() {};

	mytable(bool val);

	mytable(bool val, T newarray[size]);

	void setallbools(bool val);

	void setkeys(T newarray[size]);
	void setkey(T key, int num);

	bool Read(T key);
	void Write(T key, bool boolean);

private:
	T mainarray[size];
	bool boolarray[size];
};

template<class key, class value>
class table2D
{
public:
	table2D();

	void Addkey(key key);
	
	void Addtoarray(key index, value val);
	void Cleararray(key index);

	void ChangeValue(key keyindex, int valindex, value val);

	// Checks vector for value
	bool CheckArray(key index, value val);

	// Removes any empty values from the vector
	void Remove(key keyindex, int valindex);

	// Get array for reading
	std::vector<value> GetArray(key index);
	
	int Findkey(key index);

	typename std::vector<value>::iterator begin(key index) { return valuearray[Findkey(index)].begin(); };
	typename std::vector<value>::iterator end(key index) { return valuearray[Findkey(index)].end(); };

	void Clear();

	std::vector<key> keyarray;
	std::vector<std::vector<value>> valuearray;
};

template<class key, class value>
table2D<key, value>::table2D()
{

}

template<class key, class value>
inline void table2D<key, value>::Addkey(key key)
{
	keyarray.push_back(key);
	valuearray.push_back(std::vector<value>());
}

template<class key, class value>
inline void table2D<key, value>::Addtoarray(key index, value val)
{
	this->valuearray[Findkey(index)].push_back(val);
}

template<class key, class value>
inline void table2D<key, value>::Cleararray(key index)
{
	this->valuearray[Findkey(index)].clear();
}

template<class key, class value>
inline void table2D<key, value>::ChangeValue(key keyindex, int valindex, value val)
{
	int index = Findkey(keyindex);
	valuearray[index][valindex] = val;
}

template<class key, class value>
inline bool table2D<key, value>::CheckArray(key index, value val)
{
	for (auto i = this->valuearray[Findkey(index)].begin(); i != this->valuearray[Findkey(index)].end(); i++)
	{
		if (*i == val)
			return true;
	}
	return false;
}

template<class key, class value>
inline void table2D<key, value>::Remove(key keyindex, int valindex)
{
	int index = Findkey(keyindex);
	keyarray.erase(keyarray.begin() + index);
	valuearray[index].erase((valuearray[index].begin() + valindex));
}

template<class key, class value>
inline std::vector<value> table2D<key, value>::GetArray(key index)
{
	return valuearray[Findkey(index)];
}

template<class key, class value>
inline int table2D<key, value>::Findkey(key index)
{
	for (auto i = keyarray.begin(); i != keyarray.end(); i++)
	{
		if (*i == index)
		{
			return (i - keyarray.begin());
		}
	}
	std::cout << "Could not find key in table2D" << std::endl;
	return 0;
}

template<class key, class value>
inline void table2D<key, value>::Clear()
{
	for (auto i = valuearray.begin(); i != valuearray.end(); i++)
	{
		i->clear();
	}
}

template<class T, int size>
inline bool mytable<T, size>::Read(T key)
{
	int i;
	for (i = 0; i < (size); i++)
	{
		if (this->mainarray[i] == key)
		{
			// found key
			break;
		}
		else if (i == size - 1)
		{
			std::cout << "key not found" << std::endl;
			return false;
		}
	}
	return boolarray[i];

}

template<class T, int size>
inline void mytable<T, size>::Write(T key, bool boolean)
{
	int i;
	for (i = 0; i < (size); i++)
	{
		if (this->mainarray[i] == key)
		{
			// found key
			this->boolarray[i] = boolean;
			break;
		}
		else if (i == (size - 1))
		{
			std::cout << "key not found" << "\n";
		}
	}

}

template<class T, int size>
inline mytable<T, size>::mytable()
{
}

template<class T, int size>
inline mytable<T, size>::mytable(bool val)
{
	for (int i = 0; i < size; i++)
	{
		this->boolarray[i] = val;
	}
}

template<class T, int size>
inline mytable<T, size>::mytable(bool val, T newarray[size])
{
	for (int i = 0; i < size; i++)
	{
		this->boolarray[i] = val;
	}

	for (int i = 0; i < size; i++)
	{
		this->mainarray[i] = newarray[i];
	}
}

template<class T, int size>
inline void mytable<T, size>::setallbools(bool val)
{
	for (int i = 0; i < size; i++)
	{
		this->boolarray[i] = val;
	}
}

template<class T, int size>
inline void mytable<T, size>::setkeys(T newarray[size])
{
	for (int i = 0; i < (size); i++)
	{
		this->mainarray[i] = newarray[i];
	}
}

template<class T, int size>
inline void mytable<T, size>::setkey(T key, int num)
{
	this->mainarray[num] = key;
}