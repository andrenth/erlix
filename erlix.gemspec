# -*- ruby -*-

require 'rake'

Gem::Specification.new do |s|
  s.name        = 'erlix'
  s.version     = '0.2.0'
  s.summary     = "Run ruby as an Erlang Node!"
  s.description = s.summary
  s.authors     = ["KDr2"]
  s.email       = 'killy.draw@gmail.com'
  #s.files      = ["lib/example.rb"]
  s.homepage    = 'http://github.com/KDr2/erlix'
  s.required_ruby_version = '>= 1.9.2'
  s.extensions  << 'extconf.rb'
  s.post_install_message = "Thanks for installing!"
  s.files       = FileList['src/*.c','src/*.h'].to_a
end

