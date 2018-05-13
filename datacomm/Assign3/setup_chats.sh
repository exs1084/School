#!/bin/bash

ip -all netns del

ip netns add ns1
ip netns add ns2
ip netns add ns3
ip netns add ns4
ip netns add ns5
ip netns add ns6

ip netns exec ns1 xterm -e bash -c './peerchat -p 10000 -f 10.0.1.1:10000 -f 10.0.2.1:10000 one 11111 11; exec bash' &
ip netns exec ns2 xterm -e bash -c './peerchat two 22222 22; exec bash' &
ip netns exec ns3 xterm -e bash -c './peerchat -p 10000 -f 10.0.0.1:10000 three 33333 33; exec bash' &
ip netns exec ns4 xterm -e bash -c './peerchat four 44444 44; exec bash' &
ip netns exec ns5 xterm -e bash -c './peerchat -p 10000 -f 10.0.0.1:10000 five 55555 55; exec bash' &
ip netns exec ns6 xterm &

ip netns exec ns1 ip link add veth10 type veth peer name bveth1
ip netns exec ns1 ip link set bveth1 netns ns6
ip netns exec ns1 ifconfig lo up
ip netns exec ns1 ifconfig veth10 10.0.0.1/24

ip netns exec ns2 ip link add veth20 type veth peer name bveth2
ip netns exec ns2 ip link set bveth2 netns ns6
ip netns exec ns2 ifconfig lo up
ip netns exec ns2 ifconfig veth20 10.0.0.2/24

ip netns exec ns3 ip link add veth30 type veth peer name bveth3
ip netns exec ns3 ip link set bveth3 netns ns6
ip netns exec ns3 ifconfig lo up
ip netns exec ns3 ifconfig veth30 10.0.1.1/24

ip netns exec ns4 ip link add veth40 type veth peer name bveth4
ip netns exec ns4 ip link set bveth4 netns ns6
ip netns exec ns4 ifconfig lo up
ip netns exec ns4 ifconfig veth40 10.0.1.2/24

ip netns exec ns5 ip link add veth50 type veth peer name veth62
ip netns exec ns5 ip link set veth62 netns ns6
ip netns exec ns5 ifconfig lo up
ip netns exec ns5 ifconfig veth50 10.0.2.1/24

ip netns exec ns6 ip link add br1 type bridge
ip netns exec ns6 ip link add br2 type bridge
ip netns exec ns6 ifconfig br1 up
ip netns exec ns6 ifconfig br2 up
ip netns exec ns6 ifconfig lo up

ip netns exec ns6 ifconfig bveth1 up
ip netns exec ns6 ifconfig bveth2 up
ip netns exec ns6 ifconfig bveth3 up
ip netns exec ns6 ifconfig bveth4 up

ip netns exec ns6 ip link set bveth1 master br1
ip netns exec ns6 ip link set bveth2 master br1
ip netns exec ns6 ip link set bveth3 master br2
ip netns exec ns6 ip link set bveth4 master br2

ip netns exec ns6 ip link add veth60 type veth peer name bveth6
ip netns exec ns6 ifconfig bveth6 up
ip netns exec ns6 ip link set bveth6 master br1
ip netns exec ns6 ifconfig veth60 10.0.0.250/24
ip netns exec ns6 ip link add veth61 type veth peer name bveth62
ip netns exec ns6 ifconfig bveth62 up
ip netns exec ns6 ip link set bveth62 master br2
ip netns exec ns6 ifconfig veth61 10.0.1.250/24
ip netns exec ns6 ifconfig veth62 10.0.2.250/24
ip netns exec ns6 echo 1 > /proc/sys/net/ipv4/ip_forward



ip netns exec ns1 route add -net 0.0.0.0/0 gw 10.0.0.250
ip netns exec ns2 route add -net 0.0.0.0/0 gw 10.0.0.250
ip netns exec ns3 route add -net 0.0.0.0/0 gw 10.0.1.250
ip netns exec ns4 route add -net 0.0.0.0/0 gw 10.0.1.250
ip netns exec ns5 route add -net 0.0.0.0/0 gw 10.0.2.250

#ip netns exec ns1 wireshark &
