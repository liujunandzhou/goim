/*
 * =====================================================================================
 *
 *       Filename:  thread_new.h
 *
 *    Description:  implement the thread wrapper
 *
 *        Version:  1.0
 *        Created:  05/19/2016 11:54:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef _UTIL_THREAD_H_
#define _UTIL_THREAD_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <queue>
#include <vector>
#include <limits.h>

namespace thread_wrapper
{

	class Mutex
	{
		private:
			pthread_mutex_t mutex;
		public:
			Mutex()
			{
				pthread_mutex_init(&mutex, NULL);
			}
			~Mutex()
			{
				pthread_mutex_destroy(&mutex);
			}
			void lock()
			{
				pthread_mutex_lock(&mutex);
			}
			void unlock()
			{
				pthread_mutex_unlock(&mutex);
			}
	};

	class ReadWriteMutex 
	{
		private:
			pthread_rwlock_t rw_lock_;

		public:
			ReadWriteMutex() 
			{
				pthread_rwlock_init(&rw_lock_, NULL);
			}

			~ReadWriteMutex() 
			{
				pthread_rwlock_destroy(&rw_lock_);
			}

			void acquireRead()  
			{
				pthread_rwlock_rdlock(&rw_lock_);
			}

			void acquireWrite() 
			{
				pthread_rwlock_wrlock(&rw_lock_);
			}

			bool attemptRead()  
			{

				return !pthread_rwlock_tryrdlock(&rw_lock_); 
			}

			bool attemptWrite() 
			{ 
				return !pthread_rwlock_trywrlock(&rw_lock_); 
			}

			void release()  
			{
				pthread_rwlock_unlock(&rw_lock_);
			}
	};

	class Locking
	{

		private:
			Mutex *mutex;

			Locking(const Locking&);

			void operator=(const Locking&);

		public:
			Locking(Mutex *mutex)
			{
				this->mutex = mutex;
				this->mutex->lock();
			}

			~Locking(){

				this->mutex->unlock();
			}
	};

	class RWLocking
	{
		private:
			ReadWriteMutex *rwmutex;

			RWLocking(const RWLocking &);

			void operator=(const RWLocking &);

		public:
			RWLocking(ReadWriteMutex *rwmutex,bool write=false)
			{
				this->rwmutex=rwmutex;

				if(write){
					this->rwmutex->acquireWrite();
				}
				else{
					this->rwmutex->acquireRead();
				}
			}

			~RWLocking()
			{
				this->rwmutex->release();
			}
	};

	static pthread_t start_thread(void *(*func)(void*), void *arg, int flag)
	{
		pthread_attr_t attr;

		pthread_attr_init(&attr);

		int detachstate = PTHREAD_CREATE_JOINABLE;

		if (flag) detachstate = PTHREAD_CREATE_DETACHED;

		int r = pthread_attr_setdetachstate(&attr, detachstate);

		if (r) {
			printf("pthread_attr_setdetachstate failed\n");
			exit(1);
		}   

		r = pthread_attr_setguardsize(&attr, 0); 

		r = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

		if (r) {
			printf("pthread_attr_setstacksize failed\n");
			exit(1);
		}   

		pthread_t tid; 

		r = pthread_create(&tid, &attr, func, arg);

		if(r) {

			printf("pthread_attr_setstacksize failed\n");

			exit(1);
		}   

		pthread_attr_destroy(&attr);

		return tid; 
	}
}

#endif
