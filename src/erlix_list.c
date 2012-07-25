/*
 * project : erlix
 * author : kdr2
 *
 */

#include "erlix_term.h"

VALUE erlix_cErlixList;

VALUE erlix_list_alloc(VALUE klass){
    ErlixTerm *list;
    VALUE obj;

    list=new_erlix_term();
    list->type=TYPE_LIST;
    obj=Data_Wrap_Struct(klass,0,free_erlix_term,list);
    return obj;
}

static VALUE erlix_list_init(VALUE self,VALUE ary){
    ErlixTerm *list;
    ETERM **les;
    VALUE e;
    ErlixTerm *ep;
    int i;
    Data_Get_Struct(self,ErlixTerm,list);

    if(NIL_P(ary)){
        //empty list
        list->term=erl_mk_empty_list();
        return self;
    }
    if(TYPE(ary)==T_ARRAY){
        if(RARRAY_LEN(ary)==0){
            //empty list
            list->term=erl_mk_empty_list();
        }else{
            //check: all elements' must be ErlixTerm or auto-convertable Type
            for(i=0;i<RARRAY_LEN(ary);i++){
                e=RARRAY_PTR(ary)[i];
                if(!IS_ETERM(e) && !CAN_AUTO_CONV(e)){
                    rb_raise(rb_eTypeError,"all list's elements must be ErlixTerm or Auto-Convertable-Type!");
                }
            }
            les=(ETERM**)malloc(sizeof(ETERM*)*(RARRAY_LEN(ary)));
            for(i=0;i<RARRAY_LEN(ary);i++){
                e=RARRAY_PTR(ary)[i];
                if(IS_ETERM(e)){
                    Data_Get_Struct(e,ErlixTerm,ep);
                    *(les+i)=erl_copy_term(ep->term);
                }else{
                    *(les+i)=erlix_auto_conv(e);
                }
            }
            list->term=erl_mk_list(les,RARRAY_LEN(ary));
            //for(i=0;i<RARRAY(ary)->len;i++){
            //  erl_free_term(*(les+i));
            //}
            free(les);
        }
    }else if(TYPE(ary)==T_STRING){
        list->term=erl_mk_estring(RSTRING_PTR(ary), RSTRING_LEN(ary));
    }
    return self;
}


static VALUE erlix_list_init2(int argc, VALUE* argv,VALUE self){
    ErlixTerm *list;
    ETERM **les;
    VALUE e;
    ErlixTerm *ep;
    int i;
    Data_Get_Struct(self,ErlixTerm,list);

    if(argc==0){
        //empty list
        list->term=erl_mk_empty_list();
        return self;
    }

    if(argc==1 && (ARRAY_P(*argv)||STRING_P(*argv))){
        return erlix_list_init(self, *argv);
    }

    //check: all elements' must be ErlixTerm or auto-convertable Type
    for(i=0;i<argc;i++){
        e=*(argv+i);
        if(!IS_ETERM(e) && !CAN_AUTO_CONV(e)){
            rb_raise(rb_eTypeError,"all list's elements must be ErlixTerm or Auto-Convertable-Type!");
        }
    }
    les=(ETERM**)malloc(sizeof(ETERM*)*argc);
    for(i=0;i<argc;i++){
        e=*(argv+i);
        if(IS_ETERM(e)){
            Data_Get_Struct(e,ErlixTerm,ep);
            *(les+i)=erl_copy_term(ep->term);
        }else{
            *(les+i)=erlix_auto_conv(e);
        }
    }
    list->term=erl_mk_list(les,argc);
    //for(i=0;i<RARRAY(ary)->len;i++){
    //  erl_free_term(*(les+i));
    //}
    free(les);
    return self;
}


static VALUE erlix_list_create(int argc,VALUE *argv,VALUE klass){
    ETERM *rterm;
    ETERM **les;
    ErlixTerm *ep;
    VALUE e;
    int i;
    
    if(argc==0){
        //empty list
        rterm=erl_mk_empty_list();
    }else{
        //check: all elements' must be ErlixTerm or auto-convertable Type
        for(i=0;i<argc;i++){
            e=argv[i];
            if(!IS_ETERM(e) && !CAN_AUTO_CONV(argv[i])){
                rb_raise(rb_eTypeError,"all list's elements must be ErlixTerm or Auto-Convertable-Type!");
            }
        }
        les=(ETERM**)malloc(sizeof(ETERM*)*argc);
        for(i=0;i<argc;i++){
            e=argv[i];
            if(IS_ETERM(e)){
                Data_Get_Struct(e,ErlixTerm,ep);
                *(les+i)=erl_copy_term(ep->term);
            }else{
                *(les+i)=erlix_auto_conv(e);
            }
        }
        rterm=erl_mk_list(les,argc);
        //for(i=0;i<argc;i++){
        //  erl_free_term(*(les+i));
        //}
        free(les);
    }
    return erlix_term(rterm);
}

static VALUE erlix_list_to_ary(VALUE self){
    ErlixTerm *list;
    ETERM *hd,*tmp,*ep;
    VALUE ret;
    int len, i;
    
    Data_Get_Struct(self,ErlixTerm,list);
    len=erl_length(list->term);
    ret=rb_ary_new2(len);
    ep=list->term;
    i=1;
    while(!ERL_IS_EMPTY_LIST(ep)){
        hd=erl_hd(ep);
        rb_ary_store(ret,i,erlix_term(erl_copy_term(hd)));
        tmp=erl_tl(ep);
        //if(ep!=list->term)erl_free_term(ep);
        ep=tmp;
        //if(ERL_IS_EMPTY_LIST(ep)){
        //  erl_free_term(ep);
        //}
        ++i;
    }
    return ret;
}

static VALUE erlix_list_head(VALUE self){
    ErlixTerm *list;
    ETERM *ep;
    Data_Get_Struct(self,ErlixTerm,list);
    if(ERL_IS_EMPTY_LIST(list->term)){
        return Qnil;
    }
    ep=erl_hd(list->term);
    return erlix_term(erl_copy_term(ep));
}

static VALUE erlix_list_tail(VALUE self){
    ErlixTerm *list;
    ETERM *ep;
    
    Data_Get_Struct(self,ErlixTerm,list);
    if(ERL_IS_EMPTY_LIST(list->term)){
        return Qnil;
    }
    ep=erl_tl(list->term);
    return erlix_term(erl_copy_term(ep));
}

static VALUE erlix_list_cons(VALUE self,VALUE head){
    ErlixTerm *list;
    ETERM *ep;
    ErlixTerm *hd;
    VALUE ret;
    
    if(!IS_ETERM(head) && !CAN_AUTO_CONV(head)){
        rb_raise(rb_eTypeError,"the head must be ErlixTerm or Auto-Convertable-Type!");
        return Qnil;
    }
    
    Data_Get_Struct(self,ErlixTerm,list);
    if(IS_ETERM(head)){
        Data_Get_Struct(head,ErlixTerm,hd);
        ep=erl_cons(hd->term,list->term);
    }else{
        ep=erl_cons(erlix_auto_conv(head),list->term);
    }
    ret = erlix_term(erl_copy_term(ep));
    erl_free_term(ep);
    return ret;
}

static VALUE erlix_list_size(VALUE self){
    ErlixTerm *list;
    int len;
    Data_Get_Struct(self,ErlixTerm,list);
    len=erl_length(list->term);
    return INT2FIX(len);
}

static VALUE erlix_list_inspect(VALUE self){
    VALUE ret;
    ID concat;
    
    ret=rb_str_new2("#<Erlix::List:");
    concat=rb_intern("concat");
    rb_funcall(ret,concat,1,rb_funcall(self,rb_intern("to_s"),0));
    rb_funcall(ret,concat,1,rb_str_new2(">"));
    return ret;
}

static VALUE erlix_list_etype(VALUE self){
    return rb_str_new2("list");
}

void init_erlix_list(VALUE erlix){
    erlix_cErlixList=rb_define_class_under(erlix,"List",rb_cObject);

    rb_define_alloc_func(erlix_cErlixList,erlix_list_alloc);
    rb_define_method(erlix_cErlixList,"initialize",erlix_list_init2,-1);
    rb_define_method(erlix_cErlixList,"head",erlix_list_head,0);
    rb_define_method(erlix_cErlixList,"tail",erlix_list_tail,0);
    rb_define_method(erlix_cErlixList,"cons",erlix_list_cons,1);
    rb_define_method(erlix_cErlixList,"to_a",erlix_list_to_ary,0);
    rb_define_method(erlix_cErlixList,"size",erlix_list_size,0);
    rb_define_method(erlix_cErlixList,"inspect",erlix_list_inspect,0);
    rb_define_method(erlix_cErlixList,"etype",erlix_list_etype,0);

    rb_define_singleton_method(erlix_cErlixList,"[]",erlix_list_create,-1);

    rb_include_module(erlix_cErlixList,erlix_mErlixTerm);
}


