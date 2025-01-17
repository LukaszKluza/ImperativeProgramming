#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair* relation, int size){
    int flag = 0;
    for(int i=0; i<size; ++i){
        int temp = relation[i].first;
        for(int j=0; j<size; ++j){
            if(relation[j].first == temp && relation[j].second == temp)flag =1;
        }
        if(flag == 0)return 0;
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair* relation, int size){
    int flag = 0;
    for(int i=0; i<size; ++i){
        int temp = relation[i].first;
        for(int j=0; j<size; ++j){
            if(relation[j].first == temp && relation[j].second == temp)flag =1;
        }
        if(flag == 1)return 0;
    }
    return 1;

}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair* relation, int size){
    int flag = 0;
    for(int i=0; i<size; ++i){
        int x = relation[i].first;
        int y = relation[i].second;
        for(int j=0; j<size; ++j){
            if(relation[j].first == y && relation[j].second == x && x!= y)flag =1;
            break;
        }
        if(flag == 0)return 0;
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair* relation, int size){
    int flag = 0;
    for(int i=0; i<size; ++i){
        int x = relation[i].first;
        int y = relation[i].second;
        for(int j=0; j<size; ++j){
            if(relation[j].first == y && relation[j].second == x && x!=y)flag =1;
            break;
        }
        if(flag == 1)return 0;
    }
    return 1;

}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair* relation, int size){
    int flag = 0;
    for(int i=0; i<size; ++i){
        int x = relation[i].first;
        int y = relation[i].second;
        for(int j=0; j<size; ++j){
            if(relation[j].first == y && relation[j].second ==x)flag =1;
            break;
        }
        if(flag == 1)return 0;
    }
    return 1;

}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair* relation, int size){
    int flag = 1;
    for(int i=0; i<size; ++i){
        int x = relation[i].first;
        int y = relation[i].second;
        for(int j=0; j<size; ++j){
            if(relation[j].first == y) {
                flag = 0;
                int z = relation[j].second;
                for (int k = 0; k < size; ++k) {
                    if(relation[k].first == x && relation[k].second ==z){
                        flag = 1;
                        break;
                    }
                }
            }
        }
        if(flag == 0)return 0;
    }
    return 1;

}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair* relation, int size){
    if(is_reflexive(relation,size)==0 || is_antisymmetric(relation,size)==0 || is_transitive(relation,size) ==0){
        return 0;
    }
    return 1;

}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair* relation, int size){
    int flag = 0;
    for(int i=0; i<size; ++i){
        int x = relation[i].first;
        int y = relation[i].second;
        for(int j=0; j<size; ++j){
            if(relation[j].first == y && relation[j].second == x && x!= y)flag =1;
            break;
        }
        if(flag == 0)return 0;
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair* relation, int size){
    if(is_partial_order(relation,size)==0 || is_connected(relation,size) == 0){
        return 0;
    }
    return 1;
}

int find_max_elements(const pair* relation, int size, int* max_elements){
    int g,il=0,flag;
    for(int i=0; i<size; ++i){
        g = relation[i].second;
        flag = 1;
        for(int j=0; j<size; ++j){
            if(relation[j].first == g && relation[j].first != relation[j].second){
                flag = 0;
            }
        }
        if(flag == 1){
            int flag2=1;
            for(int k=0; k<il; ++k){
                if(max_elements[k]==g){
                    flag2=0;
                }
            }
            if(flag2 ==1){
                max_elements[il]=g;
                il++;
            }
        }
    }
    return il;
}
int find_min_elements(const pair* relation, int size, int* min_elements){
    int g,il=0,flag;
    for(int i=0; i<size; ++i){
        g = relation[i].first;
        flag = 1;
        for(int j=0; j<size; ++j){
            if(relation[j].second == g && relation[j].first != relation[j].second){
                flag = 0;
            }
        }
        if(flag == 1){
            int flag2=1;
            for(int k=0; k<il; ++k){
                if(min_elements[k]==g){
                    flag2=0;
                }
            }
            if(flag2 ==1){
                min_elements[il]=g;
                il++;
            }
        }
    }
    return il;

}
int get_domain(const pair* relation, int size , int* domain){
    int x,y,il=0,flag;
    for(int i=0; i<size; ++i){
        x = relation[i].first;
        y = relation[i].second;

        flag = 1;
        for(int j=0; j<il; ++j){
            if(domain[j]==x){
                flag = 0;
            }
        }
        if(flag == 1 ){
            domain[il]=x;
            il++;
        }
        flag = 1;
        for(int j=0; j<il; ++j){
            if(domain[j]==y){
                flag = 0;
            }
        }
        if(flag == 1 ){
            domain[il]=y;
            il++;
        }
    }
    return il;
}

// Case 3:

int composition (const pair* relation, int size1, const pair* relation2, int size2, pair* kot){
    int res = 0,x,y;
    for(int i=0; i<size1; ++i){
        y = relation[i].second;
        for(int j=0; j<size2; ++j){
            if(relation2[j].first == y){
                res+=1;
                break;
            }
        }
    }
    return res;
}

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
    int _a = *(int*)a;
    int _b = *(int*)b;
    if(_a < _b) return -1;
    else if(_a == _b) return 0;
    else return 1;
}



// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    scanf("%d",&n);
    for(int i=0; i<n; ++i) {
        scanf("%d%d", &relation[i].first, &relation[i].second);
    }
    return n;
}

void print_int_array(const int *array, int n) {
    printf("%d\n",n);
    qsort(array, n, sizeof(int), cmp_pair);
    for(int i=0; i<n; ++i){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

