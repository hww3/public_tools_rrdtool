#include "global.h"
#include "program.h"
#include "pike_types.h"
#include "interpret.h"
#include "module_support.h"
#include "array.h"
#include "backend.h"
#include "stralloc.h"
#include "mapping.h"
#include "object.h"
#include "bignum.h"
#include "threads.h"
#include "builtin_functions.h"
#include "operators.h"

#include <rrd.h>

void f_rrd_create(INT32 args) {
  int argc;
  struct array *a;
  char **argv;
  int i,j;
  int k;

  get_all_args("rrd_create",args,"%i%a",&argc,&a);
  if (a->size) {
    argv=xmalloc(sizeof(char *)*a->size);
    if (argv==NULL)
      SIMPLE_OUT_OF_MEMORY_ERROR("rrd_create",sizeof(char *)*a->size);
    for (i=j=0; i<a->size; i++) {
      if ((ITEM(a)+i)->type==PIKE_T_INT) {
	argv[j]=xmalloc(sizeof(char)*33);
	sprintf(argv[j],"%d",(ITEM(a)+i)->u.integer);
	j++;
      }
      if ((ITEM(a)+i)->type==PIKE_T_STRING)
	argv[j++]=strdup((ITEM(a)+i)->u.string->str);
    }
    k=rrd_create(j,argv);
    for (i=0; i<j; i++)
      xfree(argv[i]);
    xfree(argv);
  }
  pop_n_elems(args);
  push_int(k);
}
  
void f_rrd_fetch(INT32 args) {
  int argc;
  struct array *a;
  char **argv;
  int i,j;
  int k;
  time_t start,end;
  unsigned long step,ds_cnt;
  char **ds_namv;
  rrd_value_t *data;

  get_all_args("rrd_fetch",args,"%i%a",&argc,&a);
  if (a->size) {
    argv=xmalloc(sizeof(char *)*a->size);
    if (argv==NULL)
      SIMPLE_OUT_OF_MEMORY_ERROR("rrd_fetch",sizeof(char *)*a->size);
    for (i=j=0; i<a->size; i++) {
      if ((ITEM(a)+i)->type==PIKE_T_INT) {
	argv[j]=xmalloc(sizeof(char)*33);
	sprintf(argv[j],"%d",(ITEM(a)+i)->u.integer);
	j++;
      }
      if ((ITEM(a)+i)->type==PIKE_T_STRING)
	argv[j++]=strdup((ITEM(a)+i)->u.string->str);
    }
    k=rrd_fetch(j,argv,&start,&end,&step,&ds_cnt,&ds_namv,&data);
    for (i=0; i<j; i++)
      xfree(argv[i]);
    xfree(argv);
  }
  pop_n_elems(args);
  push_text("end");
  push_int(end);
  push_text("step");
  push_int(step);
  push_text("fields");
  for (i=0; i<ds_cnt; i++) {
    struct pike_string *pk;
    push_string(make_shared_binary_string(ds_namv[i],strlen(ds_namv[i])));
  }
  f_aggregate(ds_cnt);
  push_text("data");
  {
    rrd_value_t *datai=data;
    for (i=start+step; i<=end; i+=step) {
      for (j=0; j<ds_cnt; j++) {
	push_float(*(datai++));
      }
      f_aggregate(ds_cnt);
    }
  }
  f_aggregate((end-start)/step);
  f_aggregate_mapping(8);
  for (i=0; i<ds_cnt; i++)
    free(ds_namv[i]);
  free(ds_namv);
  free(data);
}

void f_rrd_first(INT32 args) {
  int argc;
  struct array *a;
  char **argv;
  time_t t;
  int i,j;

  get_all_args("rrd_first",args,"%i%a",&argc,&a);
  if (a->size) {
    argv=xmalloc(sizeof(char *)*a->size);
    if (argv==NULL)
      SIMPLE_OUT_OF_MEMORY_ERROR("rrd_first",sizeof(char *)*a->size);
    for (i=j=0; i<a->size; i++) {
      if ((ITEM(a)+i)->type==PIKE_T_INT) {
	argv[j]=xmalloc(sizeof(char)*33);
	sprintf(argv[j],"%d",(ITEM(a)+i)->u.integer);
	j++;
      }
      if ((ITEM(a)+i)->type==PIKE_T_STRING)
	argv[j++]=strdup((ITEM(a)+i)->u.string->str);
    }
    t=rrd_first(j,argv);
    for (i=0; i<j; i++)
      xfree(argv[i]);
    xfree(argv);
  }
  pop_n_elems(args);
  push_int(t);
}

void f_rrd_last(INT32 args) {
  int argc;
  struct array *a;
  char **argv;
  time_t t;
  int i,j;

  get_all_args("rrd_last",args,"%i%a",&argc,&a);
  if (a->size) {
    argv=xmalloc(sizeof(char *)*a->size);
    if (argv==NULL)
      SIMPLE_OUT_OF_MEMORY_ERROR("rrd_last",sizeof(char *)*a->size);
    for (i=j=0; i<a->size; i++) {
      if ((ITEM(a)+i)->type==PIKE_T_INT) {
	argv[j]=xmalloc(sizeof(char)*33);
	sprintf(argv[j],"%d",(ITEM(a)+i)->u.integer);
	j++;
      }
      if ((ITEM(a)+i)->type==PIKE_T_STRING)
	argv[j++]=strdup((ITEM(a)+i)->u.string->str);
    }
    t=rrd_last(j,argv);
    for (i=0; i<j; i++)
      xfree(argv[i]);
    xfree(argv);
  }
  pop_n_elems(args);
  push_int(t);
}

void f_rrd_graph(INT32 args) {
  int argc;
  struct array *a;
  char **argv;
  int i,j,k;

  get_all_args("rrd_graph",args,"%i%a",&argc,&a);
  if (a->size) {
    argv=xmalloc(sizeof(char *)*a->size);
    if (argv==NULL)
      SIMPLE_OUT_OF_MEMORY_ERROR("rrd_graph",sizeof(char *)*a->size);
    for (i=j=0; i<a->size; i++) {
      if ((ITEM(a)+i)->type==PIKE_T_INT) {
	argv[j]=xmalloc(sizeof(char)*33);
	sprintf(argv[j],"%d",(ITEM(a)+i)->u.integer);
	j++;
      }
      if ((ITEM(a)+i)->type==PIKE_T_STRING)
	argv[j++]=strdup((ITEM(a)+i)->u.string->str);
    }
    {
      char **prdata;
      int xsize,ysize;
      double ymin,ymax;

      k=rrd_graph(j,argv,&prdata,&xsize,&ysize,NULL,&ymin,&ymax);
    }
    for (i=0; i<j; i++)
      xfree(argv[i]);
    xfree(argv);
  }
  pop_n_elems(args);
  push_int(k);
}

void f_rrd_update(INT32 args) {
  int argc;
  struct array *a;
  char **argv;
  int i,j,k;

  get_all_args("rrd_update",args,"%i%a",&argc,&a);
  if (a->size) {
    argv=xmalloc(sizeof(char *)*a->size);
    if (argv==NULL)
      SIMPLE_OUT_OF_MEMORY_ERROR("rrd_update",sizeof(char *)*a->size);
    for (i=j=0; i<a->size; i++) {
      if ((ITEM(a)+i)->type==PIKE_T_INT) {
	argv[j]=xmalloc(sizeof(char)*33);
	sprintf(argv[j],"%d",(ITEM(a)+i)->u.integer);
	j++;
      }
      if ((ITEM(a)+i)->type==PIKE_T_STRING)
	argv[j++]=strdup((ITEM(a)+i)->u.string->str);
    }
    k=rrd_update(j,argv);
    for (i=0; i<j; i++)
      xfree(argv[i]);
    xfree(argv);
  }
  pop_n_elems(args);
  push_int(k);
}

struct program *rrd_program;

PIKE_MODULE_INIT {
  ADD_FUNCTION("rrd_create",f_rrd_create,tFunc(tInt tArray,tInt),0);
  ADD_FUNCTION("rrd_fetch",f_rrd_fetch,tFunc(tInt tArray,tMapping),0);
  ADD_FUNCTION("rrd_first",f_rrd_first,tFunc(tInt tArray,tInt),0);
  ADD_FUNCTION("rrd_last",f_rrd_last,tFunc(tInt tArray,tInt),0);
  ADD_FUNCTION("rrd_graph",f_rrd_graph,tFunc(tInt tArray,tInt),0);
  ADD_FUNCTION("rrd_update",f_rrd_update,tFunc(tInt tArray,tInt),0);
}

PIKE_MODULE_EXIT {
}
