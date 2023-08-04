#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int count; // To store the number of occurrences of the word
};

// Trie structure
struct Trie
{
    struct TrieNode *root;
};

// Creates a new Trie node
struct TrieNode *createNode()
{
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Initializes a Trie structure
struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    if (trie)
    {
        trie->root = createNode();
    }
    return trie;
}

// Inserts a word into the Trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->count++;
}

// Computes the number of occurrences of a word in the Trie structure
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct TrieNode *current = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0; // Word does not exist in the Trie
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocates the Trie structure and all its nodes recursively
void deallocateTrieNodes(struct TrieNode *node)
{
    if (node)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            deallocateTrieNodes(node->children[i]);
        }
        free(node);
    }
}

// Deallocates the Trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        deallocateTrieNodes(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Reads the dictionary from the file and stores it in the pInWords array
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char word[256];

    // Read the first line to skip the count
    fscanf(file, "%s", word);

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);

    // Print the words from the dictionary
    printf("Number of words read from the dictionary: %d\n", numWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // Create the trie and insert words
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Check occurrences of specific words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the trie and its nodes
    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
        printf("The trie has been deallocated.\n");

    // Deallocate individual words from the array
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}
