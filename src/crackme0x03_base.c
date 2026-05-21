//
// Created by Maryam Ashraf on 11/24/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* WARNING: Unknown calling convention */

int shift(char *s)

{
    size_t sVar1;
    int iVar2;
    char v3 [120];
    size_t i;

    i = 0;
    while( 1 ) {
        sVar1 = strlen(s);
        if (sVar1 <= i) break;
        v3[i] = s[i] + -3;
        i = i + 1;
    }
    v3[i] = '\0';
    iVar2 = printf("%s\n",v3);
    return iVar2;
}


/* WARNING: Unknown calling convention */

int test(int a1,int a2)

{
    int result;

    if (a1 == a2) {
        result = shift("Sdvvzrug#RN$$$#=,");
    }
    else {
        result = shift("Lqydolg#Sdvvzrug$");
    }
    return result;
}

/* WARNING: Unknown calling convention */

int main(int argc,char **argv)

{
    int passwd;

    puts("IOLI Crackme Level 0x03");
    printf("Password: ");
    scanf("%d",&passwd);
    test(passwd,0x52b23);
    return 0;
}


