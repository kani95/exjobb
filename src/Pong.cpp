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
 * @file HelloWorldSubscriber.cpp
 *
 */

#include "PongPubSubTypes.h"
#include "PingPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>




using namespace eprosima::fastdds::dds;

Pong pong_;

DataWriter* writer_;

class PongSubscriber
{
private:

    Pong hello_;

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    Publisher* publisher_;

    DataReader* reader_;


    Topic* topic_;

    Topic* topic_pong;

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

    } listener_1;

    class SubListener : public DataReaderListener, public DataWriterListener
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
                if (info.valid_data)
                {
                    //samples_++;
                    //std::cout << "Message: " << hello_.message() << " with index: " << hello_.index()
                   //             << " RECEIVED." << std::endl;
                   //pong_.index(hello_.index());
                   pong_.time(hello_.time());
                   writer_->write(&pong_);

                }
            }
        }

        Ping hello_;

        std::atomic_int samples_;

    } listener_;

public:

    PongSubscriber()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , topic_pong(nullptr)
        , reader_(nullptr)
        , type_(new PingPubSubType())
    {
    }

    virtual ~PongSubscriber()
    {
        if (reader_ != nullptr)
        {
            subscriber_->delete_datareader(reader_);
        }
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
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
        if (publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the subscriber
    bool init()
    {
        TypeSupport custom_type_support(new PingPubSubType());
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");

        participantQos.transport().use_builtin_transports = false;

        // Create a descriptor for the new transport.
        // Do not configure any listener port
        auto tcp_transport = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
        participantQos.transport().user_transports.push_back(tcp_transport);

        // Set initial peers.
        eprosima::fastrtps::rtps::Locator_t initial_peer_locator;
        initial_peer_locator.kind = LOCATOR_KIND_TCPv4;
        eprosima::fastrtps::rtps::IPLocator::setIPv4(initial_peer_locator, "127.0.0.1");
        initial_peer_locator.port = 5100;

        participantQos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator);

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

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("PingTopic", "Ping", TOPIC_QOS_DEFAULT);

       topic_pong = participant_->create_topic("PongTopic", "Pong", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (subscriber_ == nullptr)
        {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        if (reader_ == nullptr)
        {
            return false;
        }

        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr)
        {
           return false;
        }

        writer_ = publisher_->create_datawriter(topic_pong, DATAWRITER_QOS_DEFAULT, &listener_1);
        if (writer_ == nullptr)
        {
            return false;
        }

        return true;
    }

    //!Run the Subscriber
    void run(
        uint32_t samples)
    {
        while(listener_.samples_ < samples)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    PongSubscriber* mysub = new PongSubscriber();

    if(mysub->init())
    {
        mysub->run(static_cast<uint32_t>(samples));
    }

    delete mysub;
    return 0;
}
