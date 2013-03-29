#ifndef LABELS_H
#define LABELS_H
#include "nodes.h"
#include "blocks.h"

void bindLabels(Node*, Block*, unsigned int);
void inclLabels(Node*, double*);
void exclLabels(Node*, double*);
void treeLabels(Node*, Node*);

#endif
