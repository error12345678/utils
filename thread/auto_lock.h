#ifndef DUCK_LOCK_H_
#define DUCK_LOCK_H_

#include <pthread.h>


class auto_lock
{
public:
	auto_lock(pthread_mutex_t* mutex):_mutex(mutex)
	{
		pthread_mutex_lock(_mutex);
	}

	~auto_lock()
	{
		pthread_mutex_unlock(_mutex);
	}
private:
	pthread_mutex_t* _mutex;
};

class auto_spin_lock
{
public:
	auto_spin_lock(pthread_spinlock_t* spinlock):_spinlock(spinlock)
	{
		pthread_spin_lock(_spinlock);
	}
	~auto_spin_lock()
	{
		pthread_spin_unlock(_spinlock);
	}
private:
	pthread_spinlock_t* _spinlock;
};

class auto_write_lock
{
public:
	auto_write_lock(pthread_rwlock_t* rwlock):_rwlock(rwlock)
	{
		pthread_rwlock_wrlock(_rwlock);
	}
	~auto_write_lock()
	{
		pthread_rwlock_unlock(_rwlock);
	}
private:
	pthread_rwlock_t* _rwlock;
};

class auto_read_lock
{
public:
	auto_read_lock(pthread_rwlock_t* rwlock) : _rwlock(rwlock)
	{
		pthread_rwlock_rdlock(_rwlock);
	}
	~auto_read_lock()
	{
		pthread_rwlock_unlock(_rwlock);
	}
private:
	pthread_rwlock_t* _rwlock;
};

















































#endif
