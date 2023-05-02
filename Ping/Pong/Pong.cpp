// async_subscribe.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT subscriber using the C++ asynchronous client
// interface, employing callbacks to receive messages and status updates.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker.
//  - Subscribing to a topic
//  - Receiving messages through the callback API
//  - Receiving network disconnect updates and attempting manual reconnects.
//  - Using a "clean session" and manually re-subscribing to topics on
//    reconnect.
//

/*******************************************************************************
 * Copyright (c) 2013-2020 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"

const std::string SERVER_ADDRESS("tcp://127.0.0.1:1883");
const std::string CLIENT_ID("pong");

const std::string TOPIC_SUB("ping");
const std::string TOPIC_PUB("pong");

const int	QOS = 1;
const int	N_RETRY_ATTEMPTS = 5;

class action_listener : public virtual mqtt::iaction_listener
{
	std::string name;

	void on_failure(const mqtt::token& tok) override {}

	void on_success(const mqtt::token& tok) override {
		std::cout << "SUBBED TO PING" << std::endl;
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

	int count{ 0 };

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection success" << std::endl;
		std::cout << "\nSubscribing to topic '" << TOPIC_SUB << "'\n"
			<< "\tfor client " << CLIENT_ID
			<< " using QoS" << QOS << "\n"
			<< "\nPress Q<Enter> to quit\n" << std::endl;

		client.subscribe(TOPIC_SUB, QOS, nullptr, subListener);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		count += 1;
		mqtt::message_ptr new_msg(new mqtt::message(
			"pong",
			msg->get_payload_ref().data(),
			msg->get_payload().size(),
			msg->get_qos(),
			false));
		

		client.publish(new_msg);
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:

	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
		: nretry_(0), client(cli), connOpts_(connOpts), subListener("Subscription") {}
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	// A subscriber often wants the server to remember its messages when its
	// disconnected. In that case, it needs a unique ClientID and a
	// non-clean session.

	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);

	mqtt::connect_options connOpts;
	connOpts.set_clean_session(true);

	// Install the callback(s) before connecting.
	callback cb(cli, connOpts);
	cli.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		cli.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << exc << std::endl;
		return 1;
	}

	// Just block till user tells us to quit.

	while (std::tolower(std::cin.get()) != 'q')
		;

	// Disconnect

	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		cli.disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc << std::endl;
		return 1;
	}

	return 0;
}
