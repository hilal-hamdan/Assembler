#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SUFFIX_SIZE 4
#define MAX_LINE_LENGTH 80
#define MAX_FILENAME_LENGTH 256

#define MAXLINELETTERS 80
#define BINARYSIZE 10
#define DECIMALLADDRESS 100
#define MEMORYSIZE 10
#define MAXLABELLENGTH 20
#define OPERATIONSSIZE 16
#define OPERATIONSLENGTH 4
#define GUIDANCESIZE 3
#define GUIDANCLENGTH 8
#define SPACEASCIICODE 32
#define TAPASCIICODE 9
#define SUFFIX_SIZE 4
#define MAX_LINE_LENGTH 80
#define MAX_FILENAME_LENGTH 256

typedef enum {false,true} boolean;

typedef enum {SourceOperandImmediate , SourceOperandDirect , SourceOperandAccessToRecord , SourceOperandDirectRegister ,DestinationOperandImmediate , DestinationOperandDirect , DestinationOperandAccessToRecord , DestinationOperandDirectRegister , DirectTwoRegister} AddressingMethod;

char operations[OPERATIONSSIZE][OPERATIONSLENGTH] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"};
char guidance[GUIDANCESIZE][GUIDANCLENGTH] = {".data",".string",".struct"};
char registers[GUIDANCLENGTH][GUIDANCESIZE] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char kedodKind[2][10]={".entry",".extern"};
char skip[]={" \t"};
char base32Sympols[33]={'!','@','#','$','%','^','&','*','<','>','a'
            ,'b','c','d','e','f','g','h','i','j','k','l','m','n'
            ,'o','p','q','r','s','t','u','v',};

int currentLineNum =0;
boolean error = false;
struct LabelNode* labelsHead = NULL;/*Head of the memory labels data struct*/
struct LabelNode* labelsTail = NULL;/*Tail of the memory labels data struct*/
struct MemoryNode* memoryHead = NULL;/*Head of the memory data struct*/
struct MemoryNode* memoryTail = NULL;/*Tail of the memory data struct*/
struct LineNode* linesHead = NULL;/*Head of the lines to read from text data struct*/
struct LineNode* LinesTail = NULL;/*Tail of the lines to read from text data struct*/
struct DataLabelNode* DataLabelNodeHead = NULL;/*Head of the entry/extren data struct*/
struct DataLabelNode* DataLabelNodeTail = NULL;/*Tail of the entry/extren data struct*/

typedef struct macro_node
{
    char name[MAX_LINE_LENGTH];
    char *content;
    struct macro_node *next;
}macro_node;
macro_node *find_name_macro_node(char *macro_name, macro_node *macro_table);
macro_node *create_macro_node();
void insert_macro_node(macro_node **macro_table, char *macro_name, char *content);
FILE *pre_assembler(char *file_name, int namelen);
void free_macro_table(macro_node *macro_table);

/*data struct to read from text*/
typedef struct LineNode
{
    char * line;
    struct LineNode * next;
    
}LineNode;
/*data struct for memory labels*/
typedef struct LabelNode
{   
    char * labelName;
    int decimalAddress;
    struct LabelNode * next;
}LabelNode;
/*data struct for the memory*/
typedef struct MemoryNode
{
    int decimalAddress;
    int * binaryMachineCode ;
    char * firstOpLabel;
    char * secondOpLabel;
    char * label;
    struct MemoryNode * next;
}MemoryNode;
/*data struct for entry/extern labels*/
typedef struct DataLabelNode
{   
    char * labelName;
    int Address; /* 0:entry or 1:extern*/
    struct DataLabelNode * next;
}DataLabelNode;

/*DataLabelNode functions*/
/*this funtion frees the datalabel data struct*/
void freeDataLabel();
/*this funtion displays the datalabel data struct*/
void displayDataLabel();
/*end of DataLabelNode functions*/

/*LineNode functions*/
void insertNewLine(char *line);/*this funtion creats and inserts new line node to lines struct*/
void displayLines();/*this funtion displays the lines data struct*/
void freeLines();/*this funtion free the lines data struct*/
/*end LineNode functions*/

/*LabelNode functions*/
/*creats and add new label node to labels data struct */
void addLabel(char *label);
/*displays the labels data struct*/
void displayLabels();
/*free the labels data struct*/
void freeLabels();
/*this function returns true if label node exists with label equal to given label, false otherwise*/
boolean isLabelExist(char * label);
/*end LabelNode functions*/

/*MemoryNode functions*/
void freeMemory();/*free the memory data struct*/
void displayMemory();/*displays the memory data struct*/
MemoryNode * createNewMemory();/*creats new memory node and returns it*/
MemoryNode * createNewUnknownMemory();/*creats new uknow memory address node and return it*/
void addToMemory(MemoryNode * tmp);/*add memory node tmp to memory data struct*/
/*end MemoryNode functions*/

/*coding functions*/
/*this function handles the first coding algorithm part*/
void firstCoding();
/*this function handles the second coding algorithm part*/
void secondCoding();
/*end ofcoding functions*/

/*handle data lines functions*/
/*this funtion checks which kind of data line to handle*/
void handleData(int index, char *data, char * label);
/*this function handle integer data lines*/
void handleIntData(int index ,char *data, char * label);
/*this funtion handle string data lines*/
void handleSTRData(int index ,char *data, char * label);
/*this function handle struct data lines*/
void handleStructData(int index ,char *data, char * label);
/*end of handle data lines functions*/

/*handle operation lines functions*/
/*this functions check which operation to handle and call helper functions*/
void handlOperation(int index , char *data, char * label);
/*this function handle two operation lines*/
void handleTwoOperations(int index ,char * firstOperation ,char * secondOperation, char * label);
/*this function handle one operaion lines*/
void handleOneOperations(int index ,char * firstOperation, char * label);
/*this funtion handle zero operation lines*/
void handleZeroOperations(int index, char * label);
/*end of handle operation lines functions*/

/*this funtion covert inter number between 1 and 15 for operation coding*/
int * covertDecimalToBinary(int num);
/*this functions covert intger number between -128 and 128 for immediate coding*/
int * covertDecimalToBinary1(int num);
/*this function covert intger number for data coding (asci for strings) for guidance codin */
int * covertDataToBinary(int num);
/*end of covert from base x to base y functions*/

/*this functions reads lines from file f and writes into line node strcut*/
void readLinesFromFile(FILE *f);

/*this function return true if string p has name of register*/
boolean isRegister(char *p);

/*this funtion checks and updates labels decimal addresses*/
void checkLabel();

/*this funtions address memory node by addressingmethod*/
void Addressing(AddressingMethod addressingMethod, MemoryNode* tmp);

/*this function returns a copy of string without spaces and tabs and new lines*/
char * cleanWords(char *w);

/*this function handle entry/extern line by creating and adding new node to data label struct*/
void handleCodingData(char * codingWord);

/*checks and return true if line is empty or a comment line, false otherwise*/
 boolean EmptyCommentChecker(char* line);
 
 /*this function return true if given line is coding word line (entry/extern) included*/
boolean isCodingWord(char * line);

/*checks and return true if line contains label (start with label), false otherwise*/
boolean isLabeled(char * line);

/*this funtion add data label node to data label struct*/
void addDataLabelNode(DataLabelNode * tmp);

/*this function creats new data label node and return it, return null if fail*/
DataLabelNode * createNewDataLabelNode();

/*this funtion checks if word is name of operation and return it's index, otherwise return -1*/
int isOperation(char * word);

/*this funtion checks if word is name of guidance and return it's index, otherwise return -1*/
int isData(char * word);

/*this functios returns true if given word is equal to any system word*/
boolean systemWord(char * word);

/*this funtions write the binary code for extern labels*/
void handleExtern(MemoryNode * tmpMemory);
/*this funtion displays the final base 32 address and binary machine code to .as*/
void displayOutput();
/*this function convert from binary to decimal*/
int toDeci(char *str, int base);
/*helper function, to check val of char c*/
int val(char c);
/*helper functions, to swap string after coverting to base 32*/
char * swap(char *arr);


typedef struct macro_node
{
    char name[MAX_LINE_LENGTH];
    char *content;
    struct macro_node *next;
}macro_node;

macro_node *find_name_macro_node(char *macro_name, macro_node *macro_table);
macro_node *create_macro_node();
void insert_macro_node(macro_node **macro_table, char *macro_name, char *content);
FILE *pre_assembler(char *file_name, int namelen);
void free_macro_table(macro_node *macro_table);

int main(int argc, char *argv[])
{
    int i, namelen;
    FILE *out;
    for(i = 1; i < argc; i++)
    {
        namelen = strlen(argv[i]) + SUFFIX_SIZE;

        out = pre_assembler(argv[i], namelen);
        if(out == NULL)
        {
            printf("Error! pre_assembler failed with file %s",argv[i]);
            continue;
        }
    }
    return 0;
}

FILE *pre_assembler(char *file_name, int namelen)
{
    FILE *out, *in;
    char out_name[MAX_FILENAME_LENGTH];
    char in_name [MAX_FILENAME_LENGTH];
    char line[MAX_LINE_LENGTH + 1];
    char macro_name[MAX_LINE_LENGTH];
    char *content;
    unsigned int line_cnt = 0, content_size = 1;
    bool macro_def = false;
    
    
    macro_node *macro_table = NULL;

    
    strcpy(in_name, file_name);
    strcpy(out_name, file_name);
    
    in  = fopen(strcat(in_name, ".as"), "r"); 
    out = fopen(strcat(out_name, ".am"), "w");
    
    if(in == NULL)
    {
        printf("Cannot open file %s!", in_name);
        return NULL;
    }
    if(out == NULL)
    {
        printf("Cannot open file %s!", out_name);
        return NULL;
    }
    
    while(fgets(line, MAX_LINE_LENGTH, in) )
    {
        char *first_word, line_copy[MAX_LINE_LENGTH + 1];
        macro_node *curr = NULL;
        const char delim[] = " \t\n"; 
        
        line_cnt++;
        strcpy(line_copy, line);
        
        first_word = strtok(line_copy, delim);
        if(first_word == NULL)
            continue;
        /* Getting into a macro definition: */
        if(strcmp(first_word, "macro") == 0 && !macro_def)
        {
            if( (first_word = (strtok(NULL, delim))) != NULL )
            {
                strcpy(macro_name, first_word);
                if( (first_word = (strtok(NULL, delim))) != NULL )
                {
                    printf("Error! Extra word after macro name in file %s in line %d", in_name, line_cnt);
                    continue;
                }
                content = (char *)calloc(content_size, sizeof(char));
                macro_def = true;
            }
            
            else
            {
                printf("Error! No macro found in file %s in line %d",in_name ,line_cnt);
                continue;
            }
        }
        /* Getting macro's content: */
        else if(macro_def)
        {
            if(strcmp(first_word, "endmacro") == 0)
            {
                macro_def = false;
                content[(strlen(content))] = 0;
                insert_macro_node(&macro_table, macro_name, content);
                content_size = 1;
            }
            else
            {
                content[content_size] = 0;
                content_size = strlen(content) + strlen(line)+1;
                content = (char*)realloc(content, content_size*sizeof(char));
                strcat(content, line);    
            }
        }
        /* Checks if the first word is a name of macro: */
        else if( (curr = find_name_macro_node(first_word, macro_table)) != NULL )
            fprintf(out, "%s", curr->content);
        
        else
            fprintf(out, "%s", line);
    }
    free_macro_table(macro_table);
    return out;
    
}

void insert_macro_node(macro_node **macro_table, char *macro_name, char *content)
{
    macro_node *curr;
    macro_node *new_node = create_macro_node();
    strcpy(new_node->name, macro_name);
    new_node->content = content;
    
    if(*macro_table == NULL)
        *macro_table = new_node;
    
    else
    {
        curr = *macro_table;
        while(curr->next != NULL)
            curr = curr->next; 
        
        curr->next = new_node;
    }

}

macro_node *create_macro_node()
{
    macro_node *new_node = (macro_node*)calloc(1,sizeof(macro_node));
    if(new_node == NULL)
    {
        printf("Error! Memory allocation failed!");
        exit(1);
    }
    return new_node;
}

macro_node *find_name_macro_node(char *macro_name, macro_node *macro_table)
{
    if(macro_table == NULL)
        return NULL;
        
    while(strcmp(macro_name , macro_table->name) != 0)
    {   
        if(macro_table->next == NULL)
            return NULL;

       macro_table = macro_table->next;
    }
    return macro_table;
}

void free_macro_table(macro_node *macro_table)
{
    macro_node *tmp = macro_table;
    while(macro_table != NULL)
    {
        macro_table = macro_table->next;
        free(tmp->content);
        free(tmp);
        tmp = macro_table;
    }
}

void readLinesFromFile(FILE *f)
{
    char line[MAXLINELETTERS];
    while((fgets(line,MAXLINELETTERS,f)))
    {
        insertNewLine(line);
    }
}

void insertNewLine(char* line)
{
    LineNode * tmpLine;
    tmpLine = (LineNode*)malloc(sizeof(LineNode));
    tmpLine->line = malloc(sizeof(char)*MAXLINELETTERS);
    strcpy(tmpLine->line,line);
    if(linesHead == NULL)
    {
        linesHead = tmpLine;
        LinesTail = tmpLine;
    }
    else
    {
        LinesTail -> next = tmpLine;
        LinesTail = LinesTail->next;
    }
}

void firstCoding(){
    LineNode* tmpLine = linesHead;
    while (tmpLine != NULL){
        currentLineNum++;
        char * lineLabel , * currentLine;
        boolean handledDataLabel = false;
        currentLine = malloc(sizeof(char)*MAXLINELETTERS);
        currentLine = strcpy(currentLine,tmpLine->line);
        /* handling not empty/comment line*/
        if(EmptyCommentChecker(currentLine) == false){
            char * currentWord , * label = NULL;
            /*current line is labeled*/
            if(isLabeled(currentLine) == true)
            {
                currentWord = strtok(currentLine,":");
                label = cleanWords(currentWord);
                currentWord = strtok(NULL,skip);
                /* not .entry or .extrent after label*/
                if(isCodingWord(currentWord) == false)
                {
                    addLabel(label);
                }
                /*.entry or .extrent included*/
                else
                {
                    handleCodingData(currentWord);
                    handledDataLabel = true;
                }
            }else
            {
                currentWord = strtok(currentLine,skip);
            }
            if(isCodingWord(currentWord) && handledDataLabel == false)
            {
                handleCodingData(currentWord);
            }
            else{
                char * cas = cleanWords(currentWord);
                int index = isOperation(cas);
                if(index != -1)
                {
                    handlOperation(index,currentWord,label);
                }
                else
                {
                    index = isData(cas);
                    if(index != -1){
                        handleData(index,currentWord,label);
                    }
                    else
                    {
                        printf("assembler can't handler this line..!!\n");
                    }
                }
            }

        }
        tmpLine = tmpLine -> next;
    }
}

boolean EmptyCommentChecker(char * line)
{
    boolean flag = false;
    if(line == NULL)
    {
        printf("Error! null line at: %d\n",currentLineNum);
    }
    else
    {
        char * tmpLine,*firstWord;
        tmpLine = malloc(sizeof(char)*MAXLINELETTERS);
        tmpLine = strcpy(tmpLine,line);
        firstWord = strtok(tmpLine,skip);
        if(firstWord[0] == '\n' || firstWord[0] == ';')
        flag = true;
    }
    return flag;
}

boolean isLabeled(char * line)
{   
    boolean isLabeled = false;
    char * tmpLine;
    tmpLine = malloc(sizeof(char)*MAXLINELETTERS);
    tmpLine = strcpy(tmpLine,line);
    char *label;
    label = strtok(tmpLine,skip);
    if(strstr(label,":")!=NULL){
        isLabeled = true;
    }
    return isLabeled;
}

char * cleanWords(char *w)
{
    int i , j, length = strlen(w);
    char *cleanWord;
    cleanWord = malloc(sizeof(char));
    j=0;
    for(i = 0 ; i < length ; i++){
        if(w[i] != SPACEASCIICODE  && w[i] != TAPASCIICODE && w[i]!= 10){
            cleanWord[j++]=w[i];
            cleanWord = realloc(cleanWord,sizeof(char)*j);
        }
    }
    return cleanWord;
}

boolean isCodingWord(char * currentWord)
{
    boolean flag = false;
    if(strstr(kedodKind[0],currentWord) != NULL || 
        strstr(kedodKind[1],currentWord) != NULL)
        {
            flag = true;
        }
        return flag;
}

void addLabel(char * label)
{
    if(isLabelExist(label) == true)
    {
        printf("Error, there's already label with this name,at line: %d\n",currentLineNum);
        error = true;
        return;
    }
    if(isdigit(label[0]) != 0)
    {
        printf("Error, label %s name must not start with a digit,at line: %d\n",label,currentLineNum);
        error = true;
        return;
    }
    if(systemWord(label) == true)
    {
        printf("Error, label name must not be like any system words,at line: %d\n",currentLineNum);
        error = true;
        return;
    }
    LabelNode* tmpLabel;
    tmpLabel = malloc(sizeof(LabelNode));
    tmpLabel -> labelName = malloc(sizeof(char)*MAXLABELLENGTH);
    tmpLabel -> labelName = strcpy(tmpLabel -> labelName,label);
    if(labelsHead == NULL)
    {
        labelsHead = tmpLabel;
        labelsTail = tmpLabel;
    }
    else
    {
        labelsTail -> next = tmpLabel;
        labelsTail = labelsTail -> next;
    }
}

boolean systemWord(char * word)
{
    boolean isEqual = false;
    if(isRegister(word) == true)
    {
        isEqual = true;
    }
    else if(isOperation(word) != -1)
    {
        isEqual = true;
    }
    else if(isData(word) != -1)
    {
        isEqual = true;
    }else if(strcmp(kedodKind[0],word) == 0 || strcmp(kedodKind[1],word) == 0)
    {
        isEqual = true;
    }
    return isEqual;
}

boolean isLabelExist(char * label)
{
    LabelNode * tmpLabel = labelsHead;
    boolean isExist = false;
    while(tmpLabel != NULL)
    {
        if(strcmp(tmpLabel -> labelName,label) == 0)
        {
            isExist = true;
        }
        tmpLabel = tmpLabel -> next;
    }
    DataLabelNode * tmpDataLabel = DataLabelNodeHead;
    while(tmpDataLabel != NULL)
    {   
        if(strcmp(tmpDataLabel -> labelName,label) == 0)
        {
            isExist = true;
        }
        tmpDataLabel = tmpDataLabel -> next;
    }
    return isExist;
}

void handleCodingData(char * codingWord)
{
    if(strstr(kedodKind[0] , codingWord) != NULL){
        DataLabelNode * tmpDataLabel = createNewDataLabelNode();
        tmpDataLabel -> labelName = malloc(sizeof(char)*MAXLABELLENGTH);
        codingWord = strtok(NULL,skip);
        char * label = cleanWords(codingWord);
        tmpDataLabel -> labelName = strcpy(tmpDataLabel -> labelName ,label);
        tmpDataLabel -> Address = 0;
        addDataLabelNode(tmpDataLabel);
    }
    else if(strstr(kedodKind[1] , codingWord) != NULL){
        DataLabelNode * tmpDataLabel = createNewDataLabelNode();
        tmpDataLabel -> labelName = malloc(sizeof(char)*MAXLABELLENGTH);
        codingWord = strtok(NULL,skip);
        char * label = cleanWords(codingWord);
        tmpDataLabel -> labelName = strcpy(tmpDataLabel -> labelName ,label);
        tmpDataLabel -> Address = 1;
        addDataLabelNode(tmpDataLabel);
    }
}

void handlOperation(int index , char *data, char * label)
{
    switch(index){
        case 0 ... 3 : 
        case 6 :{
                char * firstOperation;
                char * secondOperation;
                data = strtok(NULL,",");
                firstOperation = cleanWords(data);
                if(firstOperation == NULL)
                {
                    printf("Error, %s requires two operations, at Line: %d",operations[index], currentLineNum);
                    error = true;
                    return;
                }
                data = strtok(NULL,skip);
                secondOperation = cleanWords(data);
                if(secondOperation == NULL)
                {
                    printf("Error, %s requires two operations, at Line: %d",operations[index], currentLineNum);
                    error = true;
                    return;
                }
                handleTwoOperations(index,firstOperation,secondOperation,label);
                }break;
        case 4 ... 5:
        case 7 ... 13:{
                char * secondOperation;
                data = strtok(NULL,skip);
                secondOperation = cleanWords(data);
                if(secondOperation == NULL)
                {
                    printf("Error, %s requires two operations, at Line: %d",operations[index], currentLineNum);
                    error = true;
                    return;
                }
                handleOneOperations(index,secondOperation,label);
                break;
        };
        case 14 ... 15:{
                handleZeroOperations(index,label);
        };
    }
}

void handleTwoOperations(int index ,char * firstOperation ,char * secondOperation , char * label){
    int * binaryCode , i;
    AddressingMethod addressMethod;
    binaryCode = covertDecimalToBinary(index);
    MemoryNode * tmpMemory;
    tmpMemory = createNewMemory();
    for(i=0;i<OPERATIONSLENGTH;i++)
    {
        tmpMemory->binaryMachineCode[i] = binaryCode[i];
    }
    if(label != NULL)
    {
        tmpMemory->label = malloc(sizeof(char)*MAXLABELLENGTH);
        tmpMemory->label = strcpy(tmpMemory->label,label);
        free(binaryCode);
        free(label);
    }
    addToMemory(tmpMemory);
    /*coding operations*/
    /*coding first operation*/
    /*immediate coding*/
    if(strstr(firstOperation,"#") != NULL)
    {
        addressMethod = SourceOperandImmediate;
        Addressing(addressMethod,tmpMemory);
        char * strNum = cleanWords(firstOperation);
        if(strstr(strNum,".")!=NULL){
            printf("Error, assembler can't handle non integer numbers, at line %d\n",currentLineNum);
            error = true;
            return;
        }
        int intNum = atoi(strNum);
        int * binaryNum = covertDecimalToBinary1(intNum);
        MemoryNode * extraMemory = createNewMemory();
        for(i=0; i<MEMORYSIZE; i++)
        {
            extraMemory -> binaryMachineCode[i] = binaryNum[i]; 
        }
        addToMemory(extraMemory);
        free(binaryNum);  
    }
    /* coding register*/
    else if(isRegister(firstOperation) == true)
    {
        /* 2 operands are rigisters*/
        if(isRegister(secondOperation) == true)
        {
            addressMethod = DirectTwoRegister;
            Addressing(addressMethod,tmpMemory);
            MemoryNode * extraMemory = createNewMemory();
            addToMemory(extraMemory);
            int i,j=0;
            int * p , * q;
            for(i = 0 ; i < GUIDANCLENGTH ; i++){
                if(strcmp(registers[i],firstOperation) == 0){
                    p = covertDecimalToBinary(i);   
                }
            }
            for(i = 0 ; i < GUIDANCLENGTH ; i++){
                if(strcmp(registers[i],secondOperation) == 0){
                    q = covertDecimalToBinary(i);   
                }
            }
            for(i = 0 ; i < OPERATIONSLENGTH ; i++){
                extraMemory -> binaryMachineCode[i] = p[i];
            }
            for(; i < GUIDANCLENGTH ; i++){
                extraMemory -> binaryMachineCode[i] = q[j++];
            }
        }
        /*one operation is register*/
        else
        {
            addressMethod = SourceOperandDirectRegister;
            Addressing(addressMethod,tmpMemory);
            int i;
            int *p;
            MemoryNode * extraMemory = createNewMemory();
            addToMemory(extraMemory);
            for(i = 0 ; i < GUIDANCLENGTH ; i++)
            {
                if(strcmp(registers[i],firstOperation) == 0)
                {
                    p = covertDecimalToBinary(i);   
                }
            }
            for(i = 0 ; i < OPERATIONSLENGTH ; i++)
            {
                extraMemory -> binaryMachineCode[i] = p[i];
            }
        }
    }
    /* coding data*/
    else if(strstr(firstOperation,".data") != NULL || (strstr(firstOperation,".") == NULL))
    {
        addressMethod = SourceOperandDirect;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * extraMemory = createNewUnknownMemory();
        addToMemory(extraMemory);
        extraMemory -> firstOpLabel = malloc(sizeof(char) * MAXLABELLENGTH);
        firstOperation = cleanWords(firstOperation);
        extraMemory -> firstOpLabel = strcpy(extraMemory -> firstOpLabel,firstOperation);
    }
    /* coding struct*/
    else if(strstr(firstOperation,".") != NULL)
    {
        addressMethod = SourceOperandAccessToRecord;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * extraMemory = createNewUnknownMemory();
        addToMemory(extraMemory);
        extraMemory -> firstOpLabel = malloc(sizeof(char) * MAXLABELLENGTH);
        firstOperation = strtok(firstOperation,".");
        extraMemory -> firstOpLabel = strcpy(extraMemory -> firstOpLabel,firstOperation);
        firstOperation = strtok(NULL,".");
        MemoryNode * extraMemory2 = createNewMemory();
        addToMemory(extraMemory2);
        firstOperation = cleanWords(firstOperation);
        int * p = covertDecimalToBinary1(atoi(firstOperation));
        int i;
        for(i = 0 ; i < GUIDANCLENGTH ; i++){
            extraMemory2 -> binaryMachineCode[i] = p[i];
        }
    }
    /* coding second operation*/
    /* only cmp could do dedod yasher on 2nd operation */
     if(strstr(secondOperation,"#") != NULL && (i = 1))
     {
        addressMethod = DestinationOperandImmediate;
        Addressing(addressMethod,tmpMemory);
        char * strNum = cleanWords(secondOperation);
        if(strstr(strNum,".")!=NULL)
        {
            printf("Error, assembler can't handle non integer numbers, at line %d\n",currentLineNum);
            error = true;
            return;
        }
        int intNum = atoi(strNum);
        int * binaryNum = covertDecimalToBinary1(intNum);
        MemoryNode * extraMemory = createNewMemory();
        for(i = 0; i < MEMORYSIZE ; i++)
        {
            extraMemory -> binaryMachineCode[i] = binaryNum[i]; 
        }
        addToMemory(extraMemory);
        free(binaryNum); 
    }
    /* coding register*/
    else if(isRegister(secondOperation) == true)
    {
        addressMethod = DestinationOperandDirectRegister;
        Addressing(addressMethod,tmpMemory);
        if(isRegister(firstOperation) == false)
        {
            MemoryNode * extraMemory = createNewMemory();
            addToMemory(extraMemory);
            int *p;
            int i,j=0;
            for(i = 0 ; i < GUIDANCLENGTH ; i++){
                if(strcmp(registers[i],secondOperation) == 0)
                {
                    p = covertDecimalToBinary(i);   
                }
            }
            for(i = OPERATIONSLENGTH ; i < GUIDANCLENGTH ; i++)
            {
                extraMemory -> binaryMachineCode[i] = p[j++];
            }
        }
    }
    /* coding data*/ 
    else if(strstr(secondOperation,".data") != NULL || (strstr(secondOperation,".")==NULL))
    {
        addressMethod = DestinationOperandDirect;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * extraMemory = createNewUnknownMemory();
        addToMemory(extraMemory);
        extraMemory -> secondOpLabel = malloc(sizeof(char)*MAXLABELLENGTH);
        secondOperation = cleanWords(secondOperation);
        extraMemory -> secondOpLabel = strcpy(extraMemory -> secondOpLabel,secondOperation);

    }
    /* coding struct*/
    else
    { 
        addressMethod = DestinationOperandAccessToRecord;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * extraMemory = createNewUnknownMemory();
        addToMemory(extraMemory);
        extraMemory -> secondOpLabel = malloc(sizeof(char)*MAXLABELLENGTH);
        secondOperation = strtok(secondOperation,".");
        extraMemory -> secondOpLabel = strcpy(extraMemory->secondOpLabel,secondOperation);
        secondOperation = strtok(NULL,".");
        MemoryNode * extraMemory2 = createNewMemory();
        addToMemory(extraMemory2);
        secondOperation = cleanWords(secondOperation);
        int * p = covertDecimalToBinary1(atoi(secondOperation));
        int i;
        for(i=0;i<GUIDANCLENGTH;i++)
        {
            extraMemory2 -> binaryMachineCode[i] = p[i];
        }
    }    
}

void secondCoding()
{
    checkLabel();
    MemoryNode * tmpMemory = memoryHead;
    while(tmpMemory != NULL)
    {
        LabelNode * tmpLabel = labelsHead;
        int i;
        if(tmpMemory -> firstOpLabel != NULL)
        {
            while( tmpLabel != NULL)
            {
                if(strcmp(tmpLabel -> labelName,tmpMemory -> firstOpLabel) == 0)
                {
                    int * memory = covertDecimalToBinary1(tmpLabel -> decimalAddress);
                    for(i = 0 ; i < MEMORYSIZE ; i++)
                    {
                        tmpMemory -> binaryMachineCode[i] = memory[i];
                    }
                    tmpMemory -> binaryMachineCode [MEMORYSIZE-2] = 1;
                    free(memory);
                }
                tmpLabel = tmpLabel->next;
            }
            DataLabelNode * tmpDataLabel = DataLabelNodeHead;
            while (tmpDataLabel != NULL)
            {
                if(strcmp(tmpDataLabel -> labelName,tmpMemory -> firstOpLabel) == 0)
                {
                    handleExtern(tmpMemory);
                }
                tmpDataLabel = tmpDataLabel -> next;
            }
        }
        if(tmpMemory -> secondOpLabel != NULL)
        {
            while(tmpLabel != NULL)
            {
                if(strcmp(tmpLabel -> labelName , tmpMemory -> secondOpLabel) == 0)
                {
                    int * memory = covertDecimalToBinary1(tmpLabel -> decimalAddress);
                    for(i = 0 ; i < MEMORYSIZE ; i++)
                    {
                        tmpMemory -> binaryMachineCode[i] = memory[i];
                    }
                    tmpMemory -> binaryMachineCode [MEMORYSIZE-2] = 1;
                    free(memory);
                }
                tmpLabel = tmpLabel -> next;
            }
            DataLabelNode * tmpDataLabel = DataLabelNodeHead;
            while (tmpDataLabel != NULL)
            {
                if(strcmp(tmpDataLabel -> labelName,tmpMemory -> secondOpLabel) == 0)
                {
                    handleExtern(tmpMemory);
                }
                tmpDataLabel = tmpDataLabel -> next;
            }
        }
        tmpMemory = tmpMemory -> next;
    }
}

void handleExtern(MemoryNode * tmpMemory)
{
    int i;
    for ( i = 0; i < MEMORYSIZE -1; i++)
    {
        tmpMemory -> binaryMachineCode [i] = 0;
    }
    tmpMemory -> binaryMachineCode [i] = 1;
    return;
}

void freeLines(){
    LineNode * tmpLine;
    while(linesHead != NULL){
        tmpLine = linesHead;
        linesHead = linesHead->next;
        free(tmpLine->line);
        free(tmpLine);
    }
}

void displayLines(){
    LineNode* tmpLine = linesHead;
    int i=0;
    printf("File Lines:\n");
    while(tmpLine != NULL){
        printf("line:%d - %s",i++,tmpLine->line);
        tmpLine = tmpLine ->next;
    }
    printf("\n\n");
}

int isData(char * word)
{
    int i,index = -1;
    char * tmpWord = cleanWords(word);
    for(i=0; i<3 ; i++)
    {
        if(strstr(guidance[i],tmpWord))
        {
            index = i;
        }
    }
    return index;
}

int isOperation(char * word){
    int i,index = -1;
    char * tmpWord = cleanWords(word);
    for(i=0; i<16 ; i++)
    {
        if(strstr(operations[i],tmpWord))
        {
            index = i;
        }
    }
    return index;
}

void displayLabels(){
    LabelNode* tmpLabel = labelsHead;
    int i=0;
    while(tmpLabel != NULL){
        printf("line:%d - labelName:%s - labelAdrress:%d\n",i++,tmpLabel->labelName,tmpLabel->decimalAddress);
        tmpLabel = tmpLabel ->next;
    }
}

void freeLabels(){
    LabelNode * tmpLabel;
    while(labelsHead != NULL){
        tmpLabel = labelsHead;
        labelsHead = labelsHead->next;
        free(tmpLabel->labelName);
        free(tmpLabel);
    }
}

void handleOneOperations(int index ,char * secondOperation, char * label){
    AddressingMethod addressMethod;
    int *p;
    int i;
    p = covertDecimalToBinary(index);
    MemoryNode * tmpMemory;
    tmpMemory = createNewMemory();
    for(i=0;i<OPERATIONSLENGTH;i++){
        tmpMemory->binaryMachineCode[i] = p[i];
    }
    if(label != NULL){
        tmpMemory->label = malloc(sizeof(char)*MAXLABELLENGTH);
        tmpMemory->label = strcpy(tmpMemory->label,label);
       // free(label);
    }
    addToMemory(tmpMemory);
    /*kedod yasher 1 option prn*/
    if(strstr(secondOperation,"#") != NULL){
        char * c = cleanWords(secondOperation);
        if(strstr(c,".")!=NULL){
            printf("not integer number..!, handling what's befor . >\n");
            return;
        }
        c = strtok(c,"#");
        int x = atoi(c);
        int * q = covertDecimalToBinary1(x);
        addressMethod = DestinationOperandImmediate;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * tmp = createNewMemory();
        int i;
        for(i=0; i<MEMORYSIZE; i++){
            tmp->binaryMachineCode[i] = q[i]; 
        }
        addToMemory(tmp);
    }
    else if(isRegister(secondOperation) == true){
        addressMethod = DestinationOperandDirectRegister;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * tmp = createNewMemory();
        addToMemory(tmp);
            int *p;
            int i,j=0;
            for(i=0;i<GUIDANCLENGTH;i++){
                if(strcmp(registers[i],secondOperation) == 0){
                    p = covertDecimalToBinary(i);   
                }
            }
            for(i=OPERATIONSLENGTH;i<GUIDANCLENGTH;i++){
                tmp->binaryMachineCode[i] = p[j++];
            }
    }
    else if(strstr(secondOperation,".data") != NULL || (strstr(secondOperation,".")==NULL)){
        addressMethod = DestinationOperandDirect;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * tmp = createNewUnknownMemory();
        addToMemory(tmp);
        tmp->secondOpLabel = malloc(sizeof(char)*MAXLABELLENGTH);
        secondOperation = cleanWords(secondOperation);
        tmp->secondOpLabel = strcpy(tmp->secondOpLabel,secondOperation);
    }else if(strstr(secondOperation,".")!= NULL){
        addressMethod = DestinationOperandAccessToRecord;
        Addressing(addressMethod,tmpMemory);
        MemoryNode * tmp1 = createNewUnknownMemory();
        addToMemory(tmp1);
        tmp1->secondOpLabel = malloc(sizeof(char)*MAXLABELLENGTH);
        secondOperation = strtok(secondOperation,".");
        tmp1->secondOpLabel = strcpy(tmp1->secondOpLabel,secondOperation);
        secondOperation = strtok(NULL,".");
        MemoryNode * tmp2 = createNewMemory();
        addToMemory(tmp2);
        secondOperation = cleanWords(secondOperation);
        int * p = covertDecimalToBinary1(atoi(secondOperation));
        int i;
        for(i=0;i<GUIDANCLENGTH;i++){
            tmp2->binaryMachineCode[i] = p[i];
        }
    }
}
    
void handleZeroOperations(int index , char * label)
{
    int *p;
    int i;
    p = covertDecimalToBinary(index);
    MemoryNode * tmpMemory;
    tmpMemory = createNewMemory();
    for(i = 0 ; i < OPERATIONSLENGTH ; i++)
    {
        tmpMemory -> binaryMachineCode[i] = p[i];
    }
    if(label != NULL)
    {
        tmpMemory -> label = malloc(sizeof(char)*MAXLABELLENGTH);
        tmpMemory -> label = strcpy(tmpMemory -> label,label);
    }
    addToMemory(tmpMemory);
}

void Addressing(AddressingMethod addressingMethod , MemoryNode* tmp){
    switch(addressingMethod)
    {
        case SourceOperandImmediate :
        {
            tmp -> binaryMachineCode[4] = 0;
            tmp -> binaryMachineCode[5] = 0;
            break;
        } 
        case SourceOperandDirect :
        {
            tmp -> binaryMachineCode[4] = 0;
            tmp -> binaryMachineCode[5] = 1;
            break;
        }
        case SourceOperandAccessToRecord :
        {
            tmp -> binaryMachineCode[4] = 1;
            tmp -> binaryMachineCode[5] = 0;
            break;
        }
        case SourceOperandDirectRegister :
        {
            tmp -> binaryMachineCode[4] = 1;
            tmp -> binaryMachineCode[5] = 1;
            break;
        }
        case DestinationOperandImmediate :
        {
            tmp -> binaryMachineCode[6] = 0;
            tmp -> binaryMachineCode[7] = 0;
            break;
        }
        case DestinationOperandDirect :
        {
            tmp -> binaryMachineCode[6] = 0;
            tmp -> binaryMachineCode[7] = 1;
            break;
        }
        case DestinationOperandAccessToRecord :
        {
            tmp -> binaryMachineCode[6] = 1;
            tmp -> binaryMachineCode[7] = 0;
            break;
        }
        case DestinationOperandDirectRegister :
        {
            tmp -> binaryMachineCode[6] = 1;
            tmp -> binaryMachineCode[7] = 1;
            break;
        }
        case DirectTwoRegister:
        {
            tmp -> binaryMachineCode[4] = 1;
            tmp -> binaryMachineCode[5] = 1;
            tmp -> binaryMachineCode[6] = 1;
            tmp -> binaryMachineCode[7] = 1;
            break;
        }
    }
}

int * covertDecimalToBinary(int num){
    int i;
    int  * binary;
    binary = malloc(sizeof(int)*OPERATIONSLENGTH);
    for(i = OPERATIONSLENGTH -1 ; i >= 0 ; i--)
    {
        binary[i] = num % 2;
        num = num / 2;
    }
    return binary;
}

void handleData(int index,char *data, char * label){
    
    /* coding .data */
    if(index==0)
    {   
        handleIntData(index,data,label);
    }
    /* coding .string*/
    else if(index==1)
    {
        handleSTRData(index,data,label);
    }
    /*coding .struct*/
    else if(index==2)
    {
        handleStructData(index,data,label);
    }
}

void handleIntData(int index, char *data, char * label)
{
    data = strtok(NULL,guidance[index]);
    int n;
    data = strtok(data,",");
    MemoryNode *tmpMemory = createNewMemory();
    if(label != NULL)
    {
        tmpMemory -> label = malloc(sizeof(char)*MAXLABELLENGTH);
        tmpMemory -> label = strcpy(tmpMemory -> label,label);
    }
    if(strstr(data,".") != NULL)
    {
        printf("not integer number..!, handling what's befor . >\n");
        return;
    }
        addToMemory(tmpMemory);
        int kk,nn;
        char * cleanData = cleanWords(data);
        nn = atoi(cleanData);
        int *q = covertDataToBinary(nn);
        for(kk = 0 ; kk < MEMORYSIZE ; kk++)
        {
            tmpMemory -> binaryMachineCode[kk] = q[kk];
        }
        while((data = strtok(NULL,",")) != NULL)
        {
            if(strstr(data,".") != NULL)
            {
                printf("not integer number..!, handling what's befor . >\n");
                return;
            }
            MemoryNode * extraMemory = createNewMemory();
            addToMemory(extraMemory);
            int k,n;
            cleanData = cleanWords(data);
            n = atoi(cleanData);
            int *p = covertDataToBinary(n);
            for(k=0;k<MEMORYSIZE;k++)
            {
               extraMemory->binaryMachineCode[k] = p[k];
            }
        }
}

void handleSTRData(int index, char *data, char * label)
{
        data = strtok(NULL,guidance[index]);
        char * c;
        int i,size =1;
        if(strstr(data,"\"") != NULL)
        {
            for(i = 0 ; data[i] != '"'; i++);
        }
        c = malloc(sizeof(char)*size);
        while(data[++i] != '"')
        {
            c[size-1] = data[i];
            c = realloc(c,sizeof(char)*(++size));
        }
       c[size-1]='\n';
       for(i = 0; i < size -1 ; i++)
       {
           MemoryNode *t = createNewMemory();
           if(label != NULL && i == 0)
            {    
                t->label = malloc(sizeof(char)*MAXLABELLENGTH);
                t->label = strcpy(t->label,label);
            }
           addToMemory(t);
           int k,n;
           n=c[i];
           int *p = covertDataToBinary(n);
           for(k=0;k<MEMORYSIZE;k++)
           {
               t->binaryMachineCode[k] = p[k];
           }
       }
       addToMemory(createNewMemory());
}

void handleStructData(int index, char *data, char * label)
{
    boolean flag = false;
    data = cleanWords(data);
    while((data = strtok(NULL,","))!=NULL)
    {
        char* word;
        word = malloc(sizeof(char)*MAXLINELETTERS);
        word = strcpy(word,data);
        word = cleanWords(word);
        /*str in struct*/
        if(word[0] == '"')
        {
            char* c;
            int size=1;
            c = malloc(sizeof(char)*size);
            while(word[size]!='"' && word[size] != '\n')
            {
                c[size-1] = word[size];
                c = realloc(c,sizeof(char)*(++size));
            }
            c[size-1] = '\n';
            int i;
            for(i=0; i<size-1 ; i++)
            {
                MemoryNode * tmp = createNewMemory();
                addToMemory(tmp);
                int *p = covertDataToBinary(c[i]);
                int j;
                for(j=0;j<10;j++)
                {
                    tmp ->binaryMachineCode[j] = p[j];
                }
                if(flag == false && label != NULL){
                    tmp -> label = malloc(sizeof(char)*(strlen(label)));
                    tmp -> label = strcpy(tmp->label,label);
                    flag = true;
                }
            }
            /*00 for \n*/
            addToMemory(createNewMemory());
            
        }
        else
        {/* handling int in struct*/
            MemoryNode * tmp = createNewMemory();
            addToMemory(tmp);
            int *p = covertDataToBinary(atoi(word));
            int j;
                for(j=0;j<10;j++)
                {
                    tmp ->binaryMachineCode[j] = p[j];
                }
                if(flag == false && label != NULL)
                {
                    tmp -> label = malloc(sizeof(char)*MAXLABELLENGTH);
                    tmp -> label = strcpy(tmp->label,label);
                    flag = true;
                }
        }
        free(word);
    }
    
}

void freeMemory(){
     MemoryNode * tmp;
    while(memoryHead != NULL)
    {
        tmp = memoryHead;
        memoryHead = memoryHead->next;
        free(tmp->binaryMachineCode);
        free(tmp->firstOpLabel);
        free(tmp->secondOpLabel);
        free(tmp);
    }
}

void displayMemory()
{
    MemoryNode *tmp = memoryHead;
    int i,j=0;
    printf("\n");
    while(tmp != NULL)
    {
        printf("%d - address %d -- ",j++ , tmp->decimalAddress);
        for( i = 0 ; i < MEMORYSIZE ; i++)
        {
            printf("%d",tmp -> binaryMachineCode[i]);
        }
        if(tmp -> firstOpLabel != NULL)
        {
            printf(" -- with first label : %s",tmp->firstOpLabel);
        }
        if(tmp -> secondOpLabel != NULL)
        {
            printf(" -- with second label : %s",tmp->secondOpLabel);
        }
        if(tmp -> label != NULL)
        {
            printf(" -- with  label : %s",tmp->label);
        }
        printf("\n");
        tmp = tmp -> next;
    }
}

boolean isRegister(char * word)
{
    int i;
    boolean isRegister = false;
    for(i = 0 ; i < MEMORYSIZE ; i++)
    {
        if(strcmp(registers[i],word) == 0)
        {
            isRegister = true;
        }    
    }
    return isRegister;
}

MemoryNode * createNewMemory()
{
    MemoryNode * tmpMemory;
    int i;
    tmpMemory = malloc(sizeof(MemoryNode));
    tmpMemory -> binaryMachineCode = malloc(sizeof(int)*MEMORYSIZE);
    for(i = 0 ; i < MEMORYSIZE ; i++){
        tmpMemory -> binaryMachineCode[i] = 0;
    }
    return tmpMemory;
}

void addToMemory(MemoryNode * tmp){
    if(memoryHead == NULL)
    {
        memoryHead = tmp;
        memoryHead->decimalAddress = 100;
        memoryTail = tmp;
    }
    else
    {
        tmp->decimalAddress = memoryTail->decimalAddress + 1;
        memoryTail->next = tmp;
        memoryTail = memoryTail->next;
    }
}

int *covertDecimalToBinary1 (int num)
{
    int *tmpBinaryCode, *binaryCode;
    int i , k , j;
    tmpBinaryCode = malloc(sizeof(int)*MEMORYSIZE);
    binaryCode = malloc(sizeof(int)*MEMORYSIZE);
    for(i=0;i<MEMORYSIZE;i++)
    {
        tmpBinaryCode[i] = 0;
    }
    for(i = BINARYSIZE -3 ; i>=0 ; i--)
    {
        k = num >> i;
        if(k & 1)
        {
            tmpBinaryCode[i] = 1;
        }
        else
        {
            tmpBinaryCode[i] = 0;
        }
    }
    
    j=0;
    for(i = BINARYSIZE - 3 ; i>=0 ; i--){
        binaryCode[j++] = tmpBinaryCode[i];
    }
    return binaryCode;
}

int * covertDataToBinary(int num){
    int *tmpBinaryCode, *binaryCode;
    int i , k , j;
    tmpBinaryCode = malloc(sizeof(int)*MEMORYSIZE);
    binaryCode = malloc(sizeof(int)*MEMORYSIZE);
    for(i = BINARYSIZE -1 ; i>=0 ; i--){
        k = num >> i;
        if(k & 1)
        {
            tmpBinaryCode[i] = 1;
        }
        else
        {
            tmpBinaryCode[i] = 0;
        }
    }
    j=0;
    for(i = BINARYSIZE -1 ; i>=0 ; i--){
        binaryCode[j++] = tmpBinaryCode[i];
    }
    return binaryCode;
}

MemoryNode * createNewUnknownMemory(){
    MemoryNode * tmpMemory;
    int i;
    tmpMemory = malloc(sizeof(MemoryNode));
    tmpMemory -> binaryMachineCode = malloc(sizeof(int)*MEMORYSIZE);
    for(i = 0 ; i < MEMORYSIZE ; i++){
        tmpMemory -> binaryMachineCode[i] = -1;
    }
    return tmpMemory;
}

void checkLabel()
{
    MemoryNode * tmpMemory = memoryHead;
    while(tmpMemory != NULL){
        LabelNode * tmpLabel = labelsHead;
        while(tmpLabel != NULL){
            if(tmpMemory -> label != NULL){
                if(strcmp(tmpLabel -> labelName,tmpMemory -> label) == 0){
                tmpLabel -> decimalAddress = tmpMemory -> decimalAddress;
                }
            }
            tmpLabel = tmpLabel -> next;
        }
        tmpMemory = tmpMemory -> next;
    }
}

void addDataLabelNode(DataLabelNode * tmp)
{
     if(isLabelExist(tmp -> labelName) == true)
    {
        printf("Error, there's already label with this name,at line: %d\n",currentLineNum);
        error = true;
        return;
    }
    if(isdigit(tmp -> labelName[0]) != 0)
    {
        printf("Error, label %s name must not start with a digit,at line: %d\n",tmp -> labelName,currentLineNum);
        error = true;
        return;
    }
    if(systemWord(tmp -> labelName) == true)
    {
        printf("Error, label name must not be like any system words,at line: %d\n",currentLineNum);
        error = true;
        return;
    }
    if(DataLabelNodeHead == NULL){
        DataLabelNodeHead = tmp;
        DataLabelNodeTail = tmp;
    }
    else
    {
        DataLabelNodeTail -> next = tmp;
        DataLabelNodeTail = DataLabelNodeTail -> next;
    }
}

DataLabelNode * createNewDataLabelNode()
{
    DataLabelNode * tmp;
    tmp = malloc(sizeof(DataLabelNode));
    if(tmp == NULL)
    {
        printf("Error, could't allocate more memory!\n");
    }
    return tmp;
}

void freeDataLabel()
{
    while(DataLabelNodeHead != NULL)
    {
        DataLabelNode* tmp = DataLabelNodeHead;
        DataLabelNodeHead = DataLabelNodeHead->next;
        free(tmp->labelName);
        free(tmp);
    }
}

void displayDataLabel()
{
    DataLabelNode * tmp = DataLabelNodeHead;
    while(tmp != NULL)
    {
        printf("Label: %s, At: %d\n",tmp->labelName,tmp->Address);
        tmp = tmp->next;
    } 
}
/*TODO -> print result into file*/
void displayOutput()
{
    MemoryNode * tmp = memoryHead;
    while(tmp!=NULL)
    { 
        char * binaryArr, *decimalArr;
        int binSize = 1;
        int decSize = 1;
        int i;
        binaryArr = malloc(sizeof(char)*(binSize));
        decimalArr = malloc(sizeof(char)*(decSize));
        char s [11];
        for(i=0;i<11;i++)
        {
            s[i] = tmp->binaryMachineCode[i] + '0';
        }
        s[i-1] = '\0';
       int x = toDeci(s,2);
       while(x>0)
       {   
        binaryArr[binSize-1] = base32Sympols[x%32];
        binSize++;
        binaryArr = realloc(binaryArr,sizeof(char)*binSize);
        x = x / 32;
       }
       int y = tmp -> decimalAddress;
       while(y >0)
       {
           decimalArr[decSize -1] = base32Sympols[y%32];
           decSize++;
           decimalArr = realloc(decimalArr,sizeof(char)*decSize);
           y = y / 32;
       }
       char *swapedBinaryArr  = swap(binaryArr);
       char *swapedDecimalArr = swap(decimalArr);
       printf("%s  ",swapedDecimalArr);
       printf("%s",swapedBinaryArr);
       printf("\n");   
        tmp = tmp -> next;
    }
}

int val(char c)
{
    if (c >= '0' && c <= '9')
        return (int)c - '0';
    else
        return (int)c - 'A' + 10;
}

int toDeci(char *str, int base)
{
    int len = strlen(str);
    int power = 1; 
    int num = 0;
    int i;
    for (i = len - 1; i >= 0; i--)
    {
        if (val(str[i]) >= base)
        {
           printf("Invalid Number");
           return -1;
        }
              num += val(str[i]) * power;
        power = power * base;
    }
    return num;
}

char * swap(char * arr){

    int i,len = strlen(arr);
    char * out;
    if(len == 0)
    {
        out = malloc(sizeof(char)*2);
        out[0] = base32Sympols[0];
        out[1] = base32Sympols[0];
        return out;
    }
    out = malloc(sizeof(char)*len);
    for(i=0; i < len ; i++){
        out[len - i - 1] = arr[i];
    }
    if(len == 1)
    {
        out = realloc(out,sizeof(char)*(len+1));
        out[1] = out[0];
        out[0] = base32Sympols[0];
    }
    return out;
}

FILE *pre_assembler(char *file_name, int namelen)
{
    FILE *out, *in;
    char out_name[MAX_FILENAME_LENGTH];
    char in_name [MAX_FILENAME_LENGTH];
    char line[MAX_LINE_LENGTH + 1];
    char macro_name[MAX_LINE_LENGTH];
    char *content;
    unsigned int line_cnt = 0, content_size = 1;
    bool macro_def = false;
    
    
    macro_node *macro_table = NULL;

    
    strcpy(in_name, file_name);
    strcpy(out_name, file_name);
    
    in  = fopen(strcat(in_name, ".as"), "r"); 
    out = fopen(strcat(out_name, ".am"), "w");
    
    if(in == NULL)
    {
        printf("Cannot open file %s!", in_name);
        return NULL;
    }
    if(out == NULL)
    {
        printf("Cannot open file %s!", out_name);
        return NULL;
    }
    
    while(fgets(line, MAX_LINE_LENGTH, in) )
    {
        char *first_word, line_copy[MAX_LINE_LENGTH + 1];
        macro_node *curr = NULL;
        const char delim[] = " \t\n"; 
        
        line_cnt++;
        strcpy(line_copy, line);
        
        first_word = strtok(line_copy, delim);
        if(first_word == NULL)
            continue;
        /* Getting into a macro definition: */
        if(strcmp(first_word, "macro") == 0 && !macro_def)
        {
            if( (first_word = (strtok(NULL, delim))) != NULL )
            {
                strcpy(macro_name, first_word);
                if( (first_word = (strtok(NULL, delim))) != NULL )
                {
                    printf("Error! Extra word after macro name in file %s in line %d", in_name, line_cnt);
                    continue;
                }
                content = (char *)calloc(content_size, sizeof(char));
                macro_def = true;
            }
            
            else
            {
                printf("Error! No macro found in file %s in line %d",in_name ,line_cnt);
                continue;
            }
        }
        /* Getting macro's content: */
        else if(macro_def)
        {
            if(strcmp(first_word, "endmacro") == 0)
            {
                macro_def = false;
                content[(strlen(content))] = 0;
                insert_macro_node(&macro_table, macro_name, content);
                content_size = 1;
            }
            else
            {
                content[content_size] = 0;
                content_size = strlen(content) + strlen(line)+1;
                content = (char*)realloc(content, content_size*sizeof(char));
                strcat(content, line);    
            }
        }
        /* Checks if the first word is a name of macro: */
        else if( (curr = find_name_macro_node(first_word, macro_table)) != NULL )
            fprintf(out, "%s", curr->content);
        
        else
            fprintf(out, "%s", line);
    }
    free_macro_table(macro_table);
    return out;
    
}

void insert_macro_node(macro_node **macro_table, char *macro_name, char *content)
{
    macro_node *curr;
    macro_node *new_node = create_macro_node();
    strcpy(new_node->name, macro_name);
    new_node->content = content;
    
    if(*macro_table == NULL)
        *macro_table = new_node;
    
    else
    {
        curr = *macro_table;
        while(curr->next != NULL)
            curr = curr->next; 
        
        curr->next = new_node;
    }

}

macro_node *create_macro_node()
{
    macro_node *new_node = (macro_node*)calloc(1,sizeof(macro_node));
    if(new_node == NULL)
    {
        printf("Error! Memory allocation failed!");
        exit(1);
    }
    return new_node;
}

macro_node *find_name_macro_node(char *macro_name, macro_node *macro_table)
{
    if(macro_table == NULL)
        return NULL;
        
    while(strcmp(macro_name , macro_table->name) != 0)
    {   
        if(macro_table->next == NULL)
            return NULL;

       macro_table = macro_table->next;
    }
    return macro_table;
}

void free_macro_table(macro_node *macro_table)
{
    macro_node *tmp = macro_table;
    while(macro_table != NULL)
    {
        macro_table = macro_table->next;
        free(tmp->content);
        free(tmp);
        tmp = macro_table;
    }
}