#include <string>
#include "utils/vector_space.h"
#include "data_service/databuf/databuf.h"
#include <Eigen/Dense>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;

typedef MatrixXd MatrixF;
typedef VectorXd VectorF;

struct PageR
{
	uint8_t score;
	uint8_t id;
};

struct Page
{
	int id;
	int pagerank;
	VectorF tfidf_vector;
};

struct PageResult
{
	std::string url;
};


struct SearchQuery : public DataBuf
{
	std::string free_text;
public:
	void deserialize(char* buf, int size) override {

		char str[size];
        strncpy(str, buf, size);
        str[size] = '\0';

		json data = json::parse(str);
		free_text = data["query"];
	}

	char* serialize() override {
		//json j;
		//j["query"] = free_text;
		//return j.dump().c_str();
	}
};

struct SearchResults : public DataBuf
{

	std::vector<PageResult> page_results;

public:

	void deserialize(char* buf, int size) override {
		char str[size];
        strncpy(str, buf, size);
        str[size] = '\0';

		json data = json::parse(str);
		json results = data["results"];

		for (auto it = results.begin();it != results.end();it++) {
			PageResult result {(*it)["url"]};
			page_results.push_back(result);
		}
	}

	char* serialize() override {
		json j_results;
		json serialized;

		for (PageResult r : page_results) {
			json j;
			j["url"] = r.url;
			j_results.push_back(j);
		}

		serialized["results"] = j_results;

		std::string dump = serialized.dump();
		size_t dump_size = dump.size();
		const char* dumpc = dump.c_str();

		char* to_return = new char[dump_size];
		strncpy(to_return, dumpc, dump_size);

		return to_return;
	}
};

struct DocumentRequest : public DataBuf
{
	std::vector<int> document_ids;
public:
	void deserialize(char* buf, int size) override {
		char str[size];
        strncpy(str, buf, size);
        str[size] = '\0';

		json data = json::parse(str);

		for (auto it = data.begin();it != data.end();it++) {
			document_ids.push_back(*it);
		}
	}

	char* serialize() override {
		json serial;

		for (int id : document_ids) {
			serial.push_back(id);
		}

		std::string dump = serial.dump();
		size_t dump_size = dump.size();
		const char* dumpc = dump.c_str();

		char* to_return = new char[dump_size];
		strncpy(to_return, dumpc, dump_size);

		return to_return;
	}
};

struct DocumentResults : public JsonBuf
{
	std::vector<struct Page> documents;

	void from_json(json j) {
		for (auto it = j.begin();it != j.end();it++) {
			json temp = *it;
			struct Page page;
			page.id = temp["ID"];
			page.pagerank = temp["PAGERANK"];
			documents.push_back(page);
		}
	}

	json to_json() {

		json to_return;
		
		for (struct Page p : documents) {
			json j;
			j["ID"] = p.id;
			j["PAGERANK"] = p.pagerank;
			to_return.push_back(j);
		}

		return to_return;
	}
};