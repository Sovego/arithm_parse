#pragma comment (linker,"/STACK:5000000000")
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char text[5100000];                               //весь текст лежит здесь
int pos=0;                                      //текущая позиция в тексте
char* substr(char *str, int start, int length )
{
    char *s;

    int len = 0;
    for (int i = 0; str[i] != '\0'; i++)
        len++;

    if (length > 0)
    {
        if (start + length < len)
            len = start + length;
    }
    else
        len = len + length;
    int newlen = len - start + 1;
    s = calloc(newlen,sizeof(char));

    int j = 0;
    for (int i = start; i<len; i++)
    {
        s[j] = str[i]; j++;
    }
    s[j] = '\0';
    return(s);
}
char token[1];                              //текущий токен
char *readToken() {
    while (isspace(text[pos])){                   //пропускаем пробельные символы ' ', '\n', '\t', и т.п.
        pos++;                                    //(см. функцию isspace из стандартной библиотеки)
    }
    if (text[pos] == 0) {
        token[0] = '\0';//если символ нулевой, то это конец текста
        return token;                        //возвращаем токен [eof]
    }
    if (text[pos] =='+' || text[pos] =='-'|| text[pos] =='*'|| text[pos] =='/'|| text[pos] =='('|| text[pos] ==')')
    {
        token[0]=text[pos++];//это простой односимвольный токен
        return token;               //копируем его в буфер token и возвращаем
    }
    int left = pos;                             //остался один случай: целое число
    while ( isdigit(text[pos]))                   //токен продолжается, пока идут цифры
        pos++;
    char* s = substr(text,left,pos);
    strcpy(token,s);
    //(см. функцию isdigit из станд. библ.)
    return token;              //копируем отрезок в буфер token и возвращаем
}
char* peekToken(){
    int oldPos = pos;
    readToken();
    pos = oldPos;
    return token;
}
double ParseMonome();
double ParseExpr() {
    double res = ParseMonome();
    char oper;//в любом случае выражение начинается с монома
    while (*peekToken()== '+' || *peekToken()== '-') {           //выражение продолжается, пока после монома идёт плюс или минус
        oper = *readToken();                       //читаем и запоминаем знак операции (плюс или минус)
        double add = ParseMonome();
        if (oper == '+') {
            res = res + add;
        } else {
            res = res - add;
        }
    }//разбираем идущий далее моном, запоминаем его значение
    //выполняем над res и add операцию oper, запоминаем в res
    return res;                                 //когда выражение закончится, в res будет его значение
}
double ParseAtom();
double ParseMonome() {
    double res;
//    if (text[pos] == '-') {
//        res = ParseAtom();
//        res = -res;
//        return res;
//    }
    res = ParseAtom();
    char op;
    while(*peekToken()=='*' || *peekToken()=='/'){
    op = *readToken();
    double add = ParseAtom();
    if(op=='*')
    {
        res = res*add;
    }else{
        res= res/add;
    }
    }
    return res;
}

double ParseAtom() {
    if (*peekToken() == '(') {                     //вариант 1: это выражение в скобках
        readToken();                              //не забываем прочитать скобку!
    double res = ParseExpr();                 //рекурсивно разбираем выражение
    readToken();                              //не забываем прочитать скобку!
    return res;
} else
    return atof(readToken());                 //вариант 2: это целое число
}
int main()
{
    //freopen("input.txt","r",stdin);
    FILE* f = fopen("output.txt","w");
    gets(text);
//    if(text[0] == '-')
//    {
//        for (int j = strlen(text); j >= 0; j--) {
//            text[j] = text[j - 1];
//        }
//        text[0]='0';
//    }
//    for (int i = 1; i <=strlen(text); i++)
//        if (text[i - 1] == '(' && (text[i] == '-' || text[i] == '+')) {
//            for (int j = strlen(text); j >= i; j--) {
//                text[j] = text[j - 1];
//            }
//            text[i]='0';
//        }
    double res = ParseExpr();
    fprintf(f,"%0.15lf\n",res);
    fclose(f);
    return 0;
}
