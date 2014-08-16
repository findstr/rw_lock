#include <assert.h>
#include <Windows.h>
#include "rw_lock.h"

static int __lock__(struct rw_lock *lock)
{
        while (InterlockedCompareExchange(&lock->lock, 1, 0) == 0)
                Sleep(0);

        return 0;
}

static int __unlock__(struct rw_lock *lock)
{
        InterlockedExchange(&lock->lock, 0);
        return 0;
}

int rwlock_init(struct rw_lock *lock)
{
        assert(lock);
        lock->lock = 0;
        lock->write_ref_cnt = 0;
        lock->read_ref_cnt = 0;

        return 0;
}

/* write higher */
int rwlock_get_write(struct rw_lock *lock, int retry)
{
	unsigned long write_cnt;
        assert(lock);
	__lock__(lock);
        write_cnt = InterlockedIncrement(&lock->write_ref_cnt);
	__unlock__(lock);
	
	assert(write_cnt >= 1);

	if (write_cnt > 1) {	/* other thread have preemptive the write lock, wait it */
		while (lock->write_ref_cnt > 1)
			Sleep(0);
	}
        while (retry) {
                if (lock->read_ref_cnt == 0)
			break;

                retry--;
                Sleep(1);
        }

        if (retry <= 0) {
                InterlockedDecrement(&lock->write_ref_cnt);
                return -1;
        }
        
        return 0;
}

int rwlock_put_write(struct rw_lock *lock)
{
        assert(lock);
        assert(lock->write_ref_cnt > 0);
        
        InterlockedDecrement(&lock->write_ref_cnt);

        return 0;
}

int rwlock_get_read(struct rw_lock *lock, int retry)
{
        assert(lock);
        
        while (retry) {
                if (lock->write_ref_cnt == 0) {
                        __lock__(lock);
                        if (lock->write_ref_cnt == 0)
                                break;
                        else
                                __unlock__(lock);
                }
                retry--;
                Sleep(1);
        }

        if (retry <= 0)
                return -1;

        InterlockedIncrement(&lock->read_ref_cnt);
        __unlock__(lock);

        return 0;
}

int rwlock_put_read(struct rw_lock *lock)
{
        assert(lock);
        assert(lock->read_ref_cnt);

        InterlockedDecrement(&lock->read_ref_cnt);

        return 0;
}
