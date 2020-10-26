#include <stdio.h>

int main()
{
    const int row = 3, col = 5;
    char mat[row][col];

    for (int  i = 0; i < row; i++) {
        printf("row %d: ", i);
        for (int j = 0; j < col; j++) {
            printf("%p ", mat[i] + j);
        }
        printf("\n");
    }
    return 0;
}
