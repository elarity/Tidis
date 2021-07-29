#include <stdio.h>
#include <string.h>
#include <limits.h>
int string2int(char *);

int main( int argc, char * argv[] ) {

    char msg[] = "*2\r\n$3get\r\n$5user1\r\n";
    //msg = "kakfk";

    printf("ret=%d\r\n", string2int(msg));
}


int string2int(char * string)
{
    int ret = 0;
    int neg = 0;
    // 处理负数.
    if ('-' == *string) {
        neg = 1;
        ++string;
    }
    while (*string >= '0' && *string <= '9') {
        ret = (ret * 10) + (*string - '0');
        if (ret > INT_MAX) {
            return -1;
        }
        ++string;
    }
    if (1 == neg) {
        ret = -ret;
    }
    return ret;
}