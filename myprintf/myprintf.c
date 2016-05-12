#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>

#define BUFFSIZE 256
#define MIN_VALUE 0.00001
static buf[BUFFSIZE];

char*
reverse(char *str) {
    
    int len = strlen(str);
    int i;
    for (i = 0; i < (len / 2); ++i) {
        char tmp = str[len - i - 1];
        str[len - i - 1] = str[i];
        str[i] = tmp;
    }
    str[len] = '\0';
    return str;
}

void
itoa (int n,char *s)
{
    int i,j,sign;
    if((sign=n)<0)
    n=-n;
    i=0;
    do{
        s[i++]=n%10+'0';
    } while ((n/=10)>0);
   
    if(sign<0)
        s[i++]='-';
    s[i]='\0';
    reverse(s);
}

void
printit(int val) {

    itoa(val, buf);
    write(STDOUT_FILENO, buf, strlen(buf));
    memset(buf, 0, sizeof(buf));
}

void printflt(double val) {
    
    int i_val = (int)val;
    int len, i;
    char tmp[7];
    double left = val - i_val;

    memset(tmp, 0, sizeof(tmp));

    itoa(i_val, buf);
    len = strlen(buf);

    if (fabs(left) < MIN_VALUE) {
        for (i = 0; i < 6; ++i)
            tmp[i] = '0';
    
    } else {
        itoa((int)(left * 1000), tmp);    
    }
    strcat(buf, ".");
    strcat(buf, tmp); 
    write(STDOUT_FILENO, buf, strlen(buf));
    memset(buf, 0, sizeof(buf));

}

void
printchar(char val) {
    
    write(STDOUT_FILENO, &val, 1);
    memset(buf, 0, sizeof(buf));
}

void
printstr(char *val) {

    strcpy(buf, val);
    write(STDOUT_FILENO, buf, strlen(buf));
    memset(buf, 0, sizeof(buf));
}

static void
myprintf(char *str, ...) {

    int len = strlen(str);
    va_list p_arg;
    char valch = 0;
    char t = 0;
    int valint = 0;
    double valflt = 0;
    char *valstr = NULL;

    va_start(p_arg, str); 
    
    do {
        if ((valch = *str++) != '%') {
            printchar(valch);
        } else {
            
           switch (t = *str++) {
               case 'd':
                    valint = va_arg(p_arg, int);
                    printit(valint);
                    break;

               case 'f':
                    valflt = va_arg(p_arg, double);
                    printflt(valflt);
                    break;

               case 's':
                    valstr = va_arg(p_arg, char*);
                    printstr(valstr);
                    break;
               default:
                    printchar('%');
                    printchar(t);
                    
                    break;
            }
        }
    } while (*str != '\0');

    va_end(p_arg);
}


int
main(int argc, char *argv[]) {

    memset(buf, 0 , sizeof(buf));
    int i = 12345;
    double f = 1234567.2700;
    char *str = "hello world!";
    myprintf("@_@%d-%%-%f--%s", i, f, str);
}
