#!/bin/sh

openssl req -new \
    -config etc/codesign.conf \
    -out certs/document.csr \
    -keyout certs/document.key

openssl ca \
    -config etc/document-ca.conf \
    -in certs/document.csr \
    -out certs/document.crt \
    -extensions codesign_ext

openssl pkcs12 -export \
    -name "SmoothStack Document Certificate" \
    -caname "SmoothStack Document CA" \
    -caname "SmoothStack Root CA" \
    -inkey certs/document.key \
    -in certs/document.crt \
    -certfile ca/document-ca-chain.pem \
    -out certs/document.p12

openssl verify -CAfile ca/document-ca-chain.pem certs/document.crt

# Revoke CA

#openssl ca \
#    -config etc/document-ca.conf \
#    -revoke ca/document-ca/01.pem \
#    -crl_reason unspecified
#
#openssl ca -gencrl \
#    -config etc/document-ca.conf \
#    -out crl/document-ca.crl