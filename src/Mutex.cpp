

#include "Mutex.h"
#include <boost/thread/mutex.hpp>


struct Mutex::Internal
{
	boost::mutex m;
};

	
Mutex::Mutex ()
{
	i = new Internal;
}

Mutex::~Mutex ()
{
	delete i;
}

bool Mutex::tryLock ()
{
	return i->m.try_lock();
}

void Mutex::lock ()
{
	i->m.lock();	
}

void Mutex::unlock ()
{
	i->m.unlock();
}
