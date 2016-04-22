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
		if (mystring[strlen(mystring) - 1] == '\n') {
		    mystring[strlen(mystring) - 1] = '\0';
		}
		strcpy(tree->nodes[i], mystring);
		i++;
	}
	fclose(f);
	//PrintTree(tree);
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
	for (i = 0; i < MAXNUMQS; i++) {
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
	printf("examining %s at position %d", mystring, pos->nodeIndex);
	char * yes = tree->nodes[YesNode(tree, pos)->nodeIndex];
	char * no = tree->nodes[NoNode(tree, pos)->nodeIndex];
	if ((strcmp(yes, "\0") == 0) && (strcmp(no, "\0") == 0)) {
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
	printf("%s", q);
	char ans;
	char mystring[MAXSTR];
	ans =fgetc(stdin);
	fgets(mystring, MAXSTR, stdin);
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
	return tree->nodes[pos->nodeIndex];
}
/*
 *	Form a guess out of the string stored at position pos in the given
 *	animal tree, and return the string that contains the guess. 
 *	(IsLeaf(tree, pos) must be true.)
 */
char *Guess (TreeType tree, PositionType pos) {
	if (pos->nodeIndex == 0 && strlen(tree->nodes[pos->nodeIndex]) == 0) {
		char * dog = "Is it a dog?";
		return dog;
	} else {
		char * dog = (char*)malloc(MAXSTR * sizeof(char));
		strcpy(dog, "");
		char * ans = (char*)malloc(MAXSTR * sizeof(char));
		strcpy(ans, tree->nodes[pos->nodeIndex]);
		strcat(dog, ans);
		return dog;
	}
}
/*
 *	Return the position of the node that corresponds to a "yes" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType YesNode (TreeType tree, PositionType pos) {
	printf("curr pos = %d\n", pos->nodeIndex);
	int index = pos->nodeIndex * 2 + 1;
	pos->nodeIndex = index;
	printf("new pos = %d\n", pos->nodeIndex);
	return pos;
}
/*
 *	Return the position of the node that corresponds to a "no" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType NoNode (TreeType tree, PositionType pos) {
	int index = pos->nodeIndex * 2 + 2;
	pos->nodeIndex = index;
	return pos;
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
	PrintTree(tree);
	char *oldguess = malloc(MAXSTR * sizeof(char));
	strcpy(oldguess, tree->nodes[pos->nodeIndex]);
	strcpy(tree->nodes[pos->nodeIndex], newQ);
	strcpy(tree->nodes[YesNode(tree, pos)->nodeIndex], oldguess);
	strcpy(tree->nodes[NoNode(tree, pos)->nodeIndex], newA);
	PrintTree(tree);
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
	char *mystring;
	mystring = *newA;
	if (mystring[strlen(mystring) - 1] == '\n') {
	    mystring[strlen(mystring) - 1] = '\0';
	}
	printf("Please, may I have a question that would be answered \"yes\" for %s and \"no\" for a(n) %s? ", tree->nodes[pos->nodeIndex], *newA);
	*newQ = malloc(MAXSTR * sizeof(char));
	fgets(*newQ, MAXSTR, stdin);
	mystring = *newQ;
	if (mystring[strlen(mystring) - 1] == '\n') {
	    mystring[strlen(mystring) - 1] = '\0';
	}
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
	//PrintTree(tree);
	//printf("heading to isleaf");
        while (!IsLeaf (tree, pos)) {
            pos = Answer(Question(tree,pos))? 
	       YesNode(tree,pos): NoNode(tree,pos);
        }
	//printf("OUtside isleaf");
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
