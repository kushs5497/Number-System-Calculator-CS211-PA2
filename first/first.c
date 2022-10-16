#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int result;
int reusltIsNULL=1;
int pointer=0;

int exponent(int len){
    int x=1;
    for(int i=0;i<len-1;i++){
        x*=2;
    }
    return x;
}

int getNum(char str[],int len){
    int value=0;
    int power = exponent(len);
    for(int i=0;i<len;i++){
        if(str[i]=='1'){
            value+=power;
        }
        power/=2;
    }
    return value;
}

int Number(char str[]){
    char strNumLen[]="0000";
    strncpy(strNumLen,&str[pointer],4);
    pointer+=4;
    int numLen=getNum(strNumLen,strlen(strNumLen));
    //printf("%s  %d\n",strNumLen,numLen);
    char strNum[numLen];
    strncpy(strNum,&str[pointer],numLen);
    pointer+=numLen;
    int num = getNum(strNum,numLen);
    return num;
}


int main(int argc, char * argv[argc+1]){
    //int pointer=0;
    //int result;
    if (argc<2 || argv[1]==NULL) return 0;
    while (pointer<strlen(argv[1])){
        char subStr[]="0000";
        strncpy(subStr,&argv[1][pointer],4);
        //printf("%s\n",subStr);
        pointer+=4;
        if(strcmp(subStr,"0001")==0){
            if(reusltIsNULL) continue;
            printf("%d\n",result);
            continue;
        }
        if(strcmp(subStr,"0010")==0){
            result=Number(argv[1]);
            reusltIsNULL=0;
        }
        if(strcmp(subStr,"0100")==0){
            if(reusltIsNULL) continue;
            char symbol[]="00";
            strncpy(symbol,&argv[1][pointer],2);
            pointer+=2;
            pointer+=4;
            int num = Number(argv[1]);
            if (strcmp(symbol,"00")==0) result+=num;
            else if (strcmp(symbol,"01")==0) result-=num;
            else result*=num;
            if(result<0){
                reusltIsNULL=1;
                printf("NaN\n");
            }
            continue;
        }
        if(strcmp(subStr,"1000")==0){
            reusltIsNULL=1;
            printf("NaN\n");
            continue;
        }
    }
    return 0;
}
