/*
 * tester.cpp
 *
 *  Created on: 2014. 10. 20.
 *      Author: david
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <thread>
#include <mutex>
#include <condition_variable>

#include "ManagedBinary.h"

typedef ashe::ManagedBinary<unsigned long long int> TestType;

static inline void __report(TestType &x) throw()
{
	std::cout << x.toString() << std::endl;
}

bool threadFlag = false;
std::mutex mtx;
std::condition_variable cv;
void threadRun(TestType *type, unsigned long long *n)
{
	std::unique_lock<std::mutex> lck(mtx);
	while(::threadFlag)
	{
		cv.wait_for(lck, std::chrono::seconds(1));
		if(! ::threadFlag)
			break;
		::__report(*type);
		std::cout << (int)((double)*n / (double)0xFFF * 100.0F) << "% ..." << std::endl;
	}
}

int main(const int argc, const char **args)
{
	TestType b;
	unsigned long long i = 0;
	srand(time(NULL));

	::threadFlag = true;
	std::thread th(::threadRun, &b, &i);

	for(i=0; i<0xFFF; ++i)
		b.resize(abs(rand())%(1024*1024*8), false);
	for(i=0; i<0xFFF; ++i)
		b.setSize(abs(rand())%(1024*1024*8));

	::threadFlag = false;
	cv.notify_all();
	th.join();
	::__report(b);

	return 0;
}
