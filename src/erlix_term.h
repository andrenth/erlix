/*
 * project : erlix
 * author : kdr2
 *
 */

#ifndef ERLIX_TERM_H_
#define ERLIX_TERM_H_

#include "ruby.h"

#include "erl_interface.h"
#include "ei.h"

#ifndef RUBY_VERSION_MINOR
#define RUBY_VERSION_MINOR 8
#endif


#ifndef RSTRING_LEN
  #define RSTRING_LEN(x) (RSTRING(x)->len)
  #define RSTRING_PTR(x) (RSTRING(x)->ptr)
#endif

#ifndef RARRAY_LEN
  #define RARRAY_LEN(x) (RARRAY(x)->len)
  #define RARRAY_PTR(x) (RARRAY(x)->ptr)
#endif


/*********** erlix_term  ***********/

#define TYPE_ATOM        (1<<0)  //DONE
#define TYPE_INT         (1<<1)  //DONE bug?
#define TYPE_UINT        (1<<2)  //DONE bug?
#define TYPE_FLOAT       (1<<3)  //DONE
#define TYPE_PID         (1<<4)  //DONE
#define TYPE_PORT        (1<<5)  //TODO now?
#define TYPE_REF         (1<<6)  //DONE
#define TYPE_TUPLE       (1<<7)  //DONE
#define TYPE_BINARY      (1<<8)  //DONE
#define TYPE_LIST        (1<<9)  //DONE
#define TYPE_EMPTY_LIST  (1<<10) //NONE
#define TYPE_CONS        (1<<11) //NONE

typedef struct _erlix_term{
  unsigned long flags;
  unsigned long type;
  ETERM *term;
}ErlixTerm;


extern VALUE erlix_cErlixInt;
extern VALUE erlix_cErlixUInt;
extern VALUE erlix_cErlixFloat;
extern VALUE erlix_cErlixPid;
extern VALUE erlix_cErlixRef;
extern VALUE erlix_cErlixAtom;
extern VALUE erlix_cErlixList;
extern VALUE erlix_cErlixTuple;
extern VALUE erlix_cErlixBinary;
extern VALUE erlix_cErlixConnection;

extern VALUE erlix_mErlixTerm;
extern VALUE erlix_mErlixNode;

/*
 * Type Automatic Convert
 *  Ruby-Type ->  Erlang-Type
 *  FixNum    ->  ErlixInt
 *  Float     ->  ErlixFloat
 *  String    ->  ErlixList
 *  Symbol    ->  ErlixAtom
 *
 */
#define CAN_AUTO_CONV(v) ((FIXNUM_P(v)) || \
                          (SYMBOL_P(v)) || \
                          (TYPE(v)==T_FLOAT) || \
                          (TYPE(v)==T_STRING))
ETERM *erlix_auto_conv(VALUE v);

#define STRING_P(s) (TYPE(s)==T_STRING)
#define ARRAY_P(s) (TYPE(s)==T_ARRAY)
#define IS_ETERM(v) (TYPE(v)==T_DATA && RDATA(v)->dfree==(RUBY_DATA_FUNC)free_erlix_term)

ErlixTerm* new_erlix_term();
void free_erlix_term(void* ptr);
VALUE erlix_term(ETERM *term);
unsigned long erlix_term_type(ETERM *term);


VALUE erlix_int_alloc(VALUE klass);
VALUE erlix_uint_alloc(VALUE klass);
VALUE erlix_float_alloc(VALUE klass);
VALUE erlix_pid_alloc(VALUE klass);
VALUE erlix_ref_alloc(VALUE klass);
VALUE erlix_atom_alloc(VALUE klass);
VALUE erlix_list_alloc(VALUE klass);
VALUE erlix_tuple_alloc(VALUE klass);
VALUE erlix_binary_alloc(VALUE klass);
VALUE erlix_message_alloc(VALUE klass);


void init_erlix_term(VALUE erlix);
void init_erlix_int(VALUE erlix);
void init_erlix_uint(VALUE erlix);
void init_erlix_float(VALUE erlix);
void init_erlix_pid(VALUE erlix);
void init_erlix_ref(VALUE erlix);
void init_erlix_atom(VALUE erlix);
void init_erlix_list(VALUE erlix);
void init_erlix_node(VALUE erlix);
void init_erlix_tuple(VALUE erlix);
void init_erlix_binary(VALUE erlix);
void init_erlix_message(VALUE erlix);
void init_erlix_connection(VALUE erlix);

#endif /* ERLIX_TERM_H_ */
