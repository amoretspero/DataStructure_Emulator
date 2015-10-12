#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma warning(disable : 4996)

char* distline = "\n==============================\n";
char* intromsg = "\nStack Emulator\n******************************\n\n\
This is stack emulator for practicing basic data structures.\n\
Emulator supports various functions for using 'in' stack,\n\
and for 'between' stacks.\n\
Type 'help' to see help message.\n\
Code is open for anyone who wants to use.\n\
\n\
\n******************************\n";

char* helpmsg = "General Help Message\n******************************\n\
In stack :\n\
push n : pushes element n(>0) into stack.\n\
pop : pops one element from stack.\n\
empty : empties current stack.\n\
print : prints current stack.\n\
current : prints currnet stack's info.\n\
exit : exits this emulator.\n\
help : prints this message.\n\
\n\
Between stacks :\n\
stack create (name) : creates stack named 'name'\n\
stack delete (name) : deletes stack named 'name'\n\
stack change (name) : change current stack to stack named 'name'\n\
stack print : prints out all of the stacks.\n\
******************************\n\n\
Need more help? Contact amoretspero@gmail.com\n\n";

bool stdin_err = false;
void stack_print(struct stack* st, bool allstack);
void stack_init(struct stack* st, char* str);

char * getline(void) {
	char * line = (char*)malloc(100), *linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;

	if (line == NULL)
		return NULL;

	for (;;) {
		c = fgetc(stdin);
		if (c == EOF)
			break;

		if (--len == 0) {
			len = lenmax;
			char * linen = (char*)realloc(linep, lenmax *= 2);

			if (linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if ((*line++ = c) == '\n')
			break;
	}
	*(line - 1) = '\0';
	return linep;
}

char* trim_whitespace(char* str)
{
	char* res = (char*)malloc(sizeof(char) * 255);
	int cnt = 0;
	int len = strlen(str);
	//printf("str : %s, len : %d\n", str, len);
	while (str[cnt] == ' ' && cnt < len)
	{
		cnt++;
	}
	char* moved = str + cnt;
	res = strcpy(res, moved);
	//printf("length of \"%s\" : %d\n", res, strlen(res));
	return res;
}

char* makelowercase(char* str)
{
	int len = strlen(str);
	char* res = (char*)malloc(sizeof(char)*(len + 1));
	int cnt = 0;
	for (cnt = 0; cnt <= len; cnt++)
	{
		if ((int)str[cnt] >= 65 && (int)str[cnt] <= 90)
		{
			res[cnt] = (char)((int)str[cnt] + 32);
		}
		else
		{
			res[cnt] = str[cnt];
		}
	}
	return res;
}

struct node
{
	int elem;
	struct node* next;
	struct node* prev;
};

struct stack
{
	struct node* base;
	struct node* top;
	char* name;
	struct stack* next;
	struct stack* prev;
};

struct stack_node
{
	struct stack* st;
	struct stack* next;
	struct stack* prev;
};

struct stackList
{
	struct stack* base;
	struct stack* top;
};

struct stackList* stacklist;
struct stack* curr_stack;

void stackListInit()
{
	struct stackList* newstl = (stackList*)malloc(sizeof(stackList) * 1);
	stacklist = newstl;
	stacklist->base = NULL;
	stacklist->top = NULL;
	curr_stack = NULL;
}

size_t get_stackList_size()
{
	size_t cnt = 0;
	struct stack* iterator = stacklist->base;
	while (iterator != NULL)
	{
		cnt++;
		iterator = iterator->next;
	}
	return cnt;
}

void stack_change(char* str)
{
	if (get_stackList_size() <= 0)
	{
		printf("Stack List is empty! Please create stack first.\n");
		printf("%s", distline);
	}
	else
	{
		bool found = false;
		struct stack* iterator = stacklist->base;
		while (iterator != NULL)
		{
			if (strcmp(iterator->name, str) == 0)
			{
				curr_stack = iterator;
				printf("Current stack changed to %s\n", iterator->name);
				printf("%s", distline);
				found = true;
				break;
			}
			iterator = iterator->next;
		}
		if (!found)
		{
			printf("%s : Stack not found.\n", str);
			printf("%s", distline);
		}
	}
}

void stack_delete(char* str)
{
	if (get_stackList_size() <= 0)
	{
		printf("Stack List is empty! Please create stack first.\n");
		printf("%s", distline);
	}
	else
	{
		struct stack* iterator = stacklist->base;
		bool deleted = false;
		while (iterator != NULL && !deleted)
		{
			if (strcmp(iterator->name, str) == 0)
			{
				if (iterator == curr_stack)
				{
					char* yesorno = (char*)malloc(sizeof(char) * 1);
					printf("Delete current stack...? [Y/N] ");
					scanf("%s", yesorno);
					stdin_err = true;
					if (strcmp(yesorno, "Y") == 0 || strcmp(yesorno, "y") == 0)
					{
						printf("Deleting current stack (%s). If there is no other stack, current stack will be NULL, otherwise, first stack on Stack List.\n", iterator->name);
						if (get_stackList_size() == 1)
						{
							curr_stack = NULL;
							stacklist->base = NULL;
							stacklist->top = NULL;
							free(iterator);
							deleted = true;
						}
						else
						{
							if (curr_stack == stacklist->base)
							{
								curr_stack = curr_stack->next;
								curr_stack->prev = NULL;
								stacklist->base = curr_stack;
								free(iterator);
								deleted = true;
							}
							else if (curr_stack == stacklist->top)
							{
								curr_stack->prev->next = NULL;
								curr_stack = stacklist->base;
								stacklist->top = iterator->prev;
								free(iterator);
								deleted = true;
							}
							else
							{
								curr_stack = stacklist->base;
								iterator->prev->next = iterator->next;
								iterator->next->prev = iterator->prev;
								free(iterator);
								deleted = true;
							}
						}
					}
					else
					{
						printf("Not deleting current stack (%s)\n", iterator->name);
						deleted = true;
					}
				}
				else
				{
					printf("Now deleting stack %s...\n", iterator->name);
					if (iterator->prev != NULL)
					{
						iterator->prev->next = iterator->next;
					}
					else
					{
						stacklist->base = iterator->next;
					}
					if (iterator->next != NULL)
					{
						iterator->next->prev = iterator->prev;
					}
					else
					{
						stacklist->top = iterator->prev;
					}
					free(iterator);
					deleted = true;
				}
			}
			if (!deleted)
			{
				iterator = iterator->next;
			}
		}

		if (deleted)
		{
			printf("Delete successful.\n");
			printf("%s", distline);
		}
		else
		{
			printf("Can't delete stack \"%s\". The stack may not exists. Delete unsuccessful.\n", str);
			printf("%s", distline);
		}
	}
}

void stack_create(char* str)
{
	struct stack* newstack = (stack*)malloc(sizeof(stack));
	stack_init(newstack, str);
	if (stacklist->top == NULL)
	{
		stacklist->base = newstack;
		stacklist->top = newstack;
	}
	else
	{
		stacklist->top->next = newstack;
		newstack->prev = stacklist->top;
		stacklist->top = newstack;
	}
	curr_stack = newstack;
	printf("Created stack %s\n", str);
	printf("%s", distline);
}

void stackList_print()
{
	struct stack* iterator = stacklist->base;
	if (iterator == NULL)
	{
		printf("There is no stack! Please create one first.\n");
	}
	else
	{
		while (iterator != NULL)
		{
			stack_print(iterator, true);
			printf("\n");
			iterator = iterator->next;
		}
	}
	printf("%s", distline);
}

void stack_init(struct stack* st, char* str)
{
	printf("Now Initializing Stack %s...\n\n", str);
	st->base = NULL;
	st->top = NULL;
	st->name = str;
	st->prev = NULL;
	st->next = NULL;
	printf("Initialize Complete.\n");
	//printf("%s", distline);
}

struct node* elem_gen(int n)
{
	struct node* res = (node*)malloc(sizeof(node));
	res->elem = n;
	res->next = NULL;
	res->prev = NULL;
	return res;
}

void stack_push(struct stack* st, int n)
{
	struct node* elem = elem_gen(n);
	if (st->base == NULL)
	{
		st->base = elem;
		st->top = elem;
		printf("Pushed Element %d Successful.\n", elem->elem);
	}
	else
	{
		st->top->next = elem;
		elem->prev = st->top;
		st->top = elem;
		printf("Pushed Element %d Successful.\n", elem->elem);
	}
	printf("%s", distline);
}

int stack_pop(struct stack* st)
{
	if (st->base == NULL)
	{
		printf("Stack is EMPTY\n");
		printf("%s", distline);
		return -1;
	}
	else
	{
		struct node* res = st->top;
		if (st->top->prev != NULL)
		{
			struct node* prev = st->top->prev;
			st->top->prev->next = NULL;
			st->top = prev;
			int res_elem = res->elem;
			free(res);
			printf("Popped Element %d Successful.\n", res_elem);
			printf("%s", distline);
			return res_elem;
		}
		else
		{
			int res_elem = st->top->elem;
			free(st->top);
			st->top = NULL;
			st->base = NULL;
			printf("Popped Element %d Successful.\n", res_elem);
			printf("%s", distline);
			return res_elem;
		}
	}
}

size_t get_size(struct stack* st)
{
	struct node* iterator = st->base;
	size_t cnt = 0;
	while (iterator != NULL)
	{
		cnt++;
		iterator = iterator->next;
	}
	return cnt;
}

bool is_empty(struct stack* st)
{
	size_t stack_size = get_size(st);
	if (stack_size == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void stack_empty(struct stack* st)
{
	if (!is_empty(st))
	{
		struct node* iterator = st->top;
		while (iterator->prev != NULL)
		{
			iterator = iterator->prev;
			free(iterator->next);
		}
		free(iterator);
		st->top = NULL;
		st->base = NULL;
	}
	printf("Empty stack done.\n");
	printf("%s", distline);
}

void stack_size(struct stack* st)
{
	printf("Stack size is : %d\n", get_size(st));
	printf("%s", distline);
}

void stack_print(struct stack* st, bool allstack)
{
	char* boundary = "---------------";
	printf("**%s**\n", st->name);
	printf("%s\n", boundary);
	if (is_empty(st))
	{
		printf("     EMPTY     \n");
		printf("%s\n", boundary);
	}
	else
	{
		struct node* iterator = st->top;
		while (iterator != NULL)
		{
			printf(" %d ", iterator->elem);
			if (iterator == st->top)
			{
				printf(" <- top\n");
			}
			else if (iterator == st->base)
			{
				printf(" <- base\n");
			}
			else
			{
				printf("\n");
			}
			printf("%s\n", boundary);
			iterator = iterator->prev;
		}
	}
	if (!allstack)
	{
		printf("%s", distline);
	}
}

void stack_info_current()
{
	printf("Current stack : %s\n", curr_stack->name);
	printf("    size : %d\n", get_size(curr_stack));
	if (curr_stack->top != NULL)
	{
		printf("    top : %d\n", curr_stack->top->elem);
	}
	else
	{
		printf("    top : (null)\n");
	}
	if (curr_stack->base != NULL)
	{
		printf("    base : %d\n", curr_stack->base->elem);
	}
	else
	{
		printf("    base : (null)\n");
	}
	printf("%s", distline);
}

char* parser_2nd(char* str)
{
	//printf("parser_2nd - str : %s\n", str);
	char* res = strtok(NULL, " ");
	//printf("parser_2nd - res : %s\n", res);
	if (res != NULL)
	{
		int num = atoi(res);
		char* return_value = (char*)malloc(sizeof(char) * 16);
		if (num > 0 && num < 1073741825)
		{
			sprintf(return_value, "%d", num);
			//printf("parser_2nd - return_value : %s\n", return_value);
			return return_value;
		}
		else
		{
			printf("Please insert valid number(0 ~ 2^30).\n");
			return NULL;
		}
	}
	else
	{
		printf("Please insert valid unmber(0 ~ 2^30).\n");
		return NULL;
	}
}

char* parser_3rd(char* str)
{
	char* res = strtok(NULL, " ");
	if (res != NULL)
	{
		return res;
	}
	else
	{
		return NULL;
	}
}

char* parser_2nd_stackcmd(char* str, char** result)
{
	char* res = makelowercase(strtok(NULL, " "));
	if (res != NULL)
	{
		if (strcmp(res, "delete") == 0)
		{
			char* res_3rd = parser_3rd(res);
			if (res_3rd != NULL)
			{
				result[1] = res;
				result[2] = res_3rd;
				return res;
			}
			else
			{
				return NULL;
			}
		}
		else if (strcmp(res, "create") == 0)
		{
			char* res_3rd = parser_3rd(res);
			if (res_3rd != NULL)
			{
				result[1] = res;
				result[2] = res_3rd;
				return res;
			}
			else
			{
				return NULL;
			}
		}
		else if (strcmp(res, "change") == 0)
		{
			char* res_3rd = parser_3rd(res);
			if (res_3rd != NULL)
			{
				result[1] = res;
				result[2] = res_3rd;
				return res;
			}
			else
			{
				return NULL;
			}
		}
		else if (strcmp(res, "print") == 0)
		{
			if (res != NULL)
			{
				result[1] = res;
				return res;
			}
			else
			{
				return NULL;
			}

		}
		else
		{
			return NULL;
		}
	}
	else
	{
		printf("Insert valid stack option.\n");
		return NULL;
	}
}

char** parser_1st(char* str)
{
	//printf("parser_1st : %s\n", str);
	char* str_origin = (char*)malloc(sizeof(char) * 255);
	strcpy(str_origin, (const char*)str);
	char** result = (char**)malloc(sizeof(char*) * 3);
	char* res = makelowercase(strtok(str, " "));
	//printf("res : %s, length : %d, str : %s\n", res, strlen(res), str);
	if (strcmp(res, "push") == 0)
	{
		//printf("Push detected\n");
		result[0] = res;
		//res = strtok(NULL, " ");
		//printf("res : %s\n", res);
		char* res_2nd = parser_2nd(res);
		//printf("res_2nd : %s\n", res_2nd);
		if (res_2nd != NULL)
		{
			result[1] = res_2nd;
			return result;
		}
		else
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
	}
	else if (strcmp(res, "pop") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "print") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "empty") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "size") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "help") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "exit") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "current") == 0)
	{
		result[0] = res;
		res = strtok(NULL, " ");
		if (res != NULL)
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}
		else
		{
			return result;
		}
	}
	else if (strcmp(res, "stack") == 0)
	{
		result[0] = res;
		char* res_2nd_stack = parser_2nd_stackcmd(res, result);
		if (res_2nd_stack != NULL)
		{
			return result;
		}
		else
		{
			printf("%s : Command not found\n", str_origin);
			return NULL;
		}

	}
	else
	{
		printf("%s : Command not found\n", str_origin);
		return NULL;
	}
}

void emulator()
{
	bool stop_emulator = false;
	struct stack* st = curr_stack;
	while (!stop_emulator)
	{
		if (!stdin_err)
		{
			printf("\ncommand > ");
		}
		if (stdin_err)
		{
			stdin_err = false;
		}
		char* cmdline = (char*)malloc(sizeof(char) * 255);
		cmdline = trim_whitespace(getline());
		char** parser_result;
		if (strcmp(cmdline, "\n") == 0 || strlen(cmdline) <= 0)
		{
			parser_result = NULL;
		}
		else
		{
			parser_result = parser_1st(cmdline);
		}
		printf("\n");
		if (parser_result != NULL && parser_result[0] != NULL)
		{
			if (strcmp(parser_result[0], "push") == 0 && curr_stack != NULL)
			{
				stack_push(curr_stack, atoi(parser_result[1]));
			}
			else if (strcmp(parser_result[0], "pop") == 0 && curr_stack != NULL)
			{
				stack_pop(curr_stack);
			}
			else if (strcmp(parser_result[0], "print") == 0 && curr_stack != NULL)
			{
				stack_print(curr_stack, false);
			}
			else if (strcmp(parser_result[0], "empty") == 0 && curr_stack != NULL)
			{
				stack_empty(curr_stack);
			}
			else if (strcmp(parser_result[0], "size") == 0 && curr_stack != NULL)
			{
				stack_size(curr_stack);
			}
			else if (strcmp(parser_result[0], "help") == 0)
			{
				printf("%s", helpmsg);
			}
			else if (strcmp(parser_result[0], "current") == 0 && curr_stack != NULL)
			{
				stack_info_current();
			}
			else if (strcmp(parser_result[0], "stack") == 0)
			{
				if (strcmp(parser_result[1], "delete") == 0)
				{
					stack_delete(parser_result[2]);
				}
				else if (strcmp(parser_result[1], "create") == 0)
				{
					stack_create(parser_result[2]);
				}
				else if (strcmp(parser_result[1], "change") == 0)
				{
					stack_change(parser_result[2]);
				}
				else if (strcmp(parser_result[1], "print") == 0)
				{
					stackList_print();
				}
				else
				{
					printf("Parser error.\n");
					stop_emulator = true;
				}
			}
			else if (strcmp(parser_result[0], "exit") == 0)
			{
				stop_emulator = true;
			}
			else if (curr_stack == NULL)
			{
				printf("There is no stack! Please create one first.\n");
				printf("%s", distline);
			}
			else
			{
				printf("Parser error.\n");
				stop_emulator = true;
			}
		}

		if (stop_emulator)
		{
			printf("Now exiting emulator...\n\n");
		}
	}
}

int main(int argc, char** argv)
{
	printf("Stack Emulator getting ready...\n\nPlease Wait :)\n");

	stackListInit();

	printf("Stack Emulator Started.\n");

	printf("%s", distline);

	printf("%s", intromsg);

	emulator();

	return 0;
}