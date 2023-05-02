//Include ops
#include <ops.h>
#include <array>
#include <vector>
#include <TimeHelper.h>
//Include a publisher for the data type we want to publish, generated from our IDL project HelloWorld.
//#include "hello/HelloDataPublisher.h"
#include <chrono>
#include "PingPong/PingDataPublisher.h"
#include "PingPong/PongDataSubscriber.h"
//Include type support for the data types we have defined in our IDL project, generated from our IDL project HelloWorld.
#include "OpsPingPong/OpsPingPongTypeFactory.h"
//Include iostream to get std::cout
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#endif

#include "ConfigFileHelper.h"

using namespace std::chrono;

class Main : ops::DataListener
{
public:
	//Use a member subscriber so we can use it from onNewData, see below.
	std::unique_ptr<PingPong::PongDataSubscriber> sub;
	std::vector<std::vector<uint64_t>> rtts;
	std::vector<uint64_t> v;
	int count{ 0 };

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
		ops::Topic topic2 = participant->createTopic("Pong");

		//Create a subscriber on that topic.
		sub.reset(new PingPong::PongDataSubscriber(topic2));

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
		PingPong::PongData data;
		if (sub.get() == subscriber)
		{
			sub->getData(&data);
			
			if (count == 0)
			{
				//v.clear();
			}
			else if (count >= 499)
			{
				count = 0;
				rtts.push_back(v);
			}
			int time = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count() - data.time;
			v.push_back(time);
			count += 1;

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

int main(const int argc, const char* args[])
{
	UNUSED(argc);
	UNUSED(args);
	using namespace ops;

	setup_alt_config("Examples/CppApps/PingPong/OpsPingPong/ops_config.xml");

	//Create a Participant (i.e. an entry point for using ops.), compare with your ops_config.xml
	ops::Participant* const participant = Participant::getInstance("PingPongDomain");
	if (!participant)
	{
		std::cout << "Create participant failed. do you have ops_config.xml on your rundirectory?" << std::endl;
		ops::TimeHelper::sleep(std::chrono::seconds(10));
		exit(1);
	}

	//Add type support for our types, to make this participant understand what we are talking
	participant->addTypeSupport(new OpsPingPong::OpsPingPongTypeFactory());

	//Now, create the Topic we wish to publish on. Might throw ops::NoSuchTopicException if no such Topic exist in ops_config.xml
	Topic const topic = participant->createTopic("Ping");

	//Create a publisher on that topic
	PingPong::PingDataPublisher pub(topic);
	pub.setName("PingPublisher"); //Optional identity of the publisher

	//Create some data to publish, this is our root object.
	PingPong::PingData data;
	Main const m;
	Sleep(5000);
	//Publish the data peridically 
	int count{ 0 };
	//uint64_t starttimer = system(".\\a.exe");
	auto starttimer = std::chrono::high_resolution_clock::now();
	auto last_call = std::chrono::high_resolution_clock::now();

	while (true)
	{
		//ops::TimeHelper::sleep(std::chrono::milliseconds(1));
		uint64_t ms = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
		auto now = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - starttimer).count() >= 10000)
		{
			std::cout << count << std::endl;
			break;
		}
		else if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call).count() >= 4)
		{

			count += 1;
			data.time = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();
			pub.write(&data);
			last_call = std::chrono::high_resolution_clock::now();
		}
	}

	ops::TimeHelper::sleep(std::chrono::milliseconds(5000));
	
	std::vector<uint64_t> delay_avrage{};
	for (std::vector<uint64_t> vec : m.rtts)
	{
		uint64_t delay{};
		size_t vecotor_size{ vec.size()};
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