#!/bin/sh

docker run -dit -p 8080:8080 -v ${PWD}:/MG-HTTPS-Server dockerserver bash