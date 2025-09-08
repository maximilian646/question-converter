#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <vector>


template <typename T>
void printV(std::vector<T> vec){
	//function to easily print out a vector structure
	for(auto i : vec){
		std::cout << i << std::endl;
		std::cout << "---" << std::endl;
	}
};

#endif
