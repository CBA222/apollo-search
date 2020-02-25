#include <vector>
#include <algorithm>
#include "matrix.h"
#include "search_index/index.h"
//#include "utils.h"

#ifndef PAGE_RANKER_H
#define PAGE_RANKER_H

struct PageR
{
	uint8_t score;
	uint8_t id;
};

struct WordHit
{
	uint8_t cap;
	uint8_t size;
};

struct Page
{
	uint8_t id;
	uint8_t pagerank;
	Matrix tfidf_vector;
	std::vector<std::vector<WordHit>> hit_lists;
};

enum WordType { TITLE = 0, PLAIN = 1 };

class PageRanker
{
private:
	Matrix type_weights;
	static const int NUM_TYPES = 1;
	static const int MAX_HITS = 1;
	static const int COUNT_INCREMENT = 1;

	static const int IDF_THRESHOLD = 1;

	static constexpr float TFIDF_WEIGHT = 0.25f;
	static constexpr float WINDOW_WEIGHT = 0.25f;
	static constexpr float PAGERANK_WEIGHT = 0.25f;

	const int VOCAB_SIZE = 100;
	Matrix IDF_WEIGHTS;

	Index *base_index;

	static WordType compute_type(WordHit);

	int compute_vector_similarity_score();
	int compute_min_window_width();
	int compute_overall_score(int cosine_sim, int min_window_width, int pagerank);

	int compute_single_token_ir_score(struct Page, char* token, std::vector<WordHit> hit_list);
	int compute_rank(int pagerank, Matrix query_vector);
	int compare_page(struct Page page_1, struct Page page_2);

	std::vector<struct Page> retrieve_to_rank(char* tokens[], unsigned int tokens_size);

	unsigned int get_idx(char* token);

public:

	PageRanker(Index *index);
	~PageRanker();

	std::vector<PageR> rank(std::vector<char *> tokens, unsigned int num_to_rank);
};

#endif