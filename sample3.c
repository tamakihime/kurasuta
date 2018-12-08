void quick(gene_corr *gene_corr_array,int start,int end){
    int pl=start;
    int pr=end;
    double center=gene_corr_array[(pl+pr)/2].corr;
    gene_corr t;

    do{
        while(gene_corr_array[pl].corr>center)pl++;
        while(gene_corr_array[pr].corr<center)pr--;
        if(pl<=pr){
            t=gene_corr_array[pl];
            gene_corr_array[pl]=gene_corr_array[pr];
            gene_corr_array[pr]=t;
            pl++;
            pr--;
        }
    }while(pl<=pr);
    if(start<pr)quick(gene_corr_array,start,pr);
    if(pl<end)quick(gene_corr_array,pl,end);