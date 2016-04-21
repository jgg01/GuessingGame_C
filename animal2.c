typedef int boolean;
#define FALSE 0
#define TRUE !FALSE
#define MAXSTR 80
#define MAXNUMQS 80
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "animal.h"

struct treeStruct {
	char *nodes[MAXNUMQS];
};

struct positionStruct {
	int nodeIndex;
};

TreeType InitTree (char *file) {
	FILE *f = fopen(file, "r");
	if (f == NULL) {
		perror("Invalid filename");
		exit(0);
	}
	TreeType tree = (TreeType)malloc(MAXNUMQS * MAXNUMQS * sizeof(TreeType));
	int i = 0;
	for(; i < MAXNUMQS; i++) {
		tree->nodes[i] = (char*) malloc(MAXSTR *sizeof(char));
	}
	i = 0;
	char mystring[MAXSTR];
	while (fgets (mystring, MAXSTR, f) != NULL) {
		if (mystring[strlen(mystring) - 1] == '\n') {
		    mystring[strlen(mystring) - 1] = '\0';
		}
		tree->nodes[i] = mystring;
		printf("input tree: %s at i %d\n", tree->nodes[i], i);
		PrintTree(tree);
		i++;
	}
	PrintTree(tree);
	fclose(f);
	return tree;
}

void WriteTree (TreeType tree, char *file) {
	FILE *f = fopen(file, "w");
	if (f == NULL) {
		perror("Invalid filename");
		exit(0);
	}
	int i;
	for (i = 0; i < MAXNUMQS; i++) {
		fputs(tree->nodes[i], f);
	}
	fclose(f);
}
void PrintTree (TreeType tree) {
	int i;
	for (i = 0; i < 5; i++) {
		printf("%d: %s\n", i, tree->nodes[i]);
	}
}
PositionType Top (TreeType tree) {
	PositionType top = (PositionType)malloc(sizeof(int) +  sizeof(PositionType));
	top->nodeIndex = 0;
	return top;
}
boolean IsLeaf (TreeType tree, PositionType pos) {
	char* mystring;
	mystring = tree->nodes[pos->nodeIndex];
	//printf("mystring is %s", mystring);
	if (mystring != NULL && strlen(mystring) > 0) {
		return TRUE;
	} else {
		return FALSE;
	}
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
    PrintTree(tree);
    printf("%s", "Think of an animal. I will try to guess what it is.\n"
		 "Please answer my questions with yes or no.\n");

    while (TRUE) {
        pos = Top (tree);
	//printf("pos is %d", pos->nodeIndex);
	//PrintTree(tree);

        while (!IsLeaf (tree, pos)) {
            pos = Answer(Question(tree,pos))? 
	       YesNode(tree,pos): NoNode(tree,pos);
        }
        if (Answer (Guess (tree, pos))) {
            //printf ("I got it right!\n");
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
