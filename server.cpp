#include <iostream>
#include <grpc++/grpc++.h>
#include "ping.grpc.pb.h"

using grpc::Server;
using grpc::Status;
using grpc::ServerContext;
using grpc::ServerBuilder;
using grpc::InsecureServerCredentials;

using std::cout;
using std::endl;
using std::unique_ptr;

class PingImpl : public Ping::Service {
	Status ping(ServerContext* context, const Msg* request, Msg* response) {
		cout << request->payload() << endl;
		response->set_payload(request->payload());
		return Status::OK;
	}
};

int main() {
	PingImpl service;
	ServerBuilder builder;
	builder.AddListeningPort("0.0.0.0:4000", InsecureServerCredentials());
	builder.RegisterService(&service);

	unique_ptr<Server> server(builder.BuildAndStart());
	server->Wait();

	return 0;
}