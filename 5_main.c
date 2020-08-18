#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include "5_header.h"

int main()
{
void* pointer;
void* address;
float digit;
int size, option, allocated, n, blk_no, i, released, j, fixed_size;
alloc_req_sat = 0; total_alloc_req = 0; dealloc_req = 0; sum_of_alloc = 0; sum_of_alloc1 = 0; sum_of_req = 0, total_size = 0; int_frag = 0;
//memset(ZEROARRAY, 0, 1024);

again:
printf("\nEnter size of memory (in MB and a power of 2, max 256MB) : ");
scanf("%d", &size);

	if(size > 256)
	{
	printf("\nPlease enter memory size no more than 256MB!\n");
	goto again;
	}
	
digit = log(size)/log(2);

	if(floor(digit) != ceil(digit))
	{
	printf("Entered memory size in not a power of 2!\n");
	goto again;
	}
	
printf("\n1. Weighted Buddy System.\n2. One Bin Memory Allocation.\n3. Exit\nSelect one option : ");
scanf("%d", &option);

	if(option == 2)
	{
	goto OneBin;
	}
	else
	{
		if(option == 3)
		{
		exit(0);
		}
	}

size = pow(2, ((int)(log(size)/log(2))) + 20);
struct ASL* ASL = get_memory_space(size);
/*struct ASL_list* LIST = ASL->LIST;
j = 0;
	while(LIST != NULL)
	{
	printf("\n%d. C = %d\tKVAL = %d\t Size = %d", ++j, LIST->C, LIST->KVAL, (int)(LIST->C*pow(2, LIST->KVAL)));
	LIST = LIST->NEXT;
	}*/

	while(1)
	{
	printf("\n1. Place new memory request.\n2. Release allocated memory block.\n3. Traverse ASL.\n4. Automatic Simulation\n5. Exit\nSelect one option : ");
	scanf("%d", &option);
	
		if(option == 5)
		{
		break;
		}
		else
		{
			if(option == 1)
			{
			printf("\nPlease enter memory block size to be allocated (in Bytes): ");
			scanf("%d", &size);
//			size = size * pow(2, 20);
			pointer = NULL;
			pointer = allocate_memory(ASL, size);
				if(pointer == NULL)
				{
				printf("\n*** Allocation request for block size %dB not fulfilled.. \tAllocation%% : %f***\n", size, (float)sum_of_alloc/total_size);
				++total_alloc_req;
				}
				else
				{
				printf("\n*** Block of size %dB allocated successfully! ***\n", size);
				++alloc_req_sat;
				++total_alloc_req;
//				struct ASL_list* LIST = ASL->LIST;
//				n = traverse_ASL(ASL);
				}
			}
			else
			{
				if(option == 2)
				{
//				n = traverse_ASL(ASL);
				again2:
				printf("\nEnter the address of block to be released : ");
				scanf("%p", &address);
					/*if(base_addr > address || last_addr < address)
					{
					printf("\nInvalid Block address!\n");
					goto again2;
					}*/
				released = release_memory(ASL, address);
					if(released == 0)
					{
					printf("\n*** Block with address %p is already free ***", address);
					}
					else
					{
					printf("\n*** Block with address %p successfully released ***", address);
					}
				++dealloc_req;
				}
				else
				{
					if(option == 3)
					{
					n = traverse_ASL(ASL);
					printf("\nTotal number of allocation requests : %d\tNumber of successful allocations : %d\tNumber of Deallocations : %d", total_alloc_req, alloc_req_sat, dealloc_req);
						if(sum_of_req != 0)
						{
						int_frag = (float)(sum_of_alloc1 - sum_of_req)/(float)sum_of_req;
						}
					printf("\nInternal Fragmentation : %f\n", int_frag);
					}
					else
					{
						if(option == 4)
						{
						double decide;
						int iterations, i = 1, allocated = 0, choice;
						float p;
						struct simulate_head* LIST = (struct simulate_head*)malloc(sizeof(struct simulate_head));
						LIST->head = NULL;
						
						prob:	
						printf("\nEnter Probability factor (between 0 - 1) of memory request at any time instant : ");
						scanf("%f", &p);
							if(p < 0 || p > 1)
							{
							printf("\nPlease enter a valid probability factor!\n");
							goto prob;
							}
						printf("\nEnter number of time instants : ");
						scanf("%d", &iterations);
							while(i <= iterations)
							{
							printf("\n~~~ Time instant no. %d ~~~\n", i);
							decide = (double)rand()/(double)RAND_MAX;
								if(decide <= p)
								{
								++total_alloc_req;
								pointer = place_req(ASL, LIST, i, iterations);
									if(pointer == NULL)
									{
									printf("\n*** Allocation request not fulfilled.. !");
									}
									else
									{
									++alloc_req_sat;
									}
								}
								else
								{
								printf("\nNo allocation request generated in this time instant!\n");
								}
							
								while(LIST->head != NULL && LIST->head->end_time <= i)
								{
								released = release_memory(ASL, LIST->head->BA);
									if(released == 0)
									{
									printf("\n*** Block with address %p is already free ***\n", LIST->head->BA);
									}
									else
									{
									printf("\n*** Block with address %p successfully released ***\n", LIST->head->BA);
									}
								struct simulate_list* old_node = LIST->head;
									if(LIST->head->NEXT != NULL)
									{
									LIST->head = LIST->head->NEXT;
									LIST->head->PREV = NULL;
									old_node->NEXT = NULL;
									}
									else
									{
									LIST->head = NULL;
									}
								free(old_node);
								++dealloc_req;
								}
							printf("\nTotal number of allocation requests : %d\tNumber of successful allocations : %d\tNumber of Deallocations : %d", total_alloc_req, alloc_req_sat, dealloc_req);
								if(sum_of_req != 0)
								{
								int_frag = (float)(sum_of_alloc1 - sum_of_req)/(float)sum_of_req;
								}
								
							printf("\tInternal Fragmentation : %f\n\n", int_frag);
							
							sel_choice:
							
							printf("\nEnter '1' to proceed or '2' to quit : ");
							scanf("%d", &choice);
							
								if(choice == 1)
								{
								i++;
								printf("\n****************************\n");
								continue;
								}
								else
								{
									if(choice == 2)
									{
									printf("\nQuiting!\n");
									break;
									}
									else
									{
									printf("\nPlease enter correct choice!");
									goto sel_choice;
									}
								}
							}
						}
						else
						{
						printf("\nIncorrect option, choose a valid option!");
						}
					}
				}
			}
		}
	}

return 0;

OneBin:

printf("\nEnter the fixed block size (in Bytes) of OneBin system : ");
scanf("%d", &fixed_size);
size = pow(2, ((int)(log(size)/log(2))) + 20);
	if(size % fixed_size != 0)
	{
	printf("\nEntered fixed size does not divide full memory in equal sizes, re-enter fixed block size!\n");
	goto OneBin;
	}
	
struct OB* OB = get_OneBin_Space(size, fixed_size);
	
	while(1)
	{
	printf("\n1. Place new memory request.\n2. Release allocated memory block.\n3. Traverse OneBin List.\n4. Exit\nSelect one option : ");
	scanf("%d", &option);
	
		if(option == 4)
		{
		break;
		}
		else
		{
			if(option == 1)
			{
			pointer = NULL;
			pointer = allocate_OB_memory(OB);
				if(pointer == NULL)
				{
				printf("\n*** Allocation request is not fulfilled! ***\n");
				}
				else
				{
				printf("\n*** Block allocated successfully! ***\n");
				n = traverse_OB(OB);
				}
			}
			else
			{
				if(option == 2)
				{
				n = traverse_OB(OB);
				again3:
				printf("\nEnter the block number to be released : ");
				scanf("%d", &blk_no);
					if(blk_no < 1 || blk_no > n)
					{
					printf("Invalid Block Number!");
					goto again3;
					}
				released = OB_release_memory(OB, blk_no);
					if(released == 0)
					{
					printf("\n*** Block number %d is already free ***", blk_no);
					}
					else
					{
					printf("\n*** Block number %d successfully released ***", blk_no);
					}
				n = traverse_OB(OB);
				}
				else
				{
					if(option == 3)
					{
					n = traverse_OB(OB);
					}
					else
					{
					printf("\nIncorrect option, choose a valid option!");
					}
				}
			}
		}
	}

return 0;
}
