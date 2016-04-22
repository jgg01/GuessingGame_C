typedef int boolean;
#define FALSE 0
#define TRUE !FALSE
#define MAXSTR 80
#define MAXNUMQS 80
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "animal.h"

TreeType TreeAtI(char str[MAXNUMQS][MAXSTR], int loc) ;
void freeTree(TreeType tree) ;

/* initialize the tree from a file whose name is specified as a command line argument. Your program should check the result of the fopen function to ensure that a legal file name was specified. When your program exits, it should save the new tree to that file, after again checking to make sure the file is writable */
struct treeStruct {
	char *string;
	struct treeStruct *left, *right;
};
	
struct positionStruct {
	struct treeStruct* node;
};

TreeType InitTree (char *file) {
	FILE *f = fopen(file, "r");
	if (f == NULL) {
		perror("Invalid filename");
		exit(0);
	}
	TreeType tree = (TreeType) malloc(MAXSTR * sizeof(char*) + 2 * sizeof(TreeType));
	char mystring[MAXSTR];
	char nodes[MAXNUMQS][MAXSTR];
	int j;

	int i = 0;
	while (fgets (mystring, sizeof(mystring), f) != NULL) {
		if (mystring[strlen(mystring) - 1] == '\n') {
		    mystring[strlen(mystring) - 1] = '\0';
		}
		strcpy(nodes[i], mystring);
		i++;
	}
	while (i < MAXNUMQS) {
		strcpy(nodes[i], "\0");
		i++;
	}
	tree->string = (char*)malloc(MAXSTR * sizeof(char));
	strcpy(tree->string, nodes[0]);
	int k = 0;
	tree->left = TreeAtI(nodes,  2*k+1);
	tree->right = TreeAtI(nodes, 2*k+2);
	
	fclose(f);
	return tree;
}

TreeType TreeAtI(char str[MAXNUMQS][MAXSTR], int loc) {
	if (loc > MAXNUMQS) {
		return NULL;
	}
	TreeType tree = (TreeType) malloc(MAXSTR * sizeof(char*) + 2 * sizeof(TreeType));
	tree->string = (char*)malloc(MAXSTR * sizeof(char));
	strcpy(tree->string, str[loc]);
	tree->left = TreeAtI(str, 2*loc+1);
	tree->right = TreeAtI(str, 2*loc+2);
	return tree;
}

void WriteTree (TreeType tree, char *file) {
	FILE *f = fopen(file, "w");
	if (f == NULL) {
		perror("Invalid filename");
		exit(0);
	}
	char nodes[MAXNUMQS][MAXSTR];
	int k = 0;
	strcpy(nodes[k], tree->string);
	TreeType orig = tree;
	int i;
	for(i = 1; i < MAXNUMQS; i++) {
		strcpy(nodes[i], "\0");
	}
	while (tree->left != NULL && k < MAXNUMQS) {
		k = k * 2 + 1;
		strcpy(nodes[k], tree->left->string);
		tree = tree->left;
	}
	tree = orig;
	k = 0;
	while (tree->right != NULL && k < MAXNUMQS) {
		k = k * 2 + 2;
		strcpy(nodes[k], tree->right->string);
		tree = tree->right;
	}
	int j = 0;
	for (; j < MAXNUMQS; j++) {
		fputs(nodes[j], f);
		fputs("\n", f);
	}
	fclose(f);

	freeTree(tree);

}
/* Frees all allocated elements of tree */
void freeTree(TreeType tree) {
	if (tree != NULL) {
		freeTree(tree->left);
		freeTree(tree->right);
		free(tree->string);	
		free(tree);
	}
}

void PrintTree (TreeType tree) {
	printf("%s\n", tree->string);
	PrintTree(tree->left);
	PrintTree(tree->right);
}
PositionType Top (TreeType tree) {
	PositionType top = (PositionType)malloc(sizeof(int) +  sizeof(PositionType));
	top->node = tree;
	return top;
}

/*
 *	Return true exactly when pos is a "leaf" of the animal tree, that is,
 *	when the string stored at pos is a guess rather than a question.
 */
boolean IsLeaf (TreeType tree, PositionType pos) {
	char* mystring = (char*)malloc(MAXSTR * sizeof(char));
	mystring = tree->string;
	if (tree->left == NULL && tree->right == NULL) {
		return TRUE;
	} else if((strcmp(pos->node->left->string, "\0") == 0) && (strcmp(pos->node->right->string, "\0") == 0)) {
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
	printf("%s ", q);
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
	return pos->node->string;
}
/*
 *	Form a guess out of the string stored at position pos in the given
 *	animal tree, and return the string that contains the guess. 
 *	(IsLeaf(tree, pos) must be true.)
 */
char *Guess (TreeType tree, PositionType pos) {
	char * dog = (char*)malloc(MAXSTR * sizeof(char));
	strcpy(dog, "Is it a ");
	char * ans = (char*)malloc(MAXSTR * sizeof(char));
	strcpy(ans, pos->node->string);
	strcat(dog, ans);
	strcat(dog, "? ");
	return dog;
}
/*
 *	Return the position of the node that corresponds to a "yes" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType YesNode (TreeType tree, PositionType pos) {
	pos->node = pos->node->left;
	return pos;
}
/*
 *	Return the position of the node that corresponds to a "no" answer
 *	to the question stored at position pos in the animal tree.
 */
PositionType NoNode (TreeType tree, PositionType pos) {
	pos->node = pos->node->right;
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
	char *oldguess = malloc(MAXSTR * sizeof(char));
	strcpy(oldguess, pos->node->string);
	strcpy(pos->node->string, newQ);
	strcpy(pos->node->left->string, oldguess);
	strcpy(pos->node->right->string, newA);
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
	printf("Please, may I have a question that would be answered \"yes\" for %s and \"no\" for a(n) %s? ", pos->node->string, *newA);
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
