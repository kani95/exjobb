#include <ops.h>
#include "PingPong/PingDataSubscriber.h"
#include "PingPong/PongDataPublisher.h"
#include "OpsPingPong/OpsPingPongTypeFactory.h"
#include <iostream>
#include <vector>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#endif

#include "ConfigFileHelper.h"

//Create a class to act as a listener for OPS data
class Main : ops::DataListener
{
public:
	//Use a member subscriber so we can use it from onNewData, see below.
	std::unique_ptr<PingPong::PingDataSubscriber> sub;
	std::unique_ptr<PingPong::PongDataPublisher> pub;
	PingPong::PongData Pongdata;
	PingPong::PingData data;

public:

	Main()
	{
		//Create a Participant (i.e. an entry point for using ops.), compare with your ops_config.xml
		ops::Participant* const participant = ops::Participant::getInstance("PingPongDomain");
		if (!participant)
		{
			std::cout << "Create participant failed. do you have ops_config.xml in your rundirectory?" << std::endl;
#ifdef _WIN32
			ops::TimeHelper::sleep(std::chrono::seconds(10));
#endif
			exit(1);
		}

		//Add type support for our types, to make this participant understand what we are talking
		participant->addTypeSupport(new OpsPingPong::OpsPingPongTypeFactory());

		//Now, create the Topic we wish to subscribe on. Might throw ops::NoSuchTopicException if no such Topic exist in ops_config.xml
		ops::Topic topic = participant->createTopic("Ping");
		ops::Topic topic2 = participant->createTopic("Pong");

		//Create a subscriber on that topic.
		sub.reset(new PingPong::PingDataSubscriber(topic));
		pub.reset(new PingPong::PongDataPublisher(topic2));
		pub -> setName("PongPublisher");

		//Add this class as a listener for new data events
		sub->addDataListener(this);

		//Start the subscription
		sub->start();

	}
	///Override from ops::DataListener, called whenever new data arrives.
#if defined(_MSC_VER) && (_MSC_VER == 1900)
#pragma warning( disable : 4373)
#endif
	virtual void onNewData(ops::DataNotifier* const subscriber) override
	{
		
		if (sub.get() == subscriber)
		{
			sub->getData(&data);
			Pongdata.time = data.time;
			pub->write(Pongdata);
		}
	}
	~Main()
	{
	}
	Main(Main const&) = delete;
	Main(Main&&) = delete;
	Main& operator =(Main&&) = delete;
	Main& operator =(Main const&) = delete;

};

//Application entry point
int main(const int argc, const char* args[])
{
	//Create an object that will listen to OPS events
	UNUSED(argc);
	UNUSED(args);

	setup_alt_config("Examples/CppApps/PingPong/OpsPingPong/ops_config.xml");

	Main const m;

	//Just keep program alive, action will take place in Main::onNewData()
	while (true)
	{
		ops::TimeHelper::sleep(std::chrono::seconds(10));
	}

	return 0;
}
