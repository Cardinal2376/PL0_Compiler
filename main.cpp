/*
 * sw complier program implemented in C
 *
 * The program has been tested on CodeBlocks
 *
 * ʹ�÷�����
 * ���к�����swԴ�����ļ���
 * �ش��Ƿ�������������
 * �ش��Ƿ�������ű�
 * fcode.txt������������
 * foutput.txt���Դ�ļ��͸��ж�Ӧ�����ɴ����׵�ַ�����޴�
 * fresult.txt������н��
 * ftable.txt������ű�
 * ferror.txt����������д�
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"
#include "my.c"

/* ������ʼ */
int main(int argc, char *argv[]) {
    //printf("������sw�����ļ���: ");
    //scanf("%s", fname);		/* �����ļ��� */
    //printf("wtf\n");

    compile();

    return 0;
}

