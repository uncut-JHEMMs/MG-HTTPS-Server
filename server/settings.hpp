#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "json.hpp"

// Print out server start options
void usage() {
	std::cout << "Usage:" << std::endl
		 << "service [-p <port>] [-C <maxConnections>] [-m <memoryLimit>]" << std::endl
		 << "[-t <timeoutTime>] [-s [-k <keyFileName>] [-c <certFileName>]] [-v] [-i]" << std::endl;
}

struct Settings {
	uint16_t port;
	unsigned long maxConns;
	unsigned long long maxMem;
	unsigned long timeoutTime;
	int maxThreads;
	std::string key;
	std::string cert;
	std::string config;
	bool secure;
	bool connLimit;
	bool memLimit;
	bool useIpv6;
	bool timeout;
	bool verbose;

	// Default settings
	Settings() : port(8080), maxConns(15), maxMem(32768), timeoutTime(180), maxThreads(5),
				 key("pki/ia.key"), cert("pki/ia.crt"), config("config.json"),
				 secure(false), connLimit(false), memLimit(false), useIpv6(false),
				 timeout(false), verbose(false) {}
};

// Get server settings from a json file
void readFromJSON(int argc, char * const argv[], Settings &s) {
	nlohmann::json j;

	if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
		optarg = argv[optind++];
	}
	if (optarg != NULL) {
		s.config = optarg;
	}

	std::ifstream configFile(s.config);
	if (!configFile.is_open()) {
		std::cout << "Failed to open " << s.config << std::endl;
		exit(1);
	}

	configFile >> j;

	if (j.contains("port")) {
		s.port = j["port"];
	}
	if (j.contains("max_threads")) {
		s.maxThreads = j["max_threads"];
	}
	if (j.contains("max_connections")) {
		s.connLimit = true;
		s.maxConns = j["max_connections"];
	}
	if (j.contains("connection_timeout")) {
		s.timeout = true;
		s.timeoutTime = j["connection_timeout"];
	}
	if (j.contains("use_ssl")) {
		s.secure = j["use_ssl"];
	}
	if (j.contains("https_mem_key")) {
		s.key = j["https_mem_key"];
	}
	if (j.contains("https_mem_cert")) {
		s.cert = j["https_mem_cert"];
	}
	if (j.contains("memory_limit")) {
		s.memLimit = true;
		s.maxMem = j["memory_limit"];
	}
	if (j.contains("use_ipv6")) {
		s.useIpv6 = j["use_ipv6"];
	}
}

// Get server settings
void getSettings(int argc, char * const argv[], Settings &s) {
	int c;
	while ((c = getopt(argc, argv, "f::p:C:m:t:k:c:svi?")) != EOF) {
		switch (c) {
			case 'f': {
				readFromJSON(argc, argv, s);
				break;
			}
			case 'p': {
				s.port = strtoul(optarg, nullptr, 10);
				break;
			}
			case 'C': {
				s.connLimit = true;
				s.maxConns = strtoul(optarg, nullptr, 10);
				break;
			}
			case 'm': {
				s.memLimit = true;
				s.maxMem = strtoull(optarg, nullptr, 10);
				break;
			}
			case 't': {
				s.timeout = true;
				s.timeoutTime = strtoul(optarg, nullptr, 10);
				break;
			}
			case 'k': {
				s.key = optarg;
				break;
			}
			case 'c': {
				s.cert = optarg;
				break;
			}
			case 's': {
				s.secure = true;
				break;
			}
			case 'v': {
				s.verbose = true;
				break;
			}
			case 'i': {
				s.useIpv6 = true;
				break;
			}
			default: {
				usage();
				exit(1);
				break;
			}
		}
	}
}

#endif