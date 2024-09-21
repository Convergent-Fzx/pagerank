/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/

#ifndef T_STRING
#define T_STRING

typedef char *string;

#endif // T_STRING

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "list.h"

// TODO: Define exactly what a `struct Graph_Repr` is in graph.c
typedef struct Graph_Repr *graph;


// TODO: Implement all the functions below in graph.c

/***************************** GENERAL INTERFACE ******************************/

/**
 * graph_create
 *
 * @brief Allocate the required memory for a new graph.
 * 
 * @return The new graph. If there's an error (e.g. out of memory),
 *         return NULL.
 */
graph graph_create(void);

/**
 * graph_destroy
 *
 * @brief Free all memory associated with the given graph.
 *
 * @param graph The graph to be destroyed. If it's NULL, do nothing.
 */
void graph_destroy(graph);

/**
 * graph_show
 * 
 * @brief Print the contents of the graph to the given file.
 *
 * The graph should be printed in the following format:
 * 
 *      vertex1
 *      vertex2
 *      vertex3
 *      ...
 *      vertexN
 *      vertex1 vertex2 weight
 *      vertex2 vertex4 weight
 *      vertexN vertexM weight
 *
 * Where the label of each vertex is first printed, then the directed edges 
 * between each vertex along with the weight of that edge.
 *
 * An ignore list is also passed, indicating that certain vertices are required
 * to be excluded from the output, including any incident edges.
 *
 * @param  graph The graph to show. If it's NULL, do nothing.
 * @param FILE * The file to print the graph to. If it's NULL, print to stdout.
 * @param   list The vertices to ignore.
 */
void graph_show(graph, FILE *, list);


/****************************** VERTEX INTERFACE ******************************/

/**
 * graph_add_vertex
 * 
 * @brief Add a vertex with the given label to the graph.
 * 
 * @param  graph The graph in which to add the vertex. If it's NULL, do nothing.
 * @param string The label of the vertex to be added.  If it's NULL, or already 
 *               in the graph, do nothing.
 */
void graph_add_vertex(graph, string);

/**
 * graph_has_vertex
 *
 * @brief Determines whether a vertex with a particular label exists in the 
 *        graph.
 *
 * @param  graph The graph to search. If it's NULL, return false.
 * @param string The label of the vertex to search for. If it's NULL, return false.
 * @return       true if a vertex with the given label exists in the graph,
 *               false otherwise.
 */
bool graph_has_vertex(graph , string);

/**
 * graph_vertices_count
 *
 * @brief Returns the number of vertices in the graph.
 * 
 * @param graph The graph to count. If it's NULL, the graph is empty.
 * @return      The number of vertices in the graph.
 */
size_t graph_vertices_count(graph);


/******************************* EDGE INTERFACE *******************************/
/**
 * graph_add_edge
 * 
 * @brief Add a new weighted, directed edge to the graph.
 * 
 * @param  graph The graph in which to add the edge. If it's NULL, do nothing.
 * @param string The label of the source vertex. If it's NULL, do nothing.
 * @param string The label of the destination vertex. If it's NULL, do nothing.
 * @param size_t The weight of the edge. If the edge already exists, do nothing.
 */
void graph_add_edge(graph, string, string, size_t);

/**
 * graph_has_edge
 *
 * @brief Determines whether an edge exists between two vertices in the graph.
 *
 * @param  graph The graph to search. If it's NULL, return false.
 * @param string The label of the source vertex. If it's NULL, return false.
 * @param string The label of the destination vertex. If it's NULL, return false.
 * @return       true if an edge between two vertices exists in the graph,
 *               false otherwise.
 */
bool graph_has_edge(graph, string, string);

/**
 * graph_set_edge
 *
 * @brief Update the weight of a edge between two vertices.
 *
 * @param  graph The graph in which to update the edge. If it's NULL, do nothing.
 * @param string The label of the source vertex. If it's NULL, do nothing.
 * @param string The label of the destination vertex. If it's NULL, do nothing.
 * @param size_t The new weight of the edge. If the edge doesn't exist, do nothing.
 */
void graph_set_edge(graph, string, string, size_t);

/**
 * graph_get_edge
 *
 * @brief Return the weight of the edge between two vertices.
 *
 * @param  graph The graph to search. If it's NULL, return 0.
 * @param string The label of the source vertex. If it's NULL, return 0.
 * @param string The label of the destination vertex. If it's NULL, return 0.
 * @return       The weight of the edge between two vertices. If the edge doesn't
 *               exist, return 0.
 */
size_t graph_get_edge(graph, string, string);

/**
 * graph_edges_count
 *
 * @brief Return the number of outgoing edges from a particular vertex in the 
 *        graph.
 *
 * @param  graph The graph to search. If it's NULL, return 0.
 * @param string The label of the vertex. If it's NULL, return 0.
 * @return       The number of outgoing edges from the vertex. If the vertex 
 *               doesn't exist, return 0.
 */
size_t graph_edges_count(graph, string);

#endif //GRAPH_H

/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/
