#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_input(char * a, char * b, char * c, size_t * la, size_t * lb, size_t * lc)
{
    int undefined = -1;
    int var_num = 0;
    int t;
    char chr;
    while((t = fgetc(stdin)) != EOF)
    {
        chr = (char)t;
        if(chr == '\n')
            break;

        if(chr == ' ')
        {
            switch (var_num)
            {
                case 0:
                    a[*la] = '\0';
                    break;
                case 1:
                    b[*lb] = '\0';
                    break;
                case 2:
                    c[*lc] = '\0';
                    break;
            }
            var_num++;
        }
        
        else if(chr == '+' || chr == '=')
            var_num--;
        
        else
        {
            if(chr == '#')
                undefined = var_num;

            switch (var_num){
                case 0:
                    a[*la] = chr;
                    (*la)++;
                    break;

                case 1:
                    b[*lb] = chr;
                    (*lb)++;
                    break;

                case 2:
                    c[*lc] = chr;
                    (*lc)++;
                    break;
            }
        }
    }

    return undefined;
}

void check_overlap(char * s, size_t * l, char * res)
{
    size_t lr = (int)strlen(res);
    size_t len = lr - *l + 1;
    char * result = calloc(len + 1, sizeof(*result));

    if(s[0] == '#')
    { 
        for(size_t i = 0; i <= lr - len - 1; i++)
        {
            if(res[lr - 1 - i] != s[*l - 1 - i])
            {
                printf("%d\n", -1);
                exit(EXIT_SUCCESS);
            }
        }

        //for(size_t i = 0; i <= len - 1; i++)
        //    result[i] = res[i];

        //result[len] = '\0';
    }
    else if(s[*l - 1] == '#')
    {
        for(size_t i = 0; i <= lr - len - 1; i++)
        {
            if(s[i] != res[i])
            {
                printf("%d\n", -1);
                exit(EXIT_SUCCESS);
            }
        }

        //for(size_t i = lr - len; i <= lr - 1; i++)
        //    result[i + len - lr] = res[i];

        //result[len] = '\0'; 
    }
    else
    {
        size_t n_place;
        
        for(size_t i = 0; i < *l; i++)
            if(s[i] == '#')
                n_place = i;

        char temp1 [n_place + 2];;
        temp1[n_place + 1] = '\0';
        size_t lt1 = n_place + 1;

        char temp2 [*l - n_place + 1];
        temp2[*l - n_place] = '\0';
        size_t lt2 = *l - n_place;

        for(size_t i = 0; i <= n_place; i++)
            temp1[i] = s[i];

        for(size_t i = 0; i < *l - n_place; i++)
            temp2[i] = s[i + n_place];

        check_overlap(temp1, &lt1, res);
        check_overlap(temp2, &lt2, res);
    }
}

int main(int argc, char * argv[])
{
    char * a, * b, * c;
    size_t la = 0, lb = 0, lc = 0;
    a = calloc(11, sizeof(*a));
    b = calloc(11, sizeof(*b));
    c = calloc(11, sizeof(*c));
    
    int undefined = -1;
    undefined = handle_input(a, b, c, &la, &lb, &lc);

    if(undefined == -1)
    {
        free(a);
        free(b);
        free(c);
        fprintf(stderr, "%s \n", "all variables are known!");
        return EXIT_SUCCESS;
    }
    
    char res[11];
    int res_int;

    switch (undefined)
    {
        case 0:
            ;
            res_int = atoi(c) - atoi(b);
            sprintf(res, "%d", res_int);
            if(strcmp(a, "#") == 0)
            {
                printf("%s + %s = %s\n", res, b, c);
                return EXIT_SUCCESS;
            }
            check_overlap(a, &la, res);
            printf("%s + %s = %s\n", res, b, c);
            break;
        
        case 1:
            ;
            res_int = atoi(c) - atoi(a);
            sprintf(res, "%d", res_int);
            if(strcmp(b, "#") == 0)
            {
                printf("%s + %s = %s\n", a, res, c);
                return EXIT_SUCCESS;
            }
            check_overlap(b, &lb, res);
            printf("%s + %s = %s\n", a, res, c);
            break;

        case 2:
            ;
            res_int = atoi(a) + atoi(b);
            sprintf(res, "%d", res_int);
            if(strcmp(c, "#") == 0)
            {
                printf("%s + %s = %s\n", a, b, res);
                return EXIT_SUCCESS;
            }
            check_overlap(c, &lc, res);
            printf("%s + %s = %s\n", a, b, res);
            break;
    }
    
    free(a);
    free(b);
    free(c);

    return EXIT_SUCCESS;
}
