#include <grpc++/grpc++.h>
#include "calculator.grpc.pb.h"
#include "constants.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using std::cin;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

using constants::InvalidFormat;
using constants::Operation;
using constants::OperationsCount;
using constants::UnknownOperation;

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
		case Operation::Addition:
			status = this->_stub->Addition(&context, request, &response);
			break;
		case Operation::Subtraction:
			status = this->_stub->Subtraction(&context, request, &response);
			break;
		case Operation::Multiplication:
			status = this->_stub->Multiplication(&context, request, &response);
			break;
		case Operation::Division:
			status = this->_stub->Division(&context, request, &response);
			break;
		default:
			throw UnknownOperation;
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

char operatorType(string expression)
{
	const char operators[OperationsCount] = {
		Operation::Addition,
		Operation::Division,
		Operation::Multiplication,
		Operation::Division};
	for (int i = 0; i < OperationsCount; i++)
	{
		if (expression.find(operators[i]) != -1)
		{
			return operators[i];
		}
	}
	throw InvalidFormat;
}
struct operands
{
	float first;
	float second;
};
operands findOperands(string expression, char operatorType)
{
	int operatorPosition = expression.find(operatorType);
	string firstPart = expression.substr(0, operatorPosition);
	string secondPart = expression.substr(operatorPosition + 1);

	try
	{
		float first = std::stof(firstPart);
		float second = std::stof(secondPart);
		return {first, second};
	}
	catch (const std::exception exception)
	{
		throw InvalidFormat;
	}
}

void makeIteration(CalculatorClient &client)
{
	string expression;
	cout << "Enter expression: ";
	std::getline(cin, expression);
	expression.erase(remove_if(expression.begin(), expression.end(), isspace), expression.end());

	char operation = operatorType(expression);
	operands args = findOperands(expression, operation);

	float result = client.Calculate(args.first, args.second, operation);
	cout << "Result: " << result << endl;
}

int main()
{
	CalculatorClient client(grpc::CreateChannel("localhost:4000", grpc::InsecureChannelCredentials()));
	while (true)
	{
		try
		{
			makeIteration(client);
		}
		catch (const invalid_argument exception)
		{
			cout << "ERROR: " << exception.what() << endl;
		}
	}

	return 0;
}