typedef int boolean;
#define FALSE 0
#define TRUE !FALSE
#define MAXSTR 80
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "animal.h"
/* initialize the tree from a file whose name is specified as a command line argument. Your program should check the result of the fopen function to ensure that a legal file name was specified. When your program exits, it should save the new tree to that file, after again checking to make sure the file is writable */
struct treeStruct {
	char *string;
	struct treeStruct *left, *right;
};
	
struct positionStruct {
	struct treeStruct* node;
};

TreeType InitTree (char *file) {
	TreeType *tree = (TreeType*)malloc(sizeof(TreeType));
	
	if (file == NULL) {
		printf("Invalid file.");
		exit(0);
	}
	while (fgets(tree->string, sizeof (char), file) != NULL) {
		//treeStruct->left
		printf("input tree: %s", tree-> string);
	}	
}
void WriteTree (TreeType tree, char *file) {
}
void PrintTree (TreeType tree) {
}
PositionType Top (TreeType tree) {
}
boolean IsLeaf (TreeType tree, PositionType pos) {
}
boolean Answer (char *q) {
}
char *Question (TreeType tree, PositionType pos) {
}
char *Guess (TreeType tree, PositionType pos) {
}
PositionType YesNode (TreeType tree, PositionType pos) {
}
PositionType NoNode (TreeType tree, PositionType pos) {
}
void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ) {
}
void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ) {
}


/*
 *  Play the "animal" game, in which the program attempts to guess an animal
 *  that the user is thinking of by asking yes or no questions. Eventually,
 *  the program either will guess the user's animal or run out of questions
 *  to ask. In the latter case, the program will ask the user to provide a
 *  yes-or-no question that would distinguish between the user's animal and
 *  the program's best guess.
 *  The data structure of questions and guesses is essentially a binary tree,
 *  with each internal node having a "yes" branch and a "no" branch. Leaves
 *  of the tree represent animals to be guessed by the program. If the program
 *  fails to guess the user's animal, it replaces one of the leaves of the tree
 *  by a node containing the new question, whose children are the program's
 *  best guess and the animal provided by the user.
 *  The structure of the program is simple. It initializes the question/guess
 *  data structure, then plays games as long as the user is interested. In each
 *  game, the program starts at the top of the tree (the root) and progresses
 *  toward the bottom (the leaves) depending on the user's responses. Once it
 *  reaches a leaf, it either has won or lost, and handles the situation as
 *  described above.
 */
int main (int argc, char *argv[])
{
    char *treefile = NULL;
    TreeType tree;
    PositionType pos;
    char *newQuestion, *newAnswer;

    if (argc > 1) {
        treefile = argv[1];
    }
    tree = InitTree (treefile);

    printf("%s", "Think of an animal. I will try to guess what it is.\n"
		 "Please answer my questions with yes or no.\n");

    while (TRUE) {
        pos = Top (tree);
        while (!IsLeaf (tree, pos)) {
            pos = Answer(Question(tree,pos))? 
	       YesNode(tree,pos): NoNode(tree,pos);
        }
        if (Answer (Guess (tree, pos))) {
            printf ("I got it right!\n");
        } else {
            GetNewInfo (tree, pos, &newAnswer, &newQuestion);
            ReplaceNode (tree, pos, newAnswer, newQuestion);
        }

        if (!Answer ("Want to play again? ")) {
            WriteTree(tree, treefile);
            exit (0);
        }
    }
}
