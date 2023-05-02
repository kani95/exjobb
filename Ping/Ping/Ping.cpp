//Include ops
#include "mqtt/async_client.h"
#include <array>
#include <cstdlib>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <cstring>
#include <chrono>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#endif

using namespace std::chrono;


class action_listener : public virtual mqtt::iaction_listener
{
	std::string name;

	void on_failure(const mqtt::token& tok) override {}

	void on_success(const mqtt::token& tok) override {
		std::cout << "SUBBED TO PONG" << std::endl;
	}

public:
	action_listener(const std::string& name) : name(name) {}
};

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
	public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;

	// The MQTT client
	mqtt::async_client& client;

	// An action listener to display the result of actions.
	action_listener subListener;

	mqtt::connect_options& connOpts_;

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection success" << std::endl;
		std::cout << "\nSubscribing to topic '" << "PONG" << "'\n"
			<< "\tfor client " << "pong"
			<< " using QoS" << 0 << "\n"
			<< "\nPress Q<Enter> to quit\n" << std::endl;

		client.subscribe("pong", 0, nullptr, subListener);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		//std::cout << msg->get_payload().size() << std::endl;
		const void* data = msg->get_payload().data();
		uint64_t ms;
		std::memcpy(&ms, data, sizeof(ms));
		if (count >= 499)
		{
			count = 0;
			rtts.push_back(v);
		}
		int time = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count() - ms;
		//std::cout << "time:" << std::chrono::duration_cast<std::chrono::microseconds>(
		//	std::chrono::system_clock::now().time_since_epoch()).count() - ms << std::endl;
		//std::cout << ms << std::endl;

		v.push_back(time);
		count += 1;
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
	std::vector<std::vector<uint64_t>> rtts;
	std::vector<uint64_t> v;
	int count{ 0 };

	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
		: nretry_(0), client(cli), connOpts_(connOpts), subListener("Subscription") {}
};


int main(const int argc, const char* args[])
{
	// TODO check what version of mqtt we are using
	std::string broker_address{ "tcp://127.0.0.1:1883" };
	int qos{ 1 };

	std::string payload{ "" }; // TODO change to 32Bytes

	mqtt::async_client client(broker_address, "");
	mqtt::connect_options connOpts;
	connOpts.set_clean_session(true);
	callback cb(client, connOpts);
	client.set_callback(cb);

	mqtt::topic tpc_ping(client, "ping", qos);

	std::cout << "init for server... " << broker_address << std::endl;
	std::cout << "ok.. " << broker_address << std::endl;

	try {
		std::cout << "\nConnecting..." << std::endl;
		client.connect()->wait();
		std::cout << "  ...OK" << std::endl;
	}
	catch (const mqtt::exception& e)
	{
		std::cerr << e << std::endl;
		exit(1);
	}

	Sleep(5000);
	int count{ 0 };
	auto starttimer = std::chrono::high_resolution_clock::now();
	auto last_call = std::chrono::high_resolution_clock::now();

	while (true)
	{
		//ops::TimeHelper::sleep(std::chrono::milliseconds(1));
		auto now = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - starttimer).count() >= 10000)
		{
			std::cout << count << std::endl;
			break;
		}
		else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call).count() >= 4)
		{

			count += 1;

			char data[32];
			uint64_t ms = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();

			memcpy(data, (void*)&ms, sizeof(uint64_t));
			//std::cout << ms << std::endl;
			client.publish("ping", data, 32, 0, false);
			last_call = std::chrono::high_resolution_clock::now();
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	//ops::TimeHelper::sleep(std::chrono::milliseconds(1000));
	std::cout << count << std::endl;
	std::vector<uint64_t> delay_avrage{};
	for (std::vector<uint64_t> vec : cb.rtts)
	{
		uint64_t delay{};
		size_t vecotor_size{ vec.size() };
		for (uint64_t time : vec)
		{
			delay += time;

		}
		delay_avrage.push_back(delay / vecotor_size);
	}

	for (uint64_t delay : delay_avrage)
	{
		std::cout << delay/2 << std::endl;
	}


	return 0;
}