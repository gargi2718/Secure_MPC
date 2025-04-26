//
// Created by 32002425 on 26-04-2025.
//

#ifndef NETWORKSESSION_H
#define NETWORKSESSION_H
#pragma once
#include <boost/asio.hpp>
#include <functional>
#include <vector>



class NetworkSession {
public:
    using ReceiveHandler = std::function<void(const std::vector<uint8_t>&)>;

    NetworkSession(boost::asio::io_context& ctx);
    void connect(const boost::asio::ip::tcp::endpoint& ep);
    void accept(uint16_t port);
    void start(ReceiveHandler onReceive);
    void send(const std::vector<uint8_t>& data);

private:
    void doRead();
    void doWrite();

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf    readBuf_;
    std::vector<uint8_t>      writeBuf_;
    ReceiveHandler            receiveHandler_;
};





#endif //NETWORKSESSION_H
