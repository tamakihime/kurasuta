//
// Created by touho on 2018/11/29.
//

#define gene_number 7000
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
            if(row1[k]=='|') {
                while(1){
                    if(row1[k]=='\t'){
                        break;
                    }
                    k++;
                }
                m++;
                k++;
                break;
            }
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
    return 0;
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
void printstructure(HASH *h1){
    FILE *OUT;
    int n=0;
    if((OUT=fopen("gene_associationexperiment.txt","w"))==NULL){
        printf("cantopen\n");
        exit(1);
    }
   for( n=0;n<gene_number;n++){
      GENE_GO *p=h1->hashtable[n];
       while(p != NULL){
           printf("%s\t",p->gene);
           fprintf(OUT,"%s\t",p->gene);
               for (int i = 0; i <p->N_GO ; i++) {
                  printf("%s\t",p->GO[i]);
                  fprintf(OUT,"%s\t",p->GO[i]);
               }
               printf("\n");
               fprintf(OUT,"\n");
           p=p->next;
       }
   }
   fclose(OUT);
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
    printstructure(&h);
}