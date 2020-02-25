#include "crawler.h"

struct memory {
	char *memory;
	size_t size;
};

Crawler::Crawler(list<char *> sl) {
	visit_count = 0;
	visited_links = new FastHashSet();
	link_queue = new FastQueue<char *>();
	page_queue = new FastQueue<char *>();

	for (list<char *>::iterator it=sl.begin();it!=sl.end();it++) {
		seed_links.push_back(*it);
	}

	curl_global_init(CURL_GLOBAL_ALL);
}

Crawler::~Crawler() {
	delete visited_links;
	delete link_queue;

	for (list<char *>::iterator it=seed_links.begin();it!=seed_links.end();it++) {
		free(*it);
	}
}

bool Crawler::add_link(char* url) {

}

void Crawler::start_crawl() {
	list<char *>::iterator it;

	vector<thread> ths;
	for (it=seed_links.begin();it != seed_links.end();it++) {
		//parse(*it);
		ths.push_back(thread(&Crawler::traverse, this, *it));

	}

	for (auto& th : ths) {
		th.join();
	}

	while (1) {
		if (visit_count > 9999999) {
			return;
		}


	}
}

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	
	size_t realsize = size * nmemb;
	struct memory *mem = (struct memory *)userp;

	char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);

	if (ptr == NULL) {
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), buffer, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return size * nmemb;
}

void Crawler::traverse(char* url) {
	
	struct memory chunk;
	chunk.memory = NULL;
	chunk.size = 0;

	CURL *curl = curl_easy_init();
	CURLcode ret;

	cout << url << endl;

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

	ret = curl_easy_perform(curl);

	if (ret != CURLE_OK) {
		cout << curl_easy_strerror(ret) << endl;
	} else {
		cout << "We got " << chunk.size / 1024 << " kB" << endl;
		page_queue->insert(chunk.memory);
	}

	curl_easy_cleanup(curl);

	//free(chunk.memory);
}

void Crawler::traverse_loop() {

	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURLM *multi_handle = curl_multi_init();
	curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 800);


	int msgs_left;
	int read_in = 0;

	FastQueue<CURL *> idle_handles;
	FastQueue<CURL *> working_handles;

	/* seed */

	while (!(link_queue->empty()) || !(page_queue->empty())) {

		CURLMsg *m = NULL;
		while (m= curl_multi_info_read(multi_handle, &msgs_left)) {
			if (m->msg == CURLMSG_DONE) {
				CURL *handle = m->easy_handle;
				char *url;
				struct memory *mem;

				curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);
				curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);

				if (m->data.result == CURLE_OK) {
					long res_status;
					curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_status);
					if (res_status == 200) {
						page_queue->insert(mem->memory);
					}
					else {

					}
				}
			}
			else {
				//
			}
		}

		while (!(link_queue->empty())) {
			
		}
	}
}

void Crawler::find_links_loop() {
	while (!(link_queue->empty()) || !(page_queue->empty())) {
		pair<bool, char*> result = page_queue.remove();

		if (result.first == true) {
			link_queue.insert("");
		}
	}
}