#include "prompt.h"

void iMan_cmd(const char *command_name)
{
    struct hostent *server;
    struct sockaddr_in server_addr;
    int sockfd;
    server = gethostbyname("man.he.net");
    if (server == NULL)
    {
        perror("Error: Unable to resolve host.");
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error: Unable to create socket.");
        return;
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); 

    bcopy((char *)server->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error: Unable to connect to the server.");
        return;
    }

    char request[10000];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);
    int kk = write(sockfd, request, strlen(request));
    if (kk < 0)
    {
        perror("Error: Unable to send request.");
        return;
    }

    char response[10000];
    while (read(sockfd, response, sizeof(response)) > 0)
    {
        
        int print_flag = 0;
        int j = 0;

        for (int i = 0; response[i]; i++)
        {
            if (response[i] == '<')
            {
                print_flag = 0;
                continue;
            }
            if (response[i] == '>')
            {
                print_flag = 1;
                continue;
            }
            if (print_flag)
            {
                response[j] = response[i];
                j++;
            }
        }
        response[j] = '\0';

        printf("%s", response);
    }
    

    // Close the socket
    close(sockfd);
}