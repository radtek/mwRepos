#ifndef __ODBC_THREAD_H__
#define __ODBC_THREAD_H__

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <stdint.h>
#include <memory>
#include <string>
#include <pthread.h>

namespace ODBC
{

static const int kMicroSecondsPerSecond = 1000 * 1000;

template<typename T>
class ThreadLocal: boost::noncopyable
{
public:
	ThreadLocal()
	{
		pthread_key_create(&pkey_, &ThreadLocal::destructor);
	}

	~ThreadLocal()
	{
		pthread_key_delete(pkey_);
	}

	T& value()
	{
		T* perThreadValue = static_cast<T* >(pthread_getspecific(pkey_));
		if (!perThreadValue)
		{
			T* newObj = new T();
			pthread_setspecific(pkey_, newObj);
			perThreadValue = newObj;
		}
		return *perThreadValue;
	}

private:
	static void destructor(void* x)
	{
		T* obj = static_cast<T*>(x);
		typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
		T_must_be_complete_type dummy; (void) dummy;
		delete obj;
	}

private:
	pthread_key_t pkey_;
};

namespace CurrentThread
{
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;

void cacheTid();

inline int tid()
{
	if (__builtin_expect(t_cachedTid == 0, 0))
	{
		cacheTid();
	}
	return t_cachedTid;
}

inline const char* tidString()
{
	return t_tidString;
}

inline int tidStringLength()
{
	return t_tidStringLength;
}

inline const char* name()
{
	return t_threadName;
}

bool isMainThread();

void sleepUsec(int64_t usec);
} // namespace CurrentThread

class Thread: boost::noncopyable
{
public:
	typedef boost::function<void()> ThreadFunc;
	explicit Thread(const ThreadFunc&, const std::string& name = std::string());
	~Thread();

	void start();
	int join();

	bool started() const
	{
		return started_;
	}

	pid_t tid() const
	{
		return *tid_;
	}

	const std::string& name() const
	{
		return name_;
	}

private:
	void setDefaultName();

private:
	bool started_;
	bool joined_;
	pthread_t   pthreadId_;
	boost::shared_ptr<pid_t> tid_;
	ThreadFunc  func_;
	std::string name_;
};

} // namespace ODBC

#endif // __ODBC_THREAD_H__
