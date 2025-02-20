#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <stdexcept>

using std::invalid_argument;
using std::map;

namespace constants
{
	const invalid_argument InvalidFormat("Invalid string format");
	const invalid_argument UnknownOperation("Unknown operation");
	const int OperationsCount = 4;
	enum Operation : char
	{
		Addition = '+',
		Subtraction = '-',
		Multiplication = '*',
		Division = '/',
	};
}

#endif