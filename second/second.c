#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int result=0;
int isNULL=1;
int pointer=0;
char *global;
int globalLen=0;

int exponent(int a, int b){
    long int x=1;
    for(int i=0;i<b;i++){
        x*=a;
    }
    return x;
}

void convert(){
    int bitString[32];
    int index=0;
    for(int i=result;i>0;i/=2){
        bitString[index]=i%2;
        index++;
    }
    printf("0b");
    for(index=index-1;index>=0;index--){
        printf("%d",bitString[index]);
    }
    printf("\n");
}

int convertStoI(char type[],char val[]){
    if(strcmp(type,"0x")==0){
        //printf("Good@ConvertStoI\n");
        int x=0;
        int e=0;
        for(int i=strlen(val)-1;i>=0;i--){
            int temp=val[i];
            //printf("temp (%c) = %d\n",val[1],temp);
            if(temp<=57)temp=temp-48;
            else if(temp<=70) temp=temp-55;
            else temp=temp-87;
            //printf("new temp = %d\n",temp);
            x+=temp*exponent(16,e);
            e++;
        }
        //printf("%s = %d\n",val,x);
        return x;
    }
    if(strcmp(type,"0h")==0){
        int x=0;
        int e=0;
        for(int i=strlen(val)-1;i>=0;i--){
            int temp=val[i]-48;
            x+=temp*exponent(8,e);
            e++;
        }
        return x;
    }
    if(strcmp(type,"0b")==0){
        int x=0;
        int e=0;
        for(int i=strlen(val)-1;i>=0;i--){
            int temp=val[i]-48;
            x+=temp*exponent(2,e);
            e++;
        }
        return x;
    }
    else {
        int x=0;
        int e=0;
        for(int i=strlen(val)-1;i>=0;i--){
            int temp=val[i]-48;
            x+=temp*exponent(10,e);
            e++;
        }
        return atoi(val);
    }
}

int getNumber(char *str){
    //printf("Good@getNum\n");
    char type[]="00"; //length type
    strncpy(type,&str[pointer],2); //printf("\t\t\tlen type: %s\n",type);
    pointer+=2;
    char lengthStr[]="0000"; // length as string
    strncpy(lengthStr,&str[pointer],4); //printf("\t\t\tlen str: %s\n",lengthStr);
    pointer+=4;
    int length=convertStoI(type,lengthStr); //printf("\t\t\tlen: %d\n",length); // covert length to int
    strncpy(type,&str[pointer],2); //printf("\t\t\tval type: %s\n",type); //value type
    pointer+=2;
    char numStr[length+1];// +1 for terminating null
    numStr[length]='\0';
    strncpy(numStr,&str[pointer],length); //printf("\t\t\tval str: (%s)\n",numStr); // value as string
    pointer+=length;
    int value=convertStoI(type,numStr); //printf("\t\t\tval: %d\n",value);
    return value;
}

int compute(char symbol[],int r,int x){
    int temp=r;
    if (strcmp(symbol,"00")==0) temp+=x;
    else if (strcmp(symbol,"01")==0) temp-=x;
    else if (strcmp(symbol,"10")==0) temp*=x;
    return temp;
}

int paraSeq(){
    //printf("Parenthese Sequence Initiated: %.*s\n",globalLen-pointer,global+pointer);
    int runningResult=0;    
    char command[]="0000";
    strncpy(command,&global[pointer],4);
    pointer+=4;
    while (pointer<globalLen && strcmp(command,"1001")!=0){
        //printf("While Loop Current Command: %s\n",command);
        if(strcmp(command,"0001")==0){
            char type[] = "00";
            strncpy(type,&global[pointer],2);
            pointer+=2;
            result=runningResult;
            //printf("\tRunning Result = %d, Printing Type: %s\n",runningResult,type);
            if(strcmp("0x",type)==0) printf("%s%X\n",type,runningResult);
            else if(strcmp("0h",type)==0) printf("%s%o\n",type,runningResult);
            else if(strcmp("00",type)==0) printf("%s%d\n",type,runningResult);
            else convert();
        }
        else if(strcmp(command,"0010")==0){
            int x = getNumber(global);
            //printf("\tRunning Result = %d + %d\n",runningResult,x);
            runningResult=x;
        }
        else if(strcmp(command,"0100")==0){
            char symbol[]="00";
            strncpy(symbol,&global[pointer],2);
            pointer+=2;
            strncpy(command,&global[pointer],4);
            pointer+=4;
            //printf("\tSymbol: %s, Next Command: %s\n",symbol,command);
            if (strcmp(command,"0010")==0){
                int x=getNumber(global);
                runningResult=compute(symbol,runningResult,x);
                //printf("\t\tRead x = %d, Running Result = %d\n",x,runningResult);
            }
            else if(strcmp(command,"1010")==0){
                int x = paraSeq();
                runningResult = compute(symbol,runningResult,x);
                //printf("\t\tReturned Value x = %d, Running Result = %d\n",x,runningResult);
            }
        }
        else if(strcmp(command,"1010")==0){
            int x = paraSeq();
            runningResult=x;
        }
        
        strncpy(command,&global[pointer],4);
        pointer+=4;
        
    }
    
    return runningResult;
}

int main(int argc, char *argv[argc + 1]){
    global=argv[1];
    globalLen=strlen(argv[1]);
    
    while (pointer<globalLen){
        int x=paraSeq();
        //printf("%d\n",x);
        result=x;
    }
    return EXIT_SUCCESS;
}

/*
while(pointer<strlen(argv[1])-1){
        char str[]="0000";
        strncpy(str,&argv[1][pointer],4);
        pointer+=4;

        if(strcmp(str,"0001")==0){
            char type[] = "00";
            strncpy(type,&argv[1][pointer],2);
            pointer+=2;
            if(strcmp("0x",type)==0) printf("%x\n",result);
            else printf("%d\n",convert(type));
        }

        if(strcmp(str,"0010")==0){
            //printf("Good@Main\n");
            int x = getNumber(argv[1]);
            result=x;
        }

        if(strcmp(str,"0100")==0){
            char symbol[]="00";
            strncpy(symbol,&argv[1][pointer],2);
            pointer+=2;
            char next[]="0000";
            strncpy(next,&argv[1][pointer],4);
            pointer+=4;
            if(strcmp(next,"0100")==0){
                int x= getNumber(argv[1]);
                result = compute(symbol,result,x);
            }
            if(strcmp(next,"1010")==0){
                int x = paraSeq(argv[1]);
                result = compute(symbol,result,x);
            }

        }

        if(strcmp(str,"1010")){
            int x = paraSeq(argv[1]);
            result=x;
        }
        
    }
    return 0;
*/

//0010 00 0001 0x A 0001 00 -> 00100000010xA000100 = 10
//0010 0x 0003 0b 101 0001 00 -> 00100x00030b101000100 = 5
//0010 0h 0011 0b 101111101 0001 00 -> 00100h00110b101111101000100 = 381
//0010 0b 0101 0h 11111 0001 00 -> 00100b01010h11111000100 = 4681
//0010 0b 0101 0h 11111 0001 0x -> 00100b01010h1111100010x = 1249
//0010 0x 000A 0x 00000AaAaA 0001 0x -> 00100x000A0x00000AaAaA00010x = 699050
//0010 00 0001 0x A 0100 00   1010 0010 0b 0011 00 111 0100 10 0010 0b 0010 0h 50 1001   0001 0x
//1010 0010 0h 0010 0h 11111111 1001 0001 0b