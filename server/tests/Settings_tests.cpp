#include <string>
#include <cstring>
#include "../settings.hpp"
#include "unit_test_framework.h"

using namespace std;

TEST(test_default_settings) {
	Settings s;
	ASSERT_EQUAL(s.port, 8080);
	ASSERT_EQUAL(s.maxConns, 15);
	ASSERT_EQUAL(s.maxMem, 32768);
	ASSERT_EQUAL(s.timeoutTime, 180);
	ASSERT_EQUAL(s.maxThreads, 5);
	ASSERT_EQUAL(s.key, "pki/ia.key");
	ASSERT_EQUAL(s.cert, "pki/ia.crt");
	ASSERT_EQUAL(s.config, "config.json");
	ASSERT_EQUAL(s.secure, false);
	ASSERT_EQUAL(s.connLimit, false);
	ASSERT_EQUAL(s.memLimit, false);
	ASSERT_EQUAL(s.useIpv6, false);
	ASSERT_EQUAL(s.timeout, false);
	ASSERT_EQUAL(s.verbose, false);
}

TEST(test_verbose) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'v', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_TRUE(s.verbose);
}

TEST(test_ipv6) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'i', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_TRUE(s.useIpv6);
}

TEST(test_secure) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 's', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_TRUE(s.secure);
}

TEST(test_cert) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'c', ' ', 'c', 'e', 'r', 't', '.', 'f', 'i', 'l', 'e', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_EQUAL(s.cert, "cert.file");
}

TEST(test_key) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'c', ' ', 'k', 'e', 'y', '.', 'f', 'i', 'l', 'e', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_EQUAL(s.cert, "key.file");
}

TEST(test_timeout) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 't', ' ', '1', '0', '0', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_TRUE(s.timeout);
	ASSERT_EQUAL(s.timeoutTime, 100);
}

TEST(test_mem) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'm', ' ', '6', '5', '5', '3', '6', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_TRUE(s.memLimit);
	ASSERT_EQUAL(s.maxMem, 65536);
}

TEST(test_connections) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'C', ' ', '1', '0', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_TRUE(s.connLimit);
	ASSERT_EQUAL(s.maxConns, 10);
}

TEST(test_port) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'p', ' ', '8', '0', '8', '1', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_EQUAL(s.port, 8081);
}

TEST(test_JSON) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'f', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_EQUAL(s.port, 8082);
}

TEST(test_JSON_given) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'f', ' ', 't', 'e', 's', 't', '.', 'j', 's', 'o', 'n', '\0'};
	char * argv[] = {arg1, options};
	getSettings(2, argv, s);
	ASSERT_EQUAL(s.port, 8083);
}

TEST(test_JSON_given_argc3) {
	Settings s;
	char arg1[] = {'\0'};
	char options[] = {'-', 'f', '\0'}; 
	char options2[] = {'t', 'e', 's', 't', '.', 'j', 's', 'o', 'n', '\0'};
	char * argv[] = {arg1, options, options2};
	getSettings(3, argv, s);
	ASSERT_EQUAL(s.port, 8083);
}

TEST_MAIN()
