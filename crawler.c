/**
 * Use libcurl and libxml2 to create a web crawler
 *
 * derived from: https://raw.githubusercontent.com/curl/curl/master/docs/examples/crawler.c
 * curl project copyright: https://raw.githubusercontent.com/curl/curl/master/COPYING
 *
 * Written by: Dylan Brotherston <d.brotherston@unsw.edu.au>
 * Updated by: Frank Tran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>

#include "list.h"
#include "graph.h"
#include "pagerank.h"

/* resizable buffer */
typedef struct memory {
    char *buf;
    size_t size;
} memory;


int    is_html     (string);
graph  follow_link (string);
void   find_links  (list, list, graph, memory *, string, string);
CURL  *make_handle (string);
size_t grow_buffer (void *, size_t, size_t, void *);
void   add_or_increment_edge(graph, string, string);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // attempt some form on normalisation by removing any queries or fragments
    if (strchr(argv[1], '?')) *strchr(argv[1], '?') = '\0';
    if (strchr(argv[1], '#')) *strchr(argv[1], '#') = '\0';

    graph web = NULL;
    if (strstr(argv[1], "cse.unsw.edu.au") || strstr(argv[1], "localhost")) {
        web = follow_link(argv[1]);
        
    } else {
        fprintf(stderr, "refusing to touch non CSE pages.");
        return EXIT_FAILURE;
    }
    
    /* NOTE: You can modify the code in this section */

    // Existing variables:
    //   web: graph object created by the above crawler code
    
    // Create an empty ignore list
    list ignore_list = list_create();
    

    /* NOTE: Uncomment this block once you have completed the 
        appropriate list functions */
    
    //char ignore_url[BUFSIZ];
    //printf("Enter a page to ignore or type 'done': ");
    //while (scanf("%s", ignore_url)) {
    //    if (strcmp(ignore_url, "done") == 0) {
    //        break;
    //    }
    //    list_add(ignore_list, ignore_url);
    //    printf("Enter another page to ignore or type 'done': ");
    //}
    
    graph_show(web, stdout, ignore_list);
 
    // Cleanup
    list_destroy(ignore_list);
    graph_destroy(web);

    return EXIT_SUCCESS;
}


// DO NOT MODIFY THE CODE BELOW

// webpage fetcher using libcurl
graph follow_link(string base_url)
{
    curl_global_init(CURL_GLOBAL_ALL);
    list queue    = list_create();
    list visited  = list_create();
    graph web = graph_create();
    list_enqueue(queue, base_url);
    list_add(visited, base_url);
    while (!list_is_empty(queue)) {
        char *url, *ctype;
        memory *mem;
        base_url = list_dequeue(queue);
        CURL *handle = make_handle(base_url);
        CURLcode res = curl_easy_perform(handle);
        nanosleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 500000000}, NULL);
        curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &ctype);
        curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);
        curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);

        if (res == CURLE_OK) {
            long res_status;
            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_status);
            if (res_status == 200) {
                printf("HTTP 200: %s\n", base_url);
                if (is_html(ctype)) {
                    find_links(queue, visited, web, mem, url, base_url);
                }
            } else {
                fprintf(stderr, "HTTP %d: %s\n", (int)res_status, base_url);
            }
        } else {
            fprintf(stderr, "Connection failure: %s\n", base_url);
        }

        free(base_url);
        free(mem->buf);
        free(mem);
        curl_easy_cleanup(handle);
    }

    list_destroy(queue);
    list_destroy(visited);
    curl_global_cleanup();
    xmlCleanupParser();
    return web;
}

// HREF finder using libxml2
void find_links(list queue, list visited, graph web, memory *mem, string url, string base_url)
{
    int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
    htmlDocPtr doc = htmlReadMemory(mem->buf, (int)mem->size, url, NULL, opts);
    if (!doc) return;

    xmlChar *xpath = (xmlChar*) "//a/@href";
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);
    if (!result) {
        xmlFreeDoc(doc);
        return;
    }
    xmlNodeSetPtr nodeset = result->nodesetval;
    if (xmlXPathNodeSetIsEmpty(nodeset)) {
        xmlXPathFreeObject(result);
        xmlFreeDoc(doc);
        return;
    }

    for (int i = 0; i < nodeset->nodeNr; i++) {
        const xmlNode *node = nodeset->nodeTab[i]->xmlChildrenNode;
        xmlChar *href = xmlNodeListGetString(doc, node, 1);
        xmlChar *orig = href;
        href = xmlBuildURI(href, (xmlChar *) url);
        xmlFree(orig);
        char *link = (char *) href;
        if (!link) continue;
        // attempt some form on normalisation by removing any queries or fragments
        if (strchr(link, '?')) *strchr(link, '?') = '\0';
        if (strchr(link, '#')) *strchr(link, '#') = '\0';
        // we only want a map of hyperlinks, so restrict the scheme to http[s]
        if (!strncmp(link, "http://", 7) || !strncmp(link, "https://", 8)) {
            // use `base_url` not url as `url` has had redirects dereferenced
            add_or_increment_edge(web, base_url, link);
            // have some manners and restrict hyperlinks to domains inside UNSW CSE, and that we haven't already visited.
            if ((strstr(link, "cse.unsw.edu.au") || strstr(link, "localhost")) && !list_contains(visited, link)) {
                list_add(visited, link);
                list_enqueue(queue, link);
            }
        }
        xmlFree(link);
    }
    xmlXPathFreeObject(result);
    xmlFreeDoc(doc);
}

size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx)
{
    size_t realsize = sz * nmemb;
    memory *mem = (memory*) ctx;
    char *ptr = realloc(mem->buf, mem->size + realsize);
    if(!ptr) {
        fprintf(stderr, "OOM\n");
        return 0;
    }
    mem->buf = ptr;
    memcpy(&(mem->buf[mem->size]), contents, realsize);
    mem->size += realsize;
    return realsize;
}

CURL *make_handle(char *url)
{
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);

    /* buffer body */
    memory *mem = malloc(sizeof(memory));
    mem->size = 0;
    mem->buf = malloc(1);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, grow_buffer);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, mem);
    curl_easy_setopt(handle, CURLOPT_PRIVATE, mem);

    /* For completeness */
    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 2L);
    curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(handle, CURLOPT_FILETIME, 1L);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "COMP9024 crawler");
    curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_easy_setopt(handle, CURLOPT_UNRESTRICTED_AUTH, 1L);
    curl_easy_setopt(handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
    curl_easy_setopt(handle, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);
    return handle;
}

int is_html(char *ctype)
{
    return ctype != NULL && strstr(ctype, "text/html");
}

void add_or_increment_edge(graph g, string vertex1, string vertex2)
{
    if (!graph_has_vertex(g, vertex1)) graph_add_vertex(g, vertex1);
    if (!graph_has_vertex(g, vertex2)) graph_add_vertex(g, vertex2);

    if (!graph_has_edge(g, vertex1, vertex2)) graph_add_edge(g, vertex1, vertex2,   1);
    else graph_set_edge(g, vertex1, vertex2,  graph_get_edge(g, vertex1, vertex2) + 1);
}
