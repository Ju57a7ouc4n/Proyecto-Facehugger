typedef struct node{
    char* filename;
    struct node *next, *prev;
}node;

typedef struct list{
    node *head, *tail;
}list;

void init_list(list *l);

void add_node(list *l, const char *filename);

void remove_node(list *l, const char *filename);

void print_debug_list(const list *file_list);