#!/bin/sh

# Remove previous certificates
rm *.pem

# 1. Generate CA's private key and self-signed certificate
openssl req -x509 -newkey rsa:4096 -days 365 -nodes -keyout certs/ca-key.pem -out certs/ca-cert.pem -subj "/C=US/ST=Michigan/L=Novi/O=Smoothstack/OU=Training/CN=smoothstack.com/emailAddress=marc.gomez@smoothstack.com"

echo "CA's self-signed certificate"
openssl x509 -in certs/ca-cert.pem -noout -text

# 2. Generate web server's private key and certificate signing request (CSR)
openssl req -newkey rsa:4096 -nodes -keyout certs/server-key.pem -out certs/server-req.pem -subj "/C=US/ST=Virginia/L=McLean/O=Smoothstack/OU=Training/CN=smoothstack.com/emailAddress=marc.gomez@smoothstack.com"

# 3. Use CA's private key to sign web server's CSR and get back the signed certificate
openssl x509 -req -in certs/server-req.pem -days 60 -CA certs/ca-cert.pem -CAkey certs/ca-key.pem -CAcreateserial -out certs/server-cert.pem -extfile certs/server-ext.cnf

echo "Server's signed certificate"
openssl x509 -in certs/server-cert.pem -noout -text

# 4. Verify certificate
openssl verify -CAfile certs/ca-cert.pem certs/server-cert.pem