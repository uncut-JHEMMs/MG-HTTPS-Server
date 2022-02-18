#ifndef SERVICE_RESOURCE_HPP
#define SERVICE_RESOURCE_HPP

#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>
#include <unordered_map>
#include <condition_variable>
#include <mutex>
#include <httpserver.hpp>

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd5"

std::string logStr;
std::mutex mut;
std::condition_variable cv;

void logging() {
	std::ofstream logFile("log.txt", std::ios_base::app);
	if (!logFile.is_open()) {
		std::cout << "Failed to open log.txt" << std::endl;
		exit(1);
	}

	while (true) {
		std::unique_lock<std::mutex> lk(mut);
		cv.wait(lk, []{return logStr != "";});
		auto clock = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(clock);
		logFile << logStr << " | " << std::ctime(&time) << std::flush;
		logStr = "";
	}
}

class hello_resource: public httpserver::http_resource {
 public:
	 hello_resource() {};

	 ~hello_resource() {};

	const std::shared_ptr<httpserver::http_response> render_GET(const httpserver::http_request &req) {
		httpserver::string_response* res = new httpserver::string_response("Hello World!\n", 200);
		{
			std::lock_guard<std::mutex> lk(mut);
			logStr = "Hello world reached";
		}
		cv.notify_one();
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
			{
				std::lock_guard<std::mutex> lk(mut);
				logStr = "Failed to digest user: " + req.get_digested_user();
			}
			cv.notify_one();

			return std::shared_ptr<httpserver::digest_auth_fail_response>
				(new httpserver::digest_auth_fail_response("Incorrect username or password\n",
					"default realm", MY_OPAQUE, false));
		}
		else {
			bool reload_nonce = false;
			if(!req.check_digest_auth("default realm", users[req.get_digested_user()], 300, &reload_nonce)) {
				{
					std::lock_guard<std::mutex> lk(mut);
					logStr = "Failed to digest user: " + req.get_digested_user();
				}
				cv.notify_one();

				return std::shared_ptr<httpserver::digest_auth_fail_response>
					(new httpserver::digest_auth_fail_response("Incorrect username or password\n",
						"default realm", MY_OPAQUE, reload_nonce));
			}
		}

		{
			std::lock_guard<std::mutex> lk(mut);
			logStr = "Successfully digested user: " + req.get_digested_user();
		}
		cv.notify_one();
		
		return std::shared_ptr<httpserver::string_response>
			(new httpserver::string_response("Successful digest\n", 200, "text/plain"));
	}
};

#endif