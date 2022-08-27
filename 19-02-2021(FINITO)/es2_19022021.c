/*Esercizio 2
Una matrice sparsa è una matrice in cui la maggior parte degli elementi sono zero, ovverosia una
matrice è sparsa se almeno (m * n) / 2 elementi sono zero.
Scrivere una funzione C che accetti in input una matrice di dimensione arbitraria e che restituisca
true se questa matrice è sparsa.
*/


# include <stdio.h>



int sparsa(size_t rows , size_t cols, int matrix[rows][cols]){

    int flag =0 ;
    size_t k = (cols*rows)/2 ; // se e piu grande di questo valore allora e sparsa altrimenti no

    for(int i = 0  ; i<rows ; i++){
        for(int j = 0  ; j<cols ; j++){
            if(matrix[i][j] == 0 )
                    flag++;
        }
    }
    if(k<flag)
        return 1 ;
    else
        return 0 ;
}

int main(void){

    int matrix[4][4]= {{2,0,0,7},{2,0,0,0},{0,4,0,6},{2,0,0,7}};
    size_t cols = 4 ;
    size_t rows = 4 ;


    if(sparsa(rows , cols , matrix)){
        printf("la matrice e sparsa\n");
        return 0 ;
    }
    printf("la matrice non e sparsa ");
}
