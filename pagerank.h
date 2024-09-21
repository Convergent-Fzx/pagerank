/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/

#ifndef PAGERANK_H
#define PAGERANK_H

#include "graph.h"
#include "list.h"


// TODO: Implement both of the functions below in graph.c

/*
 * graph_pagerank
 *
 * @brief Calculate the PageRank of each vertex in the graph, based on the 
 *        given damping factor and epsilon. An ignore list is also passed, 
 *        indicating that certain vertices are required to be excluded from the 
 *        calculation, including any incident edges.
 *
 * @param graph  The graph to be ranked. If it's NULL, do nothing.
 * @param double The damping factor to be used in the calculation.
 * @param double The epsilon value to be used in the calculation.
 * @param list   A list of vertices to be excluded from the calculation. If it's
 *               NULL, exclude nothing.
 */
void graph_pagerank(graph, double, double, list);

/*
 * graph_show_pagerank
 *
 * @brief Print the PageRank of each vertex in the graph to the given file. An 
 *        ignore list is also passed, indicating that certain vertices are 
 *        required to be excluded from the output.
 *
 * Note: You may assume that graph_pagerank has been called previously.
 *
 * The PageRanks should be printed in the following format:
 * 
 *      vertex1: rank1
 *      vertex2: rank2
 *      vertex3: rank3
 *      ...
 *      vertexN: rankN
 *
 * Where vertices are sorted first by descending rank, and then by ascending
 * label (lexicographically).
 *
 * @param graph  The graph to be ranked. If it's NULL, or empty, do nothing.
 * @param FILE * The file to print the ranks to. If it's NULL, print to stdout.
 * @param list   A list of vertices to be excluded from the output. If it's
 *               NULL, exclude nothing.
 */
void graph_show_pagerank(graph, FILE *, list);

#endif // PAGERANK_H

/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/
