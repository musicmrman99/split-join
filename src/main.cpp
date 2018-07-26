#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

// split str at chr and put result into parts
// part_cap_log2 is the base-2 log of the size of the parts vector.
// It is incremented and pow(2, part_cap_log2) elements are reserved for
// the parts vector if it needs more capacity.

// default type converter (returns a reference to its std::string argument)
const std::string& default_converter(const std::string& str) {
	return str;
}


// Split 'str' at 'chr' and append the result to 'parts' (in-place
// modification).

// If 'parts' is not a vector of strings, and is instead a vector of
// another type (given in the first template parameter), then
// 'converter' must be given to convert each part (std::string) to the
// appropriate type.

// Return 0 on success, 1 on failure.
template <class type = std::string>
int split(const std::string& str, //{
           const char& chr,
           std::vector<type>& parts,
           unsigned int& part_cap_log2,
           const type& (*const converter)(const std::string&) = &default_converter
           ) {
	std::string cur_part;
	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] == chr) {
			// increase capacity exponentially if needed
			if (parts.size()+1 > parts.capacity()) {
				parts.reserve(std::pow(2, part_cap_log2++));
			}

			// dereference:   const function* converter -> function converter
			// call function: const <type>* converter(cur_part)
			// call function: parts.push_back(<type>)
			parts.push_back((*converter)(cur_part));
			cur_part = "";
		}
		else {
			cur_part += str[i];
		}
	}

	// then add whatever is left
	// increase capacity exponentially if needed
	if (parts.size()+1 > parts.capacity()) {
		parts.reserve(std::pow(2, part_cap_log2++));
	}

	parts.push_back((*converter)(cur_part));
	return 0;
} //}

// Join 'parts', seperated by 'chr' and put into str (in-place
// modification).

// Return 0 on success, 1 on failure.
template <class type = std::string>
int join(const std::vector<type>& parts, //{
         const char& chr,
         std::string& str,
         std::vector<int> range,
		 const std::string& (*converter)(const type&) = &default_converter
         ) {
	// Lower bound
	// --------------------
	// invalid - underflow
	if (range.at(0) < (int)-parts.size()) {return 1;}
	// valid - negative offset from end
	else if (range.at(0) < 0) {
		range.at(0) = parts.size() + range.at(0);
	}
	// valid - nothing to be done
	///else if (range.at(0) < (int)parts.size()) {}
	// invalid - overflow
	else if (range.at(0) > (int)parts.size()) {return 1;}

	// Upper bound
	// --------------------
	// invalid - underflow
	if (range.at(1) < (int)-parts.size()) {return 1;}
	// valid - negative offset from end
	else if (range.at(1) < 0) {
		range.at(1) = parts.size() + range.at(1);
	}
	// valid - nothing to be done
	///else if (range.at(1) < (int)parts.size()) {}
	// invalid - overflow
	else if (range.at(1) > (int)parts.size()) {return 1;}

	// Shortcut
	// --------------------
	if (range.at(0) >= range.at(1)) {return 0;}

	// Main
	// --------------------

	str = "";
	for (unsigned int i = range.at(0); i < (unsigned int)range.at(1); i++) {
		// dereference:   const function* converter -> function converter
		// call function: const std::string* converter(cur_part)
		// dereference:   const std::string* -> std::string
		// concatenate:   str += std::string
		str += (*converter)(parts.at(i));

		// when joining, don't put a chr at the end of the resulting string.
		if (i+1 != (unsigned int)range.at(1)) {
			str += chr;
		}
	}

	return 0;
} //}

// Return the result of the python3-like split-slice-join set of operations.
std::string split_join(const std::string& str, //{
                       const char& chr,
                       const std::string& range_str
                       ) {
	// 16 (2^4) parts is a reasonable starting point for file extensions
	// and file paths.
	unsigned int parts_cap = 16;
	std::vector<std::string> parts;
	parts.reserve(parts_cap);

	split(str, chr, parts, parts_cap);

	// get start and end points (half-open range, negative indexes accepted)
	unsigned int range_cap = 2;
	std::vector<std::string> range_str_split;
	std::vector<int> range;
	range_str_split.reserve(range_cap);
	range.reserve(range_cap);

	split(range_str, ':', range_str_split, range_cap);

	if (range_str_split.at(0) == "")
		range.push_back(0);
	else
		range.push_back(std::stoi(range_str_split.at(0)));

	if (range_str_split.at(1) == "")
		range.push_back(parts.size());
	else
		range.push_back(std::stoi(range_str_split.at(1)));

	// re-join and output
	std::string output = "";
	join(parts, chr, output, range);

	return output;
} //}

// Syntax:
//   split-join (split_char) (range) [input_string]

// If no input_string is given, read from stdin.

// Return 0 on success, 1 on failure.
int main(int argc, char** argv) {
	const std::vector<std::string> args(argv, argv+argc);

	std::vector<std::string> input;
	if (args.size()-1 == 3) {
		// These variables are scoped to the if - we don't need them later.
		std::istringstream input_stream(args[3]);
		std::string line;

		while (std::getline(input_stream, line)) {
			input.push_back(line);
		}
	}
	else if (args.size()-1 == 2) {
		// All of these variables are scoped to the if - we don't need them later.
		std::string line;

		while (std::getline(std::cin, line)) {
			input.push_back(line);
		}
	}
	else {
		std::cout << "Error: 2 args required, 1 optional: recieved ";
		std::cout << std::to_string(args.size()-1);
		std::cout << std::endl;
		return 1;
	}

	// first character of first argument (not including program name)
	const char& chr = args[1][0];

	const std::string& range_str = args[2];
	if (range_str.find(':') == std::string::npos) {
		std::cout << "Error: invalid value for range_str: delimiter (':') not found" << std::endl;
		return 1;
	}
	else if (range_str.find_first_of(":") != range_str.find_last_of(":")) {
		std::cout << "Error: invalid value for range_str: more than one occurence of delimiter (':')" << std::endl;
	}

	std::string output;
	for (unsigned int i = 0; i < input.size(); i++) {
		output = split_join(input[i], chr, range_str);
		std::cout << output << std::endl;
	}

	return 0;
}

