#!/usr/bin/ruby
#-*- ruby -*-
# project : erlix
# author : kdr2
#

findei=File.join(File.dirname(__FILE__),"findei.erl")
File.chmod(0755,findei)
ei_dir=`#{findei}`.chomp
src_dir=File.join(File.dirname(__FILE__),"src")
mkmf=File.join(File.dirname(__FILE__),"extconf.rb")
cmd=[
  "ruby #{mkmf}",
  "--with-ei-dir=#{ei_dir}",
  "--with-cflags=\"-std=c99\"",
  "--with-ldflags=\"-lei -lerl_interface -lpthread\"",
  "--srcdir=#{src_dir}"].join(" ")

env=[
  "--with-ei-dir=#{ei_dir}",
  "--with-cflags=\"-std=c99\"",
  "--with-ldflags=\"-lei -lerl_interface -lpthread\"",
  "--srcdir=#{src_dir}"].join(" ")
puts cmd
puts `#{cmd}`


