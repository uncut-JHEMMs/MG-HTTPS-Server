# MG-HTTPS-Server

## Folders

- **bin**: Shell scripts to Build, Spinup, and Attach a Docker container using the given Dockerfile
- **pki**: CA files for HTTPS
- **server**: all server files

## Files

- **server/config.json**: JSON file for custom server startup options
- **server/log.txt**: log file of server interactions

## Basic Usage

Build Docker image (only needs to be done once):

```sh
bash bin/dockerBuild.sh
```

Spinup Docker container:

```sh
bash bin/dockerSpinup.sh
```

Attach to Docker container:

```sh
bash bin/dockerAttach.sh
```

Create Certificate Authority (only once):

```sh
cd server
bash genkey.sh
```

Build server executable:

```sh
make
```

Test server:

```sh
bash server_tests.sh
```

Test config:

```sh
cd tests
make
```

Run server:

```sh
cd ..
./service <options>
```

## Options and Formatting

Server startup options:
| Option | Description |
| ------ | ----------- |
| -f <optional> | Provide a custom json config file, config.json by default if no file parameter provided |
| -p <required> | Port number |
| -C <required> | Maximum connections |
| -m <required> | Memory limit (Bytes) |
| -t <required> | Timeout time (seconds) |
| -k <required> | CA Key file |
| -c <required> | CA cert file |
| -s | Use https |
| -v | Verbose |
| -i | Use IPv4 and IPv6 |

Config file format:
```json
{
	"port" : int,
	"max_threads" : int,
	"max_connections" : int,
	"connection_timeout" : int,
	"use_ssl": bool,
	"https_mem_key": string,
	"https_mem_cert": string,
	"memory_limit": int,
	"use_ipv6": bool
}
```
