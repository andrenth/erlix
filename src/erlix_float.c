/*
 * project : erlix
 * author : kdr2
 *
 */

#include <stdio.h>
#include <string.h>

#include "erlix_float.h"

VALUE erlix_cErlixFloat;

VALUE erlix_float_alloc(VALUE klass){
    ErlixTerm* efloat;
    VALUE obj;
    efloat=new_erlix_term();
    efloat->type=TYPE_FLOAT;
    obj=Data_Wrap_Struct(klass,0,free_erlix_term,efloat);
    return obj;
}

VALUE erlix_float_init(VALUE self,VALUE fix){
    ErlixTerm* efloat;
    double i=NUM2DBL(fix);
    Data_Get_Struct(self,ErlixTerm,efloat);
    efloat->term=erl_mk_float(i);
    return self;
}


VALUE erlix_float_to_fix(VALUE self){
    ErlixTerm *efloat;
    double l;
    Data_Get_Struct(self,ErlixTerm,efloat);
    l=ERL_FLOAT_VALUE(efloat->term);
    return rb_float_new(l);
}

VALUE erlix_float_inspect(VALUE self){
    VALUE ret=rb_str_new2("#<Erlix::Float:");
    ID concat=rb_intern("concat");
    rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
    rb_funcall(ret,concat,1,rb_str_new2(">"));
    return ret;
}

VALUE erlix_float_etype(VALUE self){
    return rb_str_new2("float");
}

void init_erlix_float(VALUE erlix){
    erlix_cErlixFloat=rb_define_class_under(erlix,"Float",rb_cObject);

    rb_define_alloc_func(erlix_cErlixFloat,erlix_float_alloc);
    rb_define_method(erlix_cErlixFloat,"initialize",erlix_float_init,1);
    rb_define_method(erlix_cErlixFloat,"to_i",erlix_float_to_fix,0);
    rb_define_method(erlix_cErlixFloat,"inspect",erlix_float_inspect,0);
    rb_define_method(erlix_cErlixFloat,"etype",erlix_float_etype,0);

    rb_include_module(erlix_cErlixFloat,erlix_mErlixTerm);
}
