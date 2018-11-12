#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"

#define norw 13       /* �����ָ��� */
#define txmax 100     /* ���ű����� */
#define nmax 14       /* ���ֵ����λ�� */
#define al 10         /* ��ʶ������󳤶� */
#define addrmax 2048  /* ��ַ�Ͻ�*/
#define levmax 3      /* ����������Ƕ����������*/
#define cxmax 200     /* ��������������� */
#define stacksize 500 /* ����ʱ����ջԪ�����Ϊ500�� */
#define symnum 37
#define fctnum 9

char ch;            /* ��ŵ�ǰ��ȡ���ַ���getch ʹ�� */
enum symbol sym;    /* ��ǰ�ķ��� */
char id[al+1];      /* ��ǰident�������һ���ֽ����ڴ��0 */
int num;            /* ��ǰnumber */
int cc, ll;         /* getchʹ�õļ�������cc��ʾ��ǰ�ַ�(ch)��λ�� */
int linenum;
char line[81];      /* ��ȡ�л����� */
char a[al+1];       /* ��ʱ���ţ������һ���ֽ����ڴ��0 */

char word[norw][al];        /* ������ */
enum symbol wsym[norw];     /* �����ֶ�Ӧ�ķ���ֵ */
enum symbol ssym[256];      /* ���ַ��ķ���ֵ */
bool declbegsys[symnum];    /* ��ʾ������ʼ�ķ��ż��� */
bool statbegsys[symnum];    /* ��ʾ��俪ʼ�ķ��ż��� */
bool facbegsys[symnum];     /* ��ʾ���ӿ�ʼ�ķ��ż��� */

FILE* fin;      /* ����Դ�ļ� */


void init() {
    /* ���õ��ַ����� */
    memset(ssym, 0, sizeof ssym);
    ssym['%'] = mod;
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['{'] = lbrace;
    ssym['}'] = rbrace;
    ssym['='] = becomes;
    ssym['$'] = period;
    ssym[';'] = semicolon;

    /* ���ñ���������,������ĸ˳�򣬱��ڶ��ֲ��� */
    strcpy(word[0], "call");
    strcpy(word[1], "const");
    strcpy(word[2], "else");
    strcpy(word[3], "for");
    strcpy(word[4], "func");
    strcpy(word[5], "if");
    strcpy(word[6], "in");
    strcpy(word[7], "odd");
    strcpy(word[8], "print");
    strcpy(word[9], "read");
    strcpy(word[10], "repeat");
    strcpy(word[11], "var");
    strcpy(word[12], "while");

    /* ���ñ����ַ��� */
    wsym[0] = callsym;
    wsym[1] = constsym;
    wsym[2] = elsesym;
    wsym[3] = forsym;
    wsym[4] = funcsym;
    wsym[5] = ifsym;
    wsym[6] = insym;
    wsym[7] = oddsym;
    wsym[8] = printsym;
    wsym[9] = readsym;
    wsym[10] = repeatsym;
    wsym[11] = varsym;
    wsym[12] = whilesym;


    /* ���÷��ż� */
    memset(declbegsys, false, sizeof declbegsys);
    memset(statbegsys, false, sizeof statbegsys);
    memset(facbegsys, false, sizeof facbegsys);

    /* ����������ʼ���ż� */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[funcsym] = true;

    /* ������俪ʼ���ż� */
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;
    statbegsys[readsym] = true;
    statbegsys[printsym] = true;
    statbegsys[ident] = true;
    statbegsys[forsym] = true;
    statbegsys[repeatsym] = true;

    cc = ll = 0;
    ch = ' ';
    linenum = 0;
}


void compile() {
    fin = fopen("C:\\Users\\10152130143\\Desktop\\sw_compiler-master\\sw_lcm.txt", "r");
    //fin = stdin;
    bool nxtlev[symnum];
    init();		/* ��ʼ�� */

    //getsym();

}
