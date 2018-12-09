
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define gene_number 7000
int split(char *row1, char (*column1)[300]) {
    int i,j;
    int k=0,m=0;
    for(i=0;i<40;i++){
        for(j=0;j<300;j++){
            column1[i][j]='\0';
        }
    }
    for(i=0;i<40;i++){
        for(j=0; ;j++){
            if(row1[k]!='\t'&& row1[k]!='\n'){
                column1[i][j]=row1[k];
                k++;
            }
            else if(row1[k]=='\t') {
                m++;
                k++;
                break;
            }
            else if(row1[k]=='\n'){
                m++;
                i=40;
                break;
            }
        }
    }
    printf("%d",m);
    return m;
}

unsigned int hash_function(char *gene1){
    unsigned int hash_val=0;
    while(*gene1 != '\0'){
        gene1++;
        hash_val=hash_val*37+*gene1;
    }
    return hash_val%gene_number;
}
typedef struct hash{
    char gene[300];
    char GO[100][100];
    struct hash *next;
    int N_GO;
}GENE_GO;

typedef struct {
    GENE_GO **hashtable;
}HASH;

void inithashtable(HASH *h1) {
    int n;
    h1->hashtable = (GENE_GO **) malloc(sizeof(GENE_GO *) * gene_number);
    for (n = 0; n < gene_number; ++n) {
        h1->hashtable[n] = NULL;
    }
}

void init_GENE_GO(GENE_GO *G){
    int i,j,n;
    G->N_GO=0;
    for ( i = 0; i <100 ; ++i) {
        for ( j = 0; j <100 ; ++j) {
            G->GO[i][j] = '\0';
        }
    }
    for(n=0;n<300;n++){
        G->gene[n] ='\0';
    }
}

void add_to_GENEGO_table(char*gene1, char*GO1,HASH *h1){
    unsigned int n = hash_function(gene1);
    GENE_GO *p;
    GENE_GO *p2=h1->hashtable[n];
    int i;
    while(p2 != NULL){
        if(!strcmp(p2->gene,gene1)){
            for (i = 0; i <p2->N_GO ; i++) {
                if(!strcmp(p2->GO[i],GO1)){
                    return;
                }
            }
            strcpy(p2->GO[p2->N_GO],GO1);
            p2->N_GO++;
            return;
        }
        p2=p2->next;
    }
    p=(GENE_GO *)malloc(sizeof(GENE_GO));
    init_GENE_GO(p);
    strcpy(p->gene,gene1);
    strcpy(p->GO[p->N_GO],GO1);
    p->N_GO++;
    p->next=h1->hashtable[n];
    h1->hashtable[n]=p;
}

//GOを格納する
typedef struct hash4{
    char GO[100];
    struct hash4 *next;
}GO_LIST;

typedef struct {
    GO_LIST *GO_table;
}GO_LIST_TABLE;


void init_GO_LIST(GO_LIST *G){
    int i;
    for(i=0;i<100;i++){
        G->GO[i]= (char) "\0";
    }
    G->next=NULL;
}
int add_to_GO_list(char *GO,GO_LIST_TABLE *go_list){
    GO_LIST *p1 =go_list->GO_table;
    GO_LIST *p2;
    while(p1 !=NULL){
        if(!strcmp(p1->GO,GO)){
            return 0;
        }
        p1 =p1->next;
    }
    p2 = (GO_LIST *) malloc(sizeof(GO_LIST));
    init_GO_LIST(p2);
    strcpy(p2->GO,GO);
    p2->next=go_list->GO_table;
    go_list->GO_table=p2;
    return 0;
}
//遺伝子全体をカウントするためのリスト
typedef struct hash3{
    char GO[100];
    int total_N_GO;
    struct hash3 *next;
}TOTAL_GO_COUNT;
typedef struct {
    TOTAL_GO_COUNT **hashtable;
}HASH_TOTAL_GO_COUNT;
void init_hashtablefunction(HASH_TOTAL_GO_COUNT *h1) {
    int n;
    h1->hashtable = (TOTAL_GO_COUNT **) malloc(sizeof(TOTAL_GO_COUNT*) * gene_number);
    for (n = 0; n < gene_number; ++n) {
        h1->hashtable[n] = NULL;
    }
}
void init_TOTAL_GO_COUNT(TOTAL_GO_COUNT *pHash3);

int count_total_n_go(char *GO, HASH_TOTAL_GO_COUNT *hash3){
    unsigned int n=hash_function(GO);
    TOTAL_GO_COUNT *p1 =hash3->hashtable[n];
    TOTAL_GO_COUNT *p2;
    while (p1 != NULL){
        if(!strcmp(GO,p1->GO)){
            p1->total_N_GO++;
            return 0;
        }
        p1=p1->next;
    }
    p2=(TOTAL_GO_COUNT *)malloc(sizeof(TOTAL_GO_COUNT));
    init_TOTAL_GO_COUNT(p2);
    strcpy(p2->GO,GO);
    p2->total_N_GO++;
    p2->next=hash3->hashtable[n];
    hash3->hashtable[n]=p2;
    return 0;
}

void init_TOTAL_GO_COUNT(TOTAL_GO_COUNT *pHash3) {
    int i;
    for(i=0;i<100;i++){
        pHash3->GO[i]= (char) "\0";
    }
    pHash3->next=NULL;
    pHash3->total_N_GO=0;
}
int main(){
    char row[2000];
    char column[40][300];
    HASH h;
    FILE *IN;
    if((IN=fopen("gene_association.sgd","r"))==NULL){
        printf("cantopen1\n");
        exit(1);
    }
    inithashtable(&h);
    do{
        fgets(row,2000,IN);
        split(row,column);
        if(column[8][0]=='P'){
            add_to_GENEGO_table(column[10],column[4],&h);
        }
    }while (!feof(IN));
    fclose(IN);

}
