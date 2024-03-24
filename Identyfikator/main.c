#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int ban_word(char T[], int n,int zakres){
    for(int i=0; i<zakres; ++i){
        int len = strlen(keywords[i]);
        int j = 0, licz = 0;
        while(j < len && j < n){
            if(T[j] == keywords[i][j])licz++;
            j++;
        }
        if(licz == len && licz == n)return 0;
    }
    return 1;
}
int double_word(char T[], int n,int zakres){
    /*if(1==1){
        printf("\nD\n");
        for(int i=0; i<zakres; ++i){
            int len = strlen(identifiers[i]);
            for(int j=0; j<len; ++j){
                printf("%c %c\n",identifiers[i][j], T[j]);
            }
            printf("\n\n");
        }
    }*/
    /*printf("\nT: ");
    for(int i =0; i<n; ++i){
        printf("%c",T[i]);
    }*/
    //printf("\n");
    for(int i=0; i< zakres; ++i){
        int len = strlen(identifiers[i]);
        int j = 0, licz = 0;
        while(j < len && j < n){
            if(zakres ==2 && i ==3){
                //printf("%c %c\n",T[j],identifiers[i][j]);
            }
            if(T[j] == identifiers[i][j])licz++;
            j++;
        }
        if(licz == n && licz == len)return 0;
    }
    return 1;
}

int check(char x,int type){
    char T[53] = {'_','a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    for(int i=0; i<53; ++i){
        if(T[i]==x)return 1;
    }
    if(type==0)return 0;
    char A[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for(int i=0; i<10; ++i){
        if(A[i]==x)return 1;
    }
    return 0;
}

int find_idents() {
    char buffer[128];
    int l = 0,res = 0, flag = 0,temp = 0,flag2 = 1;
    while (fgets(buffer, 128, stdin) != NULL) {
        for (int i = 0; i < 128; i++) {
            if (buffer[i] == '\0' || (flag == 0 && i > 0 && buffer[i] =='/' && buffer[i-1]=='/')) break;
            if (i > 0 && buffer[i] =='*' && buffer[i-1]=='/'){
                temp = i;
                flag = 1;
            }
            if (i > 0 && buffer[i] =='/' && buffer[i-1]=='*' && temp != i-1){
                flag = 0;
            }
            if((int)buffer[i]==39 || (int)buffer[i]==34){
                if(i > 0 && buffer[i-1]==92)flag2*=-1;
                flag2*=-1;
            }
            //printf("\n%d\n",flag2);
            if((i == 0 || (check(buffer[i],0) == 1 && check(buffer[i-1],1)==0)) && flag == 0 && flag2 == 1){
                //printf("%c ",buffer[i]);
                int k = 0;
                char W[MAX_ID_LEN]={'#'};
                while(check(buffer[i+k],1)==1){
                    //printf("%c ",buffer[i+k]);
                    W[k] = buffer[i+k];
                    k++;
                }
                /*for(int i=0; i<k; ++i){
                        printf("%c",W[i]);
                    }
                printf("\n");*/
                if(k!=0 && (buffer[i+k] == '\0' || buffer[i+k]==' ' || check(buffer[i+k],1)==0)){
                    /*for(int i=0; i<k; ++i){
                        printf("%c",W[i]);
                    }
                    printf(" %d %d \n",ban_word(W,k,32),double_word(W,k,l));*/
                    if(ban_word(W,k,32)==1 && double_word(W,k,l)==1){
                        //printf("\n %d %d\n",ban_word(W,k,32),double_word(identifiers,k,l));
                        for(int i=0; i<k; ++i){
                            //printf("%c",W[i]);
                            identifiers[l][i] = W[i];
                        }
                        l+=1;
                        //printf("%d\n",l);
                    }
                    //printf("\n");
                }
                i+=k;

            }
        }
    }
    //if(l==12)l-=1;
    return l;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}
