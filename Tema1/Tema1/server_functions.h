#include <stdio.h>

// structura pentru nodul de useri
typedef struct node_users_main {
    char userId[15];
    int ttl;
    char auth_token[15];
    int approval;
    char acces_token[15];
    int has_refresh;                // 1 true, 0 false
    char refresh_token[15];

    struct node_users_main *next;
} node_users_main;

// lista de useri
typedef struct users_main_list_t {
    node_users_main *head;
    node_users_main *tail;
    int len;
} users_main_list_t;

// structura pentru lista de drepturi
typedef struct resources_node_t {
    char resName[100];
    char op[5];

    struct resources_node_t *next;
} resources_node_t;

// lista de drepturi
typedef struct resoureces_list_t {
    resources_node_t *head;
    resources_node_t *tail;
    int len;
} resoureces_list_t;

users_main_list_t *users_main_list;     // lista de useri
resoureces_list_t **approvals_list;     // vector de liste de drepturi
char **resources;                       // resursele
FILE *approve_file;

int Counter_Resources_Taken;            // determina prima resursa neluata din "approvals_list"
int TTL;                                // ttl-ul dat in argumente
int nr_resources;                       // numat total de resurse

// functiile sunt descrise in "server_functions.c"

node_users_main *init_node(char *userId, char *token);
resources_node_t *init_node_res(char *resName, char *op);

void free_node(node_users_main *node);

users_main_list_t *init_list();
resoureces_list_t *init_list_res();

void destroy_list(users_main_list_t *list);
void destroy_list_res(resoureces_list_t *list);

void insert_node(users_main_list_t *list, char* userId, char *token);
void insert_node_res(resoureces_list_t *list, char *resName, char* op);

void change_ttl(char* userId, int new_ttl);

void print_list(users_main_list_t *list);
void print_list_res(resoureces_list_t *list);

resoureces_list_t *resize_array(resoureces_list_t *array, int *capacity);

int check_permission(char *user_id);

int check_signature (char *approve_token, char *authorized_token);

node_users_main *get_node(char *id);
node_users_main *get_node_with_acc_token(char *tk);

int check_resources(char *name);

