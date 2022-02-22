#!/bin/sh

# Stop on errors, print commands
#set -Eeuo pipefail

make
./service &

sleep 1
while ! [[ $(pidof service) ]]; do
	sleep 1
done

fails=0

# Test_hello_world
if curl -s localhost:8080/hello | grep -q 'Hello World!' && tail -n 1 log.txt | grep -q 'Hello world reached'; then
	echo "Test_hello_world successful"
else
	echo "Test_hello_world failed"
	((fails++))
fi

kill -9 `pidof service`
sleep 1

./service -s &

sleep 1
while ! [[ $(pidof service) ]]; do
	sleep 1
done

# Test_hello_world_secure
if curl -s -k https://localhost:8080/hello | grep -q 'Hello World!' && tail -n 1 log.txt | grep -q 'Hello world reached'; then
	echo "Test_hello_world_secure successful"
else
	echo "Test_hello_world_secure failed"
	((fails++))
fi

# Test_hello_world_digest
if curl -s -k --digest --user username:password https://localhost:8080/service | grep -q 'Successful digest' && tail -n 1 log.txt | grep -q 'Successfully digested user'; then
	echo "Test_hello_world_digest successful"
else
	echo "Test_hello_world_digest failed"
	((fails++))
fi

# Test_hello_world_digest_fail
if curl -s -k --digest --user username:junk https://localhost:8080/service | grep -q 'Incorrect username or password' && tail -n 1 log.txt | grep -q 'Failed to digest user'; then
	echo "Test_hello_world_digest_fail successful"
else
	echo "Test_hello_world_digest_fail failed"
	((fails++))
fi

if [ $fails -gt 0 ]; then
	echo "Some tests failed"
else
	echo "All tests passed"
fi

kill -9 `pidof service`
