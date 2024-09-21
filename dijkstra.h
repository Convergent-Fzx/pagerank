/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "list.h"


// TODO: Implement both of the functions below in graph.c

/*
 * graph_shortest_path
 *
 * @brief Find the shortest path from the given source vertex to all other
 *        vertices in the graph. An ignore list is also passed, indicating that 
 *        certain vertices are required to be excluded from the exploration, 
 *        including any incident edges.
 *
 * @param graph  The graph to be searched. If it's NULL, do nothing.
 * @param string The source vertex to start the search from. If it's NULL, or 
 *               not in the graph, do nothing.
 * @param list   A list of vertices to be excluded from the search. If it's
 *               NULL, exclude nothing.
 */
void graph_shortest_path(graph, string, list);

/*
 * graph_show_path
 *
 * @brief Print the shortest path from the given source vertex to the given 
 *        destination vertex to the given file. An ignore list is also passed, 
 *        indicating that certain vertices are required to be excluded from the 
 *        calculation, including any incident edges.
 *
 * Note: You may assume that graph_shortest_path has been called previously.
 *
 * The path should be printed in the following format:
 * 
 *      source
 *       -> vertex1
 *       -> vertex2
 *       -> ...
 *       -> destination
 *
 * If there is no path from the source to the destination then nothing should be
 * printed.
 *
 * @param graph  The graph to be searched. If it's NULL, do nothing.
 * @param FILE * The file to print the path to. If it's NULL, print to stdout.
 * @param string The destination vertex to end the search at. If it's NULL, 
 *               or not in the graph, do nothing.
 * @param list   A list of vertices to be excluded from the output. If it's
 *               NULL, exclude nothing.
 */
void graph_show_path(graph, FILE *, string, list);

#endif // DIJKSTRA_H

/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/
