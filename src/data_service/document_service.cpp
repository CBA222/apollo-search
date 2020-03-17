#include "data_service/document_service.h"

static int s_interrupted = 0;
static void s_signal_handler(int sv) {
    s_interrupted = 1;
}

static void s_catch_signals() {
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}

static string to_pq(vector<int> v) {
    string s = "(";
    for (int i : v) {
        s.append(std::to_string(i) + ",");
    }
    s.replace(s.size()-1, 1, ")");
    return s;
}

SearchDataService::SearchDataService() {
    logger = new stdout_logger();
}

SearchDataService::~SearchDataService() {
    delete logger;
    delete data_channel;
}

void SearchDataService::configure(string socket_host, string sql_host) {
    this->socket_host = socket_host;
    this->sql_host = sql_host;
}

vector<struct Page> SearchDataService::get_documents(vector<int> doc_ids) {

    string temp = to_pq(doc_ids);
    const char * values[1] {temp.c_str()};

    string x ("SELECT doc_id, page_rank FROM WebPages WHERE doc_id in ");
    x.append(temp);

    PGresult *res = PQexec(conn, x.c_str());
    
    /*
    PGresult *res = PQexecParams(
			conn,
			"SELECT doc_id, page_rank FROM WebPages WHERE doc_id IN (1,3)",
			0,
			NULL,
			NULL,
			NULL,
			NULL,
			0
		);*/

    ExecStatusType error = PQresultStatus(res);
    std::cout << PQresultStatus(res) << std::endl;
    std::cout << PQresultErrorMessage(res) << std::endl;

    vector<struct Page> ret_pages;

    int rows = PQntuples(res);
    logger->INFO("Requested " + std::to_string(doc_ids.size()) + ", got " + std::to_string(rows));

    for (int i = 0;i < rows;i++) {

        struct Page page {
            std::stoi(PQgetvalue(res, i, 0)), 
            std::stoi(PQgetvalue(res, i, 1)), 
            (VectorF)NULL
        };

        ret_pages.push_back(page);

    }

    return ret_pages;

}

void SearchDataService::start() {

    s_catch_signals();

    conn = PQconnectdb(sql_host.c_str());

    //char * host = PQuser(conn);
    //std::cout << host << std::endl;

    //PGresult *res = PQexec(conn, "SELECT * FROM WebPages");
    //std::cout << "Rows returned " << std::to_string(PQntuples(res)) << std::endl;

    //MQ::Channel channel(socket_host);
    data_channel = new MQ::Channel(socket_host);

    data_channel->onReceived([this](MQ::Message msg) {

        struct DocumentRequest request;
        request.deserialize(msg.data, msg.size);

        vector<Page> pages = get_documents(request.document_ids);

        struct DocumentResults results;
        results.documents = pages;

        data_channel->publish(results.serialize());
    });

    data_channel->consume();
    logger->INFO("start consume");

    while (1) {
        if (s_interrupted == 1) {
            data_channel->close();
            break;
        }
    }
}