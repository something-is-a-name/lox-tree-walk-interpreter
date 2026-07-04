#pragma once
#include <exception>
#include <any>

class Return : public std::exception {
public:

	explicit Return(std::any value);

	std::any value;
};