#include "node.h"
#include "tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 10000

int main(int argc, char *argv[])
{
  if (argc > 2) // if too many arguments entered, exit the program
  {
    printf("Too many arguments.\n");
    exit(1);
  }
  else if (argc == 2) // if user enters filename as argument...
  {
    // create a string from the entered filename and append implied extension
    char *fileName = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    memcpy(fileName, argv[1], strlen(argv[1]) + 1);
    realloc(fileName, (strlen(fileName)) + sizeof(char) * 6);
    strcat(fileName, ".sp2022");

    // create string for .inorder filename
    char *inorderFileName = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    memcpy(inorderFileName, argv[1], strlen(argv[1]) + 1);
    realloc(inorderFileName, (strlen(inorderFileName)) + sizeof(char) * 8);
    strcat(inorderFileName, ".inorder");

    // create string for .preorder filename
    char *preorderFileName = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    memcpy(preorderFileName, argv[1], strlen(argv[1]) + 1);
    realloc(preorderFileName, (strlen(preorderFileName)) + sizeof(char) * 9);
    strcat(preorderFileName, ".preorder");

    // create string for .postorder filename
    char *postorderFileName = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    memcpy(postorderFileName, argv[1], strlen(argv[1]) + 1);
    realloc(postorderFileName, (strlen(postorderFileName)) + sizeof(char) * 10);
    strcat(postorderFileName, ".postorder");

    // open file
    FILE *file = NULL;
    file = fopen(fileName, "r");
    if (file) // if file exists...
    {
      // check if file is empty, and exit if it is
      fseek(file, 0, SEEK_END);
      long size = ftell(file);
      if (size == 0)
      {
        printf("File is empty.\n");
        exit(1);
      }

      rewind(file); // if file isn't empty, rewind the file reading index

      // store contents of file into buffer
      char buf[SIZE];
      fread(buf, SIZE, 1, file);
      fclose(file);

      node_t *root = buildTree(buf); // build tree using buffer

      // create output files and write their corresponding tree traversals
      FILE *preorder = NULL;
      preorder = fopen(preorderFileName, "w");
      printPreorder(root, preorder);

      FILE *inorder = NULL;
      inorder = fopen(inorderFileName, "w");
      printInorder(root, inorder);

      FILE *postorder = NULL;
      postorder = fopen(postorderFileName, "w");
      printPostorder(root, postorder);

      // close files
      fclose(preorder);
      fclose(inorder);
      fclose(postorder);
    }
    else
    {
      printf("File doesn't exist");
      exit(1);
    }
  }

  if (argc == 1) // if no command line options...
  {
    char buf[SIZE];     // instantiate a character buffer
    if (isatty(0) == 1) // if stdin is from the terminal...
    {
      printf("Write input text: ");

      rewind(stdin);

      // get user input until they press enter and store input in char buffer
      fgets(buf, SIZE, stdin);

      // if user doesn't input anything into command line, exit
      if (strlen(buf) <= 1)
      {
        printf("Nothing entered.\n");
        exit(1);
      }
    }
    else // if stdin is a redirected file...
    {
      // check if file is empty, and exit if it is
      fseek(stdin, 0, SEEK_END);
      long size = ftell(stdin);
      if (size == 0)
      {
        printf("File is empty.\n");
        exit(1);
      }

      rewind(stdin);

      fread(buf, SIZE, 1, stdin); // store redirected file into char buffer
    }

    node_t *root = buildTree(buf); // build tree using buffer

    // create output files and write their corresponding tree traversals
    FILE *preorder = NULL;
    preorder = fopen("output.preorder", "w");
    printPreorder(root, preorder);

    FILE *inorder = NULL;
    inorder = fopen("output.inorder", "w");
    printInorder(root, inorder);

    FILE *postorder = NULL;
    postorder = fopen("output.postorder", "w");
    printPostorder(root, postorder);

    // close files
    fclose(preorder);
    fclose(inorder);
    fclose(postorder);
  }

  return 0;
}