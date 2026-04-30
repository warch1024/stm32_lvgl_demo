#ifndef __RETARGET_H__
#define __RETARGET_H__
// #include <stdlib.h>
#include <stdio.h>
struct __FILE
{
    int handle;
};

int fputc(int ch, FILE* f);
void _sys_exit(int return_code);




#endif



