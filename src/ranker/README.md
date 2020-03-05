SEARCH PROCESS
====================================================
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

UPDATE SETUP
==================================================
1. Crawl
2. Populate Database
3. Calculate tf-idf weights
4. Calculate PageRank
5. Populate Database (tf-idf, pagerank)
6. Create inverted index and store in file

INITIALIZE SETUP
==================================================
1. Read database
2. Create inverted index in memory (from file)


DATA MODEL
===================
CRAWL TABLE
    ROWS: each row is a document
    COLS:
        TF:
            TERM_1
            ...
            TERM_N

DOCUMENTS TABLE
    ROWS: each row is a document
    COLS:
        DOC_ID,
        URL,
        PAGERANK,
        TF-IDF:
            TERM_1
            ...
            TERM_N
        

