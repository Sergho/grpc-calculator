#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>

using std::map;

namespace constants
{
	const int OperationsCount = 4;
	const enum Operation : char {
		Addition = '+',
		Subtraction = '-',
		Multiplication = '*',
		Division = '/',
	};
}

#endif