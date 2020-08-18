struct node
{
int degree;
float coeff;
struct node* prev;
struct node* next;
};


/*void initialize(char num[]);
struct node* get_polynomial(struct ASL* ASL, struct node* pol);
void put_polynomial(struct node* pol);
void divide_polynomial(struct ASL* ASL, struct node* dividend, struct node* divisor);*/



struct ASL_node
{ 
//	int TYPE;
//	int KVAL;
//	int TAG;
//	int C;
	void* BA;
	struct ASL_node* NEXT;
	struct ASL_node* PREV;
};

struct ASL_list
{ 
	struct ASL_node* NODE;
	int KVAL;
	int C;
	struct ASL_list* NEXT;
	struct ASL_list* PREV;
};

struct ASL
{
	struct ASL_list* LIST;
};

struct OB_list
{
	void* BA;
	int TAG;
	struct OB_list* NEXT;
	struct OB_list* PREV;
};

struct OB
{
	int OB_Fixed_Size;
	struct OB_list* LIST; 
};

struct parent_type
{
	void* BA;
	int prt_TYPE;
	struct parent_type* NEXT;
	struct parent_type* PREV;
};

struct parent_type_list
{
	struct parent_type* head;
};

struct simulate_list
{
	void* BA;
	int end_time;
	struct simulate_list* NEXT;
	struct simulate_list* PREV;
};

struct simulate_head
{
	struct simulate_list* head;
};

struct ASL* get_memory_space(int size);
void* allocate_memory(struct ASL* ASL, int size);
int release_memory(struct ASL* ASL, void* BA);
struct ASL_list* split_node(struct ASL* ASL, struct ASL_list* NODE, char node_type);
int traverse_ASL(struct ASL* ASL);
void set_metadata(void* BA, int KVAL, int TAG, int TYPE);
void* allocate_OB_memory(struct OB* OB);
struct OB* get_OneBin_Space(int size, int fixed_size);
int traverse_OB(struct OB* OB);
int OB_release_memory(struct OB* OB, int blk_no);
void* place_req(struct ASL* ASL, struct simulate_head* LIST, int i, int iterations);
char* metadata;
void* base_addr; void* last_addr; void* allocated_addr;
int metadata_size, max_KVAL, max_C, alloc_req_sat, total_alloc_req, dealloc_req, sum_of_alloc, sum_of_req, sum_of_alloc1, total_size;
float int_frag;
struct parent_type_list* prnt_list;




void initialize(char num[]);
struct node* get_polynomial(struct ASL* ASL, struct node* pol);
void put_polynomial(struct node* pol);
void divide_polynomial(struct ASL* ASL, struct node* dividend, struct node* divisor);
