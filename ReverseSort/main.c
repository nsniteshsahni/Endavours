#include <stdio.h>
#define max 100000
int main ()

{

    long int number[max];

    unsigned long int i, j, a, n;
    int t;

    scanf("%d",&t);
    while(t--)
    {
    scanf("%lu", &n);

    for (i = 0; i < n; ++i)

    scanf("%ld", &number[i]);

    /*  sorting begins ... */

    for (i = 0; i < n; ++i)

    {

        for (j = i + 1; j < n; ++j)

        {

            if (number[i] < number[j])

            {

                a = number[i];

                number[i] = number[j];

                number[j] = a;

            }

        }

    }

for (i = 0; i < n; ++i)

    {

        printf("%lu", number[i]);

    }
printf("\n");
}

return 0;
}
