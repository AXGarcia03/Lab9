#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// Node for separate chaining
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Hash table structure for separate chaining
struct HashType
{
    struct Node* head; // Pointer to the head of the linked list
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz; // Simple modulo hashing for demonstration
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i=0;i<hashSz;++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashSize = 10; // Adjust the hash size as needed

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create hash table array
    struct HashType hashTable[hashSize];

    // Initialize hash table
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable[i].head = NULL;
    }

    // Insert records into hash table using separate chaining
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id, hashSize);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        if (newNode == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newNode->data = pRecords[i];
        newNode->next = hashTable[index].head;
        hashTable[index].head = newNode;
    }

    // Display records in the hash structure
    displayRecordsInHash(hashTable, hashSize);

    // Free dynamically allocated memory for records
    free(pRecords);

    // Free dynamically allocated memory for hash table
    for (int i = 0; i < hashSize; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}
