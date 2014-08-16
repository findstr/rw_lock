#ifndef _RW_LOCK_H

#ifdef __cplusplus

extern "C" {

#endif

struct rw_lock {
        unsigned long lock;
        unsigned long write_ref_cnt;
        unsigned long read_ref_cnt;
};

int rwlock_init(struct rw_lock *lock);
int rwlock_get_write(struct rw_lock *lock, int retry);
int rwlock_put_write(struct rw_lock *lock);
int rwlock_get_read(struct rw_lock *lock, int retry);
int rwlock_put_read(struct rw_lock *lock);


#ifdef __cplusplus

}

#endif // __cplusplus


#endif // !_RW_LOCK_H