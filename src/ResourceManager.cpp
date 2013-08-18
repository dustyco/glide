

#include "ResourceManager.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <set>


struct ResourceManager::Internal
{
	bool               running;
	unsigned           thread_count;
	vector<thread*>    threads;
	list<Image*>       queue;
	set<Image*>        queue_working;
	mutex              queue_mutex;
	mutex              worker_mutex;
	condition_variable worker_cond;
	
	     Internal  ();
	     ~Internal ();
	void worker    ();
};


ResourceManager::ResourceManager () { i = new Internal; }
ResourceManager::~ResourceManager () { delete i; }

void ResourceManager::submit (Image* image)
{
	// Add the image to queue
	i->queue_mutex.lock();
	i->queue.push_front(image);
	i->queue_mutex.unlock();
	
	// Wake a worker thread
	i->worker_cond.notify_one();
}

ResourceManager::Internal::Internal ()
{
	running = true;
	
	// Get number of execution units (CPU cores or hyperthreading cores)
	thread_count = thread::hardware_concurrency();
	cout << "ResourceManager: Using " << thread_count << " threads" << endl;
	
	// Init threads
	threads.resize(thread_count);
	for (unsigned i=0; i!=thread_count; ++i)
		threads[i] = new thread(bind(&ResourceManager::Internal::worker, this));
}

ResourceManager::Internal::~Internal ()
{
	// Tell threads to stop
	running = false;
	worker_cond.notify_all();
	
	// Deallocate threads
	for (unsigned i=0; i!=thread_count; ++i)
	{
		threads[i]->join();
		delete threads[i];
	}
}

void ResourceManager::Internal::worker ()
{
	bool did_something = false;
	while (true)
	{
		// Check again before waiting
		if (!did_something)
		{
			// Wait for a signal to go
			unique_lock<mutex> worker_lock(worker_mutex);
			worker_cond.wait(worker_lock);
		}
		did_something = false;
		
		// Stop if it's time
		if (!running) return;
		
		// Pick the first visible image
		Image* image;
		bool got_one = false;
		queue_mutex.lock();
		for (list<Image*>::iterator it=queue.begin(); it!=queue.end(); ++it)
		{
			// Skip if another thread has it
			if (queue_working.find(*it) != queue_working.end()) continue;
			
			// Looks good, process it
			got_one = true;
			image = *it;
			queue_working.insert(*it);
			queue.erase(it);
			break;
		}
		queue_mutex.unlock();
		if (!got_one) continue;
	
		// TODO Do work
		did_something = true;
		image->resourceUpdate();
		
		// Release the image
		queue_mutex.lock();
		queue_working.erase(image);
		queue_mutex.unlock();
	}
}



