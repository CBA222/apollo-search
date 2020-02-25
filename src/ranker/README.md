User enters query
-> Query is spell-corrected
-> Stop words stripped

Extract results to consider
-> Create set of terms in query who idf exceeds certain threshold
-> Retrieve intersection of documents containing those terms.

Using the query, over the results we consider
-> Compute cosine similarity
-> Compute minimum window width

Also consider static metrics:
-> PageRank

Given these metrics, compute a single score
-> Machine learning

Retrieve top-K documents by their score
-> Use selection to retrieve top-K
-> Rank top-K and return