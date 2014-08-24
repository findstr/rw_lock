#ifndef	_ASSIST_H
#define	_ASSIST_H

#define	ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))

#define	ALIGN_SIZE(a, align)	(((a + align - 1) / align) * align)

#define	ALIGN_SIZE_CNT(a, align)	((a + align - 1) / align)


#define OFFSET(type, member)    ((unsigned long)(&((type *)0)->member))


#define	CACL_TAKES_TIME_BEGIN(x)	LARGE_INTEGER	x##fre;\
					LARGE_INTEGER	x##ticks1, x##ticks2;\
					QueryPerformanceFrequency(&x##fre);\
					QueryPerformanceCounter(&x##ticks1)

#define	CACL_TAKES_TIME_END(x)	((int)(QueryPerformanceCounter(&x##ticks2),\
				((x##ticks2.QuadPart - x##ticks1.QuadPart) * 10 / (float)x##fre.QuadPart)))

#endif
