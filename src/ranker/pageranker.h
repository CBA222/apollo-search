#include <vector>
#include <algorithm>
#include <string>
#include <Eigen/Dense>
#include <libpq-fe.h>
//#include "matrix.h"
#include "search_index/index.h"
//#include "utils.h"
#include <parallel_hashmap/phmap.h>

using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;

using phmap::flat_hash_map;

typedef MatrixXd MatrixF;
typedef VectorXd VectorF;

#ifndef PAGE_RANKER_H
#define PAGE_RANKER_H

struct PageR
{
	uint8_t score;
	uint8_t id;
};

struct Page
{
	uint8_t id;
	uint8_t pagerank;
	VectorF tfidf_vector;
};

class PageRanker
{
private:

	static constexpr float TFIDF_WEIGHT = 0.25f;
	static constexpr float WINDOW_WEIGHT = 0.25f;
	static constexpr float PAGERANK_WEIGHT = 0.25f;

	float IDF_THRESHOLD;
	const char* TABLE_NAME;
	const int VOCAB_SIZE = 100;

	VectorF IDF_WEIGHTS;
	Index *INVERTED_INDEX;

	int compute_vector_similarity_score();
	int compute_min_window_width();
	int compute_overall_score(int cosine_sim, int min_window_width, int pagerank);

	int compute_rank(int pagerank, VectorF query_vector, VectorF document_vector);
	std::vector<int> retrieve_to_rank(char* tokens[], unsigned int tokens_size);
	
public:
	PageRanker();
	PageRanker(Index *index, VectorF idf_weights, float idf_threshold);
	~PageRanker();

	std::vector<PageR> rank(std::vector<char *> tokens, unsigned int num_to_rank);
};

#endif