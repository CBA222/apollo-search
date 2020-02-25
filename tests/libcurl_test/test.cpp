#include <curl/curl.h>
#include <iostream>
#include <cstring>

#include <sys/time.h>
#include <unistd.h>

using namespace std;

struct memory {
	char *memory;
	size_t size;
};

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

static struct timeval create_timeval(long time) {
	struct timeval timeout;

	if(time >= 0) {
		timeout.tv_sec = time / 1000;
		if(timeout.tv_sec > 1) {
			timeout.tv_sec = 1;
		}
		else {
			timeout.tv_usec = (time % 1000) * 1000;
		}
	}
}

CURL* create_handle(char *url) {

	struct memory *chunk = new struct memory;
	chunk->memory = NULL;
	chunk->size = 0;

	CURL *curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, chunk);
	curl_easy_setopt(curl, CURLOPT_PRIVATE, chunk);

	return curl;
}

int main(int argc, char* argv[]) {

	CURLM *multi_handle = curl_multi_init();

	curl_multi_add_handle(multi_handle, create_handle("https://www.google.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.yahoo.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.reddit.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.abc.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.espn.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.nba.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.hm.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.duckduckgo.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.asos.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.overstock.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.books.com/"));
	curl_multi_add_handle(multi_handle, create_handle("https://www.imdb.com/"));

	int transfers_running;
	do {
		fd_set fdread;
		fd_set fdwrite;
		fd_set fdexcep;
		int maxfd = -1;
		long curl_timeo;
		struct timeval timeout;

		curl_multi_timeout(multi_handle, &curl_timeo);
		timeout = create_timeval(curl_timeo);

		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);
		curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

		if (maxfd == -1) {
			struct timeval wait = { 0, 100 * 1000 }; /* 100ms */ 
			select(0, NULL, NULL, NULL, &wait);
		}
		else {
			select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
		}

		//curl_multi_wait ( multi_handle, NULL, 0, 1000, NULL);
		curl_multi_perform ( multi_handle, &transfers_running );

	} while (transfers_running);

	int msgs_left;
	CURLMsg *msg = NULL;
	while((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
		if(msg->msg == CURLMSG_DONE) {
			
			CURL *handle = msg->easy_handle;

			struct memory *mem;
			curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);

			cout << mem->size << endl;
		} else {

		}
	}


}