#ifndef P1_H
#define P1_H

#include <stdio.h>

/* val  value
 * arg  value that will fill after calling def made by define
 * func function
 * def  declearing function made by define
 * str  waiting to assing arg or func be define */
enum { val, arg, func, def, str };

typedef struct link {
	char name[16];
	struct link **arg;
	union { int (*func)(), val; };
	int type, size, argc;
} link;

typedef struct {
	link **func;
	int size, argc;
} link_stack;

link_stack Fx;


void  find(),
void  link_to(),
void  link_destroy(),
void  link_add_arg(),
void  link_to(),
void  link_add_val(),
void  link_add_func(),
void  link_destroy(),
void  look_func();

int   apply(),
void  put(),
void  pass(),
void  define(),
void  if_(),
void  equal(),
void  inequal(),
void  more(),
void  less(),
void  not(),
void  and(),
void  or(),
void  add(),
void  mul(),
void  sub(),
void  dvi(),
void  max(),
void  min();

link *link_crate(),
link *link_copy(),
link *link_add();

#endif
