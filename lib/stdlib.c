#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int atoi(const char *s)
{


	int res = 0;
	char sign = ' ';

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while ('0' <= *s && *s <= '9') {
		res = res * 10 + (*s - '0');
		s++;

	}

	if (sign == '-') {
		res = res * -1;
	}

		return res;
	

	
}

char* itoa(int value, char* str) {
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (value < 0) {
        isNegative = 1;
        value = -value;
    }

    // Process individual digits
    do {
        str[i++] = value % 10 + '0';
        value /= 10;
    } while (value != 0);

    // Add negative sign if needed
    if (isNegative) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}

// check if char a is integer
// returns 1 if true, 0 if false
int isInt(char c)
{



    if( c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//checks if all chars are valid digits
int isNum(char* num)
{
    for(int i = 0; i < (int)strlen(num); i++)
    {
        if(! isInt(num[i]))
        {
            return 0;
        }
    }
    return 1;
}

