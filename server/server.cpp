#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <httpserver.hpp>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

bool verbose = false;

class service_resource: public httpserver::http_resource {
public:
	service_resource();

	~service_resource();

	const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render_PUT(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render_POST(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render_HEAD(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render_OPTIONS(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render_CONNECT(const httpserver::http_request &req);
	const std::shared_ptr<httpserver::http_response> render_DELETE(const httpserver::http_request &req);
};

service_resource::service_resource() { }

service_resource::~service_resource() { }

const std::shared_ptr<httpserver::http_response> service_resource::render_GET(const httpserver::http_request &req) {
	std::cout << "service_resource::render_GET()" << std::endl;

	if (verbose) std::cout << req;
	httpserver::string_response* res = new httpserver::string_response("GET response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}


const std::shared_ptr<httpserver::http_response> service_resource::render_PUT(const httpserver::http_request &req) {
	std::cout << "service_resource::render_PUT()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("PUT response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

const std::shared_ptr<httpserver::http_response> service_resource::render_POST(const httpserver::http_request &req) {
	std::cout << "service_resource::render_POST()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("POST response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

const std::shared_ptr<httpserver::http_response> service_resource::render(const httpserver::http_request &req) {
	std::cout << "service_resource::render()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("generic response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

const std::shared_ptr<httpserver::http_response> service_resource::render_HEAD(const httpserver::http_request &req) {
	std::cout << "service_resource::render_HEAD()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("HEAD response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

const std::shared_ptr<httpserver::http_response> service_resource::render_OPTIONS(const httpserver::http_request &req) {
	std::cout << "service_resource::render_OPTIONS()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("OPTIONS response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

const std::shared_ptr<httpserver::http_response> service_resource::render_CONNECT(const httpserver::http_request &req) {
	std::cout << "service_resource::render_CONNECT()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("CONNECT response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

const std::shared_ptr<httpserver::http_response> service_resource::render_DELETE(const httpserver::http_request &req) {
	std::cout << "service_resource::render_DELETE()" << std::endl;

	if (verbose) std::cout << req;

	httpserver::string_response* res = new httpserver::string_response("DELETE response\n", 200);

	if (verbose) std::cout << *res;

	return std::shared_ptr<httpserver::http_response>(res);
}

void usage() {
	std::cout << "Usage:" << std::endl
			  << "service [-p <port>] [-C <maxConnections>] [-m <memoryLimit>]\n[-t <timeoutTime>] [-s [-k <keyFileName>] [-c <certFileName>]] [-v] [-i]" << std::endl;
}

int main(int argc, char **argv) {
	uint16_t port = 8080;
	unsigned long maxConns = 0;
	unsigned long long maxMem = 0;
	unsigned long timeoutTime = 180;
	int maxThreads = 5;
	int c;
	const char *key = "pki/ia.key";
	const char *cert = "pki/ia.crt";
	const char *config = "config.json";
	bool secure = false;
	bool connLimit = false;
	bool memLimit = false;
	bool useIpv6 = false;
	bool timeout = false;
	json j;

	while ((c = getopt(argc, argv, "f::p:C:m:t:k:c:svi?")) != EOF) {
		switch (c) {
			case 'f': {
				if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
					optarg = argv[optind++];
				}
				if (optarg != NULL) {
					config = optarg;
				}
				std::ifstream configFile(config);
				if (!configFile.is_open()) {
					std::cout << "Failed to open " << config << std::endl;
					exit(1);
				}
				configFile >> j;
				if (j.contains("port")) {
					port = j["port"];
				}
				if (j.contains("max_threads")) {
					maxThreads = j["max_threads"];
				}
				if (j.contains("max_connections")) {
					connLimit = true;
					maxConns = j["max_connections"];
				}
				if (j.contains("connection_timeout")) {
					timeout = true;
					timeoutTime = j["connection_timeout"];
				}
				if (j.contains("use_ssl")) {
					secure = true;
				}
				if (j.contains("https_mem_key")) {
					key = string(j["https_mem_key"]).c_str();
				}
				if (j.contains("https_mem_cert")) {
					cert = string(j["https_mem_cert"]).c_str();
				}
				if (j.contains("memory_limit")) {
					memLimit = true;
					maxMem = j["memory_limit"];
				}
				if (j.contains("use_ipv6")) {
					useIpv6 = true;
				}
				break;
			}
			case 'p': {
				port = strtoul(optarg, nullptr, 10);
				break;
			}
			case 'C': {
				connLimit = true;
				maxConns = strtoul(optarg, nullptr, 10);
				break;
			}
			case 'm': {
				memLimit = true;
				maxMem = strtoull(optarg, nullptr, 10);
				break;
			}
			case 't': {
				timeout = true;
				timeoutTime = strtoul(optarg, nullptr, 10);
				break;
			}
			case 'k': {
				key = optarg;
				break;
			}
			case 'c': {
				cert = optarg;
				break;
			}
			case 's': {
				secure = true;
				break;
			}
			case 'v': {
				verbose = true;
				break;
			}
			case 'i': {
				useIpv6 = true;
				break;
			}
			default: {
				usage();
				exit(1);
				break;
			}
		}
	}



	std::cout << "Using port " << port << std::endl;
	if (secure) {
		std::cout << "Key: " << key << " Certificate: " << cert
				  << std::endl;
	}

	// Use builder to define webserver configuration options
	httpserver::create_webserver cw = httpserver::create_webserver(port).max_threads(maxThreads);

	if (secure) {
		cw.use_ssl().https_mem_key(key).https_mem_cert(cert);
	}

	if (connLimit) {
		cw.max_connections(maxConns);
	}

	if (memLimit) {
		cw.memory_limit(maxMem);
	}

	if (useIpv6) {
		cw.use_ipv6().use_dual_stack();
	}

	if (timeout) {
		cw.connection_timeout(timeoutTime);
	}

	// Create webserver using the configured options
	httpserver::webserver ws = cw;

	// Create and register service resource available at /service
	service_resource res;
	ws.register_resource("/service", &res, true);

	// Start and block the webserver
	ws.start(true);

	return 0;
}