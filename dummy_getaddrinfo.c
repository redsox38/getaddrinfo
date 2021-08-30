#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dlfcn.h>
#include <errno.h>

extern int h_errno;
extern int errno;

int getaddrinfo(const char *node, 
		const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res) {

    char                *dummy_host, *dummy_ip;
    struct sockaddr_in  *s;
    struct addrinfo     *r;
    int                 __result__;
    typeof(getaddrinfo) *func;
    char                *dlerr;

    dummy_host = getenv("DUMMY_HOST");
    dummy_ip   = getenv("DUMMY_IP");

    if (dummy_host == NULL) {
        printf("error: DUMMY_HOST not set\n");
        return(EAI_NONAME);
    }

    if (!strcmp(node, dummy_host)) {
        printf("notice: bypassing DNS and resolving %s to %s\n", node, dummy_ip);
       
        r = (struct addrinfo *)malloc(sizeof(struct addrinfo));

        r->ai_family = AF_INET;
        r->ai_socktype = hints->ai_socktype;
        r->ai_protocol = 0;
        s = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
        s->sin_family = AF_INET;
        inet_aton(dummy_ip, &(s->sin_addr.s_addr)); 

        r->ai_addr = (struct sockaddr *)s;
        r->ai_addrlen = sizeof(struct sockaddr);
        r->ai_canonname = strdup(dummy_host);
        r->ai_next = NULL;
        
        *res = r;
        return(0);
    } else {
        /* just call the native getaddrinfo */
        
        func = (void *)dlsym(RTLD_NEXT, (char *)"getaddrinfo");
        if (func == NULL) {
            dlerr= dlerror(); 
            if (!dlerr) {
                dlerr= "Unknown reason";
            }

            fprintf(stderr, "failed to find native getaddrinfo(): %s!\n", dlerr);
            errno = ENOSYS;
            return(-1);
        } else {
            __result__ = func(node, service, hints, res);
            return(__result__);
        }

    }

}

