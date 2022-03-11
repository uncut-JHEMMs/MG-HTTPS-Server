# MG-HTTPS-Server

## Folders

- **bin**: Shell scripts to Build, Spinup, and Attach a Docker container using the given Dockerfile
- **pki**: CA files for HTTPS
- **server**: all server files
- **server/tests**: C++ server unit tests
- **server/performance**: Shell scripts to test server performance
- **server/output**: Output files from tests

## Important Files

- **server/config.json**: JSON file for custom server startup options
- **server/log.txt**: log file of server interactions

## Scripts

- **apm&#46;sh**: Run various performance monitoring commands while the server is running and print to /output
- **baseStats&#46;sh**: Prints basic server stats to output/InitialStats.txt and similar files
- **latency&#46;sh**: Prints server latency stats to output/scSummary.txt and output/csSummary.txt
- **performance/data/dataThroughput&#46;sh**: Prints data throughput stats to performance/data/DataSummary.txt

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

Test config code:

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
| Options | Description |
| ------- | ----------- |
| -f \<optional\> | Provide a custom json config file, config.json by default |
| -p \<required\> | Port number |
| -C \<required\> | Maximum connections |
| -m \<required\> | Memory limit (Bytes) |
| -t \<required\> | Timeout time (seconds) |
| -k \<required\> | CA Key file |
| -c \<required\> | CA cert file |
| -s | Use HTTPS |
| -v | Verbose |
| -i | Use IPv4 and IPv6 |

Config file format:
```javascript
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