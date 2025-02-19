#include <iostream>
#include <grpc++/grpc++.h>
#include "ping.grpc.pb.h"

using grpc::ClientContext;
using grpc::Channel;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;
using grpc::Status;

using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;

int main() {
	Msg request, response;
	request.set_payload("Hello, server!");

	ClientContext context;
	std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:4000", InsecureChannelCredentials());
	unique_ptr<Ping::Stub> stub = Ping::NewStub(channel);

	Status status = stub->ping(&context, request, &response);

	if (status.ok()) {
		cout << response.payload() << endl;
	} 
	else {
		cout << status.error_code() << ": " << status.error_message() << endl;
	}

	return 0;
}