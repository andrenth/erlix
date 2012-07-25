/*
 * project : erlix
 * author : kdr2
 *
 */

#include <unistd.h>
#include <stdlib.h>

#include "erlix_term.h"
#include "erlix_node.h"
#include "erlix_connection.h"

extern ErlixNode* erlix_node;
extern VALUE erlix_cErlixMessage;

VALUE erlix_cErlixConnection;

ErlixConnection* new_erlix_connection(){
    ErlixConnection* ret=(ErlixConnection*)malloc(sizeof(ErlixConnection));
    ret->status=0;
    return ret;
}

void free_erlix_connection(void *econ){
    ErlixConnection* ep=econ;
    ERLIX_CONNECTION_DEAD(ep);
    erl_close_connection(ep->sock_fd);
    free(ep);
}

VALUE erlix_connection_alloc(VALUE klass){
    ErlixConnection* ec=new_erlix_connection();
    VALUE obj;
    obj=Data_Wrap_Struct(klass,0,free_erlix_connection,ec);
    return obj;
}

static VALUE erlix_connection_init(VALUE self,VALUE pnode){
    VALUE node;
    int sockfd;
    ErlixConnection *conn;
        
    if(erlix_node==NULL){
        rb_raise(rb_eException,"call Erlix::Node.init(name,cookie) first please!");
        return Qnil;
    }
    node=StringValue(pnode);
    if((sockfd = erl_connect(RSTRING_PTR(node)))<0){
        rb_raise(rb_eException,"erl_connect failed");
        return Qnil;
    }

    Data_Get_Struct(self,ErlixConnection,conn);
    conn->peer_node=rb_str_new2(RSTRING_PTR(node));
    conn->sock_fd=sockfd;
    ERLIX_CONNECTION_BORN(conn);
    return self;
}


static VALUE erlix_connection_init_copy(VALUE copy,VALUE orig){
    ErlixConnection *tcopy,*tsrc;

    if(copy==orig)return copy;
    if(TYPE(orig)!=T_DATA||RDATA(orig)->dfree!=(RUBY_DATA_FUNC)free_erlix_connection){
        rb_raise(rb_eTypeError,"wrong argument type!");
    }
    
    Data_Get_Struct(copy,ErlixConnection,tcopy);
    Data_Get_Struct(orig,ErlixConnection,tsrc);
    
    if(!ERLIX_CONNECTION_ALIVE(tsrc)){
        rb_raise(rb_eException,"the connection has been disconnected!");
        return Qnil;
    }

    tcopy->status=tsrc->status;
    tcopy->peer_node=rb_str_new2(RSTRING_PTR(tsrc->peer_node));
    tcopy->sock_fd=tsrc->sock_fd;

    return copy;
}


static VALUE erlix_connection_to_str(VALUE self);

//peer node name
static VALUE erlix_connection_peer(VALUE self){
    ErlixConnection *con;
    Data_Get_Struct(self,ErlixConnection,con);
    if(!ERLIX_CONNECTION_ALIVE(con)){
        rb_raise(rb_eException,"the connection has been disconnected!");
        return Qnil;
    }
    return con->peer_node;
}

static VALUE erlix_connection_etype(VALUE self){
    return rb_str_new2("erlix_connection");
}

static VALUE erlix_connection_mkpid(VALUE self);

static VALUE erlix_connection_send(VALUE self,VALUE pid,VALUE term){
    ErlixConnection *con;
    ErlixTerm * ep;

    if(!IS_ETERM(term)){
        rb_raise(rb_eTypeError,"wrong argument type,the data is not an ErlixTerm!");
        return Qnil;
    }
    Data_Get_Struct(self,ErlixConnection,con);
    Data_Get_Struct(term,ErlixTerm,ep);
    if(!ERLIX_CONNECTION_ALIVE(con)){
        rb_raise(rb_eException,"the connection has been disconnected!");
        return Qnil;
    }
    if(TYPE(pid)==T_STRING){
        if(erl_reg_send(con->sock_fd,RSTRING_PTR(pid),ep->term)==1){
            return term;
        }else if(erl_errno==EIO){
            ERLIX_CONNECTION_DEAD(con);
            rb_raise(rb_eException,"IO Error!");
            return Qnil;
        }
    }
    return Qnil;
}

extern int rb_io_wait_readable(int);

static VALUE erlix_connection_recv(VALUE self){
    ErlixConnection *con;
    ErlMessage emsg;
        
    Data_Get_Struct(self,ErlixConnection,con);

    if(!ERLIX_CONNECTION_ALIVE(con)){
        rb_raise(rb_eException,"the connection has been disconnected!");
        return Qnil;
    }
    for(;;){
        //bugfixed! for 1.8.7 thread block!
        if(rb_io_wait_readable(con->sock_fd)!=Qtrue){
            rb_thread_wait_fd(con->sock_fd);
        }
        int ret;
        VALUE m;
        ErlMessage *newm;

        ret=erl_receive_msg(con->sock_fd,NULL,0,&emsg);
        if (ret == ERL_MSG) {
            m=erlix_message_alloc(erlix_cErlixMessage);
            Data_Get_Struct(m,ErlMessage,newm);
            newm->type=emsg.type;
            if(emsg.type==ERL_SEND){
                newm->msg=erl_copy_term(emsg.msg);
                newm->to=erl_copy_term(emsg.to);
            }else if(emsg.type==ERL_REG_SEND){
                newm->msg=erl_copy_term(emsg.msg);
                newm->from=erl_copy_term(emsg.from);
            }else if(emsg.type==ERL_LINK ||emsg.type==ERL_UNLINK){
                newm->from=erl_copy_term(emsg.from);
                newm->to=erl_copy_term(emsg.to);
            }else if(emsg.type==ERL_EXIT){
                newm->msg=erl_copy_term(emsg.msg);
                newm->from=erl_copy_term(emsg.from);
                newm->to=erl_copy_term(emsg.to);
            }
            memcpy(newm->to_name,emsg.to_name,MAXREGLEN);
            return m;
        }else if(ret == ERL_TICK){
            continue;
        }else{
            rb_raise(rb_eException,"receive error!");
            return Qnil;
        }
    }
    return Qnil;
}

static VALUE erlix_connection_rpc(VALUE self,VALUE module,VALUE func,VALUE args){

    VALUE m;
    VALUE f;
    ErlixTerm* a;
    ErlixConnection *con;
    ETERM *result;
    
    m=StringValue(module);
    f=StringValue(func);
    if(!IS_ETERM(args)){
        rb_raise(rb_eTypeError,"wrong argument type,the data is not an ErlixList!");
        return Qnil;
    }
    Data_Get_Struct(args,ErlixTerm,a);
    if(a->type!=TYPE_LIST){
        rb_raise(rb_eTypeError,"wrong argument type,the data is not an ErlixList!");
        return Qnil;
    }
    Data_Get_Struct(self,ErlixConnection,con);
    result=erl_rpc(con->sock_fd, RSTRING_PTR(m),RSTRING_PTR(f), a->term);
    if(result==NULL){
        if(erl_errno==ENOMEM){
            rb_raise(rb_eException,"rpc error: no memory");
        }else if(erl_errno==EIO){
            rb_raise(rb_eException,"rpc error: I/O error");
        }else if(erl_errno==ETIMEDOUT){
            rb_raise(rb_eException,"rpc error: timeout");
        }else if(erl_errno==EAGAIN){
            rb_raise(rb_eException,"rpc error: Temporary error, Try again");
        }
        return Qnil;
    }
    return erlix_term(result);
}

static VALUE erlix_connection_close(VALUE self){
    ErlixConnection *con;
    Data_Get_Struct(self,ErlixConnection,con);
    ERLIX_CONNECTION_DEAD(con);
    close(con->sock_fd);
    return Qtrue;
}

static VALUE erlix_connection_isclosed(VALUE self){
    ErlixConnection *con;
    Data_Get_Struct(self,ErlixConnection,con);
    if(ERLIX_CONNECTION_ALIVE(con)){
        return Qfalse;
    }
    return Qtrue;
}

void init_erlix_connection(VALUE erlix){
    erlix_cErlixConnection=rb_define_class_under(erlix,"Connection",rb_cObject);
    rb_define_alloc_func(erlix_cErlixConnection,erlix_connection_alloc);
    rb_define_method(erlix_cErlixConnection,"initialize",erlix_connection_init,1);
    rb_define_method(erlix_cErlixConnection,"initialize_copy",erlix_connection_init_copy,1);
    rb_define_method(erlix_cErlixConnection,"peer",erlix_connection_peer,0);
    rb_define_method(erlix_cErlixConnection,"esend",erlix_connection_send,2);
    rb_define_method(erlix_cErlixConnection,"erecv",erlix_connection_recv,0);
    rb_define_method(erlix_cErlixConnection,"rpc",erlix_connection_rpc,3);
    rb_define_method(erlix_cErlixConnection,"close",erlix_connection_close,0);
    rb_define_method(erlix_cErlixConnection,"closed?",erlix_connection_isclosed,0);
}
