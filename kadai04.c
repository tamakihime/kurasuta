#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define y 6019
char name[y][100];
#define corr_threshold 0.9
double a;


typedef struct GENE_CORRELATION{
    int gene_num1;
    int gene_num2;
    double corr;

}gene_corr;
int clustermake(gene_corr *gene_corr_aray,char name[y][100]){
    FILE *out;
    int c1,c2, **cluster;
    cluster=(int **)malloc(sizeof(int *)*y);
    for (int i=0;i<y;i++){
        cluster[i]=(int*)malloc(sizeof(int)*y);
        for (int j = 0; j <y ; ++j) {
            if(i==j){
                cluster[i][j]=1;
            } else{
                cluster[i][j]=0;
            }
        }
    }
    for(int i=0; i<y*y-1;i++){
        if(gene_corr_aray[i].corr<corr_threshold){
            break;
        }else{
            c1=c2=0;
            while (cluster[c1][gene_corr_aray[i].gene_num1]!=1)c1++;
            while(cluster[c2][gene_corr_aray[i].gene_num2]!=1)c2++;
            if(c1 !=c2){
                for(int j=0;j<y;j++){
                    cluster[c2][j]=cluster[c2][j]+cluster[c1][j];
                    cluster[c1][j]=0;
                }
            }
        }

    }
    if((out=fopen("kurasutadata.txt","w"))==NULL){
        printf("cant open infile");
        exit(1);
    }
    for (int k = 0; k <y ; ++k) {
        int flag=0;
        for (int i = 0; i <y ; ++i) {
            if(cluster[k][i]==1){
                fprintf(out,"%s ",name[i]);
                flag=1;
            }
        }
        if(flag==1) {
           fprintf(out,"\n");
        }
        flag=0;

    }
    return 0;
}
int main(void){
    FILE *in;
    int i,j;
    if((in=fopen("gene_expression.txt","r"))==NULL){
        printf("cant open infile");
        exit(1);
    }
    for (i=0;i<y;i++){
        fscanf(in,"%s",name[i]);
        for(int k=0;k<18;k++){
            fscanf(in,"%lf",&a);
        }
    }
    if((in=fopen("cluster_gene.txt","r"))==NULL){
        printf("cant open infile");
        exit(1);

    }
    gene_corr *gene_corr_array;
    gene_corr_array = (gene_corr *) malloc(sizeof(gene_corr) *2732) ;
    for(i=0;i<2732;i++){
                fscanf(in,"%d %d %lf\n",&gene_corr_array[i].gene_num1,&gene_corr_array[i].gene_num2,&gene_corr_array[i].corr);
    }
    clustermake(gene_corr_array, name);

}