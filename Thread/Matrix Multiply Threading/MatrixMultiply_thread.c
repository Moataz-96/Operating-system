#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <ctype.h>
#include <string.h>
int getSchemaMatrix(FILE *inputFile);
int** getMatrix(FILE *inputFile,int row1,int col1);
void MultiplyMatrix(void *arg);
void MultiplyMatrixByRows(void *arg);
struct Matrix
{
    int length;
    int *row;
    int *col;
    int value;
};
struct MatrixByRows
{
    int length;
    int *row;
    int **col;
    int *value;
    int numOfCols;
};
int main()
{
    FILE *inputFile = fopen("input.txt","r");
    char outputBuffer[255];
    if (inputFile == NULL)
        return 0;
    int **mat1, **mat2,**res,i,j,r1,c1,r2,c2;
    r1 = getSchemaMatrix(inputFile);
    c1 = getSchemaMatrix(inputFile);
    mat1 = getMatrix(inputFile,r1,c1);
    r2 = getSchemaMatrix(inputFile);
    c2 = getSchemaMatrix(inputFile);
    mat2 = getMatrix(inputFile,r2,c2);
    fclose(inputFile);
    if(c1!=r2)
    {
        printf("Error Matrix cannot be multiplied\n");
        exit(EXIT_SUCCESS);
    }else{
        res=(int**)calloc(r1,sizeof(int*));
        for(i=0; i<c2; i++)
            res[i]=(int*)calloc(c2,sizeof(int));
    }
    int length_of_output = r1 * c2;
    int length_of_mul = c1;
    int length_of_rows = r1;

    struct Matrix *element = malloc(length_of_output*sizeof(struct Matrix));

    //for(int k = 0 ; k < length ; k++)
    //  element[k] = malloc(sizeof(struct Matrix));

    //struct Matrix element[12];
    for(int k = 0 ; k < length_of_output ; k++)
    {
        element[k].row = (int*)malloc(length_of_mul*sizeof(int));
        element[k].col = (int*)malloc(length_of_mul*sizeof(int));
        element[k].length = length_of_mul;
    }
    int count = 0;
    for(int rows = 0 ; rows < r1 ; rows++)
    {
        for(int cols = 0 ; cols < c2 ; cols++)
        {
            for(int i = 0 ; i < length_of_mul ; i++)
            {
                element[count].row[i] = mat1[rows][i];
                element[count].col[i] = mat2[i][cols];
            }
            count++;
        }
    }


//    int sum = 0;
//    for(int i = 0 ; i < 5 ; i ++)
//        sum += element.row[i] * element.col[i];

//    printf("\nrows\n");
//    for(int k = 0 ; k < 12; k ++ ){
//        printf("\nrows\n");
//        for(int i = 0 ; i < 5 ; i ++){
//            printf("%d \t",element[k].row[i]);
//        }
//        printf("\ncols\n");
//        for(int i = 0 ; i < 5 ; i ++){
//            printf("%d \t",element[k].col[i]);
//        }
//    }


    // Start Elapsed Time
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    /* stuff to do! */

    // With Out Threading
    /*
    for(int i = 0 ; i < length_of_output ; i ++){
      MultiplyMatrix(&element[i]);
      //printf("value = %d\n",element[i].value);
    }
    */
    // With Threading

    pthread_t pid;

    for(int i = 0 ; i < length_of_output ; i++)
    {
        pthread_create(&pid,NULL,MultiplyMatrix,&element[i]);
    }
    for(int i = 0 ; i < length_of_output ; i++)
    {
        pthread_join(pid,NULL);
    }

    //OUTPUT result into output.file

    FILE *outputFile = fopen("output.txt","w");

    for(int i = 0 ; i < length_of_output ; i ++)
    {
        char value[30];
        sprintf(value,"%d",element[i].value);
        strcat(outputBuffer,value);
        strcat(outputBuffer,"  ");
       // printf("value = %d\n",element[i].value);
        if(((i+1)%c2) == 0){
            strcat(outputBuffer,"\n");
        }
    }
    strcat(outputBuffer,"\nEND1 ");



    //get Elapsed Time
    //Compute the Execution Time



    gettimeofday(&tv2, NULL);
    double time1_value = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);
    printf ("Total time of each element thread = %f seconds\n",time1_value);


    char time1[30];

    sprintf(time1,"%f",time1_value);
    strcat(outputBuffer,time1);
    strcat(outputBuffer,"\n\n\n");

    //--------------------------------------

    //Start Second Algorithm

    struct MatrixByRows *rows = malloc(length_of_rows*sizeof(struct MatrixByRows));

    //for(int k = 0 ; k < length ; k++)
    //  element[k] = malloc(sizeof(struct Matrix));

    //struct Matrix element[12];
    for(int k = 0 ; k < length_of_rows ; k++)
    {
        rows[k].row = (int*)malloc(length_of_mul*sizeof(int));
        rows[k].value = (int*)malloc(c2*sizeof(int));
        rows[k].col = (int**) malloc(length_of_mul*sizeof(int*));
        for(i=0; i<length_of_mul; i++)
            rows[k].col[i]=(int*)malloc(c2*sizeof(int));
        rows[k].length = length_of_mul;
        rows[k].numOfCols = c2;
    }
    count = 0;
    for(int numOf0rows = 0 ; numOf0rows < length_of_rows ; numOf0rows++)
    {
        for(int numOfcols = 0 ; numOfcols < c2 ; numOfcols++)
        {
            for(int i = 0 ; i < length_of_mul ; i++)
            {
                rows[count].row[i] = mat1[numOf0rows][i];
                rows[count].col[i][numOfcols] = mat2[i][numOfcols];
            }
        }
        count++;
    }



//    int sum = 0;
//    for(int i = 0 ; i < 5 ; i ++)
//        sum += element.row[i] * element.col[i];

    /* printf("\nrows\n");
    for(int k = 0 ; k < length_of_rows; k ++ )
    {
        printf("\nrows\n");
        for(int i = 0 ; i < length_of_mul ; i ++)
        {
            // printf("%d \t",rows[k].row[i]);
        }
        printf("\ncols\n");
        for(int i = 0 ; i < length_of_mul; i++)
            for(int j = 0 ; j < c2 ; j ++)
            {
                //   printf("%d \t",rows[k].col[i][j]);
            }

    }*/


    // Start Elapsed Time
    struct timeval  tv3, tv4;
    gettimeofday(&tv3, NULL);
    /* stuff to do! */

    // With Out Threading
    /*
    for(int i = 0 ; i < length_of_rows ; i ++){
      MultiplyMatrixByRows(&rows[i]);
      for(int j = 0 ; j < rows[i].numOfCols ; j++)
          printf("%d\t",rows[i].value[j]);
      printf("\n");
    }
    */
    // With Threading

    pthread_t pid_rows;

    for(int i = 0 ; i < length_of_rows ; i++)
    {
        pthread_create(&pid_rows,NULL,MultiplyMatrixByRows,&rows[i]);
    }
    for(int i = 0 ; i < length_of_rows ; i++)
    {
        pthread_join(pid_rows,NULL);
    }

    for(int i = 0 ; i < length_of_rows ; i ++)
    {
        for(int j = 0 ; j < c2 ; j ++){
          //  printf("value = %d\n",rows[i].value[j]);
            char value[30];
            sprintf(value,"%d",rows[i].value[j]);
            strcat(outputBuffer,value);
            strcat(outputBuffer," ");

        }
        strcat(outputBuffer,"\n");
    }
    strcat(outputBuffer,"\nEND2 ");
    //get Elapsed Time
    //Compute the Execution Time




    gettimeofday(&tv4, NULL);
    double time2_value = (double) (tv4.tv_usec - tv3.tv_usec) / 1000000 +
            (double) (tv4.tv_sec - tv3.tv_sec);
    printf ("Total time of each row thread = %f seconds\n",time2_value);

    char time2[30];

    sprintf(time2,"%f",time2_value);
    strcat(outputBuffer,time2);
    strcat(outputBuffer,"\n\n\n");

    //output to file then close it
    fprintf(outputFile,outputBuffer);
    fclose(outputFile);

    //--------------------------------------
    return 0;
}
int getSchemaMatrix(FILE *inputFile)
{
    char c;
    int ret;
    while(!feof(inputFile))
    {
        c = fgetc(inputFile);
        if(!isspace(c))
        {
            ret = c - '0';
            break;
        }
    }
    return ret;
}
int** getMatrix(FILE *inputFile,int numOfrows,int numOfcols){
    char c;
    int i,j,value;
    int **matrix;
    matrix= (int**) malloc(numOfrows*sizeof(int*));
    for(i=0; i<numOfrows; i++)
        matrix[i]=(int*)malloc(numOfrows*sizeof(int));

    for(i = 0 ; i < numOfrows ; i ++)
    {
        for(j = 0 ; j < numOfcols ; j ++)
        {
            while(!feof(inputFile))
            {
                c = fgetc(inputFile);
                if(!isspace(c)){
                    if(c == '-'){
                        c = fgetc(inputFile);
                        matrix[i][j] = ((c-'0')-2*(c-'0'));
                        //printf("%d\t",matrix[i][j]);
                    }else{
                      //  printf("%d\t",(c-'0'));
                        matrix[i][j] = (c-'0');
                    }
                    break;
                }
            }
        }
        //printf("\n");
    }
    return matrix;
}
void MultiplyMatrix(void *arg)
{

    struct Matrix *element = (struct Matrix*)arg;

    int sum = 0;
    for(int i = 0 ; i < element->length ; i ++)
        sum += element->row[i] * element->col[i];
    element->value = sum;
    pthread_exit(0);
}
void MultiplyMatrixByRows(void *arg)
{
    struct MatrixByRows *element = (struct MatrixByRows*)arg;
    int sum = 0;
    for(int numOfCols = 0 ; numOfCols < element->numOfCols ; numOfCols++)
    {
        sum = 0;
        for(int i = 0 ; i < element->length ; i ++)
        {
            sum += element->row[i] * element->col[i][numOfCols];
        }
        element->value[numOfCols] = sum;
    }
    pthread_exit(0);
}
