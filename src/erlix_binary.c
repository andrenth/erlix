/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_term.h"

VALUE erlix_cErlixBinary;

VALUE erlix_binary_alloc(VALUE klass){
    ErlixTerm* binary;
    VALUE obj;
    binary=new_erlix_term();
    binary->type=TYPE_BINARY;
    obj=Data_Wrap_Struct(klass,0,free_erlix_term,binary);
    return obj;
}

static VALUE erlix_binary_init(VALUE self,VALUE string){
    ErlixTerm* binary;
    VALUE str=StringValue(string);
    Data_Get_Struct(self,ErlixTerm,binary);
    //TODO length limit
    binary->term=erl_mk_binary(RSTRING_PTR(str),(uint32_t)RSTRING_LEN(str));
    return self;
}


static VALUE erlix_binary_data(VALUE self){
    ErlixTerm* binary;
    Data_Get_Struct(self,ErlixTerm,binary);
    return rb_str_new(ERL_BIN_PTR(binary->term),ERL_BIN_SIZE(binary->term));
}

static VALUE erlix_binary_size(VALUE self){
    ErlixTerm *binary;
    int l;
    Data_Get_Struct(self,ErlixTerm,binary);
    l=ERL_BIN_SIZE(binary->term);
    return INT2FIX(l);
}

static VALUE erlix_binary_inspect(VALUE self){
    VALUE ret=rb_str_new2("#<Erlix::Binary:");
    ID concat=rb_intern("concat");
    rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
    rb_funcall(ret,concat,1,rb_str_new2(">"));
    return ret;
}

static VALUE erlix_binary_etype(VALUE self){
    return rb_str_new2("binary");
}

void init_erlix_binary(VALUE erlix){
    erlix_cErlixBinary=rb_define_class_under(erlix, "Binary",rb_cObject);

    rb_define_alloc_func(erlix_cErlixBinary,erlix_binary_alloc);
    rb_define_method(erlix_cErlixBinary,"initialize",erlix_binary_init,1);
    rb_define_method(erlix_cErlixBinary,"data",erlix_binary_data,0);
    rb_define_method(erlix_cErlixBinary,"size",erlix_binary_size,0);
    rb_define_method(erlix_cErlixBinary,"inspect",erlix_binary_inspect,0);
    rb_define_method(erlix_cErlixBinary,"etype",erlix_binary_etype,0);

    rb_include_module(erlix_cErlixBinary,erlix_mErlixTerm);
}
