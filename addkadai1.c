#include <stdio.h>
#include <stdlib.h>

//
// Created by touho on 2018/11/29.
//
int split(char *row1,char column1[40][300]);

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

int main(){
    FILE *in;
    char line[4056];
    char column1[40][300];
    if((in=fopen("gene_association.sgd","r"))==NULL){
        printf("cant open infile");
        exit(1);
    }
    while(fgets(line,4056,in)){
        split(line,column1);
    }
}
