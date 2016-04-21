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
	int i;
	for(i = 0; i < MAXNUMQS; i++) {
		tree->nodes[i] = (char*) malloc(MAXSTR *sizeof(char));
	}
	char mystring[MAXSTR];
	i = 0;
	while (fgets (mystring, sizeof(mystring), f) != NULL) {
		//printf("FIRST input tree: %s at i 0", tree->nodes[0]);
		//printf("%s\n with i %d\n", mystring, i);
		if (mystring[strlen(mystring) - 1] == '\n') {
		    mystring[strlen(mystring) - 1] = '\0';
		}
		strcpy(tree->nodes[i], mystring);
		//printf("CHECK: FIRST input tree: %s at i 0 with string %s\n", tree->nodes[0], mystring);
		//printf("input tree: %s at i %d with string %s\n", tree->nodes[i], i, mystring);
		//PrintTree(tree);
		i++;
	}
	//PrintTree(tree);
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
		fputs("\n", f);
	}
	fclose(f);
}
void PrintTree (TreeType tree) {
	printf("PRINTTREE START: %s at i 0 with string\n", tree->nodes[0]);
	int i;
	for (i = 0; i < 10; i++) {
		printf("%d: %s\n", i, tree->nodes[i]);
	}
	printf("end call to print tree %s\n\n\n", tree->nodes[0]);
}
PositionType Top (TreeType tree) {
	PositionType top = (PositionType)malloc(sizeof(int) +  sizeof(PositionType));
	top->nodeIndex = 0;
	return top;
}
boolean IsLeaf (TreeType tree, PositionType pos) {
	char* mystring;
	mystring = tree->nodes[pos->nodeIndex];
	printf("mystring is %s\n", mystring);
	if (mystring != NULL && strlen(mystring) > 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}
/*
 *	Print the given question, read the user's answer; return true 
 *	if the answer supplied starts with"y", false otherwise.
 */
boolean Answer (char *q) {
	printf("inside answer");
	printf("%s\n", q);
	char ans;
	ans =fgetc(stdin);
	if (ans == 'y') {
		return TRUE;
	} else if (ans == 'n') {
		return FALSE;
	} else {
		printf("Please respond 'yes' or 'no'. Lets try again:");
		return Answer(q);
	}
}

/*
 *	Form a question out of the string stored at position pos in the given
 *	animal tree, and return the string that contains the question.
 */
char *Question (TreeType tree, PositionType pos) {
	//PrintTree(tree);
	printf("inside question");
	if (pos->nodeIndex == 0 && strlen(tree->nodes[pos->nodeIndex]) == 0) {
		char * dog = "Is it a dog?";
		printf("returning %s\n", dog);
		return dog;
	} else {
		//return "Is it a dog?";
		printf("returning %s\n", tree->nodes[pos->nodeIndex]);
		return tree->nodes[pos->nodeIndex];
	}
}
/*
 *	Form a guess out of the string stored at position pos in the given
 *	animal tree, and return the string that contains the guess. 
 *	(IsLeaf(tree, pos) must be true.)
 */
char *Guess (TreeType tree, PositionType pos) {
	//PrintTree(tree);
	printf("inside guess");
	if (pos->nodeIndex == 0 && strlen(tree->nodes[pos->nodeIndex]) == 0) {
		char * dog = "Is it a dog?";
		printf("returning %s\n", dog);
		return dog;
	} else {
		//return "Is it a dog?";
		printf("returning %s\n", tree->nodes[pos->nodeIndex]);
		return tree->nodes[pos->nodeIndex];
	}
}
/*
 *	Return the position of the node that corresponds to a "yes" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType YesNode (TreeType tree, PositionType pos) {
}
/*
 *	Return the position of the node that corresponds to a "no" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType NoNode (TreeType tree, PositionType pos) {
}

/*
 *	Replace the node at position pos in the given animal tree by a node
 *	containing the given new question whose left child (down the tree in 
 *	the "yes" direction) contains the string stored at position pos, and 
 *	whose right child contains the new answer newA. Example:
 *	It's ok to copy the guess from the old node into the new node, and
 *	to copy the new question into the guess node.
 */
void ReplaceNode (TreeType tree, PositionType pos, char *newA, char *newQ) {
}

/*
 *	Admit that you guessed wrong, ask the player what animal he/she was 
 *	thinking of, and return this in *newA.  Also ask for a question that 
 *	would be answered "yes" for what you guessed and "no" for what the 
 *	player was thinking of, and return this in *newQ.  (The node with
 *	your guess is at position pos in the tree.)
 */
void GetNewInfo (TreeType tree, PositionType pos, char **newA, char **newQ) {
	printf("I guessed wrong?! What animal are you thinking of?\n");
	*newA = malloc(MAXSTR * sizeof(char));
	fgets(*newA, MAXSTR, stdin);
	//printf("%s", *newA);
	//printf("Please, may I have a question that would be answered \"yes\" for a(n) %s and \"no\" for a(n) %s?\n", tree->nodes[pos->nodeIndex], *newA);
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
	printf("pos is %d\n", pos->nodeIndex);
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
