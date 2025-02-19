#include <iostream>
#include <grpc++/grpc++.h>
#include "calculator.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using std::cout;
using std::endl;
using std::invalid_argument;
using std::shared_ptr;
using std::unique_ptr;

class CalculatorClient
{
public:
	CalculatorClient(shared_ptr<Channel> channel) : _stub(Calculator::NewStub(channel)) {}
	float Calculate(float first, float second, char operation)
	{
		OperandsRequest request;
		Response response;
		request.set_first(first);
		request.set_second(second);

		ClientContext context;
		Status status;

		switch (operation)
		{
		case '+':
			status = this->_stub->Addition(&context, request, &response);
			break;
		case '-':
			status = this->_stub->Subtraction(&context, request, &response);
			break;
		case '*':
			status = this->_stub->Multiplication(&context, request, &response);
			break;
		case '/':
			status = this->_stub->Division(&context, request, &response);
			break;
		default:
			throw invalid_argument("Unknown operation");
		}

		if (status.ok())
		{
			return response.result();
		}
		else
		{
			throw invalid_argument(status.error_message());
		}
	}

private:
	unique_ptr<Calculator::Stub> _stub;
};

int main()
{
	CalculatorClient client(grpc::CreateChannel("localhost:4000", grpc::InsecureChannelCredentials()));
	float result;
	try
	{
		result = client.Calculate(5, 0, '/');
	}
	catch (const std::invalid_argument exception)
	{
		cout << "ERROR: " << exception.what() << endl;
	}
	cout << result << endl;
	return 0;
}