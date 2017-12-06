#include <iostream>
#include <string>
#include <regex>

using namespace std;

string calculate_add_and_subtr(string);
string calculate_mult_and_div(string);
string find_and_calculate_parentheses(string);
bool fast_sanity_check(string);

int main()
{
	smatch result;
	string input;

	while (cout << "Enter arithmetic expression: (enter q to exit)" << endl && std::getline(cin, input) && input != "q")
	{
		input = std::regex_replace(input, std::regex("[[:space:]]*"), ""); //delete spaces in input string
		string initial_input = input;

		cout << "-----------------------------------------------------------------------------" << endl;

		if (!fast_sanity_check(input))
		{
			cout << "Please enter correct expression" << endl;
			continue;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		input = find_and_calculate_parentheses(input);

		if (input == "error")
		{
			cout << "Division by zero error" << endl;
			continue;
		}

		input = calculate_mult_and_div(input);

		if (input == "error")
		{
			cout << "Division by zero error" << endl;
			continue;
		}

		input = calculate_add_and_subtr(input);

		cout << "Input expression: " << initial_input << endl << endl;
		cout << "Result: " << input << endl;
	}
	cout << "Finished" << endl;
	return 0;
}

string find_and_calculate_parentheses(string input_string)
{
	regex extract_parenthesis
	(
		"(\\()([^\\(]+?)(\\))"
	);

	smatch res;

	string full_match;
	string matched_group;

	while (regex_search(input_string, res, extract_parenthesis))
	{
		full_match = res[0];
		matched_group = res[2];

		matched_group = calculate_mult_and_div(matched_group);
		if (matched_group == "error")
		{
			return "error";
		}
		matched_group = calculate_add_and_subtr(matched_group);

		size_t f = input_string.find(full_match);
		input_string.replace(f, string(full_match).length(), matched_group);
	}
	return input_string;
}

string calculate_mult_and_div(string input_string)
{
	regex extract_mult_and_div
	(
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
		"(\\*|/)"
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
	);

	smatch res;

	while (regex_search(input_string, res, extract_mult_and_div))
	{
		string group_before_calculation_mult_and_div = res[0];

		double operation_result;

		double operand1 = std::atof(res[1].str().c_str());      //OPERAND1
		double operand2 = std::atof(res[4].str().c_str());      //OPERAND2
		char operation_sign = res[3].str()[0];                  //SIGN

		if (operand2 == 0 && operation_sign == '/')
		{
			return "error";
		}

		switch (operation_sign)
		{
		case '*':
			operation_result = operand1 * operand2;
			break;
		case '/':
			operation_result = operand1 / operand2;
			break;
		default:
			cout << "Something went wrong in switch instruction";
			break;
		}

		size_t f = input_string.find(group_before_calculation_mult_and_div);
		input_string.replace(f, string(group_before_calculation_mult_and_div).length(), std::to_string(operation_result));
	}
	return input_string;
}

string calculate_add_and_subtr(string input_string)
{
	smatch res;

	std::regex extract_add_and_subtr
	(
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
		"(\\+|-)"
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
	);

	while (regex_search(input_string, res, extract_add_and_subtr))
	{
		string group_before_calculation_add_and_subtr = res[0];

		double operation_result;

		double operand1 = std::atof(res[1].str().c_str());      //OPERAND1
		double operand2 = std::atof(res[4].str().c_str());      //OPERAND2
		char operation_sign = res[3].str()[0];                  //SIGN								

		switch (operation_sign)
		{
		case '+':
			operation_result = operand1 + operand2;
			break;
		case '-':
			operation_result = operand1 - operand2;
			break;
		default:
			cout << "Something went wrong in switch instruction";
			break;
		}

		size_t f = input_string.find(group_before_calculation_add_and_subtr);
		input_string.replace(f, string(group_before_calculation_add_and_subtr).length(), std::to_string(operation_result));
	}
	return input_string;
}

bool fast_sanity_check(string input_string)
{
	regex extract_mult_and_div
	(
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
		"(\\*|/)"
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
	);

	regex extract_add_and_subtr
	(
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
		"(\\+|-)"
		"(\\+|-?[0-9]+(\\.[0-9]+)?)"
	);
	smatch res;

	if (regex_search(input_string, res, extract_mult_and_div) || regex_search(input_string, res, extract_add_and_subtr) && !regex_search(input_string, res, regex(".*(\\+{2})|(-{2}).*")))
	{
		return true;
	}
	else
		return false;
}