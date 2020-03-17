#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct DataBuf {
public:
    virtual void serialize(char buf[]) {};
    virtual char* serialize() {};
    virtual void deserialize(char* buf, int size) {};
};

struct JsonBuf : public DataBuf {
public:
    virtual void from_json(json) {}
    virtual json to_json() {}

    void deserialize(char* buf, int size) override {
		char str[size];
        strncpy(str, buf, size);
        str[size] = '\0';

		json data = json::parse(str);

		from_json(data);
	}

    char* serialize() override {
        json serial;

		serial = to_json();

		std::string dump = serial.dump();
		size_t dump_size = dump.size();
		const char* dumpc = dump.c_str();

		char* to_return = new char[dump_size];
		strncpy(to_return, dumpc, dump_size);
        return to_return;
    }
};