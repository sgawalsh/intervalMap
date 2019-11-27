// thinkCell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <map>
#include <limits>

#include <time.h>  

template<typename K, typename V>
class interval_map {
	std::map<K, V> m_map;

public:
	// constructor associates whole range of K with val by inserting (K_min, val)
	// into the map
	interval_map(V const& val) {
		m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
	}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		if (keyBegin < keyEnd && std::numeric_limits<K>::lowest() < keyBegin && std::numeric_limits<K>::lowest() < keyEnd) {
			V endPrev;
			V beginPrev = NULL;
			typename std::map<K, V>::iterator it = m_map.begin();
			while (it->first < keyEnd) {
				endPrev = it->second;
				if (keyBegin < it->first) {
					if (beginPrev == NULL) {
						--it;
						beginPrev = it->second;
						++it;
					}
					it = m_map.erase(it);
				}
				else { ++it; }
				if (it == m_map.end()) {
					break;
				}
			}
			if (beginPrev == NULL) {
				--it;
				beginPrev = it->second;
				++it;
			}
			if (!(val == beginPrev)) {
				//m_map.insert(m_map.end(), std::make_pair(keyBegin, val));
				m_map[keyBegin] = val;
			}
			if (!(val == endPrev)) {
				it = m_map.insert(m_map.end(), std::make_pair(keyEnd, endPrev));
				while (true) {
					--it;
					V prevVal = it->second;
					++it;
					if (it->second == prevVal) {
						it = m_map.erase(it);
						--it;
					}
					else { break; }
					if (it == m_map.begin()) { break; }
				}
			}
			
		}
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		return (--m_map.upper_bound(key))->second;
	}

	void printMap() {
		for (std::map<K, V>::iterator it = m_map.begin(); it != m_map.end(); ++it)
		{
			std::cout << it->first << " " << it->second << "\n";
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of unsigned int intervals to char.

int main()
{
	interval_map<int, char> test ('a');

	srand(time(NULL));
	int keyStart, keyEnd;
	const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char val;

	for (int i = 0; i < 100; ++i) {
		keyStart = rand() % 100;
		keyEnd = rand() % 100;
		val = a_to_z[rand() % 25];
		std::cout << "Start: " << keyStart << "End: " << keyEnd << ", Value: " << val << std::endl;
		test.assign(keyStart, keyEnd, val);
		test.printMap();
	}
}