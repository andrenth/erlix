/*
 * project : erlix
 * author : kdr2
 *
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "erlix_node.h"


ErlixNode *erlix_node=NULL;

VALUE erlix_mErlixNode;

VALUE erlix_node_init(VALUE self,VALUE sname,VALUE cookie){
    VALUE str_name;
    int creation;
    struct in_addr self_addr;
    char host[64];
    int nn_len;
    char *nname;
    if(erlix_node!=NULL){
        rb_raise(rb_eException,"Erlix::Node has been inited already!");
        return Qnil;
    }
    str_name=StringValue(sname);
    creation=rand()%100;
    self_addr.s_addr=htonl(INADDR_ANY);
    memset(host,0,64);
    if(gethostname(host,64)!=0){
        rb_raise(rb_eException,"gethostname error!");
        return Qnil;
    }
    nn_len=strlen(host)+RSTRING_LEN(str_name)+2;
    nname=(char*)malloc(nn_len);
    memset(nname,0,nn_len);
    sprintf(nname,"%s@%s",RSTRING_PTR(str_name),host);
    if(!erl_connect_xinit(host,
                          RSTRING_PTR(str_name),
                          nname,
                          &self_addr,
                          NIL_P(cookie)?NULL:RSTRING_PTR(cookie),
                          creation)){
        free(nname);
        rb_raise(rb_eException,"erl_connect_xinit error!");
        return Qnil;
    }
    free(nname);
    erlix_node=(ErlixNode*)malloc(sizeof(ErlixNode));
    return self;
}

VALUE erlix_node_inited(VALUE self){
    if(erlix_node==NULL)return Qfalse;
    return Qtrue;
}

VALUE erlix_node_name(VALUE self){
    if(erlix_node==NULL)return Qnil;
    return rb_str_new2(erl_thisnodename());
}

VALUE erlix_node_creation(VALUE self){
    if(erlix_node==NULL)return Qnil;
    return INT2FIX(erl_thiscreation());
}

//TODO
VALUE erlix_node_publish(VALUE self);

//TODO
VALUE erlix_node_accept(VALUE self);

void init_erlix_node(VALUE erlix){
    erlix_mErlixNode=rb_define_module_under(erlix,ERLIX_NODE_MOD);
    rb_define_module_function(erlix_mErlixNode,"init",erlix_node_init,2);
    rb_define_module_function(erlix_mErlixNode,"init?",erlix_node_inited,0);
    rb_define_module_function(erlix_mErlixNode,"name",erlix_node_name,0);
    rb_define_module_function(erlix_mErlixNode,"creation",erlix_node_creation,0);
}
