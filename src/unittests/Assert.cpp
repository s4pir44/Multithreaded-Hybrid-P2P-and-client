/*
 * Assert.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: demo
 */

#include "Assert.h"

template<class T>
bool Assert::assertFalse(T arg1, T arg2) {
	return arg1 != arg2;
}

template<class T>
bool Assert::assertTrue(T arg1, T arg2) {
	return arg1 == arg2;
}
