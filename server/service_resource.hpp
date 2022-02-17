#ifndef SERVICE_RESOURCE_HPP
#define SERVICE_RESOURCE_HPP

#include <string>
#include <unordered_map>
#include <httpserver.hpp>

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd5"

class hello_resource: public httpserver::http_resource {
 public:
     hello_resource() {};

     ~hello_resource() {};

	const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request &req) {
	    std::cout << "hello GET" << std::endl;
	    httpserver::string_response* res = new httpserver::string_response("Hello World!\n", 200);
	    return std::shared_ptr<httpserver::http_response>(res);
	}
};


class service_resource: public httpserver::http_resource {
private:
	std::unordered_map<std::string, std::string> users;

public:
	service_resource() {
		users["username"] = "password";
	}

	~service_resource() { }

	const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request &req) {
		if (req.get_digested_user() == "") {
			return std::shared_ptr<httpserver::digest_auth_fail_response>
				(new httpserver::digest_auth_fail_response("FAIL\n",
					"default realm", MY_OPAQUE, true));
		}
		else if (users.find(req.get_digested_user()) == users.end()) {
			return std::shared_ptr<httpserver::digest_auth_fail_response>
				(new httpserver::digest_auth_fail_response("Incorrect username or password\n",
					"default realm", MY_OPAQUE, false));
		}
		else {
			bool reload_nonce = false;
			if(!req.check_digest_auth("default realm", users[req.get_digested_user()], 300, &reload_nonce)) {
				return std::shared_ptr<httpserver::digest_auth_fail_response>
					(new httpserver::digest_auth_fail_response("Incorrect username or password\n",
						"default realm", MY_OPAQUE, reload_nonce));
			}
		}
		std::cout << "GET success, user: " << req.get_digested_user() << std::endl;
		return std::shared_ptr<httpserver::string_response>
			(new httpserver::string_response("SUCCESS\n", 200, "text/plain"));
	}
};

#endif