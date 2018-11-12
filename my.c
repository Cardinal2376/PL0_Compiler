#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compile.h"

#define norw 13       /* 保留字个数 */
#define txmax 100     /* 符号表容量 */
#define nmax 14       /* 数字的最大位数 */
#define al 10         /* 标识符的最大长度 */
#define addrmax 2048  /* 地址上界*/
#define levmax 3      /* 最大允许过程嵌套声明层数*/
#define cxmax 200     /* 最多的虚拟机代码数 */
#define stacksize 500 /* 运行时数据栈元素最多为500个 */
#define symnum 37
#define fctnum 9

char ch;            /* 存放当前读取的字符，getch 使用 */
enum symbol sym;    /* 当前的符号 */
char id[al+1];      /* 当前ident，多出的一个字节用于存放0 */
int num;            /* 当前number */
int cc, ll;         /* getch使用的计数器，cc表示当前字符(ch)的位置 */
int linenum;
char line[81];      /* 读取行缓冲区 */
char a[al+1];       /* 临时符号，多出的一个字节用于存放0 */

char word[norw][al];        /* 保留字 */
enum symbol wsym[norw];     /* 保留字对应的符号值 */
enum symbol ssym[256];      /* 单字符的符号值 */
bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
bool facbegsys[symnum];     /* 表示因子开始的符号集合 */

FILE* fin;      /* 输入源文件 */


void init() {
    /* 设置单字符符号 */
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

    /* 设置保留字名字,按照字母顺序，便于二分查找 */
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

    /* 设置保留字符号 */
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


    /* 设置符号集 */
    memset(declbegsys, false, sizeof declbegsys);
    memset(statbegsys, false, sizeof statbegsys);
    memset(facbegsys, false, sizeof facbegsys);

    /* 设置声明开始符号集 */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[funcsym] = true;

    /* 设置语句开始符号集 */
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
    init();		/* 初始化 */

    //getsym();

}
