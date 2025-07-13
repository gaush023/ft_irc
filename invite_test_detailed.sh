#!/bin/bash

echo "Testing INVITE functionality step by step..."

# Kill existing server and start fresh
pkill -f ircserv
sleep 1
nohup ./ircserv 6667 test123 > server_test.log 2>&1 &
sleep 2

echo "Step 1: Admin creates channel and sets invite-only mode"
{
echo "PASS test123"
sleep 0.5
echo "NICK admin"
sleep 0.5
echo "USER admin 0 * :Admin User"
sleep 0.5
echo "JOIN #test"
sleep 0.5
echo "MODE #test +i"
sleep 0.5
echo "PRIVMSG #test :Channel is now invite-only"
sleep 10
} | nc 127.0.0.1 6667 > admin_session.log &
ADMIN_PID=$!

sleep 3

echo "Step 2: User connects but doesn't join yet"
{
echo "PASS test123"
sleep 0.5
echo "NICK user1"
sleep 0.5
echo "USER user1 0 * :User One"
sleep 5
echo "JOIN #test"
sleep 1
echo "PRIVMSG #test :I joined without invite?"
sleep 3
} | nc 127.0.0.1 6667 > user_session.log &
USER_PID=$!

sleep 8

echo "Step 3: Admin invites user"
{
echo "PASS test123"
sleep 0.5
echo "NICK admin2"
sleep 0.5
echo "USER admin2 0 * :Admin Two"
sleep 0.5
echo "JOIN #test"
sleep 0.5
echo "INVITE user1 #test"
sleep 2
echo "PRIVMSG #test :Invited user1"
sleep 3
} | nc 127.0.0.1 6667 > admin2_session.log &

wait

echo "=== Admin Session Log ==="
cat admin_session.log
echo "=== User Session Log ==="
cat user_session.log
echo "=== Admin2 Session Log ==="
cat admin2_session.log
echo "=== Server Log ==="
cat server_test.log

rm -f admin_session.log user_session.log admin2_session.log server_test.log