#!/bin/sh

mkdir -p ca/document-ca/private ca/document-ca/db crl certs
chmod 700 ca/document-ca/private

cp /dev/null ca/document-ca/db/document-ca.db
cp /dev/null ca/document-ca/db/document-ca.db.attr
echo 01 > ca/document-ca/db/document-ca.crt.srl
echo 01 > ca/document-ca/db/document-ca.crl.srl

openssl req -new \
    -config etc/document-ca.conf \
    -out ca/document-ca.csr \
    -keyout ca/document-ca/private/document-ca.key

openssl ca \
    -config etc/root-ca.conf \
    -in ca/document-ca.csr \
    -out ca/document-ca.crt \
    -extensions signing_ca_ext

openssl ca -gencrl \
    -config etc/document-ca.conf \
    -out crl/document-ca.crl

cat ca/document-ca.crt ca/root-ca.crt > \
    ca/document-ca-chain.pem

openssl verify -CAfile ca/root-ca.crt ca/document-ca-chain.pem
