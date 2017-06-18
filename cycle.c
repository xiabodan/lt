#include <stdio.h>
#include <stdlib.h>

int func(int** r, int order, int origin, int originVal);

int main() {

    int m = 31;
    int** cycle = (int**)malloc(sizeof(int*) * m);
    for (int i=0;i<m;i++) {
        cycle[i] = (int*)malloc(sizeof(int)*m);
    }

    int originVal = 1;
    for (int j=0;j<=m/2;j++) {
        int order = m-(2*j);
        func(cycle, order, j, originVal);
        originVal += 4*order-4;
    }

    for (int i=0;i<m;i++) {
        for (int j=0;j<m;j++) {
            printf(" %03d", cycle[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int func(int** r, int order, int origin, int originVal) {
    for (int i=0;i<order;i++) {
        r[origin][origin+i] = originVal + i;
    }
    for (int i=0;i<order;i++) {
        r[origin+i][origin+order-1] = r[origin][origin+order-1] + i;
    }
    for (int i=0;i<order;i++) {
        r[order+origin-1][order+origin-1-i] = r[origin+order-1][origin+order-1] + i;
    }
    for (int i=0;i<order-1;i++) {
        r[origin+order-1-i][origin] = r[origin+order-1][origin] + i;
    }
}


