#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

void print(mpz_t* x, int d)
{
	char* number = NULL;
	number = mpz_get_str(number, 10, *x);
	int n = strlen(number);

	char* zerosPtr = &(number[n-1]);

	int zerosCount = 0;


	//usunięcie niepotrzebnych zer z tyłu
	//usuwamy maksymalnie d zer
	//nie usuwamy pierwszej cyfry
	//po tej operacji zostaje n-zerosCount liczb
	while ((*zerosPtr == '0') && (zerosPtr != number) && (zerosCount < d))
	{
		--zerosPtr;
		++zerosCount;
	}
	//w tym miejscu kończymy napis
	*(zerosPtr+1) = 0;


	//jeżeli liczba nie posiada części ułamkowej
	if ((d == zerosCount) || (number[0] == '0' && (strlen(number) == 1)))
	{
		printf("%s\n", number);
	}
	//jeżeli liczba nie posiada części całkowitej
	else if (d >= n)
	{
		char* formatStr = (char*)malloc((2+d-n+zerosCount+3+1)*sizeof(char));
		strcpy(formatStr, "0.");

		memset(formatStr+2, '0', d-n+zerosCount);
		*(formatStr+2+d-n+zerosCount) = 0;
		strcat(formatStr, "%s\n");

		printf(formatStr, number);
	}
	//jeżeli liczba posiada część całkowitą i ułamkową
	else
	{
		//wydzielenie części całkowitej
		char temp = number[n-d];
		number[n-d] = 0;
		//wypisanie części całkowitej
		printf("%s.", number);

		//wypisanie części ułamkowej
		number[n-d] = temp;
		printf("%s\n", number+n-d);
	}
	
}

mpz_t numbersRead[17000000];

int main(int argc, char* argv[])
{

	int d = 0;
	if (argc == 2)
		d = atoi(argv[1]);

	mpz_t x;
	mpz_init(x);
	mpz_t avg;
	mpz_init(avg);
	mpz_t var;
	mpz_init(var);
	mpz_t precision;
	mpz_init(precision);

	mpz_ui_pow_ui(precision, 10, d);


	//liczba elementów na wejściu
	int n = 0;

	while (mpz_inp_str(numbersRead[n], stdin, 10) != 0)
	{
		mpz_add(avg, numbersRead[n], avg);
		mpz_addmul(var, numbersRead[n], numbersRead[n]);
		++n;
	}

	int periodLength = 1;

	int i, j;

	for (i = 0; i < periodLength; ++i)
	{
		for (j = periodLength; j < n; ++j)
		{
			if (mpz_cmp(numbersRead[j % periodLength], numbersRead[j]))
			{
				++periodLength;
				break;
			}
		}
	}

	mpz_t avgSqr;
	mpz_init(avgSqr);
	mpz_mul(avgSqr, avg, avg);

	mpz_mul_ui(var, var, n);
	mpz_sub(var, var, avgSqr);
	mpz_mul(var, var, precision);

	mpz_t nSqr;
	mpz_init(nSqr);
	mpz_set_ui(nSqr, n);
	mpz_mul_ui(nSqr, nSqr, n);

	mpz_div(var, var, nSqr);

	mpz_mul(avg, avg, precision);
	mpz_div_ui(avg, avg, n);

	print(&avg, d);
	print(&var, d);
	printf("%d\n", periodLength);

	return 0;
}
