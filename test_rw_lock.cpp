// test_read_write_lock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <time.h>
#include "rw_lock.h"
#define USE_RW_LOCK     1

struct test {
        int current;
        int buff[1024];
} buff;

struct rw_lock lock;

DWORD WINAPI read_thread(LPVOID lp)
{
        struct test *t = (struct test *)lp;

        while (1) {
                rwlock_get_read(&lock, 1000000);
                if (t->buff[t->current] == 1) {
                        printf("--->read--curr index%d<---\n", t->current);
                } else {
                        printf("--->read--occurs error<---\n");
                        system("PAUSE");
 
                }

                rwlock_put_read(&lock);

                Sleep(1);
        }
}

DWORD WINAPI write_thread(LPVOID lp)
{
        struct test *t = (struct test *)lp;
        while (1) {
                rwlock_get_write(&lock, 1000);
                
                t->buff[t->current] = 0;
                Sleep(2);
                t->current = rand() % 1024;
                Sleep(2);
                t->buff[t->current]++;

                printf("--->write--curr index%d<---\n", t->current);

                rwlock_put_write(&lock);
                Sleep(8);

        }
}


int rt_begin()
{
        HANDLE hThread = CreateThread(NULL, 0, read_thread, &buff, 0, NULL);
	CloseHandle(hThread);

        return 0;
}

int wt_begin()
{
        HANDLE hThread = CreateThread(NULL, 0, write_thread, &buff, 0, NULL);
	CloseHandle(hThread);

        return 0;
}

static int init()
{
        memset(&buff, 0, sizeof(buff));
        buff.buff[buff.current] = 1;
 
        srand((int)time(NULL));

        rwlock_init(&lock);

        return 0;
}

#if USE_RW_LOCK



int _tmain(int argc, _TCHAR* argv[])
{
        init();

        for (int i = 0; i < 64; i++)
                rt_begin();

        wt_begin();
        wt_begin();


        while (1)
                Sleep(10000);

	return 0;
}

#else

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}


#endif

