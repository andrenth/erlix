
$:.unshift "."
require "erlix"

a1=Erlix::Atom.new("atom1")
puts a1
a2=Erlix::Atom.new("atom2")
puts a2
f=Erlix::Float.new(17.0)
puts f
b=Erlix::Binary.new("data\0data")
puts b
puts b.data
i=Erlix::Int.new(101)
puts i
t=Erlix::Tuple.new([a1,f,b,a2,i])
puts t
list=Erlix::List.new(a1,i,t)
puts list
puts t[2]
puts t[2].class
puts t[3].class
puts t.nth(3)==t[3]
puts list.head
puts list.tail
list2=list.cons(b)
puts list2

c=Erlix::List.new(["string-to-list",1,:symbol_to_atom,1.00])
puts c
puts c.head.class

c=Erlix::List["string-to-list",1,:symbol_to_atom,1.00]
puts c
t=Erlix::Tuple["string-to-list",1,:symbol_to_atom,1.00]
puts t

puts  t[1]
puts t[3]
puts sl=%w[abc def xyz]
puts Erlix::Tuple[*sl]

puts list2
puts list2.match("[B,Atom,101,Tuple]")
puts list2.match("[nomatch,B,Atom,101,Tuple]")
t2=list2.mget("[B,Atom,101,Tuple]","Tuple")
puts t2
