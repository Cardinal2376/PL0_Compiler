//
//  my.cpp
//  PL0
//
//  Created by Cardinal on 2018/11/15.
//  Copyright © 2018年 Cardinal. All rights reserved.
//

#include "compile.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cctype>
#include <vector>
#include <map>


#define norw 13       /* 保留字个数 */
#define nmax 14       /* 数字的最大位数 */
#define addrmax 2048  /* 地址上界*/
#define levmax 3      /* 最大允许过程嵌套声明层数*/
#define cxmax 200     /* 最多的虚拟机代码数 */
#define stacksize 500 /* 运行时数据栈元素最多为500个 */
#define symnum 37
#define fctnum 9

const int max_length = 10;  /* max length of ident */
const int table_max = 100;  /* capacity of the symbol table */

char ch;                    /* used by function "getch", to read and store a character */
enum symbol sym;            /* current symbol */
int current_char, line_length;  /* used by function "getch" */
int linenum;
int num;                    /* current number value */
char line[81];              /* line buffer for reading */
int cx;                     /* pointer for the virtual machine */
std::map<std::string, symbol> wsym; /* the sym of key word */
std::map<int, symbol> ssym;         /* the sym of single char */

/* Table Structure */
struct tableStruct {
    char name[max_length];
    enum object kind;       /* const, variable or procedure */
    int val;                /* only for const */
    int level;
    int addr;
    int size;
    int type;               /* int or char */
};

struct tableStruct table[table_max];

bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
bool facbegsys[symnum];     /* 表示因子开始的符号集合 */

FILE* fin;      /* 输入源文件 */
FILE* ftable;
/* On Developing */
std::vector<std::string> symbol_word {
    "nul",      "ident",    "number",   "plus",     "minus",
    "times",    "slash",    "eql",      "neq",      "lss",
    "leq",      "gtr",      "geq",      "lparen",   "rparen",
    "semicolon","period",   "becomes",  "ifsym",    "elsesym",
    "whilesym", "printsym", "readsym",  "range",    "callsym",
    "varsym",   "funcsym",  "forsym",   "insym",    "lbrace",
    "rbrace",   "mod",      "add",      "sub",      "constsym",
    "oddsym",   "repeatsym"
};

void init() {
    /* 设置单字符符号 */
    ssym.clear();
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
    
    /* initialize the key word symbol map */
    wsym["call"] = callsym;
    wsym["const"] = constsym;
    wsym["else"] = elsesym;
    wsym["for"] = forsym;
    wsym["func"] = funcsym;
    wsym["if"] = ifsym;
    wsym["in"] = insym;
    wsym["odd"] = oddsym;
    wsym["print"] = printsym;
    wsym["read"] = readsym;
    wsym["repeat"] = repeatsym;
    wsym["var"] = varsym;
    wsym["while"] = whilesym;
    wsym["char"] = charsym;
    
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

    /* 设置因子开始符号集 */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;

    current_char = line_length = 0;
    ch = ' ';
    linenum = 0;
}


void compile() {
    //fin = fopen("C:\\Users\\10152130143\\Desktop\\sw_compiler-master\\sw_lcm.txt", "r");
    //fin = stdin;
    fin = fopen("/Users/cardinal/PL0_Compiler/test_lcm.txt", "r");
    ftable = fopen("/Users/cardinal/PL0_Compiler/ftable.txt", "r");
    bool nxtlev[symnum];
    init();        /* 初始化 */
    do {
        getsym();
        std::cout << symbol_word[sym] <<  std::endl;
    } while(sym != period);
    
}

/*
 * 用数组实现集合的集合运算
 */
int inset(int e, bool* s) {
    return s[e];
}

/*
 word analysis
 */
void getsym() {
    //remove blanks
    while(ch == ' ' || ch == '\n' || ch == '\t') {
        getch();
    }
    if(ch == '$') sym = period;
    else if(isalpha(ch)) {
        //recognize symbol or ident
        std::string ident_in = "";
        //这里长度报错怎么办
        while(isalnum(ch) && ident_in.length() < max_length) {
            ident_in.push_back(ch);
            getch();
        }
        //judge if be the key word
        if(wsym.find(ident_in) != wsym.end()) {
            sym = wsym[ident_in];
        } else {
            sym = ident;
        }
    } else if(isdigit(ch)){
        num = 0;
        int k = 0;
        sym = number;
        do {
            num = 10 * num + ch - '0';
            k++;
            getch();
        } while(isdigit(ch));
        if(k > nmax) {
            //error(14);
        }
    } else {
        if(ch == '<') {
            getch();
            if(ch == '=') {
                sym = leq;
                getch();
            } else {
                sym = lss;
            }
        } else if(ch == '>') {
            getch();
            if(ch == '=') {
                sym = geq;
                getch();
            } else {
                sym = gtr;
            }
        } else if(ch == '=') {
            getch();
            if(ch == '=') {
                sym = eql;
                getch();
            } else
                sym = becomes;
        } else if(ch == '!') {
            getch();
            if(ch == '=') {
                sym = neq;
                getch();
            } else
                sym = nul;
        } else if(ch == '/') {
            getch();
            if(ch == '/') {    //行注释
                current_char = line_length;
                getch();
                getsym();
            } else if(ch == '*') { //块注释
                int flag = 0;
                while(ch != '$') {
                    getch();
                    if(ch == '*') flag = 1;
                    else if(ch == '/' && flag == 1) {
                        getch();
                        getsym();
                        break;
                    } else {
                        flag = 0;
                    }
                }
                if(ch == '$') sym = period;
            } else {    //除号
                sym = slash;
                getch();
            }
        } else if(ch == '+') {
            getch();
            if(ch == '+') {
                sym = add;
                getch();
            } else {
                sym = plus;
            }
        } else if(ch == '-') {
            getch();
            if(ch == '-') {
                sym = sub;
                getch();
            } else {
                sym = minus;
            }
        } else {
            //符号开头，不在表里怎么办，应该报一个error
            if(ssym.find(ch) != ssym.end()) {
                sym = ssym[ch];
                if(sym != period) {
                    getch();
                }
            } else {
                //error
            }
        }
    }
}
/*
 to get a single character
 */
void getch() {
    if(current_char == line_length) {
        //if char remain in buffer, o.w. get another char
        if(feof(fin)) {
            ch = '$';
            return;
        }
        line_length = 0;
        current_char = 0;
        linenum++;
        ch = ' ';
        while(ch != '\n') {
            if(fscanf(fin, "%c", &ch) == EOF) {
                line[line_length] = 0;
                break;
            }
            line[line_length] = ch;
            line_length++;
        }
    }
    ch = line[current_char];
    current_char++;
}

/*
 * 生成虚拟机代码
 *
 * x: instruction.f;
 * y: instruction.l;
 * z: instruction.a;
 */
void gen(enum fct x, int y, int z ) {
    if (cx >= cxmax) {
        printf("生成的虚拟机代码程序过长!\n");  /* 生成的虚拟机代码程序过长 */
        exit(1);
    }
    if ( z >= addrmax) {
        printf("地址偏移越界!\n");    /* 地址偏移越界 */
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}

/*
syntax analysis
lev: current level
tx:  tail of the table
*/
void block(int lev, int tx) {
    int dx;
    int tx0 = tx; /* preserve original tx */
    int cx0; /* preserve original cx */

    dx = 3; /* leave 3 space for Static Link, Dynamic Link and Return Address */
    table[tx].addr = cx;
    gen(jmp, 0, 0); /* generate jmp instruction but left for filled in later */

    if(lev > 1) { /* too much level */
        //error(30);
    }
    if(sym == nul) sym = period;
    do {
        while(sym == varsym || sym == constsym) { /* to handle declaration statement */
            if (sym == varsym) {
                getsym();
                vardeclaration(&tx, lev, &dx);
                if (sym == semicolon) {
                    getsym();
                } else {
                    error(9); /* 漏掉了分号 */
                }
            } else {
                getsym();
                constdeclaration(&tx, lev, &dx);    /* dx的值会被constdeclaration改变，使用指针 */
                if (sym == semicolon) { /* 遇到分号结束定义常量 */
                    getsym();
                } else {
                    error(9);   /* 漏掉了分号 */
                }
            }
        }

        while (sym == funcsym) { /* 遇到过程声明符号，开始处理过程声明 */
            getsym();

            if (sym == ident) {
                enter(function, &tx, lev, &dx); /* 填写符号表 */
                getsym();
            } else {
                error(0);   /* procedure后应为标识符 */
            }

            /* 没有参数 */
            if (sym == lparen) getsym();
            else error(19);
            if (sym == rparen) getsym();
            else error(19);
            if (sym == lbrace) getsym();
            else error(20);
        
            block(lev + 1, tx, nxtlev); /* 递归调用 */

            if(sym == rbrace) {
                getsym();
            } else {
                error(38);  /* 漏掉了'}' */
            }
        }

        if (sym == period) break;
    } while (inset(sym, declbegsys));    /* 直到没有声明符号 */

    code[table[tx0].adr].a = cx;    /* 把前面生成的跳转语句的跳转位置改成当前位置 */
    table[tx0].adr = cx;            /* 记录当前过程代码地址 */
    table[tx0].size = dx;           /* 声明部分中每增加一条声明都会给dx增加1，声明部分已经结束，dx就是当前过程数据的size */
    cx0 = cx;
    gen(ini, 0, dx);                /* 生成指令，此指令执行时在数据栈中为被调用的过程开辟dx个单元的数据区 */

    for (i = 1; i <= tx; i++) {
        switch (table[i].kind) {
        case constant:
            fprintf(ftable, "%d const %s ", i, table[i].name);
            fprintf(ftable, "val=%d\n", table[i].val);
            break;
        case variable:
            fprintf(ftable, "%d var   %s ", i, table[i].name);
            fprintf(ftable, "lev=%d addr=%d\n", table[i].level, table[i].adr);
            break;
        case function:
            fprintf(ftable,"%d func  %s ", i, table[i].name);
            fprintf(ftable,"lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
            break;
        }
    }
    fprintf(ftable,"\n");

    statement(nxtlev, &tx, lev);
    gen(opr, 0, 0);                     /* 每个过程出口都要使用的释放数据段指令 */

}


/*
 * 在符号表中加入一项
 *
 * k:      标识符的种类为const，var或procedure
 * ptx:    符号表尾指针的指针，为了可以改变符号表尾指针的值
 * lev:    标识符所在的层次
 * pdx:    dx为当前应分配的变量的相对地址，分配后要增加1
 *
 */
void enter(enum object k, int* ptx, int lev, int* pdx) {
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* 符号表的name域记录标识符的名字 */
    table[(*ptx)].kind = k;
    switch (k) {
    case constant:  /* 常量 */
        if (num > addrmax) {
            error(15);  /* 常数越界 */
            num = 0;
        }
        table[(*ptx)].val = num; /* 登记常数的值 */
        break;
    case variable:  /* 变量 */
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    case function:  /* 过程 */
        table[(*ptx)].level = lev;
        break;
    }
}



/*
 * 查找标识符在符号表中的位置，从tx开始倒序查找标识符
 * 找到则返回在符号表中的位置，否则返回0
 *
 * id:    要查找的名字
 * tx:    当前符号表尾指针
 */
int position(char* id, int tx) {
    int i;
    strcpy(table[0].name, id);
    i = tx;
    while (strcmp(table[i].name, id) != 0) {
        i--;
    }
    return i;
}


/*
 * 常量声明处理
 */
void constdeclaration(int* ptx, int lev, int* pdx) {
    if (sym == ident) {
        getsym();
        if (sym == becomes) {
            getsym();
            if (sym == number) {
                enter(constant, ptx, lev, pdx);
                getsym();
            } else {
                error(32);  /* 常量声明中的=后应是数字 */
            }
        } else {
            error(8);   /* 常量声明中的标识符后应是= */
        }
    } else {
        error(0);   /* const后应是标识符 */
    }
}


/*
 * 变量声明处理
 */
void vardeclaration(int* ptx,int lev,int* pdx) {
    if (sym == ident) {
        enter(variable, ptx, lev, pdx); // 填写符号表
        getsym();
    } else {
        error(0);   /* var后面应是标识符 */
    }
}


