/*
 *Author: Raul Valencia
 *Small CMD Manager
*/

#include "listnode.h"
#include "exec.h"

/* TypeDef*/
typedef char String[10000];

/* Structure For History*/
typedef struct Historian {
	String argument;
	struct Historian *next;
}Historian;

/* Function Prototypes*/

/* Change Directories*/
void quit(Historian *headhist, ListNode *headList);
void freeHist(Historian *head, Historian * Node);
int decide(char *command);
void cd(char *word);

/* History*/
Historian * makeNode(Historian *head, char *pointer);
void insertHist(Historian *head, Historian *newNode, int *counter);
int isEmpty(char *buffer);
void clearHistory(Historian *head, int *count);
void printHistory(Historian *head);
int isNum(char *buffer);
int isEmpty(char *buffer);

/* New ListNode*/
void freeListNode(ListNode *listNode);
ListNode * makeListNode(char *buffer, Historian *histNode, ListNode *listNode, int *id);
void insertListNode(ListNode *headList, ListNode *newNode);
void printListNode(ListNode *head);

/* For open*/
void openFile(char *buffer, Historian *headHist, ListNode *headList);

/* For execute*/
ListNode * findNode(ListNode * head, int index);

/* Functions*/

/* Quits and frees everything*/
void quit(Historian *headhist, ListNode *headList) {
	if (headhist != NULL) {
		freeHist(headhist, NULL);
	}
	if (headList != NULL) {
		freeListNode(headList);
	}
	exit(0);
}

/* Checks if the creation of a Node returns null*/
void freeHist(Historian *head, Historian * Node) {

	Historian *curr = head->next;
	Historian *temp;

	while (curr != NULL) {
		temp = curr;
		curr = curr->next;
		free(temp);
	}

	free(head);
}

/* Depending on command returns a number*/
int decide(char *command) {
	if (strcmp(command, "quit") == 0) {
		return 1;
	}

	if (strcmp(command, "cd") == 0) {
		return 2;
	}

	if (strcmp(command, "history") == 0) {
		return 3;
	}

	if (strcmp(command, "new") == 0) {
		return 4;
	}

	if (strcmp(command, "list") == 0) {
		return 5;
	}

	if (strcmp(command, "open") == 0) {
		return 6;
	}

	if (strcmp(command, "execute") == 0) {
		return 7;
	}

	return 0;
}

/* Changes Directories*/
void cd(char *word) {
	/* Tokenizes string by copying it to another one*/
	String dir;
	strcpy(dir, word);
	char *tokens = strtok(dir, " ");
	int counter = 0;

	/* Goes through each token!*/
	while (tokens != NULL) {
		tokens = strtok(NULL, " ");
		counter += 1;
	}

	/* if there is too many arguments then it returns*/
	if (counter > 1) {
		fprintf(stderr, "error: too many arguments provided\n");
		return;
	}

	/* Changes directory, if there is an issue then it gets */
	if (chdir(word) != 0) {
		fprintf(stderr, "error:%s\n", strerror(errno));
	}
}

/* Checks if a string is an empty line*/
int isEmpty(char *buffer) {
	/* if line contains null byte, then its an empty line*/
	if (buffer[0] == '\0') {
		return 1;
	}

	int buffLen = strlen(buffer);

	/* Checks for whitespace only*/
	for (int i=0; i < buffLen; i++) {
		if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\r' && buffer[i] != '\n') {
			return 0;
		}
	}
	return 1;
}

/* Makes a new node for history*/
Historian * makeNode(Historian *head, char *pointer) {
	Historian *newNode = malloc(sizeof(Historian));
	/* Checks if the new node is null*/
	if (!newNode){freeHist(head, newNode); exit(1);}

	/* Copies argument!*/
	strcpy(newNode->argument, pointer);
	newNode->next = NULL;

	/* returns new node*/
	return newNode;
}

/* A version of a queue list, First one in, first one out
 * Once it reaches the limit of 99 arguments, first arg is gone, and
 * new argument is added at the end
 */
void insertHist(Historian *head, Historian *newNode, int *counter) {
	/* Reaching limit, First in, first our*/
	if (*counter > 99) {
		Historian *first = head->next;
		Historian *second = (head->next)->next;

		head->next = second;
		free(first);
	}

	/* Normal adding node to the end*/
	Historian *previus = head;
	Historian *current = head->next;

	while (current != NULL) {
		previus = current;
		current = current->next;
	}

	previus->next = newNode;

	/* adds to the counter how many have been added*/
	*counter += 1;
}

/* Prints all history of the linked list!*/
void printHistory(Historian *head) {
	if (!head) {return;}
	Historian *curr = head->next;
	int count = 0;

	while(curr != NULL) {
		printf("%d: %s", count, curr->argument);
		count++;
		curr = curr->next;
	}
}

/* Clears everything in the history!*/
void clearHistory(Historian *head, int *count) {
	Historian * curr = head->next;
	Historian * temp;

	while(curr != NULL) {
		temp = curr;
		curr = curr->next;
		free(temp);
	}

	head->next = NULL;
	*count = 0;
}

/* Checks if a string is a number*/
int isNum(char *buffer) {
	int buffLen = strlen(buffer);

	for (int i=0; i < buffLen; i++) {
		if (!(buffer[i] >= '0' && buffer[i] <= '9')) {return 0;}
	}
	return 1;
}

/* Checks if index is in the history list, if it is, returns argument
 * else NULL
 */
char * isInHist(char *buffer, Historian *head) {
	int index;
	int count = 0;
	sscanf(buffer, "%d", &index);

	Historian *curr = head->next;

	while (curr != NULL && count < index) {
		curr = curr->next;
		count++;
	}

	if (!curr){return NULL;}
	return curr->argument;
}

/* Frees List Node*/
void freeListNode(ListNode *listNode) {
	ListNode *curr = listNode->next;
	ListNode *temp;
	int argLength;


	while (curr != NULL) {
		temp = curr;
		curr = curr->next;
		argLength = temp->arguments_length;
		for (int i=0; i < argLength; i++) {
			free(temp->arguments[i]);
		}
		free(temp->arguments);
		free(temp->command);
		free(temp->file_contents);
		free(temp);
	}

	free(listNode);
}

/* Makes a List node while handling cases for NULL*/
ListNode * makeListNode(char *buffer, Historian *histNode, ListNode *listNode, int *id) {
	ListNode * newNode = malloc(sizeof(ListNode));
	if (!newNode) {
		freeHist(histNode, NULL);
		freeListNode(listNode);
		exit(1);
	}

	/* For command*/
	String command;
	sscanf(buffer, "new %s ", command);
	int commandlen = strlen(command);
	newNode->command = malloc(sizeof(char) * (commandlen + 1));
	if (!commandlen) {
		free(newNode);
		freeHist(histNode, NULL);
		freeListNode(listNode);
		exit(1);
	}
	strcpy(newNode->command, command);

	/* For arguments*/
	String args;
	sscanf(buffer, "new %[^\n]", args);

	/* Checks how many arguments there are in here*/
	String tokens;
	strcpy(tokens, args);
	char * countingTokens = strtok(tokens, " ");
	int lenOfArgs = 0;

	while (countingTokens != NULL) {
		countingTokens = strtok(NULL, " ");
		lenOfArgs++;
	}

	/* Resets tokens to original and tries to add to each argument*/
	strcpy(tokens, args);
	countingTokens = strtok(tokens, " ");

	newNode->arguments = malloc(sizeof(char *) * (lenOfArgs+1));
	if (newNode->arguments == NULL) {
		free(newNode->command);
		free(newNode);
		freeHist(histNode, NULL);
		freeListNode(listNode);
		exit(1);
	}

	for (int i=0; i<lenOfArgs; i++) {
		(newNode->arguments)[i] = malloc(sizeof(char) * 10000);
		if ((newNode->arguments)[i] == NULL) {
			for (int j=0; j<i; j++) {
				free((newNode->arguments)[j]);
			}
			free(newNode->arguments);
			free(newNode->command);
			free(newNode);
			freeHist(histNode, NULL);
			freeListNode(listNode);
			exit(1);
		}
		strcpy(newNode->arguments[i], countingTokens);
		countingTokens = strtok(NULL, " ");
	}
	/* Adds NULL to the end*/
	newNode->arguments[lenOfArgs] = NULL;

	newNode->arguments_length = lenOfArgs;

	newNode->file_contents = NULL;
	newNode->next = NULL;
	newNode->id= *id;
	(*id)++;
	return newNode;
}

/* Insert List Node*/
void insertListNode(ListNode *headList, ListNode *newNode) {
	ListNode *prev = headList;
	ListNode *curr = headList->next;

	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
	}

	prev->next = newNode;
}

/* Print the list node*/
void printListNode(ListNode *head) {
	int counter = 0;
	ListNode *curr = head->next;
	ListNode *prev= head;

	/* Goes thorugh the whole list*/
	while(curr != NULL) {
		prev = curr;

		if (!(prev->file_contents)) {
			printf("List Node %d\n"
				   "\tCommand: %s\n"
				   "\tFile Contents:\n", counter, prev->command);
		}

		/* In case file contents is not empty*/
		else {
			printf("List Node %d\n"
				   "\tCommand: %s\n"
				   "\tFile Contents:\n", counter, prev->command);

			String lines;
			strcpy(lines, prev->file_contents);
			char * tokens = strtok(lines, "\n");

			while (tokens != NULL) {
				printf("\t\t%s\n", tokens);
				tokens = strtok(NULL, "\n");
			}
		}

		curr = curr->next;
		counter++;
	}

}

/* Opens a file*/
void openFile(char *buffer, Historian *headHist, ListNode *headList) {
	/* Tokenizes the strings and checks for arguments*/
	String arg;
	int counter = 0;
	strcpy(arg, buffer);

	char * tokens = strtok(arg, " ");
	while( tokens != NULL) {
		tokens = strtok(NULL, " ");
		counter++;
	}

	/* If an argument doesnot equal 3, then it has the wrong number of arguments*/
	if (counter != 3){fprintf(stderr, "error: incorrect number of arguments\n"); return;}
	strcpy(arg, buffer);
	tokens = strtok(arg, " ");
	for (int i = 0; i < counter-1; i++) {
		tokens = strtok(NULL, " ");
	}

	/* We open the file, and if it exists then it will not return NULL*/
	FILE * file = fopen(tokens, "r");
	if (!file){fprintf(stderr, "error: file cannot be opened\n");}

	/* Searches the Node to which they want to copy file contents to*/
	int num;
	strcpy(arg, buffer);
	sscanf(arg, "open %d ", &num);

	ListNode *curr = headList->next;
	for (int i=0; i<num; i++) {
		if (!curr) {
			break;
		}
		curr = curr->next;
	}
	/* If the index is not valid then it will return NULL*/
	if (!curr){ fclose(file); fprintf(stderr, "error: %s\n", "Id does not exist"); return;}

	/* We use fseek() to search for the size of the file*/
	fseek(file, 0, SEEK_END);
	int size = ftell(file);

	char * complete= malloc(sizeof(char) * (size+1));
	if (!complete) {
		freeHist(headHist, NULL);
		freeListNode(headList);
		fclose(file);
		exit(1);
	}

	/* Read the file and copy its contents*/
	fseek(file, 0, SEEK_SET);
	int bytesRead = fread(complete, sizeof(char), size, file);

	complete[bytesRead] = '\0';
	curr->file_contents = complete;
	fclose(file);
}

/* Finds Node for execute command, if it exists then it returns that node, else it doesn't*/
ListNode * findNode(ListNode * head, int index) {
	ListNode *curr = head->next;

	for (int i=0; i < index; i++) {
		if (curr == NULL) {
			fprintf(stderr, "error: %s\n", "Id does not exist");
			return NULL;
		}
		curr = curr->next;
	}

	/* If arguments are higher than 127 then too many aarguments where given*/
	if (curr->arguments_length > 127) {
		fprintf(stderr, "error: %s\n", "too many arguments");
		return NULL;
	}

	return curr;
}

int main() {
	String buffer;
	String command;

	/*History Head initialization*/
	Historian *dummy_historian = malloc(sizeof(Historian));
	if (!dummy_historian){exit(1);}
	dummy_historian->argument[0] = '\0';
	dummy_historian->next = NULL;
	int queuesize = 0;
	int runHist = 0;

	/*ListNode head initialization*/
	ListNode *dummy_listNode = malloc(sizeof(Historian));
	if (!dummy_listNode){free(dummy_historian); exit(1);}
	/* Stack initiation*/
	char * commandList = " ";
	char * argsList[1];
	char * argu = " ";
	argsList[0] = argu;
	char * fileList = " ";
	/* Setting dummy values*/
	dummy_listNode->command = commandList;
	dummy_listNode->arguments =  argsList;
	dummy_listNode->next = NULL;
	dummy_listNode->id = -1;
	dummy_listNode->arguments_length= -1;
	dummy_listNode->file_contents = fileList;
	int index =0;

	while(1) {
		/* If check if it asks for input or not, depending if they ran history [index] correctly*/
		if (!runHist) {
			printf("SCMD$");
			fflush(stdout);
			if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
				quit(dummy_historian, dummy_listNode);
			}
		}

		/* If buffer is an empty line then it skips*/
		if (isEmpty(buffer)) {continue;}
		sscanf(buffer, "%s ", command);

		/* Resets the flag for checking if history [index] was ran*/
		runHist = 0;

		/* Check for command inputted, and decides which command to use */
		switch(decide(command)) {
			/* for quit*/
			case 1:
				quit(dummy_historian, dummy_listNode);
				exit(0);

			/* for cd*/
			case 2: {
				String arg;
				/* Insert to history list*/
				insertHist(dummy_historian, makeNode(dummy_historian, buffer), &queuesize);
				sscanf(buffer, "cd %[^\n]", arg);
				cd(arg);
				break;
			}

			/* for history*/
			case 3: {
				String arg;
				arg[0] = '\0';
				sscanf(buffer, "history %s ", arg);

				/* for -c*/
				if (strcmp(arg, "-c") == 0) {
					clearHistory(dummy_historian, &queuesize);
					break;
				}

				/* If arguments are empty then it wants to print the list*/
				if (arg[0] == '\0') {
					printHistory(dummy_historian);
					break;
				}

				/* If argument is a number then it wants to run the command at specific
				 * history index
				 */
				if (isNum(arg)) {
					char *histArg = isInHist(arg, dummy_historian);
					if (histArg) {
						strcpy(buffer, histArg);
						runHist = 1;
						break;
					}
					fprintf(stderr, "error: %s\n", "Index in history list does not exist");
				}
				break;
			}

			/* For new keyword*/
			case 4: {
				/* As always we add it to history*/
				insertHist(dummy_historian, makeNode(dummy_historian, buffer), &queuesize);
				String arg;
				arg[0] = '\0';
				sscanf(buffer, "new %s", arg);


				if (arg[0] != '\0') {
					insertListNode(dummy_listNode, makeListNode(buffer, dummy_historian, dummy_listNode, &index));
					break;
				}
				fprintf(stderr, "error: too few arguments provided\n");
				break;
			}

			/* for list command*/
			case 5:
				insertHist(dummy_historian, makeNode(dummy_historian, buffer), &queuesize);
				printListNode(dummy_listNode);
				break;

			/* for open command*/
			case 6: {
				insertHist(dummy_historian, makeNode(dummy_historian, buffer), &queuesize);
				String arg;
				strcpy(arg, buffer);
				if(arg[strlen(arg) - 1] == '\n'){arg[strlen(arg) - 1] = '\0';}
				openFile(arg, dummy_historian, dummy_listNode);
				break;
			}

			/* for execute command*/
			case 7: {
				insertHist(dummy_historian, makeNode(dummy_historian, buffer), &queuesize);
				String arg;
				sscanf(buffer, "execute %[^\n]", arg);
				String tokenized;
				strcpy(tokenized, arg);
				char *token = strtok(tokenized, " ");
				int argsNum = 0;

				while (token != NULL) {
					token = strtok(NULL, " ");
					argsNum++;
				}
				/* Exactly 1 argument*/
				if (argsNum != 1) {fprintf(stderr, "error: incorrect number of arguments\n"); continue;}
				int executeInd;
				sscanf(buffer, "execute %d ", &executeInd);

				/* Finds index node and checks it*/
				ListNode *trialNode = findNode(dummy_listNode, executeInd);
				if (trialNode) {
					/* Shifts it to the right and gets bits from 8 to 15*/
					int result = run_command(trialNode);
					result = result >> 8;
					result = result & 0xff;

					if (result) {
						fprintf(stderr, "error: %s\n", strerror(result));
					}
				}
				break;
			}

			default:
				/* Adds wrong arugments to the list*/
				insertHist(dummy_historian, makeNode(dummy_historian, buffer), &queuesize);
				break;
		}
	}
}
