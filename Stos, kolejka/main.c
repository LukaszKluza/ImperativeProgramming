#include <stdio.h>
#include <stdlib.h>

int n = 52,  seed, TYPE, games;
int G0[52], G1[52],Len[2]={26,26}, Out[2];

void cbuff_push(int val,int cbuff[],int k){
    int id = (Out[k]+Len[k])%n;
    Len[k] += 1;
    cbuff[id] = val;
}

int cbuff_pop(int cbuff[], int k){
    Len[k]--;
    int res = cbuff[Out[k]];
    cbuff[Out[k]] = -1;
    Out[k] = (Out[k]+1)%52;
    return res;
}

void cbuff_print(int cbuff[],int k){
    for(int i=Out[k]; i<52; ++i) printf("%d ",cbuff[i]);
    for(int i=0; i<Out[k]; ++i) printf("%d ",cbuff[i]);
}

void swap (int array[], int i, int k){
    int temp = array[i];
    array[i] = array[k];
    array[k] = temp;
}

void rand_permutation(int array[]){
    for(int i=0; i<n; ++i)array[i]=i;
    int k;
    for(int i=0; i<n-1; ++i){
        k = rand()%(n-i)+i, swap(array,i,k);
    }
}

int GAME(int type){int a,b, il_kon;
    while (games > 0 && Len[0] > 0 && Len[1] > 0){
        a = Out[0], b = Out[0], games--, il_kon=1;
        while(((G0[a%n])/4 == (G1[b%n])/4) && type == 0 && games > 0){
            games-=2, a+=2, b+=2;
            il_kon+=2;
            if(il_kon > Len[0] || il_kon > Len[1]) return 1;
        }
        if(((G0[a%n])/4 == (G1[b%n])/4) && type == 1){
            int k0 = cbuff_pop(G0,0);
            int k1 = cbuff_pop(G1,1);
            cbuff_push(k0,G0,0);
            cbuff_push(k1,G1,1);}
        else{
            int win = ((G0[a%n])/4 < (G1[b%n])/4);
            if(win == 0){
                for(int i=0; i<il_kon; ++i){
                    int k0 = cbuff_pop(G0,0);
                    cbuff_push(k0,G0,0);
                }
                for(int i=0; i<il_kon; ++i){
                    int k1 = cbuff_pop(G1,1);
                    cbuff_push(k1,G0,0);
                }
            }
            else{
                for(int i=0; i<il_kon; ++i){
                    int k1 = cbuff_pop(G1,1);
                    cbuff_push(k1,G1,1);
                }
                for(int i=0; i<il_kon; ++i){
                    int k0 = cbuff_pop(G0,0);
                    cbuff_push(k0,G1,1);
                }
            }
        }
    }
    if(games == 0) return 0;
    if(il_kon > Len[0]) return 3;
    return 2;
}

int main() {
    scanf("%d %d" "%d", &seed, &TYPE,&games);
    int K[n], all_games = games;
    srand((unsigned) seed);
    for(int i=0; i<n; ++i){
        K[i] = i,G0[i] = -1, G1[i] = -1;
    }
    rand_permutation(K);
    for(int i=0; i<n/2; ++i){
        G0[i] = K[i], G1[i] = K[i+n/2];
    }
    int res = GAME(TYPE);
    if(res == 0 || res == 1){
        printf(" %d %d %d ",res, Len[0],Len[1]);
    }
    else if(res == 2){
        printf("%d %d",res, all_games-games-(TYPE==0));
    }
    else{
        printf("%d\n",res);
        cbuff_print(G1,1);
    }
    return 0;}
