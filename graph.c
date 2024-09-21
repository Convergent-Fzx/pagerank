#include "list.h"
#include "graph.h"
#include "pagerank.h"
#include "dijkstra.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct Vertex {
    char *label;
    struct Edge *edges;
    struct Vertex *next;
    double oldrank;
    double pagerank;
    int dist;
    char *pred;
} Vertex;

typedef struct Edge {
    Vertex *destination;
    Vertex *source;
    int weight;
    struct Edge *next;
} Edge;

typedef struct Graph_Repr {
    char *vertex;
    struct Vertex *vertices;
    struct Graph_Repr *next;
    int nv;
    int ne;
} Graph_Repr;

typedef struct graph {
    int nv;
    int ne;
    struct Graph_Repr *vertices;
} *Graph;

char *my_strdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, str, len);
    }
    return copy;
}

//graph ref ###########################################################################################
graph graph_create(void) {
    Graph_Repr *new_graph = (Graph_Repr*)malloc(sizeof(struct Graph_Repr));
    assert(new_graph != NULL && "Memory allocation failed for new_graph");

    new_graph->vertices = NULL;
    return new_graph;
}

void graph_destroy(graph graph_to_destroy) {
    if (graph_to_destroy == NULL)
        return;
    while (graph_to_destroy->vertices != NULL) {
        Vertex *current_vertex = graph_to_destroy->vertices;
        graph_to_destroy->vertices = graph_to_destroy->vertices->next;
        free(current_vertex->label);
        Edge *current_edge = current_vertex->edges;
        while (current_edge != NULL) {
            Edge *temp = current_edge;
            current_edge = current_edge->next;
            free(temp);
        }
        free(current_vertex);
    }
    free(graph_to_destroy);
}

void graph_show(graph g, FILE *crawler, list ignore_list) {
    // Check if the graph is NULL
    if (g == NULL) {
        return;
    }

    // If the file to print to is NULL, print to stdout
    if (crawler == NULL) {
        crawler = stdout;
    }

    // Start iterating through the vertices of the graph
    Vertex *current_vertex = g->vertices;

    // Print vertices first
    while (current_vertex != NULL) {
        // Check if the current vertex should be ignored
        if (!list_contains(ignore_list, current_vertex->label)) {
            // Print the label of the current vertex
            fprintf(crawler, "%s\n", current_vertex->label);
        }

        // Move to the next vertex
        current_vertex = current_vertex->next;
    }

    // Start iterating through the vertices again to collect edges
    current_vertex = g->vertices;

    while (current_vertex != NULL) {
        // Check if the current vertex should be ignored
        if (!list_contains(ignore_list, current_vertex->label)) {
            // Create an array to store edges
            Edge **edges = malloc(1000 * sizeof(Edge *));
            assert(edges != NULL);
            int edge_count = 0;

            // Iterate through the edges of the current vertex
            Edge *current_edge = current_vertex->edges;
            while (current_edge != NULL) {
                // Check if the destination vertex should be ignored
                if (!list_contains(ignore_list, current_edge->destination->label)) {
                    edges[edge_count++] = current_edge;
                }
                // Move to the next edge
                current_edge = current_edge->next;
            }

            // Print the edges in reverse order
            for (int i = edge_count - 1; i >= 0; i--) {
                fprintf(crawler, "%s %s %d\n", current_vertex->label, edges[i]->destination->label, edges[i]->weight);
            }

            // Free the memory allocated for the edges array
            free(edges);
        }

        // Move to the next vertex
        current_vertex = current_vertex->next;
    }
}

// vertex ref####################################################################################
void graph_add_vertex(graph g, string label) {
    if (g == NULL || label == NULL)
        return;

    if (graph_has_vertex(g, label))
        return;

    Vertex *new_vertex = (Vertex *)malloc(sizeof(Vertex));
    assert(new_vertex != NULL);

    new_vertex->label = my_strdup(label);
    assert (new_vertex->label != NULL) ;

    new_vertex->edges = NULL;
    new_vertex->next = NULL;

    if (g->vertices == NULL) {
        g->vertices = new_vertex;
    } else {
        Vertex *current_vertex = g->vertices;
        while (current_vertex->next != NULL) {
            current_vertex = current_vertex->next;
        }
        current_vertex->next = new_vertex;
    }

}


bool graph_has_vertex(graph g, string label) {
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        if (strcmp(current_vertex->label, label) == 0) {
            // Vertex already exists, do nothing
            return true;
        }
        current_vertex = current_vertex->next;
    }
    return false;
}


size_t graph_vertices_count(graph g) {
    size_t count = 0;
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        count++;
        current_vertex = current_vertex->next;
    }
    return count;
}


//EDGE REF##########################################################################################
void graph_add_edge(graph g, string labelstart, string labelend, size_t weight) {
    if (g == NULL || labelstart == NULL || labelend == NULL)
        return;
    
    // Check if the vertices exist in the graph
    Vertex *start_vertex = NULL;
    Vertex *end_vertex = NULL;
    Vertex *current_vertex = g->vertices;

    // Search for start and end vertices in the graph
    while (current_vertex != NULL && (start_vertex == NULL || end_vertex == NULL)) {
        if (strcmp(current_vertex->label, labelstart) == 0)
            start_vertex = current_vertex;
        if (strcmp(current_vertex->label, labelend) == 0)
            end_vertex = current_vertex;
        current_vertex = current_vertex->next;
    }

    // If either start or end vertex is not found, create them
    if (start_vertex == NULL) {
        start_vertex = (Vertex *)malloc(sizeof(Vertex));
        assert(start_vertex != NULL);
        start_vertex->label = my_strdup(labelstart);
        assert(start_vertex->label != NULL);
        start_vertex->edges = NULL;
        start_vertex->next = g->vertices;
        g->vertices = start_vertex;
    }
    if (end_vertex == NULL) {
        end_vertex = (Vertex *)malloc(sizeof(Vertex));
        assert(end_vertex != NULL);
        end_vertex->label = my_strdup(labelend);
        assert(end_vertex->label != NULL);
        end_vertex->edges = NULL;
        end_vertex->next = g->vertices;
        g->vertices = end_vertex;
    }

    // Check if the edge already exists
    Edge *current_edge = start_vertex->edges;
    while (current_edge != NULL) {
        if (current_edge->destination == end_vertex)
            return; // Edge already exists
        current_edge = current_edge->next;
    }

    // Create a new edge
    Edge *new_edge = (Edge *)malloc(sizeof(Edge));
    assert(new_edge != NULL);

    // Set new edge information
    new_edge->weight = weight;
    new_edge->next = start_vertex->edges;
    new_edge->source = start_vertex;
    new_edge->destination = end_vertex;

    // Update pointers
    start_vertex->edges = new_edge;
}

bool graph_has_edge(graph g,string labelstart,string labelend){
    if (g == NULL || labelstart == NULL || labelend == NULL)
        return false;
    
    // check all the vertex
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        // If the labelstart matches the current vertex's label
        if (strcmp(current_vertex->label, labelstart) == 0) {
            // Check if labelend exists in the edges
            Edge *current_edge = current_vertex->edges;
            while (current_edge != NULL) {
                // If labelend matches the label of the destination vertex in the edge
                if (strcmp(current_edge->destination->label, labelend) == 0) {
                    // Edge already exists, do nothing
                    return true;
                }
                current_edge = current_edge->next;
            }
        }
    }
    return false;
}

void graph_set_edge(graph g,string labelstart,string labelend,size_t weight){
    if (g == NULL || labelstart == NULL || labelend == NULL)
        return;
    // check all the vertex
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        // If the labelstart matches the current vertex's label
        if (strcmp(current_vertex->label, labelstart) == 0) {
            // Check if labelend exists in the edges
            Edge *current_edge = current_vertex->edges;
            while (current_edge != NULL) {
                // If labelend matches the label of the destination vertex in the edge
                if (strcmp(current_edge->destination->label, labelend) == 0) {
                    // Edge already exists, change the weight
                    current_edge -> weight = weight;
                    return;
                }
                current_edge = current_edge->next;
            }
        }
    return;
    }
}

size_t graph_get_edge(graph g, string labelstart, string labelend) {
    if (g == NULL || labelstart == NULL || labelend == NULL)
        return 0;
    
    // Check all the vertices
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        // If the labelstart matches the current vertex's label
        if (strcmp(current_vertex->label, labelstart) == 0) {
            // Check if labelend exists in the edges of this vertex
            Edge *current_edge = current_vertex->edges;
            while (current_edge != NULL) {
                // If labelend matches the label of the destination vertex in the edge
                if (strcmp(current_edge->destination->label, labelend) == 0) {
                    // Edge exists, return its weight
                    return current_edge->weight;
                }
                current_edge = current_edge->next;
            }
        }
        current_vertex = current_vertex->next;
    }
    // If the edge does not exist, return 0
    return 0;
}

size_t graph_edges_count(graph g,string labelstart){
    if(g == NULL || labelstart == NULL)
       return 0;
    
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        // if the labelstart matches the current vertex's label
        if (strcmp(current_vertex->label, labelstart) == 0) {
            // count the number of outgoing edges
            size_t count = 0;
            Edge *current_edge = current_vertex->edges;
            while (current_edge != NULL) {
                count++;
                current_edge = current_edge->next;
            }
            return count;
        }
        current_vertex = current_vertex->next;
    }
    // vertex not found
    return 0;
}
//########################################################################################################
//count the number of edges from source but the destination is not in ignore list
size_t graph_edges_count_without_ignorelist(graph g,string labelstart,list ignore){
    if (g == NULL || labelstart == NULL)
       return 0;
    
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        
        if (strcmp(current_vertex->label, labelstart) == 0) {
            
            size_t count = 0;
            Edge *current_edge = current_vertex->edges;
            while (current_edge != NULL) {
                // check if destination is in ignorelist
                if (!list_contains(ignore, current_edge->destination->label)) {
                    count++;
                }
                current_edge = current_edge->next;
            }
            return count;
        }
        current_vertex = current_vertex->next;
    }
    // no end
    return 0;
}

//count the number of vertices which are not in ignore list
size_t graph_vertices_count_without_ignorelist(graph g,list ignore){
    size_t count = 0;
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        if (!list_contains(ignore, current_vertex->label)) {
            count++;
        }
        current_vertex = current_vertex->next;
    }
    return count;
}

void pagerank_initialize(graph g){
    int num_vertex = graph_vertices_count(g);
    Vertex *current = g->vertices;
    while (current != NULL){
        current -> oldrank = 0.0;
        current -> pagerank = 1.0/num_vertex;
        current = current ->next;
    }
}

void graph_pagerank(graph g, double damping_factor, double epsilon, list ignore) {
    int num_vertices = graph_vertices_count_without_ignorelist(g,ignore);

    // Initialize pagerank values
    pagerank_initialize(g);

    while (true) {
        // Set it converged
        bool converged = true;

        // Iterate through all vertices to update pagerank values
        Vertex *current_vertex = g->vertices;
        while (current_vertex != NULL) {
            // Skip vertex if it's in the ignore list
            if (!list_contains(ignore, current_vertex->label)) {
                // Update oldrank to current pagerank
                current_vertex->oldrank = current_vertex->pagerank;
            }
            current_vertex = current_vertex->next;
        }

        // Calculate sink_rank for vertices with no outbound edges or all outbound edges in ignore list
        double sink_rank = 0;
        Vertex *sink_vertex = g->vertices;
        while (sink_vertex != NULL) {
            bool all_edges_in_ignore = true;
            if (!list_contains(ignore, sink_vertex->label)) {
                Edge *current_edge = sink_vertex->edges;
                while (current_edge != NULL) {
                    if (!list_contains(ignore, current_edge->destination->label)) {
                        all_edges_in_ignore = false;
                        break;
                    }
                    current_edge = current_edge->next;
                }
                if (current_edge == NULL || all_edges_in_ignore) {
                    sink_rank += (damping_factor * sink_vertex->oldrank) / num_vertices;
                }
            }
            sink_vertex = sink_vertex->next;
        }

        // Iterate through all vertices to update pagerank values
        current_vertex = g->vertices;
        while (current_vertex != NULL) {
            // Skip vertex if it's in the ignore list
            if (!list_contains(ignore, current_vertex->label)) {
                // Recalculate pagerank for current vertex
                double new_pagerank = sink_rank + ((1 - damping_factor) / num_vertices);

                // Calculate the contribution from incoming edges
                Vertex *source_vertex = g->vertices;
                while (source_vertex != NULL) {
                    // Skip source vertex if it's in the ignore list
                    if (!list_contains(ignore, source_vertex->label)) {
                        Edge *current_edge = source_vertex->edges;
                        while (current_edge != NULL) {
                            // Skip edge if its destination is in the ignore list
                            if (!list_contains(ignore, current_edge->destination->label)) {
                                if (current_edge->destination == current_vertex) {
                                    // If the current edge points to the current vertex, consider its contribution
                                    size_t num_out_edges = graph_edges_count_without_ignorelist(g, source_vertex->label, ignore);
                                    if (num_out_edges > 0) {
                                        new_pagerank += (damping_factor * source_vertex->oldrank) / num_out_edges;
                                    }
                                }
                            }
                            current_edge = current_edge->next;
                        }
                    }
                    source_vertex = source_vertex->next;
                }

                // Update the PageRank value of the current vertex
                current_vertex->pagerank = new_pagerank;

                // Check if convergence condition is met
                if (fabs(current_vertex->pagerank - current_vertex->oldrank) > epsilon) {
                    converged = false;
                }
            }
            current_vertex = current_vertex->next;
        }

        // If PageRank values of all vertices have converged, exit the loop
        if (converged) {
            break;
        }
    }
}


typedef struct {
    char *label;
    double pagerank;
} VertexRank;

// Comparison function for qsort to sort VertexRank array by descending rank and then by ascending label
int compare_vertex_ranks(const void *a, const void *b) {
    const VertexRank *vertex_rank_a = (const VertexRank *)a;
    const VertexRank *vertex_rank_b = (const VertexRank *)b;

    // First, compare by descending rank
    if (vertex_rank_a->pagerank > vertex_rank_b->pagerank) {
        return -1;
    } else if (vertex_rank_a->pagerank < vertex_rank_b->pagerank) {
        return 1;
    } else {
        // If ranks are equal, compare by ascending label
        return strcmp(vertex_rank_a->label, vertex_rank_b->label);
    }
}

void graph_show_pagerank(graph g, FILE *output_file, list ignore_list) {
    // Check if the graph is NULL or empty
    if (g == NULL || g->vertices == NULL) {
        return;
    }

    // If the output file is NULL, print to stdout
    if (output_file == NULL) {
        output_file = stdout;
    }

    // Create an array to store vertices and their ranks
    VertexRank *vertex_ranks = malloc(graph_vertices_count(g) * sizeof(VertexRank));
    assert (vertex_ranks != NULL);

    // Fill the array with vertices and their ranks
    int index = 0;
    Vertex *current_vertex = g->vertices;
    while (current_vertex != NULL) {
        // Skip vertex if it's in the ignore list
        if (!list_contains(ignore_list, current_vertex->label)) {
            vertex_ranks[index].label = current_vertex->label;
            vertex_ranks[index].pagerank = current_vertex->pagerank;
            index++;
        }
        current_vertex = current_vertex->next;
    }

    // Sort the array by descending rank and then by ascending label
    qsort(vertex_ranks, index, sizeof(VertexRank), compare_vertex_ranks);

    // Print the sorted vertex ranks to the output file
    for (int i = 0; i < index; i++) {
        fprintf(output_file, "%s: %.3f\n", vertex_ranks[i].label, vertex_ranks[i].pagerank);
    }

    // Free the allocated memory for the array
    free(vertex_ranks);
}

//################################################################################################
Vertex *find_min_v(graph g, list set) {
    Vertex *curr_v = g->vertices;
    int min_dist = 99999;
    Vertex *min_v = NULL;
    while (curr_v != NULL) {
        if (list_contains(set, curr_v->label) && curr_v->dist < min_dist) {
            min_dist = curr_v->dist;
            min_v = curr_v;
        }
        curr_v = curr_v->next;
    }
    return min_v;
}


void graph_shortest_path(graph G, string source, list L) {
    Vertex *curr_v = G->vertices;
    list vset = list_create();
    while (curr_v != NULL) {
        curr_v->dist = 1000000;
        curr_v->pred = NULL;
        if (strcmp(curr_v->label, source) == 0) {
            curr_v->dist = 0;
        }
        if (!list_contains(L, curr_v->label)) {
            list_add(vset, curr_v->label);
        }
        curr_v = curr_v->next; 
    }//initialize dist, pred, vset

    while (list_length(vset) != 0) {
        Vertex *min_v = find_min_v(G, vset);   //min_v points to the vertex with minimal dist in set
        if (min_v == NULL) {
            return;
        }

        Edge *curr_e = min_v->edges;  //pointer to the edge of min_v
        while (curr_e != NULL) {
            if (!list_contains(L, curr_e->destination->label)) {  //if the edge id not to ignore
                curr_v = G->vertices;
                while (curr_v != NULL) {
                    if (strcmp(curr_v->label, curr_e->destination->label) == 0) {
                        break;  //find the vertex that is the destination of the edge
                    }
                    curr_v = curr_v->next;
                }

                if (min_v->dist + 1 < curr_v->dist) {   //if relaxation is needed
                    curr_v->dist = min_v->dist + 1;
                    curr_v->pred = min_v->label;
                }
                //printf("%d,%s\n", curr_v->dist, curr_v->name_of_v);        
            }    
            curr_e = curr_e->next;
        }
        list_remove(vset, min_v->label); //remove this vertex in vset
    }
}

void graph_show_path(graph G, FILE *file, string dest_v, list L) {
    FILE *out_file = (file != NULL ? file : stdout);
    list path = list_create();
    Vertex *current = NULL;
    Vertex *curr_v = G->vertices;
    while (curr_v != NULL) {
        if (strcmp(curr_v->label, dest_v) == 0) {  //find the vertex if the destination of the path
            current = curr_v;
            if (current->pred == NULL) {   //if there is no path
                return;
            }
            list_push(path, current->label);    //push the name
            break;
        }
        curr_v = curr_v->next;
    }
    if (curr_v == NULL) {   //if the destination is not found
        return;
    }
    //now there must exist a path
    while (current->pred != NULL) { //find the previous vertex
        list_push(path, current->pred);    //push the name
        curr_v = G->vertices;
        while (curr_v != NULL) {
            if (strcmp(curr_v->label, current->pred) == 0) {  //find the previous vertex
                current = curr_v;
                break;
            }
            curr_v = curr_v->next;
        }
    }
    //now the 'current' vertex's pred is null
    if (current->dist != 0) {   //if it is not the source
        Vertex *source = G->vertices;
        while (source != NULL) {
            if (source->dist == 0) { //find the source
                break;
            }
            source = source->next;
        }
        graph_shortest_path(G, source->label, L);     //recalculate the shortest path
        graph_show_path(G, file, dest_v, L);            //show again
    } else {  // if it is the source, print the path
        fprintf(out_file, "%s\n", list_pop(path));
        int n = list_length(path);
        for (int i = 0; i < n; i ++) {
            fprintf(out_file, "  -> %s\n", list_pop(path));
        }
    }

}






