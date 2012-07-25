Erlix
============================================================

Changelog
------------------------------------------------------------

erlix-0.5 changlog [alpha]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Ruby 1.9.x support
* All classes (Term/Int/List/Tuple/Atom/...) are under the ``Erlix`` module now
* Convenient new for Erlix::List and Erlix::Tuple
  
20090430 update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* features
  
  - Type auto-convert
  - [] singleton method for ErlixTuple and ErlixList
  - add ErlixTerm.to_s, remove ErlixTerm.puts

erlix-0.3 changelog
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* bugfix:
  
  - IO block bug in ErlixConnection#erecv

* feature:
  
  - ErlixList#new("string")
  - ErlixConnection#close
  - ErlixConnection#closed?
  - ErlixConnection#rpc("module","function",ErlixTermList)
  - ErlixConnection#peer



Intsall erlix
------------------------------------------------------------

On Linux/Unix only, need erlang installed.

::
   
   unzip erlix-version.zip
   cd erlix-version
   ./configure
   make
   make install

Tutorial
------------------------------------------------------------

Play with the Erlix::Term
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Erlix::Term is a Module, represents the erlang-term, It is mixed into
some particular classes,like Erlix::Atom,Erlix::Int,Erlix::List... Now we
will play with these classes in irb:

First,open irb,and require the erlix lib:

::
   
   kdr2@kdr2-pc:~$ irb
   irb(main):001:0> require "erlix"
   => true
   irb(main):002:0>

Find all Erlix::Term's subclasses:

::
   
   irb(main):002:0> ObjectSpace.each_object(Class).inject([]){|a,i|a<< i if i.ancestors.any?{|k|k==Erlix::Term};a}
   => [Erlix::Binary, Erlix::Tuple, Erlix::List, Erlix::Atom, Erlix::Ref, Erlix::Pid, Erlix::Float, Erlix::UInt, Erlix::Int] 
   irb(main):003:0>

We see,there are 9 Erlix::Term classes we hava implemented:

- Erlix::Binary
- Erlix::Tuple
- Erlix::List
- Erlix::Atom
- Erlix::Ref
- Erlix::Pid
- Erlix::Float
- Erlix::UInt
- Erlix::Int


Here we go on:

::

   1.9.3-p194 :015 >a1=Erlix::Atom.new("atom1")
    => #<Erlix::Atom:atom1> 
   1.9.3-p194 :016 > a2=Erlix::Atom.new("atom2")
    => #<Erlix::Atom:atom2> 
   1.9.3-p194 :017 > f=Erlix::Float.new(17.0)
    => #<Erlix::Float:17.000000> 
   1.9.3-p194 :018 > b=Erlix::Binary.new("data\0data")
    => #<Erlix::Binary:#Bin> 
   1.9.3-p194 :019 > b.data
    => "data\x00data" 
   1.9.3-p194 :020 > i=Erlix::Int.new(101)
    => #<Erlix::Int:101> 
   1.9.3-p194 :021 > t=Erlix::Tuple.new([a1,f,b,a2,i])
    => #<Erlix::Tuple:{atom1,17.000000,#Bin,atom2,101}> 
   1.9.3-p194 :022 > list=Erlix::List.new([a1,i,t])
    => #<Erlix::List:[atom1,101,{atom1,17.000000,#Bin,atom2,101}]> 
   1.9.3-p194 :023 > t[2]
    => #<Erlix::Float:17.000000> 
   1.9.3-p194 :024 > t[2].class
    => Erlix::Float 
   1.9.3-p194 :025 > t[3].class
    => Erlix::Binary 
   1.9.3-p194 :026 > t.nth(3)==t[3]
    => true 
   1.9.3-p194 :027 > list.head
    => #<Erlix::Atom:atom1> 
   1.9.3-p194 :028 > list.tail
    => #<Erlix::List:[101,{atom1,17.000000,#Bin,atom2,101}]> 
   1.9.3-p194 :029 > list2=list.cons(b)
    => #<Erlix::List:[#Bin,atom1,101,{atom1,17.000000,#Bin,atom2,101}]> 
   1.9.3-p194 :030 > 
   


Some Ruby-Type var can be auto-converted to particular Erlang-Type

::
   
   |Ruby-Type | -> | Erlang-Type   |
   |FixNum    | -> | Erlix::Int    |
   |Float     | -> | Erlix::Float  |
   |String    | -> | Erlix::List   |
   |Symbol    | -> | Erlix::Atom   |
   

::

   1.9.3-p194 :032 >c=Erlix::List.new(["string-to-list",1,:symbol_to_atom,1.00])
    => #<Erlix::List:["string-to-list",1,symbol_to_atom,1.000000]> 
   1.9.3-p194 :033 > c.head.class
    => Erlix::List 
   1.9.3-p194 :034 > 


Use singleton method [] to create Erlix::Tuple/Erlix::List

::

   1.9.3-p194 :037 > c=Erlix::List["string-to-list",1,:symbol_to_atom,1.00]
    => #<Erlix::List:["string-to-list",1,symbol_to_atom,1.000000]> 
   1.9.3-p194 :038 > t=Erlix::Tuple["string-to-list",1,:symbol_to_atom,1.00]
    => #<Erlix::Tuple:{"string-to-list",1,symbol_to_atom,1.000000}> 
   1.9.3-p194 :039 > t[1]
    => #<Erlix::List:"string-to-list"> 
   1.9.3-p194 :040 > t[3]
    => #<Erlix::Atom:symbol_to_atom> 
   1.9.3-p194 :041 > sl=%w[abc def xyz]
    => ["abc", "def", "xyz"] 
   1.9.3-p194 :042 > Erlix::Tuple[*sl]
    => #<Erlix::Tuple:{"abc","def","xyz"}> 
   1.9.3-p194 :043 > 



And we can use ``match`` to test a Erlix::Term's format, use ``mget`` to get
a particular Erlix::Term inside another Erlix::Term: 

::

   1.9.3-p194 :043 > list2
    => #<Erlix::List:[#Bin,atom1,101,{atom1,17.000000,#Bin,atom2,101}]> 
   1.9.3-p194 :044 > list2.match("[B,Atom,101,Tuple]")
    => true 
   1.9.3-p194 :045 > list2.match("[nomatch,B,Atom,101,Tuple]")
    => false 
   1.9.3-p194 :046 > t2=list2.mget("[B,Atom,101,Tuple]","Tuple")
    => #<Erlix::Tuple:{atom1,17.000000,#Bin,atom2,101}> 
   1.9.3-p194 :047 > 


Play with the Real-Erlang-Node
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

First write a erlang module for our test:

::
   
  -module(test).
  -compile(export_all).
  loop()->
      %% receive msg,the msg's format is {FromPid,Msg}
      %% the FromPid is a pid in our ruby program,
      %% we use "FromPid ! Msg" to echo the Msg back
      receive
          {FromPid,Msg} ->
              io:format("From:[~w], MSG From Ruby:[~w] ~n",[FromPid,Msg]),
              FromPid ! Msg,
              loop()
      end.
  
  run()->
      Pid=spawn(fun loop/0),
      %% Associates the name "my_pid" with Pid
      %% so that our ruby code can find this pid and send message to it!
      register(my_pid,Pid).


Then we satrt an Erlang-Node to run the test module,you must give the
erlang-node a short name or name: 

::
   
   kdr2@kdr2-pc:~/work/erlix/test$ erl -sname foo
   Erlang (BEAM) emulator version 5.6.5 [source] [smp:2] [async-threads:0] [kernel-poll:false]

   Eshell V5.6.5  (abort with ^G)
   (foo@kdr2-pc)1> c(test).
   {ok,test}
   (foo@kdr2-pc)2> test:run().
   true
   (foo@kdr2-pc)3>


My hostname is "kdr2-pc", with the option "-sname foo",the erlang-node
name is "foo@kdr2-pc". 

The time we write ruby code with erlix now:

::
   
   #!/usr/bin/ruby
   # -*- coding: utf-8 -*-

   require "erlix"
   
   # init Erlix::Node,
   #  the first argument is the short-name of the Erlix::Node
   #  the second argument is the erlang cookie, use nil it's will read ~/.erlang.cookie
   # after init, my Erlix::Node's name is inited to "ruby@kdr2-pc"
   Erlix::Node.init("ruby",nil)

   # connect to the real Erlang-Node:
   c=Erlix::Connection.new("foo@kdr2-pc")
   puts "connect ok"

   # create a new Pid with the connection
   # we will use this Pid as the FromPid
   p=Erlix::Pid.new(c)

   # make a Erlix::Tuple {Pid,test_atom} and send it to the real erlang-node
   c.esend("my_pid",Erlix::Tuple.new([p,Erlix::Atom.new("test_atom")]))
   puts "send ok"

   # start a new thread to receive the msg from the real erlang-node
   puts "receiving"
   t=Thread.new{
       while true do
           m=c.erecv
           puts m.mtype
           puts m.message
           puts m.class
           puts m.from
           puts m.to
      end
   }

   t.join



Run the test code:

::
   
   kdr2@kdr2-pc:~/work/erlix$ ruby test/erlix_test.rb
   connect ok
   send ok
   receiving
   ERL_SEND
   test_atom
   Erlix::Message
   nil
   <3.6.3>
   ...


The output of erlang:

::
   
   (foo@kdr2-pc)2> test:run().
   true
   From:[<6027.3.6>], MSG From Ruby:[test_atom]
   (foo@kdr2-pc)3>

   


About the class Erlix::Message
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The method ``Erlix::Connection#erecv`` return an instance of ``Erlix::Message``,
``Erlix::Message`` represents the struct ``ErlMessge`` in erl_interface,it has several fields:
    
    - type
    - msg
    - from
    - to
     
You can call ``Erlix::Message#mtype``, ``Erlix::Message#message``, ``Erlix::Message#from``,
``Erlix::Message#to`` to get them.
    
There's the description of ErlMessage from the erl_connect manual:

::
   
     This function receives the message into the specified buffer, and decodes into the (ErlMessage *) emsg.

              fd is an open descriptor to an Erlang connection.

              bufp is a buffer large enough to hold the expected message.

              bufsize indicates the size of bufp.

              emsg is a pointer to an ErlMessage structure, into which the message will be decoded. ErlMessage is defined as follows:

              typedef struct {
                int type;
                ETERM *msg;
                ETERM *to;
                ETERM *from;
                char to_name[MAXREGLEN];
              } ErlMessage;
              
     Note:
       The definition of ErlMessage has changed since earlier versions of Erl_Interface.

       type identifies the type of message, one of ERL_SEND, ERL_REG_SEND, ERL_LINK, ERL_UNLINK and ERL_EXIT.

       If  type  contains  ERL_SEND  this indicates that an ordinary send operation has taken place, and emsg->to contains the Pid of the
       recipient. If type contains ERL_REG_SEND then a registered send operation took place, and  emsg->from  contains  the  Pid  of  the
       sender. In both cases, the actual message will be in emsg->msg.

       If  type contains one of ERL_LINK or ERL_UNLINK, then emsg->to and emsg->from contain the pids of the sender and receipient of the
       link or unlink. emsg->msg is not used in these cases.

       If type contains ERL_EXIT, then this indicates that a link has been broken. In this case, emsg->to and emsg->from contain the pids
       of the linked processes, and emsg->msg contains the reason for the exit.


Erlix RPC
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::
   
   c=Erlix::Connection.new("foo@kdr2-pc")
   
   #rpc call
   fmt=Erlix::List.new("abc~n")
   tmp=Erlix::List.new
   args=Erlix::List.new([fmt,tmp])
   ret=c.rpc("io","format",args)
   puts ret;
   puts ret.class


.. Note::

   Before you make a ``Erlix::Connection#rpc`` call,  ``Erlix::Connection#erecv`` and
   ``Erlix::Connection#esend`` calls must be stopped(There may be an thread runing and
   blocking on erecv call,you can make rpc-thead and erecv-thread as two mutual exclusive
   threads). After the ``Erlix::Connection#rpc``  call returned, ``Erlix::Connection#erecv``
   and ``Erlix::Connection#esend`` can be called again.


