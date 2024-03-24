#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))
#define MAX_LINE 128


#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    char buffer[MAX_LINE];
    *nl = 0, *nw = 0, *nc = 0;
    while (fgets(buffer, MAX_LINE, stdin) != NULL) {
        for (int i = 0; i < MAX_LINE; i++) {
            if (buffer[i] == '\0') break;
            *nc+=1;
            if(((int)(buffer[i])>=FIRST_CHAR && (int)(buffer[i]) <=LAST_CHAR) && (i==0 || (int)(buffer[i-1])<FIRST_CHAR)){
                *nw+=1;
            }
        }
        *nl+=1;
    }
}

void char_count(int char_no, int *n_char, int *cnt) {
    char buffer[MAX_LINE];
    while (fgets(buffer, MAX_LINE, stdin) != NULL) {
        for (int i = 0; i < MAX_LINE; i++) {
            if (buffer[i] == '\0') break;
            else if((int)(buffer[i])>FIRST_CHAR && (int)(buffer[i])<LAST_CHAR){
                count[(int)(buffer[i])]+=1;
            }
        }
    }
    int Z[MAX_CHARS];
    int k = FIRST_CHAR;
    for(int i=0; i<MAX_CHARS; ++i){
        Z[i]=k;
        k++;
    }
    qsort(Z, MAX_CHARS,sizeof(int),cmp);
    *n_char = Z[char_no-1];
    *cnt = count[*n_char];
}

void bigram_count(int bigram_no, int bigram[]) {
    char buffer[MAX_LINE];
    while (fgets(buffer, MAX_LINE, stdin) != NULL) {
        for (int i = 0; i < MAX_LINE; i++) {
            if (buffer[i] == '\0') break;
            else if(i != 0 && (int)(buffer[i])>FIRST_CHAR && (int)(buffer[i])<LAST_CHAR && (int)(buffer[i-1])>FIRST_CHAR &&
                    (int)(buffer[i-1])<LAST_CHAR){
                int znak = 0;
                znak = ((int)(buffer[i-1])-FIRST_CHAR)*MAX_CHARS + ((int)(buffer[i])-FIRST_CHAR);
                count[znak]+=1;
            }
        }
    }
    int Z[MAX_BIGRAMS];
    for(int i=0; i<MAX_BIGRAMS; ++i){
        Z[i]=i;
    }
    qsort(Z, MAX_BIGRAMS,sizeof(int),cmp_di);
    int znak = Z[bigram_no-1];
    bigram[0] = znak/MAX_CHARS+FIRST_CHAR;
    bigram[1] = znak%MAX_CHARS+FIRST_CHAR;
    bigram[2] = count[znak];

}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    char buffer[MAX_LINE];
    int lin = 0, bl = 0,flag = 0,temp=0;
    while (fgets(buffer, MAX_LINE, stdin) != NULL) {
        for (int i = 0; i < MAX_LINE; i++) {
            if (buffer[i] == '\0') break;
            else if(i > 0 && buffer[i] == '/'&& buffer[i-1]=='/' && flag == 0){
                //printf("%d %d %d %c %c ",bl,flag,i,buffer[i-1],buffer[i]);
                lin++;
                break;
            }
            else if(i > 0 && buffer[i] == '*'&& buffer[i-1]=='/' && flag == 0){
                bl++;
                flag = 1;
                temp = i;
            }
            else if(i > 0 && buffer[i] == '/'&& buffer[i-1]=='*' && flag == 1 && temp!=i-1){
                //printf("KO");
                flag = 0;
            }
        }
    }
    *line_comment_counter = lin;
    *block_comment_counter = bl;
    //printf("%d %d\n",bl,lin);
}


int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

