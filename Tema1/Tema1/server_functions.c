#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_functions.h"

// initializare nod pentru lista de useri
node_users_main *init_node(char *userId, char *token)
{ 
    node_users_main *aux = malloc (sizeof(*aux));

    strcpy(aux->userId, userId);
    strcpy(aux->auth_token, token);
    strcpy(aux->acces_token, "");
    strcpy(aux->refresh_token, "");
    
    aux->approval = -1;
    aux->ttl = -1;
    aux->has_refresh = 1;

    aux->next = NULL;
    
    return aux;
}

// initializare nod pentru lista de drepturi
resources_node_t *init_node_res(char *resName, char *op)
{
    resources_node_t *aux = malloc(sizeof(*aux));

    strcpy(aux->resName, resName);
    strcpy(aux->op, op);

    aux->next = NULL;

    return aux;
}

// eliberare nod pentru useri
void free_node(node_users_main *node)
{
    free(node->userId);
    free(node->auth_token);
    free(node->refresh_token);
    free(node);
}

// initializare lista useri
users_main_list_t *init_list()
{
    users_main_list_t *list = malloc (sizeof(*list));

    list->head = NULL;
    list->tail = NULL;
    list->len = 0;

    return list;
}

// initializare lista drepturi
resoureces_list_t *init_list_res()
{
    resoureces_list_t *list = malloc (sizeof(*list));
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;

    return list;

}

// distrugere lista useri
void destroy_list(users_main_list_t *list)
{
    if(list->head == NULL){
        free(list);
        return;
    }

    node_users_main *aux, *auxx;
    aux = list->head;

    while(aux != NULL){
        auxx = aux->next;
        free_node(aux);
        aux = auxx;
    }

    free(list);
}

// distrugere lisata drepturi
void destroy_list_res(resoureces_list_t *list) 
{
    if(list->head == NULL){
        free(list);
        return;
    }

    resources_node_t *aux, *auxx;
    aux = list->head;

    while(aux != NULL){
        auxx = aux->next;
        free(aux);
        aux = auxx;
    }

    free(list);
}

// adaugare nod useri
void insert_node(users_main_list_t *list, char *userId, char* token)
{
    node_users_main *aici;
    aici = init_node(userId, token);

    if (list->len == 0) {
        list->head = aici;
        list->tail = aici;
        aici->next = NULL;
    } else {
        list->tail->next = aici;
        aici->next = NULL;
        list->tail = aici;
    }

    list->len ++;
}

// adaugare nod drepturi
void insert_node_res(resoureces_list_t *list, char *resName, char* op)
{
    resources_node_t *aici;
    aici = init_node_res(resName, op);

    if (list->len == 0) {
        list->head = aici;
        list->tail = aici;
        aici->next = NULL;
    } else {
        list->tail->next = aici;
        aici->next = NULL;
        list->tail = aici;
    }

    list->len ++;
}

// schimbare ttl pentru un anumit user
void change_ttl(char* userId, int new_ttl)
{
    node_users_main *aux = users_main_list->head;
	int maxLen = users_main_list->len;
	int cnt = 0;

    while (cnt < maxLen) {
		if (strcmp(aux->userId, userId) == 0) {
            aux->ttl = new_ttl;
            fflush(stdout);
		}

        cnt++;
        aux = aux->next;
	}
}

// print lista useri (pentru debug)
void print_list(users_main_list_t *list)
{
    node_users_main *aux = list->head;

    printf("%s\n", "Lista construita:");
    fflush(stdout);

    while (aux->next != NULL) {

        printf("%s-%s-%d-%d-%s-%s-%d\n", aux->userId, aux->auth_token, aux->ttl,
                 aux->approval, aux->acces_token, aux->refresh_token, aux->has_refresh);
        aux = aux->next;
    }


    printf("%s-%s-%d-%d-%s-%s-%d\n", aux->userId, aux->auth_token, aux->ttl,
                 aux->approval, aux->acces_token, aux->refresh_token, aux->has_refresh);
    fflush(stdout);
}

// print lista drepturi (pentru debug)
void print_list_res(resoureces_list_t *list)
{
    resources_node_t *aux = list->head;

    printf("%s\n", "Resurse construite:");
    fflush(stdout);

    while (aux->next != NULL) {
        printf("%s->%s\n", aux->resName, aux->op);

        aux = aux->next;
    }

    printf("%s->%s\n", aux->resName, aux->op);
    fflush(stdout);
}

// marire dimensiune pentru lista de drepturi daca e nevoie
resoureces_list_t *resize_array(resoureces_list_t *array, int *capacity) {
    *capacity += 10;
    array = realloc(array, (*capacity) * sizeof(resoureces_list_t));

    if (array == NULL) {
        perror("Eroare la realloc");
        exit(1);
    }

    return array;
}

// verificare "permission denied" in lista
int check_request_permission_denied(resoureces_list_t *resources)
{
    // 0 e bine, 1 are permisiune proasta

    resources_node_t *node = resources->head;
    int maxLen = resources->len;
	int cnt = 0;

    while (cnt < maxLen) {
        if (strcmp(node->resName, "*") == 0 && strcmp(node->op, "-") == 0) {
            return 1;
        }

        node = node->next;
		cnt++;
    }

    return 0;
}

// gaseste userul pentru verificarea requestului de "permission denied"
int check_permission(char *user_id)
{
    // 0 e bine, 1 are permisiune proasta

    node_users_main *aux = users_main_list->head;
	int maxLen = users_main_list->len;
	int cnt = 0;
    resoureces_list_t *resources;

    while (cnt < maxLen) {

		if (strcmp(user_id, aux->userId) == 0) {
            
            resources = approvals_list[aux->approval];

            int res = check_request_permission_denied(resources);

            return res;
		}

        aux = aux->next;
		cnt++;
    }

    return 1;
}

// verificare daca semnatura este corecta
int check_signature (char *approve_token, char *authorized_token)
{
    if (strcmp(approve_token, authorized_token) != 0) {
        char *modified_token = strdup(approve_token);
        modified_token[strlen(modified_token) - 1] = 'z' 
                        - modified_token[strlen(modified_token) - 1];

        if (strcmp(modified_token, authorized_token) == 0) {
            return 1;
        }
    }

    return 0;
}

// face rost de structura unui anumit user pe baza id-ului
node_users_main *get_node(char *id)
{
    node_users_main *aux = users_main_list->head;
	int maxLen = users_main_list->len;
	int cnt = 0;

    while (cnt < maxLen) {
		if (strcmp(aux->userId, id) == 0) {
            return aux;
		}

        cnt++;
        aux = aux->next;
	}

    return NULL;
}

// face rost de structura unui anumit user pe baza tokenului de acces
node_users_main *get_node_with_acc_token(char *tk)
{
    node_users_main *aux = users_main_list->head;
	int maxLen = users_main_list->len;
	int cnt = 0;

    while (cnt < maxLen) {
		if (strcmp(aux->acces_token, tk) == 0) {
            return aux;
		}

        cnt++;
        aux = aux->next;
	}

    return NULL;
}

// verifica existenta unei resurse
int check_resources(char *name)
{
    // 0 exista, 1 nu exista

    for (int i=0; i<nr_resources; i++) {
        if (strcmp(resources[i], name) == 0) {
            return 0;
        }
    }

    return 1;
}
