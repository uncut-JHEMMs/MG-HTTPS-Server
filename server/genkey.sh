#!/bin/sh

# Stop on errors, print commands
set -Eeuo pipefail
set -x

mkdir -p pki

# Generate 4096-bit long RSA key for root CA and store it in ca.key
openssl genrsa -out pki/ca.key 4096

# Create self-signed root CA certificate ca.crt
openssl req -new -x509 -days 1826 -key pki/ca.key -out pki/ca.crt -subj "/C=US/ST=Michigan/L=Novi/O=Smoothstack/OU=Training/CN=smoothstack.com/emailAddress=marc.gomez@smoothstack.com"

# Generate key for subordinate CA that will be used for actual signing
openssl genrsa -out pki/ia.key 4096

# Request a certificate for subordinate CA
openssl req -new -key pki/ia.key -out pki/ia.csr -subj "/C=US/ST=Virginia/L=McLean/O=Smoothstack/OU=Training/CN=smoothstack.com/emailAddress=marc.gomez@smoothstack.com"

# Process request for subordinate CA certificate and have root CA sign it
openssl x509 -req -days 730 -in pki/ia.csr -CA pki/ca.crt -CAkey pki/ca.key -set_serial 01 -out pki/ia.crt

# Package keys and certs in PKCS12 file
openssl pkcs12 -export -out pki/ia.p12 -inkey pki/ia.key -in pki/ia.crt -chain -CAfile pki/ca.crt