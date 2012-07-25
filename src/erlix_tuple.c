/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_term.h"

VALUE erlix_cErlixTuple;

VALUE erlix_tuple_alloc(VALUE klass){
    VALUE obj;    
    ErlixTerm *tuple=new_erlix_term();
    tuple->type=TYPE_TUPLE;
    obj=Data_Wrap_Struct(klass,0,free_erlix_term,tuple);
    return obj;
}

static VALUE erlix_tuple_init(VALUE self,VALUE ary){
    ErlixTerm *tuple;
    VALUE array, e;
    int i;
    ETERM **tes;
    ErlixTerm *ep;
    
    Data_Get_Struct(self,ErlixTerm,tuple);

    array=rb_check_array_type(ary);
    if(NIL_P(array)||RARRAY_LEN(array)==0){
        //empty tuple
        tuple->term=erl_mk_tuple(NULL,0);
        return self;
    }
    //check: all elements' must be Erlix::Term or auto-convertable Type
    for(i=0;i<RARRAY_LEN(array);i++){
        e=RARRAY_PTR(array)[i];
        if(!IS_ETERM(e) && !CAN_AUTO_CONV(e)){
            rb_raise(rb_eTypeError,"all tuple's elements must be Erlix::Term or Auto-Convertable-Type!");
        }
    }
    tes=(ETERM**)malloc(sizeof(ETERM*)*(RARRAY_LEN(array)));
    for(i=0;i<RARRAY_LEN(array);i++){
        VALUE e=RARRAY_PTR(array)[i];
        if(IS_ETERM(e)){
            Data_Get_Struct(e,ErlixTerm,ep);
            *(tes+i)=erl_copy_term(ep->term);
        }else{
            *(tes+i)=erlix_auto_conv(e);
        }
    }
    //TODO
    tuple->term=erl_mk_tuple(tes,(uint32_t)RARRAY_LEN(array));
    //for(i=0;i<RARRAY(array)->len;i++){
    //  erl_free_term(*(tes+i));
    //}
    free(tes);
    return self;
}

static VALUE erlix_tuple_create(int argc,VALUE *argv,VALUE klass){
    
    ETERM *rterm=NULL;
    ETERM **tes;
    ErlixTerm *ep;
    int i;
    
    if(argc==0){
        //empty tuple
        rterm=erl_mk_tuple(NULL,0);
    }else if(argc>0){
        //check: all elements' must be Erlix::Term
        //or automatic conversions
        for(i=0;i<argc;i++){
            if(!IS_ETERM(argv[i]) && !CAN_AUTO_CONV(argv[i])){
                rb_raise(rb_eTypeError,"all tuple's elements must be Erlix::Term!");
            }
        }
        tes=(ETERM**)malloc(sizeof(ETERM*)*argc);
        for(i=0;i<argc;i++){
            if(IS_ETERM(argv[i])){
                Data_Get_Struct(argv[i],ErlixTerm,ep);
                *(tes+i)=erl_copy_term(ep->term);
            }else{
                *(tes+i)=erlix_auto_conv(argv[i]);
            }
        }
        rterm=erl_mk_tuple(tes,argc);
        //for(i=0;i<argc;i++){
        //  erl_free_term(*(tes+i));
        //}
        free(tes);
    }
    return erlix_term(rterm);
}

static VALUE erlix_tuple_nth(VALUE self,VALUE index){
    ErlixTerm *tuple;
    ETERM *e;
    
    if(TYPE(index)!=T_FIXNUM){
        rb_raise(rb_eTypeError,"index must be a fixnum(1..len)!");
        return Qnil;
    }
    Data_Get_Struct(self,ErlixTerm,tuple);
    if(ERL_TUPLE_SIZE(tuple->term)<FIX2INT(index)||FIX2INT(index)<=0){
        return Qnil;
    }
    e=erl_element(FIX2INT(index),tuple->term);
    return erlix_term(erl_copy_term(e));
}

static VALUE erlix_tuple_to_ary(VALUE self){
    ErlixTerm *tuple;
    int len, i;
    ETERM *e;
    VALUE ret;
    
    Data_Get_Struct(self,ErlixTerm,tuple);
    len=ERL_TUPLE_SIZE(tuple->term);
    ret=rb_ary_new2(len);
    for(i=0;i<len;i++){
        e=erl_element(i+1,tuple->term);
        rb_ary_store(ret,i,erlix_term(erl_copy_term(e)));
    }
    return ret;
}


static VALUE erlix_tuple_size(VALUE self){
    ErlixTerm *tuple;
    int len;
    
    Data_Get_Struct(self,ErlixTerm,tuple);
    len=ERL_TUPLE_SIZE(tuple->term);
    return INT2FIX(len);
}

static VALUE erlix_tuple_inspect(VALUE self){
    VALUE ret=rb_str_new2("#<Erlix::Tuple:");
    ID concat=rb_intern("concat");
    rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
    rb_funcall(ret,concat,1,rb_str_new2(">"));
    return ret;
}

static VALUE erlix_tuple_etype(VALUE self){
    return rb_str_new2("tuple");
}

void init_erlix_tuple(VALUE erlix){
    erlix_cErlixTuple=rb_define_class_under(erlix,"Tuple",rb_cObject);

    rb_define_alloc_func(erlix_cErlixTuple,erlix_tuple_alloc);
    rb_define_method(erlix_cErlixTuple,"initialize",erlix_tuple_init,1);
    rb_define_method(erlix_cErlixTuple,"nth",erlix_tuple_nth,1);
    rb_define_method(erlix_cErlixTuple,"[]",erlix_tuple_nth,1);
    rb_define_method(erlix_cErlixTuple,"to_a",erlix_tuple_to_ary,0);
    rb_define_method(erlix_cErlixTuple,"size",erlix_tuple_size,0);
    rb_define_method(erlix_cErlixTuple,"inspect",erlix_tuple_inspect,0);
    rb_define_method(erlix_cErlixTuple,"etype",erlix_tuple_etype,0);

    rb_define_singleton_method(erlix_cErlixTuple, "[]", erlix_tuple_create, -1);

    rb_include_module(erlix_cErlixTuple,erlix_mErlixTerm);
}



