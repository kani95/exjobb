// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file HelloWorldPublisher.cpp
 *
 */

#include "PingPubSubTypes.h"
#include "PongPubSubTypes.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>
#include <windows.h>

using namespace eprosima::fastdds::dds;

int count{0};
std::vector<std::vector<uint64_t>> rtts;
std::vector<uint64_t> v;


class PingPublisher
{
private:

    Ping ping_;

    DomainParticipant* participant_;

    Publisher* publisher_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_pong;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;

    class PubListener : public DataWriterListener
    {
    public:

        PubListener()
            : matched_(0)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(
                DataWriter*,
                const PublicationMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                matched_ = info.total_count;
                std::cout << "Publisher matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                matched_ = info.total_count;
                std::cout << "Publisher unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
            }
        }

        std::atomic_int matched_;

    } listener_;

    class SubListener : public DataReaderListener
    {
    public:

        SubListener()
            : samples_(0)
        {
        }

        ~SubListener() override
        {
        }

        void on_subscription_matched(
            DataReader*,
            const SubscriptionMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                    << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        void on_data_available(
            DataReader* reader) override
        {
            SampleInfo info;
            if (reader->take_next_sample(&hello_, &info) == ReturnCode_t::RETCODE_OK)
            {
              
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
                        std::chrono::system_clock::now().time_since_epoch()).count() - hello_.time();
                    v.push_back(time);
                    count += 1;

                

            }
        }

        Pong hello_;

        std::atomic_int samples_;

    } listener_1;

public:

    PingPublisher()
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new PingPubSubType())
    {
    }

    virtual ~PingPublisher()
    {
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
        }
        if (reader_ != nullptr)
        {
            subscriber_->delete_datareader(reader_);
        }
        if (publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        if (topic_pong)
        {
            participant_->delete_topic(topic_pong);
        }
        if (subscriber_ != nullptr)
        {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the publisher
    bool init()
    {
        TypeSupport custom_type_support(new PingPubSubType());


        DomainParticipantQos participantQos;
        participantQos.name("Participant_publisher");

        auto tcp_transport = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
        tcp_transport->sendBufferSize = 0;
        tcp_transport->receiveBufferSize = 0;
        tcp_transport->add_listener_port(5100);
        tcp_transport->set_WAN_address("127.0.0.1");

        // Link the Transport Layer to the Participant.
        participantQos.transport().user_transports.push_back(tcp_transport);

        // Avoid using the default transport
        participantQos.transport().use_builtin_transports = false;
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
        custom_type_support.register_type(participant_, custom_type_support.get_type_name());
        custom_type_support.register_type(participant_, "Ping");
        custom_type_support.register_type(participant_, "Pong");
        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the publications Topic
        topic_ = participant_->create_topic("PingTopic", "Ping", TOPIC_QOS_DEFAULT);
        topic_pong = participant_->create_topic("PongTopic", "Pong", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr || topic_pong == nullptr)
        {
            return false;
        }

        // Create the Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

        if (publisher_ == nullptr)
        {
            return false;
        }

        // Create the DataWriter
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);

        if (writer_ == nullptr)
        {
            return false;
        }

        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (subscriber_ == nullptr)
        {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_pong, DATAREADER_QOS_DEFAULT, &listener_1);

        if (reader_ == nullptr)
        {
            return false;
        }
        return true;
    }

    //!Send a publication
    bool publish()
    {
        if (listener_.matched_ > 0)
        {
            //hello_.index(hello_.index() + 1);
            //writer_->write(&hello_);
            return true;
        }
        return false;
    }

    //!Run the Publisher
    void run(
            uint32_t samples)
    {
        if (listener_.matched_ > 0)
        {
            int count{ 0 };
            //uint64_t starttimer = system(".\\a.exe");
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
                    uint64_t ms = std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                    ping_.time() = ms;
                    writer_ -> write(&ping_);
                    last_call = std::chrono::high_resolution_clock::now();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }



    }
};

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting publisher." << std::endl;
    int samples = 10;

    PingPublisher* mypub = new PingPublisher();
    if(mypub->init())
    {
        Sleep(5000);
        mypub->run(static_cast<uint32_t>(samples));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::vector<uint64_t> delay_avrage{};
    for (std::vector<uint64_t> vec : rtts)
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


    delete mypub;
    return 0;
}
