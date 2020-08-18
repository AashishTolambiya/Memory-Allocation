#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include "1_header.h"

void initialize(char num[])
{
int i, length = strlen(num);
	for(i=0; i <length; i++)
	{
	num[i] = ' ';
	}
}

struct node* get_polynomial(struct ASL* ASL, struct node* pol)
{
struct node *new_node, *next_node, *old;
int degree, last_degree, y, x, i, len=0, j=0, br_open, comma_sep;
float coeff;
char ch[100], num[100];
again:

br_open=0; comma_sep=0;
getyx(stdscr, y, x);
last_degree = INT_MAX;
x=15;
i=0;
scanw("%s",ch);
len = strlen(ch);
	for(i=0; i<len; i++)
	{
		if(ch[i] == '(')
		{
		br_open = 1;
		continue;
		}
		
		if(ch[i] == ')' && br_open && comma_sep)
		{
//		new_node = (struct node*)malloc(sizeof(struct node));
		new_node = allocate_memory(ASL, sizeof(struct node));
		new_node->degree = degree;
		new_node->coeff  = atof(num);
		new_node->next = NULL;

			if(degree >= last_degree || new_node->coeff == 0)
			{
				if(pol != NULL) 
				{
					while(pol->next != NULL)
					{
					next_node = pol->next;
					pol->next = pol->next->next;
					free(next_node);
					}
					
				pol = NULL;
				}
			
			clear();
			printw("Polynomial not in required format, please enter again!\n");
			goto again;
			}

			if(pol == NULL)
			{
			new_node->prev = NULL;
			pol = new_node;
			}
		
			else
			{
			old = pol;
				while(old->next != NULL)
				{
				old = old->next;
				}
			old->next = new_node;
			new_node->prev = old;
			}
		
		last_degree = new_node->degree;
		br_open = 0;
		j=0;
		comma_sep=0;
		initialize(num);
		continue;
		}
		
		if(ch[i] == ',' && br_open)
		{
		degree = atoi(num);
		j=0;
		comma_sep=1;
		initialize(num);
		continue;
		}
		
		if(br_open)
		{
		num[j] = ch[i];
		j++;
		}		
	}

return pol;
}

void put_polynomial(struct node* pol)
{
	if(pol != NULL)
	{
	printw("(%d, %f)", pol->degree, pol->coeff);
	put_polynomial(pol->next);
	}
}

void divide_polynomial(struct ASL* ASL, struct node* dividend, struct node* divisor)
{
struct node *quotient = NULL, *remainder = NULL, *subtractor = NULL, *new_node = NULL, *old = NULL, *dividend_first = NULL, *divisor_first = NULL, *new_node_1 = NULL, *dividend_prev_node = NULL;
int flag=0;

dividend_first = dividend; divisor_first = divisor;
	while(dividend->degree >= divisor->degree)
	{
//	new_node = (struct node*)malloc(sizeof(struct node));
	new_node = allocate_memory(ASL, sizeof(struct node));
	new_node->degree = dividend->degree - divisor->degree;
	new_node->coeff = dividend->coeff / divisor->coeff;
	new_node->next = NULL;
		if(quotient == NULL)
		{
		new_node->prev = NULL;
		quotient = new_node;
		}
		
		else
		{
		old = quotient;
			while(old->next != NULL)
			{
			old = old->next;
			}
		old->next = new_node;
		new_node->prev = old;
		}
				
		while(divisor != NULL)
		{
//		new_node_1 = (struct node*)malloc(sizeof(struct node));
		new_node_1 = allocate_memory(ASL, sizeof(struct node));
		new_node_1->degree = divisor->degree + new_node->degree;
		new_node_1->coeff  = divisor->coeff  * new_node->coeff * (-1);
		new_node_1->next   = NULL;
			if(subtractor == NULL)
			{
			new_node_1->prev = NULL;
			subtractor = new_node_1;
			}
			
			else
			{
			old = subtractor;
				while(old->next != NULL)
				{
				old = old->next;
				}
			old->next = new_node_1;
			new_node_1->prev = old;
			}
		
		divisor = divisor->next;			
		}
		
		while(dividend != NULL && subtractor != NULL)
		{
			if(dividend->degree == subtractor->degree)
			{
			dividend->coeff = dividend->coeff + subtractor->coeff;
			dividend_prev_node = dividend;
			dividend   = dividend->next;
			subtractor = subtractor->next;
			}
			else
			{
				if(dividend->degree > subtractor->degree)
				{
				dividend_prev_node = dividend;
				dividend = dividend->next;
				}
				else
				{
//				new_node_1           = (struct node*)malloc(sizeof(struct node));
				new_node_1           = allocate_memory(ASL, sizeof(struct node));
				new_node_1->degree   = subtractor->degree;
				new_node_1->coeff    = subtractor->coeff;
				dividend->prev->next = new_node_1;
				new_node_1->prev     = dividend->prev;
				dividend->prev       = new_node_1;
				new_node_1->next     = dividend;
				subtractor           = subtractor->next;
				}
			}
		}
		
		if(dividend == NULL)
		{	
		dividend = dividend_prev_node;
			while(subtractor != NULL)
			{
//			new_node_1           = (struct node*)malloc(sizeof(struct node));
			new_node_1           = allocate_memory(ASL, sizeof(struct node));
			new_node_1->degree   = subtractor->degree;
			new_node_1->coeff    = subtractor->coeff;
			dividend->next       = new_node_1;
			new_node_1->prev     = dividend;
			new_node_1->next     = NULL;
			dividend             = dividend->next;
			subtractor           = subtractor->next;
			}		
		}
		
		dividend = dividend_first;
		
			while(dividend != NULL)
			{
				if(dividend->coeff == 0)
				{
					if(dividend == dividend_first)
					{
						if(dividend->next != NULL)
						{
						dividend = dividend->next;
						dividend->prev->next = NULL;
						free(dividend->prev);
						dividend->prev = NULL;
						dividend_first = dividend;
						}
						else
						{
						dividend->degree = 0;
						dividend = dividend->next;
						}
					}
					else
					{
						if(dividend->next != NULL)
						{			
						dividend->prev->next = dividend->next;
						dividend->next->prev = dividend->prev;
						old = dividend;
						dividend = dividend->next;
						old->next = NULL;
						old->prev = NULL;
						free(old);
						}
						else
						{
						dividend->prev->next = NULL;
						dividend->prev = NULL;
						free(dividend);
						}
					}
				}
				else
				{
				dividend = dividend->next;
				}
			}
		
		dividend = dividend_first;
		divisor = divisor_first;
			if(divisor->degree == 0 && dividend->degree == 0)
			{
				if(!flag)
				{
				flag++;
				}
				else
				{
				break;
				}
			}
	}

remainder = dividend;

printw("\nQuotient polynomial (Degree, Coefficient):\n");

put_polynomial(quotient);

printw("\nRemainder polynomial (Degree, Coefficient):\n");

put_polynomial(remainder);

}







struct ASL* get_memory_space(int size)
{
int i, j = 1, k = 0;
metadata = (char*) calloc(size, sizeof(char));
	if(metadata == NULL)
	{
	printf("Memory not allocated!");
	exit(0);
	}
metadata_size = size;
prnt_list = (struct parent_type_list*)malloc(sizeof(struct parent_type_list));
prnt_list->head = NULL;
//printf("\nRequest Size : %d\t Size of Metadata : %ld", size, sizeof(metadata));
struct ASL* ASL = (struct ASL*) malloc(sizeof(struct ASL));
ASL->LIST = (struct ASL_list*) malloc(sizeof(struct ASL_list));
ASL->LIST->NODE = NULL;
ASL->LIST->KVAL = 0;
ASL->LIST->C = 1;
ASL->LIST->NEXT = NULL;
ASL->LIST->PREV = NULL;
struct ASL_list* old_node = ASL->LIST;

	for(i = 1; i< (2 * (int)(log(size)/log(2))); i++)
	{
	struct ASL_list* new_node = (struct ASL_list*) malloc(sizeof(struct ASL_list));
	old_node->NEXT = new_node;
	new_node->NEXT = NULL;
	new_node->PREV = old_node;
	new_node->NODE = NULL;
		if(i%2 == 0)
		{
		new_node->KVAL = k;
		k++;
		new_node->C = 3;
		}
		else
		{
		new_node->KVAL = j;
		j++;
		new_node->C = 1;
		}
	old_node = new_node;
	}
	
old_node->NODE = (struct ASL_node*) malloc(sizeof(struct ASL_node));

old_node->NODE->BA = malloc(size);
base_addr = old_node->NODE->BA;
printf("\nBase Address of allocated area : %p\n", base_addr);
last_addr = (void *)(base_addr + size - 1);
total_size = size;
old_node->NODE->NEXT = NULL;
old_node->NODE->PREV = NULL;
set_metadata(old_node->NODE->BA, (int)(log(size)/log(2)), 0, 11);
max_KVAL = (int)(log(size)/log(2));
max_C = old_node->C;
/*printf("\nFirst Address of address chunk : %p", ASL->NODE->BA);
printf("\nLast Address of address chunk  : %p", (void *)(ASL->NODE->BA + size - 1));
printf("\nSize of memory space (in Bytes): %d", size);
printf("\nKVAL : %d\n", ASL->NODE->KVAL);*/
printf("\nMemory Space Reserved for Weighted Buddy System successfully!\n");
return ASL;
}

void set_metadata(void* BA, int KVAL, int TAG, int TYPE)
{
int index;
index = BA - base_addr;
metadata[index] = (metadata[index] & 224) | KVAL;
//printf("\nValue at index %d after KVAL update : %d\n", index, (metadata[index]&255));
metadata[index] = (metadata[index] & 127) | TAG<<7;
//printf("\nValue at index %d after TAG update : %d\n", index, (metadata[index]&255));
//printf("\nTAG Updated : %d", (int)(metadata[index] & 128));
metadata[index] = (metadata[index] & 159) | TYPE<<5;
//printf("\nValue at index %d after TYPE update : %d\n", index, (metadata[index]&255));
//printf("\nValue at index %d after metadata update : %d\n", index, (metadata[index]&255));
}

void* allocate_memory(struct ASL* ASL, int size)
{
int TYPE;
struct ASL_list* NODE = ASL->LIST;
struct ASL_list* PNTR;
void* address;

	while(NODE != NULL)
	{
		if(NODE->NODE == NULL || NODE->C*pow(2, NODE->KVAL) < size)
		{
		NODE = NODE->NEXT;
		}
		else
		{
		break;
		}
	}
	
	if(NODE == NULL)
	{
	return NULL;
	}

again1:
PNTR = ASL->LIST;

	if(NODE->C == 1)
	{
		if(pow(2, (NODE->KVAL-2)) >= size)
		{
			while(PNTR != NULL)
			{
				if(PNTR->KVAL == NODE->KVAL-2 && PNTR->C == 1)
				{
				break;
				}
				else
				{
				PNTR = PNTR->NEXT;
				}
			}
	
			if(PNTR != NULL)
			{			
			NODE = split_node(ASL, NODE, 'R');
			goto again1;
			}
		}
		else
		{
			if(3*pow(2, (NODE->KVAL-2)) >= size)
			{
				while(PNTR != NULL)
				{
					if(PNTR->KVAL == NODE->KVAL-2 && PNTR->C == 3)
					{
					break;
					}
					else
					{
					PNTR = PNTR->NEXT;
					}
				}
	
				if(PNTR != NULL)
				{			
				NODE = split_node(ASL, NODE, 'L');
				goto again1;
				}
			}
		}
	}
	else
	{
		if(pow(2, (NODE->KVAL)) >= size)
		{
			while(PNTR != NULL)
			{
				if(PNTR->KVAL == NODE->KVAL && PNTR->C == 1)
				{
				break;
				}
				else
				{
				PNTR = PNTR->NEXT;
				}
			}
	
			if(PNTR != NULL)
			{			
			NODE = split_node(ASL, NODE, 'R');
			goto again1;
			}
		}
		else
		{
			if(pow(2, (NODE->KVAL+1)) >= size)
			{
				while(PNTR != NULL)
				{
					if(PNTR->KVAL == NODE->KVAL+1 && PNTR->C == 1)
					{
					break;
					}
					else
					{
					PNTR = PNTR->NEXT;
					}
				}
	
				if(PNTR != NULL)
				{			
				NODE = split_node(ASL, NODE, 'L');
				goto again1;
				}
			}	
		}
	}

printf("\nBase address of allocated block of %d size is : %p\n", (int)(NODE->C * pow(2, NODE->KVAL)), NODE->NODE->BA);
allocated_addr = NODE->NODE->BA;
sum_of_alloc = sum_of_alloc + (int)(NODE->C * pow(2, NODE->KVAL));
sum_of_alloc1 = sum_of_alloc1 + (int)(NODE->C * pow(2, NODE->KVAL));;
sum_of_req = sum_of_req + size;
set_metadata(NODE->NODE->BA, NODE->KVAL, 1, 11);
struct ASL_node* PARENT = NODE->NODE;

	if(NODE->NODE->NEXT == NULL)
	{
	NODE->NODE = NULL;
	}
	else
	{
	NODE->NODE = NODE->NODE->NEXT;
	PARENT->NEXT = NULL;
	NODE->NODE->PREV = NULL;
	}

free(PARENT);
return allocated_addr;
}

struct ASL_list* split_node(struct ASL* ASL, struct ASL_list* NODE, char node_type)
{
int KVAL, C, TYPE_1, TYPE_2, comp_KVAL_1, comp_C_1, comp_KVAL_2, comp_C_2, PRTYPE;
void* addr;
struct ASL_list* ASL_TRV    = ASL->LIST;
struct ASL_list* return_node;
struct ASL_node* NODE1 = (struct ASL_node*) malloc(sizeof(struct ASL_node));
struct ASL_node* NODE2 = (struct ASL_node*) malloc(sizeof(struct ASL_node));

	if(NODE->C == 1)
	{
	comp_KVAL_1 = NODE->KVAL - 2;
	comp_C_1 = 3;
	TYPE_1 = 11;
	comp_KVAL_2 = NODE->KVAL - 2;
	comp_C_2 = 1;
	TYPE_2 = 11;
	}
	else
	{
	comp_KVAL_1 = NODE->KVAL + 1;
	comp_C_1 = 1;
	TYPE_1 = 01;
	comp_KVAL_2 = NODE->KVAL;
	comp_C_2 = 1;
	TYPE_2 = 10;
	}

NODE1->BA = NODE->NODE->BA;
PRTYPE = (int)((metadata[NODE->NODE->BA - base_addr] & 96)>>5);

	while(ASL_TRV != NULL)
	{
		if(ASL_TRV->KVAL == comp_KVAL_1 && ASL_TRV->C == comp_C_1)
		{
		break;
		}
		else
		{
/*			if(ASL_TRV->C * ASL_TRV->KVAL > comp_C_1 * comp_KVAL_1)
			{
			ASL_TRV = NULL;
			break;
			}*/
		ASL_TRV = ASL_TRV->NEXT;
		}
	}
	
	if(ASL_TRV == NULL)
	{
	printf("\nBlock size error in ASL list_1!\n");
	exit(0);
	}
	
	if(node_type == 'L')
	{
	return_node = ASL_TRV;
	}
	
KVAL = ASL_TRV->KVAL;
C = ASL_TRV->C;

	if(ASL_TRV->NODE == NULL)
	{
	NODE1->NEXT = NULL;
	NODE1->PREV = NULL;
	ASL_TRV->NODE = NODE1;
	}
	else
	{
	struct ASL_node* temp = ASL_TRV->NODE;
		while(temp->NEXT != NULL)
		{
		temp = temp->NEXT;
		}
		
	NODE1->NEXT = NULL;
	temp->NEXT = NODE1;
	NODE1->PREV = temp;
	}

set_metadata(NODE1->BA, KVAL, 0, TYPE_1);
NODE2->BA = (void*)(NODE->NODE->BA + (int)(comp_C_1*pow(2, comp_KVAL_1)));
//printf("\nNode1->BA : %p\tNode2->BA : %p\n", NODE1->BA, NODE2->BA);

struct ASL_list* ASL_TRV1 = ASL->LIST;

	while(ASL_TRV1 != NULL)
	{
		if(ASL_TRV1->KVAL == comp_KVAL_2 && ASL_TRV1->C == comp_C_2)
		{
		break;
		}
		else
		{
/*			if(ASL_TRV1->C * ASL_TRV1->KVAL > comp_C_2 * comp_KVAL_2)
			{
			ASL_TRV1 = NULL;
			break;
			}*/
		ASL_TRV1 = ASL_TRV1->NEXT;
		}
	}

	if(ASL_TRV1 == NULL)
	{
	printf("\nBlock size error in ASL list_2!\n");
	exit(0);
	}

	if(node_type == 'R')
	{
	return_node = ASL_TRV1;
	}

KVAL = ASL_TRV1->KVAL;
C = ASL_TRV1->C;

	if(ASL_TRV1->NODE == NULL)
	{
	NODE2->NEXT = NULL;
	NODE2->PREV = NULL;
	ASL_TRV1->NODE = NODE2;
	}
	else
	{
	struct ASL_node* temp = ASL_TRV1->NODE;

		while(temp->NEXT != NULL)
		{
		temp = temp->NEXT;
		}
		
	NODE2->NEXT = NULL;
	temp->NEXT = NODE2;
	NODE2->PREV = temp;
	}

set_metadata(NODE2->BA, KVAL, 0, TYPE_2);

struct ASL_node* PARENT = NODE->NODE;

	if(TYPE_1 == 11)
	{
		if(prnt_list->head == NULL)
		{
		prnt_list->head = (struct parent_type*)malloc(sizeof(struct parent_type));
		prnt_list->head->BA = NODE1->BA;
		struct parent_type* prt_NODE1 = (struct parent_type*)malloc(sizeof(struct parent_type));
		prt_NODE1->BA = NODE2->BA;
		prnt_list->head->NEXT = prt_NODE1;
		prnt_list->head->PREV = NULL;
		prt_NODE1->NEXT = NULL;
		prt_NODE1->PREV = prnt_list->head; 
		prnt_list->head->prt_TYPE = PRTYPE;
		prt_NODE1->prt_TYPE = PRTYPE;
		}
		else
		{
		struct parent_type* prt_NODE1 = (struct parent_type*)malloc(sizeof(struct parent_type));
		prt_NODE1->BA = NODE1->BA;
		struct parent_type* prt_NODE2 = (struct parent_type*)malloc(sizeof(struct parent_type));
		prt_NODE2->BA = NODE2->BA;
		prt_NODE1->NEXT = prt_NODE2;
		prt_NODE1->PREV = NULL;
		prt_NODE2->NEXT = prnt_list->head;
		prt_NODE2->PREV = prt_NODE1;
		prnt_list->head->PREV = prt_NODE2; 
		prnt_list->head = prt_NODE1; 
		prt_NODE1->prt_TYPE = PRTYPE;
		prt_NODE2->prt_TYPE = PRTYPE;		
		}
	}
	
	if(NODE->NODE->NEXT == NULL)
	{
	NODE->NODE = NULL;
	}
	else
	{
	NODE->NODE = NODE->NODE->NEXT;
	PARENT->NEXT = NULL;
	NODE->NODE->PREV = NULL;
	}
	
free(PARENT);

return return_node;
}

int traverse_ASL(struct ASL* ASL)
{
int i=0, j, k, count;
printf("\n");
struct ASL_list* LIST = ASL->LIST;

printf("\nList of available block chunks !\n");
	while(LIST != NULL)
	{
		if(LIST->NODE != NULL)
		{
		printf("\n%d. List of block size : %d", ++i, (int)(LIST->C * pow(2, LIST->KVAL)));
		struct ASL_node* NODE = LIST->NODE;
		j = 0;
			while(NODE != NULL)
			{
			printf("\n\t%d. Address : %p", ++j, NODE->BA);
			NODE = NODE->NEXT;
			}
		printf("\n");
		}
	LIST = LIST->NEXT;
	}
j = 0;
printf("\nChar metadata content !\n");

//count = (int)(sizeof(metadata)/sizeof(metadata[0]));
//printf("\nMetadata Count : %d", count);
	for(k=0; k<metadata_size; k++)
	{
//		if((int)(metadata[k] & 128) == 128)
		if(((int)(metadata[k] & 96)>>5) != 0)
		{
		printf("\n%d. Index : %d\tKVAL : %d\tTYPE : %d\tTAG : %d", ++j, k, (int)(metadata[k] & 31), (int)((metadata[k] & 96)>>5), (int)(metadata[k] & 128));
		printf("\n");
		}
//	printf("\n");
	}
printf("\n");

return i;
}

int release_memory(struct ASL* ASL, void* BA)
{
int index, buddy_index, TYPE, KVAL, TAG, C, buddy_TYPE, buddy_KVAL, buddy_TAG, new_KVAL, prt_TYPE, right_buddy_KVAL, right_buddy_C, left_buddy_KVAL, left_buddy_C, flag = 0;
int buddy_req_KVAL;
void* buddy_addr; void* left_buddy_addr; void* right_buddy_addr;

start:

index = buddy_index = TYPE = KVAL = TAG = C = buddy_TYPE = buddy_KVAL = buddy_TAG = new_KVAL = prt_TYPE = right_buddy_KVAL = right_buddy_C = buddy_req_KVAL = left_buddy_KVAL = left_buddy_C = 0;
buddy_addr = NULL; left_buddy_addr = NULL; right_buddy_addr = NULL;
index = BA - base_addr;

	if((int)(metadata[index] & 128) == 0 && flag == 0)
	{
	printf("\n***Requested block is already free!***\n");
	return 0;
	}
	
KVAL = (int)(metadata[index] & 31);
TYPE = (int)((metadata[index] & 96)>>5);
	
	if(TYPE == 3)
	{
//	C = 1;
		if(index == 0 && KVAL == max_KVAL)
		{
		struct ASL_node* NODE = (struct ASL_node*) malloc(sizeof(struct ASL_node));
		NODE->BA = BA;

		struct ASL_list* ASL_TRV2 = ASL->LIST;

			while(ASL_TRV2->NEXT != NULL)
			{
			ASL_TRV2 = ASL_TRV2->NEXT;
			}

		NODE->NEXT = NULL;
		NODE->PREV = NULL;
		ASL_TRV2->NODE = NODE;
		sum_of_alloc = sum_of_alloc - (BA - base_addr);
		return 1; //all allocated blocks released and merged
		}
		
		if(index % (int)(pow(2, KVAL+2)) == 0)
		{
		buddy_addr = (void*)(BA + (int)(3 * pow(2, KVAL)));
		left_buddy_addr = BA;
		right_buddy_addr = buddy_addr;
		right_buddy_KVAL = KVAL;
		left_buddy_KVAL = KVAL;
		right_buddy_C = 1;
		left_buddy_C = 3;
		C = 3;
		}
		else
		{
			if((index % (int)(pow(2, KVAL+2))) == (3 * pow(2, KVAL)))
			{
			buddy_addr = (void*)(BA - (int)(3 * pow(2, KVAL)));
			left_buddy_addr = buddy_addr;
			right_buddy_addr = BA;
			right_buddy_KVAL = KVAL;
			left_buddy_KVAL = KVAL;
			right_buddy_C = 1;
			left_buddy_C = 3;
			C = 1;
			}
			else
			{
			printf("\nBuddy identification error_1!\n");
			return 0;
			}
		}
	new_KVAL = KVAL + 2;
	buddy_req_KVAL = KVAL;
	}
	else
	{
//	C = 3;
	C = 1;
		if(TYPE == 1)
		{
		buddy_addr = (void*)(BA + (int)(pow(2, KVAL)));
		left_buddy_addr = BA;
		right_buddy_addr = buddy_addr;
		right_buddy_KVAL = KVAL-1;
		left_buddy_KVAL = KVAL;
		right_buddy_C = 1;
		left_buddy_C = 1;
		new_KVAL = KVAL - 1;
		buddy_req_KVAL = KVAL - 1;
		}
		else
		{
			if(TYPE == 2)
			{
			buddy_addr = (void*)(BA - (int)(pow(2, KVAL+1)));
			left_buddy_addr = buddy_addr;
			right_buddy_addr = BA;
			right_buddy_KVAL = KVAL;
			left_buddy_KVAL = KVAL + 1;
			right_buddy_C = 1;
			left_buddy_C = 1;
			new_KVAL = KVAL;
			buddy_req_KVAL = KVAL + 1;
			}
			else
			{
			printf("\nInvalid value for TYPE field encountered!\n");
			return 0;
			}
		}
	}

buddy_index = buddy_addr - base_addr;
buddy_KVAL  = (int)(metadata[buddy_index] & 31);
buddy_TAG   = (int)(metadata[buddy_index] & 128);
buddy_TYPE  = (int)((metadata[buddy_index] & 96)>>5);

/*	if(buddy_KVAL != buddy_req_KVAL)
	{
	printf("\nBuddy KVAL values does not match, cannot merge further!\n");
	return 1;
	}*/
	
	if(buddy_TAG == 128 || buddy_KVAL != buddy_req_KVAL)
	{
	struct ASL_node* NODE = (struct ASL_node*) malloc(sizeof(struct ASL_node));
	NODE->BA = BA;
	
	struct ASL_list* ASL_TRV = ASL->LIST;

		while(ASL_TRV != NULL)
		{
			if(ASL_TRV->KVAL == KVAL && ASL_TRV->C == C)
			{
			break;
			}
			else
			{
			ASL_TRV = ASL_TRV->NEXT;
			}
		}

		if(ASL_TRV == NULL)
		{
		printf("\nBlock size error in ASL list_2!\n");
		exit(0);
		}
	
		if(ASL_TRV->NODE == NULL)
		{
		NODE->NEXT = NULL;
		NODE->PREV = NULL;
		ASL_TRV->NODE = NODE;
		}
		else
		{
		struct ASL_node* trv_node = ASL_TRV->NODE;

			while(trv_node->NEXT != NULL)
			{
			trv_node = trv_node->NEXT;
			}
		trv_node->NEXT = NODE;
		NODE->PREV = trv_node;
		NODE->NEXT = NULL;
		}
	sum_of_alloc = sum_of_alloc - (BA - base_addr);
	set_metadata(BA, KVAL, 0, TYPE);
	return 1;
	}
	
//left_buddy_ind = left_buddy_addr - base_addr;
//right_buddy_ind = right_buddy_addr - base_addr;
	
	if(buddy_TYPE == 3)
	{
	struct parent_type* GET_TYPE = prnt_list->head;
		while(GET_TYPE != NULL)
		{
			if(GET_TYPE->BA == buddy_addr)
			{
			prt_TYPE = GET_TYPE->prt_TYPE;
			break;
			}
			else
			{
			GET_TYPE = GET_TYPE->NEXT;
			}
		}
//	new_KVAL = KVAL + 2;
		
		if(GET_TYPE->PREV != NULL)
		{
		GET_TYPE->PREV->NEXT = GET_TYPE->NEXT;
			
			if(GET_TYPE->NEXT != NULL)
			{
			GET_TYPE->NEXT->PREV = GET_TYPE->PREV;
			}
		GET_TYPE->NEXT = NULL;
		GET_TYPE->PREV = NULL;
		}
		else
		{
			if(GET_TYPE->NEXT != NULL)
			{
			GET_TYPE->NEXT->PREV = NULL;
			}
		prnt_list->head = GET_TYPE->NEXT;
		GET_TYPE->NEXT = NULL;
		}
	free(GET_TYPE);
	}
	else
	{
	prt_TYPE = 3;
//	new_KVAL = KVAL;
	}

set_metadata(left_buddy_addr, new_KVAL, 0, prt_TYPE);
set_metadata(right_buddy_addr, 0, 0, 0);
printf("\nBuddies with address %p and %p have been merged!\n", left_buddy_addr, right_buddy_addr);
struct ASL_list* ASL_TRV1 = ASL->LIST;

	while(ASL_TRV1 != NULL)
	{
		if(ASL_TRV1->KVAL == right_buddy_KVAL && ASL_TRV1->C == right_buddy_C)
		{
		break;
		}
		else
		{
		ASL_TRV1 = ASL_TRV1->NEXT;
		}
	}

	if(ASL_TRV1 == NULL)
	{
	printf("\nBlock size error in ASL list_2!\n");
	exit(0);
	}

struct ASL_node* PARENT = ASL_TRV1->NODE;
	
	if(ASL_TRV1->NODE != NULL)
	{
		if(ASL_TRV1->NODE->NEXT == NULL)
		{
		ASL_TRV1->NODE = NULL;
		}
		else
		{
		ASL_TRV1->NODE = ASL_TRV1->NODE->NEXT;
		PARENT->NEXT = NULL;
		ASL_TRV1->NODE->PREV = NULL;
		}
	free(PARENT);
	}

ASL_TRV1 = ASL->LIST;

	while(ASL_TRV1 != NULL)
	{
		if(ASL_TRV1->KVAL == left_buddy_KVAL && ASL_TRV1->C == left_buddy_C)
		{
		break;
		}
		else
		{
		ASL_TRV1 = ASL_TRV1->NEXT;
		}
	}

	if(ASL_TRV1 == NULL)
	{
	printf("\nBlock size error in ASL list_2!\n");
	exit(0);
	}

PARENT = ASL_TRV1->NODE;
	
	if(ASL_TRV1->NODE != NULL)
	{
		if(ASL_TRV1->NODE->NEXT == NULL)
		{
		ASL_TRV1->NODE = NULL;
		}
		else
		{
		ASL_TRV1->NODE = ASL_TRV1->NODE->NEXT;
		PARENT->NEXT = NULL;
		ASL_TRV1->NODE->PREV = NULL;
		}
	free(PARENT);
	}

BA = left_buddy_addr;
flag = 1;
goto start;
}

struct OB* get_OneBin_Space(int size, int fixed_size)
{
int count, i;
struct OB* OB = (struct OB*)malloc(sizeof(struct OB));
OB->OB_Fixed_Size = fixed_size;
count = size / fixed_size;
OB->LIST = (struct OB_list*)malloc(sizeof(struct OB_list));
OB->LIST->BA = (void*)malloc(size);
OB->LIST->TAG = 0;
OB->LIST->NEXT = NULL;
OB->LIST->PREV = NULL;
struct OB_list* old_node = OB->LIST;
	for(i=1; i<count; i++)
	{
	struct OB_list* new_node = (struct OB_list*)malloc(sizeof(struct OB_list));
	new_node->TAG = 0;
	new_node->BA  = (void*)(old_node->BA + fixed_size - 1);
	old_node->NEXT = new_node;
	new_node->PREV = old_node;
	new_node->NEXT = NULL;
	old_node = new_node;
	} 
printf("\nMemory Space Reserved for OneBin System successfully!\n");
return OB;
}

void* allocate_OB_memory(struct OB* OB)
{
struct OB_list* TEMP = OB->LIST;
//int required_size = ((int)ceil(OB->OB_Fixed_Size / size)) * OB->OB_Fixed_Size;

	while(TEMP != NULL)
	{
//		if(TEMP->TAG == 1 || TEMP->SIZE < required_size)
		if(TEMP->TAG == 1)
		{
		TEMP = TEMP->NEXT;
		}
		else
		{
		break;
		}
	}
	
	if(TEMP == NULL)
	{
	return NULL;
	}

TEMP->TAG = 1;

/*	if(TEMP->SIZE == required_size)
	{
	return 1;
	}

struct OB_list* new_node = (struct OB_list*)malloc(sizeof(struct OB_list));
new_node->BA = (void*)(TEMP->BA + required_size - 1);
new_node->SIZE = TEMP->SIZE - required_size;
new_node->TAG = 0;
new_node->NEXT = TEMP->NEXT;
	if(TEMP->NEXT != NULL)
	{
	TEMP->NEXT->PREV = new_node;
	}
TEMP->NEXT = new_node;
new_node->PREV = TEMP;
TEMP->SIZE = required_size;*/

return TEMP->BA;
}

int traverse_OB(struct OB* OB)
{
int i=0, j, k, count;
printf("\n");
struct OB_list* LIST = OB->LIST;

printf("\nList of occupied block chunks !\n");
	while(LIST != NULL)
	{
		if(LIST->TAG == 1)
		{
		printf("\n%d. Address : %p", ++i, LIST->BA);
		}
	LIST = LIST->NEXT;
	}
/*printf("\nList of occupied block chunks !\n");

count = (int)(sizeof(metadata)/sizeof(metadata[0]));
printf("\nMetadata Count : %d", count);
	for(k=0; k<count; k++)
	{
		if((metadata[k] & 128) == 1)
		{
		printf("\nKVAL : %d\tTYPE : %d", (int)(metadata[k] & 31), (int)(metadata[k] & 96));
		}
	printf("\n");
	}*/
printf("\n");

return i;
}

int OB_release_memory(struct OB* OB, int blk_no)
{
int i=0;
struct OB_list* NODE = OB->LIST;
	if(NODE->TAG == 1)
	{
	i++;
	}

	while(i != blk_no)
	{
	NODE = NODE->NEXT;
		if(NODE->TAG == 1)
		{
		i++;
		}
	}
	
	NODE->TAG = 0;
	
return 1;
}

void* place_req(struct ASL* ASL, struct simulate_head* LIST, int i, int iterations)
{
int size, time, allocated;
struct simulate_list* list = LIST->head;
struct simulate_list* NODE = (struct simulate_list*)malloc(sizeof(struct simulate_list));
allocated_addr = NULL;
void* pointer;

size = (int)rand() % (int)(pow(2, max_KVAL));
	if(iterations != i)
	{
	time = (int)rand() % (int)(iterations - i);
	}
	else
	{
	time = 0;
	}
printf("\nAllocation request for size %dB generated, to be released at time interval %d\n", size, time + i);
//delay(2000);

pointer = allocate_memory(ASL, size);
	if(pointer == NULL)
	{
	printf("\n*** Allocation request for block size %dB not fulfilled.. \tAllocation%% : %f***\n", size, (float)sum_of_alloc/total_size);
	return NULL;
	}
	
NODE->BA = allocated_addr;
NODE->end_time = i + time;
NODE->NEXT = NULL;
NODE->PREV = NULL;
	if(list == NULL)
	{
	LIST->head = NODE;
	printf("\n*** Allocation request for block size %dB fulfilled succesfully ***\n", size);
	return pointer;
	}
	
	while(list->NEXT != NULL && list->end_time <= NODE->end_time)
	{
	list = list->NEXT;	
	}
	
NODE->PREV = list->PREV;
list->PREV = NODE;
NODE->NEXT = list;
	if(NODE->PREV != NULL)
	{
	NODE->PREV->NEXT = NODE;
	}
	else
	{
	LIST->head = NODE;
	}

printf("\n*** Allocation request for block size %dB fulfilled succesfully ***\n", size);
return pointer;
}
