#!/bin/sh

SAN=DNS:smoothstack.com,DNS:www.smoothstack.com \
openssl req -new \
    -config etc/server.conf \
    -out certs/smoothstack.com.csr \
    -keyout certs/smoothstack.com.key \
    -subj "/C=US/ST=Virginia/L=McLean/O=SmoothStack/OU=Training/CN=smoothstack.com/emailAddress=marc.gomez@smoothstack.com"

openssl ca \
    -config etc/tls-ca.conf \
    -in certs/smoothstack.com.csr \
    -out certs/smoothstack.com.crt \
    -extensions server_ext

openssl pkcs12 -export \
    -name "smoothstack.com (Network Component)" \
    -caname "SmoothStack TLS CA" \
    -caname "SmoothStack Root CA" \
    -inkey certs/smoothstack.com.key \
    -in certs/smoothstack.com.crt \
    -certfile ca/tls-ca-chain.pem \
    -out certs/smoothstack.com.p12

openssl req -new \
    -config etc/client.conf \
    -out certs/marc.csr \
    -keyout certs/marc.key

openssl ca \
    -config etc/tls-ca.conf \
    -in certs/marc.csr \
    -out certs/marc.crt \
    -policy extern_pol \
    -extensions client_ext

openssl pkcs12 -export \
    -name "Marc Gomez (Network Access)" \
    -caname "SmoothStack TLS CA" \
    -caname "SmoothStack Root CA" \
    -inkey certs/marc.key \
    -in certs/marc.crt \
    -certfile ca/tls-ca-chain.pem \
    -out certs/marc.p12

openssl verify -CAfile ca/tls-ca-chain.pem certs/marc.crt

# Revoke CA

#openssl ca \
#    -config etc/tls-ca.conf \
#    -revoke ca/tls-ca/01.pem \
#    -crl_reason affiliationChanged
#
#openssl ca -gencrl \
#    -config etc/tls-ca.conf \
#    -out crl/tls-ca.crl