/*
 * project : erlix
 * author : kdr2
 *
 */
#include <stdlib.h>

#include "erlix_term.h"
#include "erlix_int.h"
#include "erlix_uint.h"
#include "erlix_float.h"
#include "erlix_pid.h"
#include "erlix_ref.h"
#include "erlix_atom.h"
#include "erlix_list.h"
#include "erlix_node.h"
#include "erlix_tuple.h"
#include "erlix_binary.h"
#include "erlix_message.h"
#include "erlix_connection.h"



void Init_erlix(){
    VALUE mod_erlix;
    mod_erlix=rb_define_module("Erlix");
    erl_init(NULL,0);
    init_erlix_term(mod_erlix);
    init_erlix_int(mod_erlix);
    init_erlix_uint(mod_erlix);
    init_erlix_float(mod_erlix);
    init_erlix_pid(mod_erlix);
    init_erlix_ref(mod_erlix);
    init_erlix_atom(mod_erlix);
    init_erlix_list(mod_erlix);
    init_erlix_node(mod_erlix);
    init_erlix_tuple(mod_erlix);
    init_erlix_binary(mod_erlix);
    init_erlix_message(mod_erlix);
    init_erlix_connection(mod_erlix);
}
