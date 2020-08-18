#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include "1_header.h"

/*struct node
{
int degree;
float coeff;
struct node* prev;
struct node* next;
};*/

int main()
{
char choice;
struct node *new_pol = NULL, *new_pol1, *new_pol2;
initscr();
raw();
struct ASL* ASL = get_memory_space((int)pow(2, 28));
printw("\nEnter the polynomial as pairs of degree and coefficients, ordered in decreasing value of degree: \n");
new_pol = get_polynomial(ASL, new_pol);

printw("\nFollowing polynomial entered (Degree, Coefficient):\n");

put_polynomial(new_pol);

again2:

new_pol1 = NULL; new_pol2 = NULL;
printw("\nEnter Dividend Polynomial :");
new_pol1 = get_polynomial(ASL, new_pol1);

printw("\nEnter Divisor Polynomial :");
new_pol2 = get_polynomial(ASL, new_pol2);

divide_polynomial(ASL, new_pol1, new_pol2);

again3:

printw("\n\n Press 'D' to divide more, or 'E' to Exit !!!");

choice = getch();

	if(choice == 'D' || choice == 'd')
	{
	clear();
	goto again2;
	}

	else
	{
		if(choice != 'E' && choice != 'e')
		{
		printw("\n\nInvalid choice !");
		goto again3;
		}
	}
//getch();
endwin();
return 0;
}
