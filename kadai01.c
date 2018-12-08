# include<stdio.h>
#include <stdlib.h>


#define n 18
#define y 6178

char name[y][100];
void print_gene_expresion(char name[y][100], double**data){
    FILE *out;
    int i,j;
    int count=0;
    if((out=fopen("gene_expression.txt","w"))==NULL){
        printf("cant open outfile");
        exit(1);
    }
    for(i=0;i<y;i++){
        count=0;
        for (j = 0; j <n ; j++) {
            if(data[i][j]==1000){
                count+=1;
            }

        }
        if(count<n*0.2){
            fprintf(out,"%s\t",name[i]);
                double write [18]={};
                write[0]=data[i][0];
                for(int k=0;k<4;k++) {   //先頭の１０００を取り除く
                    if (write[0] == 1000) {
                        for (int l = 0; l < k + 1; l++) {
                            write[l] = data[i][k];
                        }

                    }
                }
                write[17]=data[i][17];
                for(int k=17;k>13;k--){  //最後の１０００を取り除く
                    if ( write[17]== 1000) {
                        for(int l=17;l>k;l--){
                            write[l]=data[i][k];
                        }

                    }
                }
                for(int k=0;k<n;k++){
                    if(write[k]==0){
                        double left=0;
                        double right=0;
                        if(data[i][k]==1000){
                            for(int t=k-1;t>-1;t--){
                                if(data[i][t]!=1000){
                                    left=data[i][t];
                                    break;
                                }
                            }
                            for(int t=k+1;t<18;t++){
                                if(data[i][t]!=1000){
                                    right=data[i][t];
                                    break;
                                }
                            }
                            write[k]=(right+left)/2;
                        }
                        else{
                            write[k]=data[i][k];
                        }

                    }

                }
                for(int k=0;k<n;k++) {
                    fprintf(out, "%lf\t", write[k]);
                }
            fprintf(out,"\n");
        }
    }
}
int main(void){
    FILE *in;
    int i,j;
    double **data;
    if((in=fopen("yeast_microarray_data.txt","r"))==NULL){
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
    print_gene_expresion(name,data);
    free(data);
    return 0;
}
