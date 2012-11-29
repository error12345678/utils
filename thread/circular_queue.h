#ifndef PANDARIA_CIRCULAR_QUEUE_H_
#define PANDARIA_CIRCULAR_QUEUE_H_

#include "auto_lock.h"

template <class T>
class circular_queue
{
public:
	circular_queue(unsigned long capacity) : _begin_cursor(0),_end_cursor(0),_to_quit(false)
	{
		_capacity = roundup_power_of_two(capacity);
		_data = new(std::nothrow) T [capacity];
		pthread_mutex_init(&_mutex,NULL);
		pthread_cond_init(&_cond,NULL);
	}
	~circular_queue()
	{
		if(_data)
		{
			delete [] _data;
		}
		pthread_cond_destroy(&_cond);
		pthread_mutex_destroy(&_mutex);
	}
public:
	bool pop(T& item,bool blocking)
	{
		auto_lock lock(&_mutex);
		if(_to_quit)
		{
			return false;
		}
		if(empty())
		{
			if(!blocking)
			{
				return false;
			}
			while(empty())
			{
				if(_to_quit)
				{
					return false;
				}
				pthread_cond_wait(&_cond,&_mutex);
			}
		}
		item = _data[_begin_cursor];
// 		++_begin_cursor;
// 		if(_begin_cursor == _capacity)
// 		{
// 			_begin_cursor = 0;
// 		}
		_begin_cursor = (_begin_cursor + 1)&(_capacity-1);
		return true;
	}

	bool push(const T& item)
	{
		auto_lock lock(&_mutex);
		if(_to_quit)
		{
			return false;
		}
		if(full())
		{
			return false;
		}
		bool is_empty = empty();
		_data[_end_cursor] = item;
//		_end_cursor = (_end_cursor + 1)%_capacity;
		_end_cursor = (_end_cursor + 1)&(_capacity-1);
		if(is_empty)
		{
			pthread_cond_signal(&_cond);
		}
		return true;

	}

	void clear()
	{
		auto_lock lock(&_mutex);
		delete [] _data;
		_data = NULL;
	}

	void deeply_clear()
	{
		auto_lock lock(&_mutex);
		for(unsigned long i=_begin_cursor;i != _end_cursor;)
		{
			delete _data[i];
			i = (i+1)&(_capacity-1);
		}
		delete [] _data;
		_data = NULL;
	}

	template<class OP>
	void do_for_each(OP func)
	{
		auto_lock lock(&_mutex);
		for(unsigned long i=_begin_cursor;i != _end_cursor;)
		{
			func(_data[i]);
			i = (i+1)&(_capacity-1);
		}
	}


	//ask all waiting thread to continue
	void signal_to_quit()
	{
		auto_lock lock(&_mutex);
		if(_to_quit)
		{
			return ;
		}
		_to_quit = true;
		pthread_cond_broadcast(&_cond);
	}
protected:
	inline bool empty()
	{
		return _begin_cursor == _end_cursor;
	}
	inline bool full()
	{
		return _begin_cursor == (_end_cursor + 1)&(_capacity-1);
	}

	unsigned long roundup_power_of_two(unsigned long val)
	{
		/*val is the power of 2*/
		if((val & (val-1)) == 0)
			return val;

		/*expand val to power of 2*/
		unsigned long maxulong = (unsigned long)((unsigned long)~0);
		unsigned long andv = ~(maxulong&(maxulong>>1));
		while((andv & val) == 0)
			andv = andv>>1;

		return andv<<1;
	}
private:
	T* _data;
	unsigned long _capacity;
	pthread_mutex_t _mutex;
	pthread_cond_t _cond;
	unsigned long _begin_cursor;
	unsigned long _end_cursor;
	volatile bool _to_quit;
};

#endif

