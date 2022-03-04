#!/bin/sh

docker run --privileged -dit -p 8080:8080 -v ${PWD}:/MG-HTTPS-Server dockerserver bash

