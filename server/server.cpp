#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <thread>
#include <httpserver.hpp>
#include "service_resource.hpp"
#include "settings.hpp"

int main(int argc, char **argv) {
	Settings s;
	getSettings(argc, argv, s);

	std::cout << "Using port " << s.port << std::endl;
	if (s.secure) {
		std::cout << "Key: " << s.key << " Certificate: " << s.cert << std::endl;
	}

	// Use builder to define webserver configuration options
	httpserver::create_webserver cw = httpserver::create_webserver(s.port).max_threads(s.maxThreads);

	if (s.secure) {
		cw.use_ssl().https_mem_key(s.key).https_mem_cert(s.cert);
	}

	if (s.connLimit) {
		cw.max_connections(s.maxConns);
	}

	if (s.memLimit) {
		cw.memory_limit(s.maxMem);
	}

	if (s.useIpv6) {
		cw.use_ipv6().use_dual_stack();
	}

	if (s.timeout) {
		cw.connection_timeout(s.timeoutTime);
	}

	// Create webserver using the configured options
	httpserver::webserver ws = cw;

	// Create logging thread
	std::thread t(logging);

	// Create and register service resource available at /service
	service_resource res;
	hello_resource res2;
	ws.register_resource("/service", &res, true);
	ws.register_resource("/hello", &res2, true);

	// Start and block the webserver
	ws.start(true);

	t.join();

	return 0;
}