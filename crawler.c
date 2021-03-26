/* A simple client program for server.c

   To compile: gcc client.c -o client

   To run: start the server, then the client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <regex.h>

#include <tidy.h>
#include <tidybuffio.h>

#include "crawler.h"


#define PORT 80
#define URL_MAX_LENGTH 1000
#define MAX_URL 100
#define BUFFSIZE 100000

int request_len;
enum CONSTEXPR { MAX_REQUEST_LEN = 1024};
char request[MAX_REQUEST_LEN];
char request_format[] =  "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: seanw4\r\nConnection: close\r\n\r\n";
// char  url_list[1000000];
char  url_list[MAX_URL][URL_MAX_LENGTH];
char  visited_url[MAX_URL][URL_MAX_LENGTH];
int visit_count = 0;
int url_counter= 0;


/* Strips the http:// protiocol if present in given url*/
const char * strip_protocol(const char * url){
    fprintf( stderr, "ENTERING STRIP PROTOCOL FUNCTION \n");
    char * fixed_url;
    fprintf(stderr, "before stripping protocol with --- %s\n", url);

    if (strstr(url, "http://")!=NULL){
        fixed_url = url+7;

        fprintf(stderr, "after stripping protocol with http:// ---- %s\n", fixed_url);

        return fixed_url;
        //strcpy(fixed_url, url+7); 

    }

    if (strncmp(url, "//", 2)==0){
        fixed_url = url+2;
        return fixed_url;
    }
    // If protocol not preset, just return the url as is
    return url;
}


/* Get the host name of given absolute url */
const char * get_hostname(const char * url){
    char * remove_protocol = strip_protocol(url);
    char *ptr = strtok(remove_protocol, "/");
    return ptr;
}

/* Generates the absolute url   must add current url*/
const char * absolute_url(const char * url, const char * current_url) {
    char curr_copy[1000];
    strcpy(curr_copy, current_url);
    fprintf(stderr, "Curr copy %s\n", curr_copy);

    char protocol[1000];
    strcpy(protocol, "http:");

    char url_copy[1000];
    strcpy(url_copy, url);

    char full_protocol[1000];
    strcpy(full_protocol, "http://");

    char hostname[1000];
    strcpy(hostname, get_hostname(current_url));

    char doubleslash[3];
    strncpy(doubleslash, url_copy, 2);
    doubleslash[2] = '\0';

    char html[1000];

    char html_last[1000];
    strcpy(html_last, current_url);

    if (sscanf(url, ""))

    fprintf(stderr, "absolute url before testing\n");

    // Chaking if url is already absolute
    if (strstr(url, "http://")!=NULL){
        fprintf(stderr, "absolute url %s\n", url);
        return url;
    }
    fprintf(stderr, "absolute url before testing\n");
    fprintf(stderr, "double shlash value %s\n", doubleslash);

    // Checking if given url starts with a partial protocol
    if(strstr(doubleslash, "//")!=NULL){
        fprintf(stderr, "absolute url first if //:\n");
        fprintf(stderr, "absolute url in double slash comparison\n");
        strcat(protocol, url);
        fprintf(stderr, "absolute url %s\n", protocol);
        char *p = protocol;        
        const char *q = p; 
        return q;
    }

    fprintf(stderr, "absolute url before testing\n");

    // Chacking if url is relative
    if (strstr(doubleslash, "/")!=NULL){
        fprintf(stderr, "absolute url first if /:\n");
        strcat(full_protocol, hostname);
        strcat(full_protocol, url);
        fprintf(stderr, "absolute url %s\n", protocol);
        char *p = full_protocol;       
        const char *q = p; 
        return q;
    }
    fprintf(stderr, "absolute url before testing\n");
    if(strstr(url, ".html")!=NULL){
        if(strstr(curr_copy, ".html")!=NULL){

            int i;
            int index;
            for(i = 0; i <= strlen(curr_copy); i++){
  		        if(current_url[i] == "/"){
  			        index = i;	
 		        }
	        }
            strncpy(html, curr_copy, index);
            strcat(html, url);
            fprintf(stderr, "html found in both %s\n", html);
            char *p = html;       
            const char *q = p; 
            fprintf(stderr, "html found in both %s\n", q);
            return q;
        }else{
            strcat(curr_copy, "/");
            fprintf(stderr, "vurrcopy %s\n", curr_copy);
            strcat(curr_copy, url);
            fprintf(stderr, "html found in one %s\n", curr_copy);
            char *p = curr_copy;       
            const char *q = p; 
            fprintf(stderr, "html found in one %s\n", q);
            return q;
        }
    }
    strcat(full_protocol, url);
    fprintf(stderr, "fullprotocol %s\n", full_protocol);
    char *p = full_protocol;        
    const char *q = p; 
    return q;

}



/* Extracts the path from a url*/
const char * get_path(const char * url){

    char * remove_protocol = strip_protocol(url);

    if (strstr(url, "/")!=NULL){
        char *ptr = strtok(remove_protocol, "/");
        ptr = strtok(NULL," ");
        // strcat(path_link, ptr);
        return ptr;
    }else{
        return "";
    }
}

/* Checks a url if only fist component of hostname is different*/
bool check_host(const char * url, const char * current_url){
    printf("s");
    char curr_url[10000];
    strcpy(curr_url, current_url);

    char url_fix[1000];
    strcpy(url_fix, url);


    char * target_host = get_hostname(url_fix);
    char * current_host = get_hostname(curr_url);
    char *target_token = strtok(target_host, ".");
    char *current_token = strtok(current_host, ".");
    target_token = strtok(NULL, ".");
    current_token = strtok(NULL, ".");
    // printf(" %s %s", target_token, current_token);
    if(strcmp(target_token, current_token)==0){
        return true;
    }
    return false;
}


/*
chacks for invalid or unwanted urls
*/
bool valid_url(const char* url){

    regex_t regex;
    int return_value;

    return_value = regcomp(&regex,"%..",0);
    return_value = regexec(&regex, url, 0, NULL, 0);

    if (return_value == 0) {
        return false;
    }

    if (strstr(url, "?")!=NULL){
        return false;
    }
    if (strstr(url, "#")!=NULL){
        return false;
    }
    if (strstr(url, "..")!=NULL){
        return false;
    }
    if (strstr(url, "./")!=NULL){
        return false;
    }
    return true;
}


int check_status_code(char * buffer){
    if (strstr(buffer, "HTTP/1.1 200")!=NULL){
        return 0;
    }
    if (strstr(buffer, "HTTP/1.1 404")!=NULL){
        return 1;
    }
    if (strstr(buffer, "HTTP/1.1 401")!=NULL){
        return 2;
    }
    return 3;
}

/* Duplicate checker to prevent previously visited urls from being fetched */
bool duplicate(const char  *url, char  url_list[MAX_URL][URL_MAX_LENGTH], int url_counter){
    for(int i = 0; i < url_counter; i++){
        if (strcmp(url_list[i], url) == 0){
            // Duplicate found
            fprintf(stderr, "DUPLICATE FOUND %s\n", url);
            return true; 
        }
    }
    // No duplicate found.
    return false;
}

/* Takes a url and parses though its HTML to retrieve all href attributes */
void parse_page(TidyNode node, char output[MAX_URL][URL_MAX_LENGTH], const char * current_url) {
  
  TidyNode child;
//   printf("testing parse: %s\n", current_url);
  
  for (child = tidyGetChild(node); child != NULL; child = tidyGetNext(child)) {

    TidyAttr href = tidyAttrGetById(child, TidyAttr_HREF);

    if (href) {
        if (url_counter < 100) {
            if (strlen(tidyAttrValue(href)) < URL_MAX_LENGTH+1) {
                if(valid_url(tidyAttrValue(href))){
                    // if(!check_host(absolute_url(tidyAttrValue(href), current_url),current_url)){
                        // printf("Url found: %s\n", absolute_url(tidyAttrValue(href), current_url));
                        if (!duplicate(absolute_url(tidyAttrValue(href), current_url), url_list, url_counter)){
                            fprintf(stderr, "INBSIDE pARSING FUNCTION\n");
                            fprintf(stderr, "urlcounterbefore strcpy: %d\n", url_counter);
                            strcpy(url_list[url_counter], absolute_url(tidyAttrValue(href), current_url));
                            url_counter ++;
                            fprintf(stderr, "urlcounter: %d\n", url_counter);
                           
                        }
                        
                    // }
                }
            }
        }
    }
    parse_page(child, output, current_url);
  }
}

bool mime_test(char * buffer){
    if (strstr(buffer, "text/html")!=NULL){
        return true;
    }else{
        return false;
    }
}

void generate_socket(){

}

void fetch_page(const char* url, char * buffer){
    
    // const char * url_fix = url; 
    char fixed_url[10000];
    strcpy(fixed_url, url);

    char url_fix[1000];
    strcpy(url_fix, url);

    char path[10000];
    strcpy(path, get_path(url));

    char hostname[10000];
    strcpy(hostname, get_hostname(url_fix));

    // printf("printing url in fetch:%s \n", fixed_url);
    // printf("url path:%s \n", path);
    // printf("url path:%s \n", path);
    
    // char url_path[1000];
    // strcpy(url_path, get_path(url_fix));
    // char url_host[1000];
    // printf("url path:%s \n", url_path);
    // strcpy(url_path, get_path(url));
    int sockfd, portno, m;
    struct sockaddr_in serv_addr; 
    struct hostent * server;

    // char buffer[100000];


    portno = PORT;

    // printf("visited:%d %s \n", visit_count, url);

    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system
     */
    server = gethostbyname(get_hostname(url));

    // printf("printing url in fetch 2:%s \n", fixed_url);
    // printf("url path 2:%s \n", path);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(PORT);

    /* Create TCP socket -- active open
    * Preliminary steps: Setup: creation of active open socket
    */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
        exit(0);
    }

    /* Do processing
    */

    // printf("url path:%s \n", path);
    int n;
    char format[] =  "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: seanw4\r\nConnection: close\r\n\r\n";

    

    snprintf(request, MAX_REQUEST_LEN, format, path,hostname);
    // printf("String is : %s",get_path(url));
    // printf("String is : %s",strip_protocol(url));
    if (write(sockfd, request, strlen(request)) == -1){
        perror("ERROR writing to socket");
        exit(0);
    }
    
    while ((m = read(sockfd, buffer, BUFFSIZE-1))>0){
        // printf("%s", buffer);
    }

    
        
    close(sockfd);
    if (check_status_code(buffer)==0){
        // if(!duplicate(fixed_url, visited_url, visit_count)){
            strcpy(visited_url[visit_count], fixed_url);
            visit_count++;
            // printf("visited:%d %s \n", visit_count, fixed_url);
            TidyDoc tdoc = tidyCreate();   
            tidyParseString (tdoc, buffer);
            memset(buffer, 0, sizeof(buffer));
            fprintf(stderr, "url to parse : %s \n", fixed_url);
            // if(mime_test(buffer)){
                parse_page(tidyGetHtml(tdoc), url_list, fixed_url);
            // }
            
        // }
    }else if (check_status_code(buffer)==1){
        // strcpy(visited_url[visit_count], fixed_url);

        // if(!duplicate(fixed_url, visited_url, visit_count)){
        strcpy(visited_url[visit_count], fixed_url);
        visit_count++;
        // }
        memset(buffer, 0, sizeof(buffer));
    }else if (check_status_code(buffer)==2){

    }
    memset(buffer, 0, sizeof(buffer));

    if (n<0){
        perror("no read");
        exit(0);
    }
}

// void handle_auth(char[] hostname, char[] path){
//     int sockfd, portno, n; 
//     struct sockaddr_in serv_addr; 
//     struct hostent * server;
//     char format_reconnect[] = "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: seanw4\r\nAuthorization: Basic Z2J1bGtpbjpwYXNzd29yZA==\r\nConnection: close\r\n\r\n";

//     char buffer[100000];

//     if (argc < 2)
//     {
//         fprintf(stderr, "usage %s hostname port\n", argv[0]);
//         exit(0);
//     }

//     /* Translate host name into peer's IP address ;
//      * This is name translation service by the operating system
//      */
//     server = gethostbyname(hostname);

//     if (server == NULL)
//     {
//         fprintf(stderr, "ERROR, no such host\n");
//         exit(0);
//     }
//     /* Building data structures for socket */

//     bzero((char *)&serv_addr, sizeof(serv_addr));

//     serv_addr.sin_family = AF_INET;

//     bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

//     serv_addr.sin_port = htons(PORT);

//     /* Create TCP socket -- active open
//     * Preliminary steps: Setup: creation of active open socket
//     */

//     sockfd = socket(AF_INET, SOCK_STREAM, 0);

//     if (sockfd < 0){
//         perror("ERROR opening socket");
//         exit(0);
//     }

//     if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
//         perror("ERROR connecting");
//         exit(0);
//     }
    

//     /* Do processing
//     */
//     snprintf(request, MAX_REQUEST_LEN, format_reconnect, path, hostname);

//     if (write(sockfd, request, strlen(request)) == -1){
//         perror("ERROR writing to socket");
//         exit(0);
//     }

//     while ((n = read(sockfd, buffer, BUFFSIZE-1))>0){
//         // printf("%s", buffer);
//         // fetch_page(url_list[0],buffer);
//     }
//     if (n<0){
//         perror("no read");
//         exit(0);
//     }

//     close(sockfd);
// }



int main(int argc, char ** argv)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr; 
    struct hostent * server;

    fprintf( stderr, "START OF MAIN \n");

    char fixed_url[10000];
    strcpy(fixed_url, argv[1]);

    char path_url[10000];
    strcpy(path_url, argv[1]);

    fprintf( stderr, "fixed url %s \n", fixed_url);


    char test_url[10000];
    strcpy(test_url, argv[1]);

    fprintf( stderr, "test url %s \n", test_url);

    char hostname[10000];
    strcpy(hostname, get_hostname(test_url));

    fprintf( stderr, "hostname %s \n", hostname);
    // absolute_url(hostname,hostname);

    char path[10000];

    if(strcmp(strip_protocol(path_url), hostname)!=0){
        fprintf( stderr, "testing path comparison not the same%s \n", hostname);
        strcpy(path, get_path(argv[1]));
    }else{
        strcpy(path, "");
    }

    // strcpy(path, get_path(argv[1]));

    fprintf( stderr, "path %s \n", path);




    // char path[1000];
    // strcpy(path, get_path(fixed_url));
    // fprintf( stderr, "path %s \n", path);

    char buffer[100000];

    if (argc < 2)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }


    portno = PORT;

    


    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system
     */
    server = gethostbyname(hostname);

    // fprintf( stderr, "no protocol%s \n ", strip_protocol(fixed_url));
    // fprintf( stderr, "full url %s \n", fixed_url);
    // fprintf( stderr, "fpath %s \n", get_path(fixed_url));
    // fprintf( stderr, "my %s \n", path);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(PORT);

    /* Create TCP socket -- active open
    * Preliminary steps: Setup: creation of active open socket
    */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
        exit(0);
    }
    

    /* Do processing
    */
    snprintf(request, MAX_REQUEST_LEN, request_format, path, hostname);

    if (write(sockfd, request, strlen(request)) == -1){
        perror("ERROR writing to socket");
        exit(0);
    }

    while ((n = read(sockfd, buffer, BUFFSIZE-1))>0){
        // printf("%s", buffer);
        // fetch_page(url_list[0],buffer);
    }

    fprintf( stderr, "BEFORE ADDING VISITED URL TO LIST \n");

    strcpy(visited_url[visit_count], fixed_url);
    visit_count++;
    strcpy(url_list[url_counter], fixed_url);
    url_counter++;


    if (n<0){
        perror("no read");
        exit(0);
    }

    close(sockfd);

    /* Parse content of the buffer into a tidy doc to allow for tree like traversal*/
    TidyDoc tdoc = tidyCreate();   
    tidyParseString (tdoc, buffer);

    fprintf( stderr, "BEFORE FIRST PARSE \n");


    parse_page(tidyGetHtml(tdoc), url_list, test_url);
    memset(buffer, 0, sizeof(buffer));

    fprintf( stderr, "AFTER FIRST PARSE \n");

    // fetch_page(url_list[1],buffer);
    // printf("%s \n",get_path(url_list[5]));
    fprintf( stderr, "BEFORE FETCH LOOP IN MAIN MAIN \n");

    int loop; 
    int m;
    for(loop=1; loop<url_counter; loop++){
        fprintf( stderr, "FETCHING: %s\n",url_list[loop] );
        if(!duplicate(url_list[loop], visited_url, visit_count)){
            fetch_page(url_list[loop],buffer);
        }
        
		// printf("prinbting from lsit %s\n",url_list[loop]);
	}
    fprintf( stderr, "AFTER FETCH LOOP IN MAIN MAIN \n");
    // for(loop=0; loop<url_counter; loop++){
    //     // fetch_page(url_list[loop],buffer);
	// 	printf("prinbting from lsit %s\n",url_list[loop]);
	// }
    fprintf( stderr, "BEFORE PRINTING VISITED LIST \n");
    for(m=0; m<visit_count; m++){
        // fetch_page(url_list[loop],buffer);
		printf("%d: %s\n", m, visited_url[m]); 
	} 
    fprintf( stderr, "AFTER PRINTING VISITED LIST \n");

		
    return 0;
}


