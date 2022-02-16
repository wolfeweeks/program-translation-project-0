#include "node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief create an array of strings from buf
 *
 * @param buf file or string containing words separated by whitespace
 * @return array of strings found within buf
 */
char **createStringsArray(char buf[])
{
  int c;            // stores a character from buf
  int bufIndex = 0; // stores current buf scan position
  int numberOfWords = 0;

  // get the number of words in the file
  do
  {
    c = (int)buf[bufIndex++]; // get next char in buf and increment bufindex

    if (c == EOF || c == ' ' || c == '\n' || c == '\t')
    {
      numberOfWords++;
    }

    // elif used to handle differences between stdin input and file input
    else if (c == 0)
    {
      if (buf[bufIndex - 2] != '\n')
      {
        numberOfWords++;
      }
    }
  } while (c != EOF && c != 0);

  // allocate indexes equal to the number of words
  char **words = (char **)malloc(numberOfWords * (sizeof(char *) + 1));

  bufIndex = 0;

  // for each word, allocate enough space for the length of each string
  int i;
  for (i = 0; i < numberOfWords; i++)
  {
    int wordLength = 0;

    // get the length of the word
    do
    {
      c = buf[bufIndex++];

      if (c != '\n' && c != ' ' && c != EOF && c != '\t' && c != 0 && c != '\r')
      {
        wordLength++;
      }

    } while (c != '\n' && c != ' ' && c != EOF && c != '\t' && c != 0);

    // move buffer to correct position
    if (i == numberOfWords - 1)
    {
      bufIndex = bufIndex - wordLength - 1;
    }
    else
    {
      if (c == ' ' || c == '\t')
      {
        bufIndex = bufIndex - (wordLength + 1);
      }
      else
      {
        bufIndex = bufIndex - (wordLength + 2);
      }
    }

    // allocate enough memory for the length of the word
    char *ptrToWord = (char *)malloc((wordLength + 1) * sizeof(char));

    // append each letter in the word to its index in words array
    for (int j = 0; j < wordLength; j++)
    {
      int nextCharInWord = buf[bufIndex];
      bufIndex++;
      ptrToWord[j] = nextCharInWord;
    }
    ptrToWord[wordLength] = '\0'; // add string terminating char to end of word

    words[i] = ptrToWord;

    // move buffer to correct position
    if (c == ' ' || c == '\t')
    {
      bufIndex = bufIndex + 1;
    }
    else
    {
      bufIndex = bufIndex + 2;
    }
  }

  // add null element for checking when array has been fully looped over
  words[numberOfWords] = NULL;

  return words;
}

/**
 * @brief returns the last char in a string
 *
 * @param string
 * @return last char in string
 */
char getLastChar(char string[])
{
  char lastLetter = string[(strlen(string) - 1)];
  // fix for some weird bug that would sometimes add a \e to the end of the string
  if (lastLetter == 25)
  {
    lastLetter = string[(strlen(string) - 2)];
  }
  return lastLetter;
}

/**
 * @brief inserts a new node into the correct position in the tree
 *
 * @param tree pointer to pointer of parent node
 * @param depth depth of new node
 * @param string word being used for node's value
 */
void insertNode(node_t **tree, int depth, char *string)
{
  node_t *temp = NULL;

  char value = getLastChar(string);

  // if node is at correct depth, create a new node and add it to the tree
  if (*tree == NULL)
  {
    temp = (node_t *)malloc(sizeof(node_t));
    temp->left = temp->right = NULL;
    temp->string = string;
    temp->value = value;
    temp->depth = depth;
    *tree = temp;
    return;
  }

  // if new node's value is less than it's parent, insert it to the left
  if (value < (*tree)->value)
  {
    insertNode((&(*tree)->left), depth + 1, string);
  }
  // if new node's value is equal to it's parent, insert it in the middle
  else if (value == (*tree)->value)
  {
    insertNode((&(*tree)->middle), depth + 1, string);
  }
  // if new node's value is greater than it's parent, insert it to the right
  else
  {
    insertNode((&(*tree)->right), depth + 1, string);
  }
}

/**
 * @brief writes the preorder traversal of the tree to the file passed in
 *
 * @param node pointer to the root node of the tree
 * @param file file to write to
 */
void printPreorder(node_t *node, FILE *file)
{
  // if not a leaf node, print the node's value spaced in by 2x the node's depth
  // and use recursion to continue printing the nodes in preorder
  if (node != NULL)
  {
    int i;
    for (i = 0; i < node->depth; i++)
    {
      fprintf(file, "  ");
    }
    fprintf(file, "%c:%s\n", node->value, node->string);

    printPreorder(node->left, file);
    printPreorder(node->middle, file);
    printPreorder(node->right, file);
  }
}

/**
 * @brief writes the inorder traversal of the tree to the file passed in
 *
 * @param node pointer to the root node of the tree
 * @param file file to write to
 */
void printInorder(node_t *node, FILE *file)
{
  // if not a leaf node, print the node's value spaced in by 2x the node's depth
  // and use recursion to continue printing the nodes inorder
  if (node != NULL)
  {
    printInorder(node->left, file);

    int i;
    for (i = 0; i < node->depth; i++)
    {
      fprintf(file, "  ");
    }
    fprintf(file, "%c:%s\n", node->value, node->string);

    printInorder(node->middle, file);
    printInorder(node->right, file);
  }
}

/**
 * @brief writes the postorder traversal of the tree to the file passed in
 *
 * @param node pointer to the root node of the tree
 * @param file file to write to
 */
void printPostorder(node_t *node, FILE *file)
{
  // if not a leaf node, print the node's value spaced in by 2x the node's depth
  // and use recursion to continue printing the nodes in postorder
  if (node != NULL)
  {
    printPostorder(node->left, file);
    printPostorder(node->middle, file);
    printPostorder(node->right, file);

    int i;
    for (i = 0; i < node->depth; i++)
    {
      fprintf(file, "  ");
    }
    fprintf(file, "%c:%s\n", node->value, node->string);
  }
}

/**
 * @brief
 * creates a trinary? tree from the words within the buffer
 *
 * @param buf file or string containing words separated by whitespace
 * @return pointer to the root node of the tree
 */
node_t *buildTree(char buf[])
{
  // create an array of words within buffer
  char **words = createStringsArray(buf);

  node_t *root = NULL; // pointer to the root node of the tree

  // for each word in words array, insert new node into tree
  int i;
  for (i = 0;; i++)
  {
    if (words[i] == NULL)
    {
      break;
    }

    insertNode(&root, 0, words[i]);
  }

  return root; // return pointer to root node
}