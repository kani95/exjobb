// TestAll_Publish.cpp : Defines the entry point for the console application.
//

//Include ops
#include <ops.h>
//Include a publisher for the data type we want to publish, generated from our IDL project OpsPingPong.
#include "hello/HelloDataPublisher.h"
//Include type support for the data types we have defined in our IDL project, generated from our IDL project OpsPingPong.
#include "OpsPingPong/OpsPingPongTypeFactory.h"
//Include iostream to get std::cout
#include <iostream>
#include <windows.h>


int main(int argc, const char* args[])
{
	using namespace ops;

	//Create a Participant (i.e. an entry point for using ops.), compare with your ops_config.xml
	ops::Participant* participant = Participant::getInstance("HelloDomain");
	if(!participant)
	{
		std::cout << "Create participant failed. do you have ops_config.xml on your rundirectory?" << std::endl;
		ops::TimeHelper::sleep(std::chrono::seconds(10)); exit(1);
	}

	//Add type support for our types, to make this participant understand what we are talking
	participant->addTypeSupport(new OpsPingPong::OpsPingPongTypeFactory());

	//Now, create the Topic we wish to publish on. Might throw ops::NoSuchTopicException if no such Topic exist in ops_config.xml
	Topic topic = participant->createTopic("HelloTopic");

	//Create a publisher on that topic
	hello::HelloDataPublisher pub(topic);
	pub.setName("HelloTopicPublisher"); //Optional identity of the publisher

	//Create some data to publish, this is our root object.
	hello::HelloData data;


	//Publish the data periodically
	int mainSleep = 1000;
	while(true)
	{
		pub.write(&data);
		std::cout << "Writing data"  <<  std::endl;
		ops::TimeHelper::sleep(std::chrono::milliseconds(mainSleep));
	}

	return 0;
}
