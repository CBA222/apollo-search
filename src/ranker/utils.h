#ifndef UTILS_H
#define UTILS_H

bool page_cmp(struct PageR a, struct PageR b) {
	return a.score < b.score;
}

#endif