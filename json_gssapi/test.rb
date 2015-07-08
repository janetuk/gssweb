#!/usr/bin/ruby

require 'open3'

Open3.popen2('./json_gssapi') do |inIo, outIo, wait_thr|
  puts "gdb --pid=#{wait_thr.pid}"
  while(true) do
    a = gets
    inIo.puts  [a.length].pack('L') + a
    inIo.flush

    outIo.flush
    b = outIo.read(4)
    puts b
    c = outIo.read(b.unpack('L')[0])
    puts c
  end
end

