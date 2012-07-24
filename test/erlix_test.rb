#!/usr/bin/ruby
# -*- coding: utf-8 -*-
$:.unshift(".")

require "erlix"

#sleep 10
# init Erlix::Node,
#   *the first argument is the short-name of the Erlix::Node
#   *the second argument is the erlang cookie, use nil it's will read ~/.erlang.cookie
# after init, my Erlix::Node's name is inited to "ruby@KDr2-MacBookPro"
Erlix::Node.init("ruby",nil)

# connect to the real Erlang-Node:
c=Erlix::Connection.new("foo@KDr2-MacBookPro")
puts "connect ok"


#rpc call
fmt=Erlix::List.new("abc~n")
tmp=Erlix::List.new(nil)
args=Erlix::List.new([fmt,tmp])
ret=c.rpc("io","format",args)
puts ret;
puts ret.class

# create a new Pid with the connection
# we will use this Pid as the FromPid
p=Erlix::Pid.new(c)

# make a Erlix::Tuple {Pid,test_atom} and send it to the real erlang-node
c.esend("my_pid",Erlix::Tuple.new([p,Erlix::Atom.new("test_atom")]))
puts "send ok"

# start a new thread to receive the msg from the real erlang-node
puts "receiving"
t=Thread.start("thread recv"){ |name|
  while true do
    puts "+++++"
    m=c.erecv
    puts m.mtype
    puts m.message
    puts m.class
    puts "from: #{m.from} #{m.from.class}"
    puts "to: #{m.to}"
    puts "-----"
  end
}

#send more test:
3.times{
c.esend("my_pid",Erlix::Tuple.new([p,Erlix::Atom.new("test_atom")]))
}

#t.join
sleep 5

