// test_read_write_lock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <time.h>
#include "rw_lock.h"
#include "assist.h"

#define USE_RW_LOCK     1

struct test {
        volatile int current;
        volatile int buff[1024];
} buff;

HANDLE hMutex;
struct rw_lock lock;

DWORD WINAPI read_thread(LPVOID lp)
{
        unsigned long cnt = (unsigned long) 0x1ffff;
        struct test *t = (struct test *)lp;

        while (cnt-- > 0) {
#if USE_RW_LOCK
                rwlock_get_read(&lock, 0x7fffffff);
#else
                WaitForSingleObject(hMutex, INFINITE);
#endif
                if (t->buff[t->current] == 1) {
                        //printf("--->read--curr index%d<---\n", t->current);
                } else {
                        printf("--->read--occurs error<---\n");
                        system("PAUSE");
 
                }
#if USE_RW_LOCK
                rwlock_put_read(&lock);
#else
                ReleaseMutex(hMutex);
#endif

                //Sleep(1);
        }

        return 0;
}

DWORD WINAPI write_thread(LPVOID lp)
{
        unsigned long cnt = (unsigned long) 0xf;
        struct test *t = (struct test *)lp;
        while (cnt-- > 0) {
#if USE_RW_LOCK
                rwlock_get_write(&lock, 0x7fffffff);
#else
                WaitForSingleObject(hMutex, INFINITE);
#endif
                t->buff[t->current] = 0;
                //Sleep(2);
                t->current = rand() % 1024;
                //Sleep(2);
                t->buff[t->current]++;

                //printf("--->write--curr index%d<---\n", t->current);
#if USE_RW_LOCK
                rwlock_put_write(&lock);
#else
                ReleaseMutex(hMutex);
#endif
                //Sleep(8);

        }
        return 0;
}


HANDLE rt_begin()
{
        HANDLE hThread = CreateThread(NULL, 0, read_thread, &buff, 0, NULL);

        return hThread;
}

HANDLE wt_begin()
{
        HANDLE hThread = CreateThread(NULL, 0, write_thread, &buff, 0, NULL);

        return hThread;
}

static int init()
{
        memset(&buff, 0, sizeof(buff));
        buff.buff[buff.current] = 1;
 
        srand((int)time(NULL));

        rwlock_init(&lock);

        hMutex = CreateMutex(NULL, FALSE, NULL);

        return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
        int i;
        HANDLE htbl[64];

        init();
 
        CACL_TAKES_TIME_BEGIN(tt);

        for (i = 0; i < 62; i++)
                htbl[i] = rt_begin();

        htbl[i++] = wt_begin();
        htbl[i++] = wt_begin();

        DWORD ret = WaitForMultipleObjects(64, htbl, TRUE, INFINITE);

        int takes = CACL_TAKES_TIME_END(tt);

        printf("-------takes %d ms\n", takes);

        system("pause");
        while (1)
                Sleep(10000);

	return 0;
}

