#!/usr/bin/ruby
#
# project : erlix
# author : kdr2
#
require "mkmf"
=begin
findei=File.join(File.dirname(__FILE__),"findei.erl")
File.chmod(0755,findei)
ei_dir=`#{findei}`.chomp
src_dir=File.join(File.dirname(__FILE__),"src")
mkmf=File.join(File.dirname(__FILE__),"extconf.rb")
ext_argv=["--with-ei-dir=#{ei_dir}",
  "--with-cflags=\"-Wall -std=c99\"",
  "--with-ldflags=\"-lei -lerl_interface -lpthread\"",
  "--srcdir=#{src_dir}"]

ext_argv.each{|x|ARGV.push x}
=end

puts ARGV

dir_config("ei")

if have_library("ei","erl_init") and
    have_library("erl_interface","erl_init")
then
  create_makefile("erlix")
else
  puts "error: erl_interface not found!"
end
