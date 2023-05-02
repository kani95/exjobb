
//Auto generated code example for subscribing to HelloTopic on domain HelloDomain generated from project OpsPingPong
#include <ops.h>
#include "hello/HelloDataSubscriber.h"
#include "OpsPingPong/OpsPingPongTypeFactory.h"
#include <iostream>
#include <vector>

//Create a class to act as a listener for OPS data and deadlines
class Main : ops::DataListener, ops::DeadlineMissedListener
{
public:
	//Use a member subscriber so we can use it from onNewData, see below.
	hello::HelloDataSubscriber* sub;

public:

	Main()
	{
		//Create a Participant (i.e. an entry point for using ops.), compare with your ops_config.xml
		ops::Participant* participant = ops::Participant::getInstance("HelloDomain");
		if(!participant)
		{
			std::cout << "Create participant failed. do you have ops_config.xml in your rundirectory?" << std::endl;
			ops::TimeHelper::sleep(std::chrono::seconds(10)); exit(1);
		}

		//Add type support for our types, to make this participant understand what we are talking
		participant->addTypeSupport(new OpsPingPong::OpsPingPongTypeFactory());

		//Now, create the Topic we wish to subscribe on. Might throw ops::NoSuchTopicException if no such Topic exist in ops_config.xml
		ops::Topic topic = participant->createTopic("HelloTopic");

		//Create a subscriber on that topic.
		sub = new hello::HelloDataSubscriber(topic);

		//Tell the subscriber that we expect data at least once every 1500 ms
		sub->setDeadlineQoS(1500);

		//Add this class as a listener for new data events
		sub->addDataListener(this);

		//Add this class as a listener for deadline missed events
		sub->deadlineMissedEvent.addDeadlineMissedListener(this);

		//Start the subscription
		sub->start();

	}
	///Override from ops::DataListener, called whenever new data arrives.
	void onNewData(ops::DataNotifier* subscriber)
	{
		hello::HelloData data;
		if(sub == subscriber)
		{
			sub->getData(&data);
			std::cout << "Data received. " << std::endl;
		}
	}
	///Override from ops::DeadlineMissedListener, called if no new data has arrived within deadlineQoS.
	void onDeadlineMissed(ops::DeadlineMissedEvent* evt)
	{
		std::cout << "Deadline Missed!" << std::endl;
	}
	~Main()
	{
		delete sub;
	}

};

//Application entry point
int main(int argc, char* args)
{
	ops::Participant* participant = ops::Participant::getInstance("TestAllDomain");


	//Create an object that will listen to OPS events
	Main m;

	//Just keep program alive, action will take place in Main::onNewData()
	while(true)
	{
		ops::TimeHelper::sleep(std::chrono::seconds(10));
	}

	return 0;
}
