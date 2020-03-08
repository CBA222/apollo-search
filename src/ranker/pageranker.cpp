#include "pageranker.h"

bool page_cmp(struct PageR a, struct PageR b) {
	return a.score < b.score;
}

float bin_to_float(char * ray) {
	return 0.0;
}

PageRanker::PageRanker(
	Index *index, 
	VectorF idf_weights, 
	float idf_threshold) 
	{
	INVERTED_INDEX = index;
	IDF_WEIGHTS = idf_weights;
	IDF_THRESHOLD = idf_threshold;
	std::vector<char *> tokens;
	VECTOR_SPACE = VectorSpace(tokens);
}

PageRanker::~PageRanker() {
}

std::vector<int> PageRanker::retrieve_to_rank(char* tokens[], unsigned int tokens_size) {

	std::vector<char *> tokens_to_use;
	bool in_set;

	for (unsigned int i = 0;i < tokens_size;i++) {
		//unsigned int idx = INVERTED_INDEX->get_idx(tokens[i], in_set);
		unsigned int idx = 0;
		if (IDF_WEIGHTS(idx, 0) > IDF_THRESHOLD) {
			tokens_to_use.push_back(tokens[i]);
		}
	}

	std::vector<int> intersection = INVERTED_INDEX->get_intersection(tokens_to_use.data(), tokens_to_use.size());

	return intersection;
}

int PageRanker::compute_overall_score(int cosine_sim, int min_window_width, int pagerank) {
	return (TFIDF_WEIGHT * cosine_sim) + (WINDOW_WEIGHT * min_window_width) + (PAGERANK_WEIGHT * pagerank);
}


int PageRanker::compute_rank(int pagerank, VectorF query_vector, VectorF document_vector) {
	// Compute cosine similarity

	int cosine_sim;
	int min_window_width;

	cosine_sim = query_vector.dot(document_vector) / (query_vector.norm() * document_vector.norm());

	return compute_overall_score(cosine_sim, min_window_width, pagerank);
}


std::vector<PageR> PageRanker::rank(std::vector<char *> tokens, unsigned int num_to_rank) {

	const char* conn_info = "";
	PGconn *conn = PQconnectdb(conn_info);

	std::vector<int> universe = retrieve_to_rank(tokens.data(), tokens.size());
	std::vector<struct PageR> universe_ranked;

	VectorF query_vector = VectorF(VOCAB_SIZE);

	for (std::vector<char *>::iterator it=tokens.begin();it!=tokens.end();it++) {
		bool in_set;
		unsigned int idx = VECTOR_SPACE.get_idx(*it, in_set);
		query_vector(idx, 0) = 1.0 * IDF_WEIGHTS(idx, 0);
	}

	for (std::vector<int>::iterator it=universe.begin();it!=universe.end();it++) {
		struct PageR temp;
		int doc_id = *it;

		int pagerank;
		VectorF document_vector = VectorF(VOCAB_SIZE);

		const char* c = std::to_string(*it).c_str();
		const char* values[2] = {TABLE_NAME, c};
		int lengths[2] = {sizeof(TABLE_NAME), sizeof(c)};
		int binary[2] = {0, 0};

		PGresult *res = PQexecParams(
			conn,
			"SELECT TOP 1 FROM $1 WHERE ID = $2",
			2,
			NULL,
			values,
			lengths,
			binary,
			0
		);

		for (int i = 0;i < VOCAB_SIZE;i++) {
			document_vector(i, 0) = bin_to_float(PQgetvalue(res, 0, i));
		}

		temp.id = doc_id;
		temp.score = compute_rank(pagerank, query_vector, document_vector);
		universe_ranked.push_back(temp);
	}

	std::nth_element(universe_ranked.begin(), universe_ranked.begin() + num_to_rank, universe_ranked.end(), page_cmp);
	std::vector<PageR> top_pages(universe_ranked.begin(), universe_ranked.begin() + num_to_rank);
	std::sort(top_pages.begin(), top_pages.end(), page_cmp);

	return top_pages;
}