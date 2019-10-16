#include <iostream>
#include <string>
#include "jsonr\Json.hpp"

int main() {
	auto obj = Json::CreateJson<Json::Object>();

	if (obj) {
		auto& object = *obj;
		object["Name"] = Json::CreateJson<Json::String>("EliorCohen");
		object["Number"] = Json::CreateJson<Json::Number>(12345);
		object["Null"] = Json::CreateJson<Json::Null>();
		object["Obj"] = Json::CreateJson<Json::Object>();
		auto array = object["Array"] = Json::CreateJson<Json::Array>();

		// fill array
		auto arrayPtr = Json::ConvertJson<Json::Array>(array);
		for (auto i = 0; i < 10; ++i) {
			arrayPtr->push_back(Json::CreateJson<Json::Number>(i));
		}

		// write to file
		Json::FileWriter writer;
		writer.write("myJSON.json", obj);
	}

	try {
		// read
		Json::FileReader reader;
		auto obj = Json::ConvertJson<Json::Object>(reader.parseFile("myJSON.json"));
		// or parse from std string
		// auto obj = Json::ConvertJson<Json::Object>(reader.parseFile("{}"));

		auto str = obj->get<Json::String>("Name")->toSTDString();
		auto number = obj->get<Json::Number>("Number")->toLongLong();

		std::cout << "Name: " << str << std::endl;
		std::cout << "Number: " << number << std::endl;

		auto arrayPtr = obj->get<Json::Array>("Array");

		if (arrayPtr) {
			auto const& array = *arrayPtr;
			for (auto i = 0U; i < array.count(); ++i)
			{
				auto item = Json::ConvertJson<Json::Number>(array[i])->toLongLong();
				std::cout << "Array Numbers[" << i << "]: " << i << std::endl;
			}
		}
	}
	catch (Json::ParserException const& e) {
		std::cout << e.getMessage() << std::endl;
	}

	return EXIT_SUCCESS;
}
