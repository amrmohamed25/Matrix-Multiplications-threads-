#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>



int rows,columns,columns2;           //global variables used in many functions
int **matrix1; //input Matrix 1
int **matrix2; //input Matrix 2
int **resultMatrix1; //Output Matrix 1 from Element Method
int **resultMatrix2; //Output Matrix 2 from Row Method

typedef struct
{
    int rowIndex,columnIndex;
} Element;//Struct used for passing data to thread functions



void* multiplyElementsElementMethod(void *arg)
{
    Element* element = (Element *)arg;//getting indexes from the struct
    for(int i=0; i<columns; i++)
    {
        resultMatrix1[element->rowIndex][element->columnIndex] += matrix1[element->rowIndex][i]*matrix2[i][element->columnIndex]; //calculating element data
    }
}

void* multiplyElementsRowMethod(void *arg)
{
    int rowIndex = *((int *) arg);//getting row Index value
    for(int j=0; j<columns2; j++)//Multiplying row by all columns and getting the data in result
    {
        for(int k=0; k<columns; k++)
        {
            resultMatrix2[rowIndex][j] += matrix1[rowIndex][k]*matrix2[k][j];//calculating row
        }
    }
}

int getInput(char *token)
{
    char *numbers=malloc(strlen(token));//allocating data for the string
    strcpy(numbers,token);//copying it
    int temp=atoi(numbers);//converting it to int
    free(numbers);//freeing using string
    return temp;//returning data
}

int checkLine(char *token)//checking if line is empty or not
{
    if(token==NULL)
    {
        printf("Incorrect file format\n");
        return 0;
    }
    return 1;
}

void writeInFile(char str[])
{
    FILE *pFile;
    pFile=fopen("output.txt", "w");//append in the given location
    if(pFile!=NULL)
    {
        fprintf(pFile, "%s", str);//.19s is used in printing to remove new line /n
    }
    fclose(pFile);
}

int** allocateMatrix(int r, int col)
{
    int** arr=(int **)malloc(r*sizeof(int *));
    for (int i = 0; i < r; i++)
        arr[i] = (int*)malloc(col * sizeof(int));
    return arr;
}

void freeMatrix(int **arr,int r,int col)
{
    for (int i = 0; i < r; i++)
        free(arr[i]);

    free(arr);
}

void initializeMatrix(int **arr,int r,int col)
{
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<col; j++)
        {
            arr[i][j]=0;
        }
    }
}

int scanInput(char *filename)
{
    int scans=0;//a counter to indicate whether we scanned the data of the first matrix or second matrix
//    char line[2000];
    char *line=NULL;
    size_t line_size=0;
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File not found\n");
        return 0;
    }
    while(scans!=2)//if scans==2 then both data of first matrix and second matrix are scanned
    {
//        fgets(line,2000,fp);//scaning line
        getline(&line,&line_size,fp);
        char *token=strtok(line," \n");//removing spaces and new lines to scan row of first or second matrix
        if(checkLine(token)==0) return 0;//checking if the line is empty or not
        if(scans==0)//if scans ==0 we are scanning rows of the first matrix
        {
            rows=getInput(token);//get number of rows of the first matrix
            if(rows<=0)//checking if user entered correct input
            {
                printf("Dimensions should be positive integer\n");
                fclose(fp);
                writeInFile("Dimensions should be positive integer\n");
                return 0;
            }

        }
        else//we are scanning rows of the second matrix
        {
            int temp=getInput(token);//checking for inner dimensions rule
            if(temp!=columns)
            {
                printf("Inner dimensions should match\n");
                fclose(fp);
                writeInFile("Inner dimensions should match\n");
                return 0;
            }
        }
        token=strtok(NULL," \n");//tokenize to scan columns of first or second matrix
        if(checkLine(token)==0) return 0;//checking if line is empty
        if(scans==0)//scanning columns of first matrix
        {
            columns=getInput(token);
            if(columns<=0)
            {
                printf("Dimensions should be positive integer\n");
                fclose(fp);
                writeInFile("Dimensions should be positive integer\n");
                return 0;
            }
            matrix1=allocateMatrix(rows,columns);
        }
        else//scanning columns of second matrix
        {
            columns2=getInput(token);
            if(columns2<=0)
            {
                printf("Dimensions should be positive integer\n");
                fclose(fp);
                writeInFile("Dimensions should be positive integer\n");
                free(matrix1);
                return 0;
            }
            matrix2=allocateMatrix(columns,columns2);

        }
        int lines;//lines to ensure user entered correct number of rows
        if(scans==0)
            lines=rows;//lines = rows of first matrix
        else
            lines=columns;//line = columns of first matrix = rows of second matrix
        int i=0,j=0;
        while (getline(&line,&line_size,fp)!=-1&& lines--)//scanning till lines are done or elements are scanned
        {
            token=strtok(line," \n");//removing spaces and new lines
            while(token)
            {
                if(scans==0)//scanning first matrix
                    matrix1[i][j++]=getInput(token);
                else//scanning second matrix
                    matrix2[i][j++]=getInput(token);
                token=strtok(NULL," \n");
                if((j==columns&&scans==0)||(j==columns2&&scans==1))//if user got to columns then go to next row
                {
                    i++;
                    j=0;
                }
            }
            if((j<columns && j!=0&&scans==0)||(j<columns2&&j!=0&&scans==1))//checking if number of elements in the row are less than number of columns
            {
                printf("Incorrect file format\n");
                fclose(fp);
                free(matrix1);
                free(matrix2);
                writeInFile("Input should be positive integer\n");
                return 0;
            }
            if((i==rows&&scans==0)||(i==columns&&scans==1))//if user reached number of rows break
                break;
        }
        if((i!=rows&&scans==0)||(i!=columns&&scans==1))//if the scanned rows are less than expected incorrect inputs
        {
            printf("Incorrect file format\n");
            fclose(fp);
            free(matrix1);
            free(matrix2);
            writeInFile("Input should be positive integer\n");
            return 0;
        }
        scans++;

    }
    fclose(fp);
    free(line);
    return 1;
}


int main(int argc,char **argv)
{
    //char filename[200];//will scan the file in it
    //printf("Please enter name of the file:\n");
    //fgets(filename,200,stdin);
    //strtok(filename," \n");//removing new lines and spaces
    if(scanInput(argv[1])==0) //scanning input
        return 0;
    resultMatrix1=allocateMatrix(rows,columns2);
    initializeMatrix(resultMatrix1,rows,columns2);
    resultMatrix2=allocateMatrix(rows,columns2);
    initializeMatrix(resultMatrix2,rows,columns2);
    pthread_t newThreads1[rows*columns2];//creating array of threads to be able to make each multiplication in a thread and to be able to join them to get the correct result
    int threadNum = 0;
    Element threadData1[rows*columns2];//creating array of struct that has the indexes that will be passed to the thread functions so that data is saved
    clock_t t1;
    t1 = clock();//starting timer
    for(int i=0; i<rows; i++)//two loops are used so that element * element is done inside the thread
    {
        for(int j=0; j<columns2; j++)
        {
            threadData1[threadNum].rowIndex=i;//passing row index
            threadData1[threadNum].columnIndex=j;//passing column index
            pthread_create(&newThreads1[threadNum], NULL, multiplyElementsElementMethod, (void *) &threadData1[threadNum]);
            threadNum++;
        }
    }
    for(int i=0; i<threadNum; i++)
        pthread_join(newThreads1[i],NULL );//joining threads to get correct results since each thread did a small work
    t1 = clock() - t1;//stopping time


    pthread_t newThreads2[rows];//new threads to be able to make each multiplication in a thread and to be able to join them to get the correct result
    int threadData2[rows];//array of row indexes that will be used in thread function
    threadNum=0;
    clock_t t2 = clock();//starting time
    for(int i=0; i<rows; i++)
    {
        threadData2[threadNum]=i;//passing row index
        pthread_create(&newThreads2[threadNum], NULL, multiplyElementsRowMethod, (void *) &threadData2[threadNum]);//creating a thread
        threadNum++;
    }
    for(int i=0; i<threadNum; i++)
        pthread_join(newThreads2[i],NULL );//joining threads to get correct results since each thread did a small work
    t2 = clock() - t2;//stopping time

    FILE *pFile;
    pFile=fopen("output.txt", "w");
    if(pFile!=NULL)
    {
        fprintf(pFile,"By Row\n");
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<columns2; j++)
            {
                fprintf(pFile,"%d	",resultMatrix2[i][j]);
            }
            fprintf(pFile,"\n");
        }
        fprintf(pFile,"END2 Elapsed Time:%lf\n\n",((double)t2)/CLOCKS_PER_SEC);
        fprintf(pFile,"By Element\n");
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<columns2; j++)
            {
                fprintf(pFile,"%d	",resultMatrix1[i][j]);
            }
            fprintf(pFile,"\n");
        }
        fprintf(pFile,"END1 Elapsed Time:%lf",((double)t1)/CLOCKS_PER_SEC);
        fprintf(pFile,"\n\n");

    }
    fclose(pFile);
    freeMatrix(matrix1,rows,columns);
    freeMatrix(matrix2,columns,columns2);
    freeMatrix(resultMatrix1,rows,columns2);
    freeMatrix(resultMatrix2,rows,columns2);

    return 0;
}
