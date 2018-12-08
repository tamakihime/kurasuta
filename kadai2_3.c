#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  n 18
#define y 6019
#define corr_threshold 0.9
char name[y][100];
double corr_single(double a[],double b[]);
void insert_gene_corr_structure_array(double **data);

typedef struct GENE_CORRELATION{
    int gene_num1;
    int gene_num2;
    double corr;

}gene_corr;

double corr_single(double a[],double b[]){
    int i;
    double mean_a,mean_b;
    double var_a,var_b;
    double corr;

    for (mean_a=0,i=0;i<n;i++) mean_a+=a[i]/n;
    for (mean_b=0,i=0;i<n;i++) mean_b+=b[i]/n;
    for (var_a=0,i=0;i<n;i++)var_a+=(a[i]-mean_a)*(a[i]-mean_a)/n;
    for (var_b=0,i=0;i<n;i++)var_b+=(b[i]-mean_b)*(b[i]-mean_b)/n;
    for(corr=0,i=0;i<n;i++)corr+=(a[i]-mean_a)/sqrt(var_a)*(b[i]-mean_b)/sqrt(var_b)/n;
    return corr;
}
//kadai3↓
void quick(gene_corr *gene_corr_array,int start,int end) {
    int pl = start;
    int pr = end;
    double center = gene_corr_array[(pl + pr) / 2].corr;
    gene_corr t;

    do {
        while (gene_corr_array[pl].corr > center)pl++;
        while (gene_corr_array[pr].corr < center)pr--;
        if (pl <= pr) {
            t = gene_corr_array[pl];
            gene_corr_array[pl] = gene_corr_array[pr];
            gene_corr_array[pr] = t;
            pl++;
            pr--;
        }
    } while (pl <= pr);
    if (start < pr)quick(gene_corr_array, start, pr);
    if (pl < end)quick(gene_corr_array, pl, end);
}
//kadai3↑
void insert_gene_corr_structure_array(double **data) {
    FILE *out;
    int i, j;
    gene_corr *gene_corr_array;
    double correlation;


    if ((out = fopen("cluster_gene.txt", "w")) == NULL) {
        printf("cant be open outfile\n");
        exit(1);
    }


    gene_corr_array = (gene_corr *) malloc(sizeof(gene_corr) * (y * y));

    for (i = 0; i < y; i++) {
        for (j=0;j<y;j++){
            if(i<j) {
                correlation = corr_single(data[i], data[j]);
                if (corr_threshold< correlation){
                    gene_corr_array[i*y+j].corr=correlation;
                    gene_corr_array[i*y+j].gene_num1=i;
                    gene_corr_array[i*y+j].gene_num2=j;
                }
                else{
                    gene_corr_array[i*y+j].corr=-5;
                }
            } else{
                gene_corr_array[i*y+j].corr=-5;
            }
        }

    }
    quick(gene_corr_array,0,6019*6019-1); //kadai3
    for(i=0;i<y;i++){
        for(j=0;j<y;j++){
            if(gene_corr_array[i*y+j].corr!=-5){
                fprintf(out,"%d %d %lf\n",gene_corr_array[i*y+j].gene_num1,gene_corr_array[i*y+j].gene_num2,gene_corr_array[i*y+j].corr);
            }
        }
    }
    free(gene_corr_array);
}

int main(void){
    FILE *in;
    int i,j;
    double **data;
    if((in=fopen("gene_expression.txt","r"))==NULL){
        printf("cant open infile");
        exit(1);
    }
    data=(double **)malloc(sizeof(double *)*y);
    for (i=0;i<y;i++){
        data[i]=(double *)malloc(sizeof(double)*n);
        fscanf(in,"%s",name[i]);
        for(j=0;j<n;j++){
            fscanf(in,"%lf",&data[i][j]);
        }
    }
    insert_gene_corr_structure_array(data);
    free(data);
}