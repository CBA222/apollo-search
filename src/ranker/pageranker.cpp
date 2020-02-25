#include "pageranker.h"

bool page_cmp(struct PageR a, struct PageR b) {
	return a.score < b.score;
}

PageRanker::PageRanker(Index *index) {
	base_index = index;
}

WordType PageRanker::compute_type(WordHit) {

}

int PageRanker::compute_single_token_ir_score(struct Page, char* token, std::vector<WordHit> hit_list) {
	Matrix *count_weights = new Matrix(NUM_TYPES, 1);

	for (std::vector<WordHit>::iterator it=hit_list.begin();it!=hit_list.end();it++) {
		WordType it_type = compute_type(*it);
		
		if (count_weights[it_type][1] < MAX_HITS) {
			count_weights[it_type][1] += 1;
		}
	}

	int ir_score = Matrix::dot_product(type_weights, *count_weights);
	return ir_score;
}

std::vector<struct Page> PageRanker::retrieve_to_rank(char* tokens[], unsigned int tokens_size) {

	std::vector<char *> tokens_to_use;
	bool in_set;

	for (unsigned int i = 0;i < tokens_size;i++) {
		unsigned int idx = base_index->get_idx(tokens[i], in_set);
		if (IDF_WEIGHTS[idx] > IDF_THRESHOLD) {
			tokens_to_use.push_back(tokens[i]);
		}
	}

	std::vector<int> intersection = base_index->get_intersection(tokens_to_use.data(), tokens_to_use.size());

	std::vector<struct Page> to_return;

	return to_return;
}

int PageRanker::compute_overall_score(int cosine_sim, int min_window_width, int pagerank) {
	return (TFIDF_WEIGHT * cosine_sim) + (WINDOW_WEIGHT * min_window_width) + (PAGERANK_WEIGHT * pagerank);
}


int PageRanker::compute_rank(int pagerank, Matrix query_vector) {
	// Compute cosine similarity

	int cosine_sim;
	int min_window_width;

	return compute_overall_score(cosine_sim, min_window_width, pagerank);

}


std::vector<PageR> PageRanker::rank(std::vector<char *> tokens, unsigned int num_to_rank) {

	std::vector<struct Page> universe = retrieve_to_rank(tokens.data(), tokens.size());
	std::vector<struct PageR> universe_ranked;

	Matrix *query_vector = new Matrix(VOCAB_SIZE, 1);

	for (std::vector<struct Page>::iterator it=universe.begin();it!=universe.end();it++) {
		struct PageR temp;
		struct Page p = *it;

		temp.id = p.id;
		temp.score = compute_rank((int)p.pagerank, *query_vector);
		universe_ranked.push_back(temp);
	}

	std::nth_element(universe_ranked.begin(), universe_ranked.begin() + num_to_rank, universe_ranked.end(), page_cmp);
	std::vector<PageR> top_pages(universe_ranked.begin(), universe_ranked.begin() + num_to_rank);
	std::sort(top_pages.begin(), top_pages.end(), page_cmp);

	return top_pages;
}