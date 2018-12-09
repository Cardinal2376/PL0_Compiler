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
FILE* fin;      /* 输入源文件 */
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
    
    current_char = line_length = 0;
    ch = ' ';
    linenum = 0;
}


void compile() {
    //fin = fopen("C:\\Users\\10152130143\\Desktop\\sw_compiler-master\\sw_lcm.txt", "r");
    //fin = stdin;
    fin = fopen("/Users/cardinal/PL0_Compiler/test_lcm.txt", "r");
    bool nxtlev[symnum];
    init();        /* 初始化 */
    do {
        getsym();
        std::cout << symbol_word[sym] <<  std::endl;
    } while(sym != period);
    
    
}

/*
 词法分析
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

