#!/bin/bash

echo "=== Testing INVITE functionality ==="

# Test 1: Admin creates channel and sets invite-only
echo "Test 1: Admin setup"
(
echo "PASS test123"
echo "NICK admin"  
echo "USER admin 0 * :Admin User"
sleep 1
echo "JOIN #test"
sleep 1
echo "MODE #test +i"
sleep 1
echo "QUIT"
) | nc 127.0.0.1 6667 &

sleep 3

# Test 2: User tries to join invite-only channel (should fail)
echo "Test 2: User tries to join without invite"
(
echo "PASS test123"
echo "NICK user1"
echo "USER user1 0 * :User One"
sleep 1
echo "JOIN #test"
sleep 1
echo "QUIT"
) | nc 127.0.0.1 6667 &

sleep 3

# Test 3: Admin invites user
echo "Test 3: Admin invites user, then user joins"
(
echo "PASS test123"
echo "NICK admin2"
echo "USER admin2 0 * :Admin Two"
sleep 1
echo "JOIN #test"
sleep 1
echo "INVITE user2 #test"
sleep 2
echo "QUIT"
) | nc 127.0.0.1 6667 &

sleep 1

(
echo "PASS test123"
echo "NICK user2"
echo "USER user2 0 * :User Two"
sleep 2
echo "JOIN #test"
sleep 1
echo "QUIT"
) | nc 127.0.0.1 6667 &

wait
echo "=== Tests completed ==="