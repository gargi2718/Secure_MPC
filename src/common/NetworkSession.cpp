//
// Created by 32002425 on 26-04-2025.
//

#include "NetworkSession.h"
// NetworkSession.cpp
#include "NetworkSession.h"

NetworkSession::NetworkSession(boost::asio::io_context& ctx)
  : socket_(ctx) {}

void NetworkSession::connect(const boost::asio::ip::tcp::endpoint& ep) {
    socket_.connect(ep);
    doRead();
}

void NetworkSession::accept(uint16_t port) {
    boost::asio::ip::tcp::acceptor acceptor(socket_.get_executor().context(),
                                             {boost::asio::ip::tcp::v4(), port});
    acceptor.accept(socket_);
    doRead();
}

void NetworkSession::start(ReceiveHandler onReceive) {
    receiveHandler_ = std::move(onReceive);
    doRead();
}

void NetworkSession::send(const std::vector<uint8_t>& data) {
    bool writing = !writeBuf_.empty();
    writeBuf_.insert(writeBuf_.end(), data.begin(), data.end());
    if (!writing) doWrite();
}

void NetworkSession::doRead() {
    boost::asio::async_read_until(socket_, readBuf_, '\0',
      [this](auto ec, std::size_t n){
        if (!ec) {
          std::istream is(&readBuf_);
          std::vector<uint8_t> data(n);
          is.read(reinterpret_cast<char*>(data.data()), n);
          if (receiveHandler_) receiveHandler_(data);
          doRead();
        }
    });
}

void NetworkSession::doWrite() {
    boost::asio::async_write(socket_,
      boost::asio::buffer(writeBuf_),
      [this](auto, auto){
        writeBuf_.clear();
      });
}
