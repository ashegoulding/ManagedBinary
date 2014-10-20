/*
 * tester.cpp
 *
 *  Created on: 2014. 10. 20.
 *      Author: david
 */

#include <iostream>

#include "ManagedBinary.h"

int main(const int argc, const char **args)
{
	ashe::ManagedBinary<unsigned char> bm;
	bm.setSize(1024);

	return 0;
}
