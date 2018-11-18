/*
 * Assert.h
 *
 *  Created on: Aug 23, 2017
 *      Author: demo
 */

#ifndef UNITTESTS_ASSERT_H_
#define UNITTESTS_ASSERT_H_

class Assert {

public:
	template<class T>
	static bool assertTrue(T arg1,T arg2);
	template<class T>
	static bool assertFalse(T arg1,T arg2);
};

#endif /* UNITTESTS_ASSERT_H_ */
