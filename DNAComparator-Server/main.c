#include <stdio.h>
#include <omp.h>

// funcion para leer archivo y partir las lineas

// funcion para evaluar archivo contra el archivo con el valor de ADN

//funcion para desplegar la respuesta
/* formato de la respuesta 
GCCTCCTGCTGCTGCTGCTCTCC a partir del caracter 21
GGACCTCCCAGGCCAGTGCCGGG a partir del caracter x
AAGACCTTCTCCTCCTGCAAATA a partir del caracter x
TTCTTCTGGAAGACCTTCTCCTC a partir del caracter x
CCAGGCGGCAGGAAGGCGCACCCCCCCAGCAATCCGTGCGCCGG no se encontro

El archivo cubre el 15% del genoma de referencia
X secuencias mapeadas
X secuencias no mapeadas
*/
//funcion main
int main() {

    #pragma omp parallel
    {
        int i;
        // aqui va el multithreding para evaluar todas las diferentes lineas del archivo a leer
        #pragma omp parallel for 
            for(i = 0; i<12; i++)
             printf("Esta en un for paralelo");
              
    }

}