#include <iostream>
#include <memory>

#include <grpc++/grpc++.h>
#include "calculator.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

using std::cout;
using std::endl;
using std::unique_ptr;

class CalculatorImpl : public Calculator::Service
{
	Status Addition(ServerContext *context, const OperandsRequest *request, Response *response)
	{
		response->set_result(request->first() + request->second());
		return Status::OK;
	}
	Status Subtraction(ServerContext *context, const OperandsRequest *request, Response *response)
	{
		response->set_result(request->first() - request->second());
		return Status::OK;
	}
	Status Multiplication(ServerContext *context, const OperandsRequest *request, Response *response)
	{
		response->set_result(request->first() * request->second());
		return Status::OK;
	}
	Status Division(ServerContext *context, const OperandsRequest *request, Response *response)
	{
		if (request->second() == 0)
			return Status(StatusCode::INVALID_ARGUMENT, "Division by zero");
		response->set_result(request->first() / request->second());
		return Status::OK;
	}
};

void SetupServer()
{
	CalculatorImpl service;
	ServerBuilder builder;
	builder.AddListeningPort("0.0.0.0:4000", grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	unique_ptr<Server> server(builder.BuildAndStart());
	server->Wait();
}

int main()
{
	SetupServer();
	return 0;
}