#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "1_b.h"

void find(link *root, char *name, int vals) {
	if(root->type == str || root->type == def) {
		if(!strcmp(root->name, name)) {
			root->type = val;
			root->val  = vals;
			return;
		}
		for(int i = 0; i < root->argc; i++)
			find(root->arg[i], name, vals);
	}
	if(root->type == func)
		for(int i = 0; i < root->argc; i++)
			find(root->arg[i], name, vals);
}

int apply(link *root) { 
	switch(root->type) {
		case val:
			return root->val;
		case arg:
			return root->val;
		case func:
			return root->func(root);
	}

	if(root->type == str || root->type == def)
		for(int i = 0; i < Fx.argc; i++)
			if(!strcmp(root->name, Fx.func[i]->name)) {
				root->type = def;

				link *Bar = link_copy(Fx.func[i]->arg[Fx.func[i]->argc - 1]);

				for(int j = 0; j < Fx.func[i]->argc - 1; j++)
					find(Bar, Fx.func[i]->arg[j]->name, apply(root->arg[j]));

				int BUFF = apply(Bar);

				link_destroy(Bar);
				return BUFF;
			}

	return 0;
}

/* structural function */
int put(link *root) {
	int buff;
	for(int i = 0; i < root->argc; i++) {
		buff = apply(root->arg[i]);
		if(root->arg[i]->type == str)
			printf("%s ", root->arg[i]->name);
		else printf("%d ", buff);
	}

	printf("\n");
	return 0;
}

int pass(link *root) { return apply(root->arg[0]); }

int define(link *root) {
	root->arg[0]->type = def;

	for(int i = 1; i < root->argc - 1; i++) {
		root->arg[i]->type = arg;
		link_to(root->arg[0], root->arg[i]);

	}

	link_to(root->arg[0], root->arg[root->argc - 1]);
	return 0;
}

int if_(link *root) {
	if(apply(root->arg[0]) == 1)
		return apply(root->arg[1]);
	if(apply(root->arg[0]) == 0 && root->argc > 2)
		return apply(root->arg[2]);
	return 0;
}

/* compairison */
int equal(link *root) {
	int buff = 1;
	for(int i = 0; i < root->argc - 1; i++) 
		if(apply(root->arg[i]) != apply(root->arg[i + 1])) buff = 0;
	return buff;
}

int inequal(link *root) {
	int buff = 1;
	for(int i = 0; i < root->argc - 1; i++) 
		if(apply(root->arg[i]) == apply(root->arg[i + 1])) buff = 0;
	return buff;
}

int more(link *root) {
	if(apply(root->arg[0]) > apply(root->arg[1]))
		return 1;
	return 0;
}

int less(link *root) {
	if(apply(root->arg[0]) < apply(root->arg[1]))
		return 1;
	return 0;
}

/* logic */
int not(link *root) { return !apply(root->arg[0]); }

int and(link *root) {
	int buff = 1;
	for(int i = 0; i < root->argc; i++) 
		if(apply(root->arg[i]) == 0) 
			buff = 0;
	return buff;
}

int or(link *root) {
	int buff = 0;
	for(int i = 0; i < root->argc; i++) 
		if(apply(root->arg[i]) == 1) 
			buff = 1;
	return buff;
}

/* primetive math operator. */
int add(link *root) {
	int buff = 0, i = 0;
	for(; i < root->argc; i++) buff += apply(root->arg[i]);
	return buff;
}

int mul(link *root) {
	int buff = 1, i = 0;
	for(; i < root->argc; i++) buff *= apply(root->arg[i]);
	return buff;
}

int sub(link *root) {
	int buff = apply(root->arg[0]), i = 1;
	for(; i < root->argc; i++) buff -= apply(root->arg[i]);
	return buff;
}

int dvi(link *root) {
	int buff = apply(root->arg[0]), i = 1;
	for(; i < root->argc; i++) buff /= apply(root->arg[i]);
	return buff;
}

int max(link *root) {
	int buff = 0;
	for(int i = 0; i < root->argc; i++) 
		if(apply(root->arg[i]) > buff) 
			buff = apply(root->arg[i]);
	return buff;
}

int min(link *root) {
	int buff = apply(root->arg[0]), i = 1;
	for(; i < root->argc; i++) 
		if(apply(root->arg[i]) < buff)
			buff = apply(root->arg[i]);
	return buff;
}

/* linker */
link *link_copy(link *root) {
	link *Foo = link_crate();

	Foo->type = root->type;

	switch(root->type) {
		case func:
			Foo->func = root->func;
			break;
		case str: 
			sprintf(Foo->name, "%s", root->name);
			break;
		case def:
			sprintf(Foo->name, "%s", root->name);
			break;
		case arg:
			sprintf(Foo->name, "%s", root->name);
			break;
		case val:
			Foo->val = root->val;
			break;
	}

	for(int i = 0; i < root->argc; i++) {
		link_to(Foo, link_copy(root->arg[i]));
	}

	return Foo;
}


link *link_crate() {
	link *foo = malloc(sizeof(link) * 1);
	foo->arg  = malloc(sizeof(link) * 2);
	foo->argc = 0;
	foo->size = 2;

	return foo;
}

link *link_add(link *To) {
	link *foo = malloc(sizeof(link) * 1);
	foo->arg  = malloc(sizeof(link) * 2);
	foo->argc = 0;
	foo->size = 2;

	if(To->argc >= To->size) {
		To->size *= 2;
		To->arg   = realloc(To->arg, sizeof(link) * To->size);
	}
	To->arg[To->argc++] = foo;

	return foo;
}

void link_to(link *To, link *From) {
	if(To->argc == To->size) {
		To->size *= 2;
		To->arg   = realloc(To->arg, sizeof(link) * To->size);
	}
	To->arg[To->argc++] = From;
}

void link_add_val(link *target, int N) {
	target->type = val;
	target->val  = N;
}

void link_add_func(link *target, int (*Fx)()) {
	target->type = func;
	target->func = Fx;
}

void link_destroy(link *root) {
	for(int i = 0; i < root->argc; i++)
		link_destroy(root->arg[i]);

	if(root->size != 0) {
		free(root->arg);
		root->size = 0;
		free(root);
	}
}

void look_func(link *root, char *string) {
	char *func_name[] = { "put", "define", "if", "max", "min", "and", "or", "not", "=", "!=", ">", "<", "+", "-", "*", "/" };
	int (*func_ptr[])() = { put, define, if_, max, min, and, or, not, equal, inequal, more, less, add, sub, mul, dvi };

	for(int i = 0; i < 16; i++)
		if(!strcmp(string, func_name[i])) {
			link_add_func(root, func_ptr[i]); 
			return;
		}

	root->type = str;
	sprintf(root->name, "%s", string);
}

int main(int argc, char *file_name[]) {
	if(argc != 2) return 0;
	FILE *file      = fopen(file_name[1], "r");
	char *file_buff = malloc(sizeof(char) * 255);
	int  old_argc, open, close, i;

	/* init def save  && stack */
	Fx.func = malloc(sizeof(link) * 2);
	Fx.size = 2;
	Fx.argc = 0;

	link_stack  stack;
	link       *stack_ptr;
	stack.func = malloc(sizeof(link) * 2);
	stack.size = 2;
	stack.argc = 0;

	fscanf(file, "%s", file_buff);
	for(; !feof(file);) {
		open  = 0;
		close = 0;
		for(int i = 0; i < (int)strlen(file_buff); i++) {
			if(file_buff[i] == '(') ++open;
			if(file_buff[i] == ')') ++close;
		}
		old_argc = stack.argc;
		stack.argc += open - close;
		if(stack.argc >= stack.size) {
			stack.size *= 2;
			stack.func  = realloc(stack.func, sizeof(link) * stack.size);
		}

		/* root func */
		if(stack.argc == 1 && open  == 1) {
			stack_ptr = stack.func[stack.argc - 1] 
				= link_crate();
			look_func(stack_ptr, file_buff + 1);
		}

		/* func */
		if(stack.argc > 1  && open  == 1) {
			stack_ptr = stack.func[stack.argc - 1] 
				= link_add(stack.func[stack.argc - 2]);
			look_func(stack_ptr, file_buff + 1);
		}

		/* arg claer cut */
		if(open == 0 && close == 0) {
			stack_ptr = link_add(stack.func[stack.argc - 1]);
			if(file_buff[0] > 57) {
				stack_ptr->type = str;
				sprintf(stack_ptr->name, "%s", file_buff);
			} else link_add_val(stack_ptr, atoi(file_buff));
		}

		/* arg before apply */
		if(open == 0 && close >  0) {
			char *Foo = malloc(sizeof(char) * 16);
			for(i = 0; i < (int)strlen(file_buff) - close; i++)
				Foo[i] = file_buff[i]; 
			Foo[i] = '\0';

			/* is it only close palythesys? */ 
			if((int)strlen(file_buff) != close) {
				stack_ptr = link_add(stack.func[old_argc - 1]);
				if(file_buff[0] > 57) {
					stack_ptr->type = str;
					sprintf(stack_ptr->name, "%s", Foo);
				} else link_add_val(stack_ptr, atoi(Foo));
			}

			free(Foo);
		}

		/* apply && destroy */
		if(stack.argc == 0 && open == 0) {
			apply(stack.func[0]);

			/* is stack.func[0] define? if not don't detroy it, if yes save on Fx */
			if(stack.func[0]->type == func && stack.func[0]->func == define) {
				if(Fx.argc >= Fx.size) {
					Fx.size *= 2;
					Fx.func  = realloc(Fx.func, sizeof(link) * Fx.size);
				}
				Fx.func[Fx.argc++] = stack.func[0]->arg[0];
			} else link_destroy(stack.func[0]);
		}

		fscanf(file, "%s", file_buff);
	}

	for(int i = 0; i < Fx.argc; i++) link_destroy(Fx.func[i]);
	free(Fx.func);

	return 0;
}
