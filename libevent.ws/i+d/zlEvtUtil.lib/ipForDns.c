// Atencion esto lo dejo porque prefiero getHostByName que lo añado a libstd
   


// He usado el documento https://github.com/nmathewson/libevent-book/blob/master/Ref9_dns.txt
// La funcion getaddrinfo utiliza tanto host como puerto 
// El host puede ser ip o dns, y el puerto puede ser numerico o por ejemplo "http"
// Aquí solo usaremos el host
zlEvtIpOfHost(string host)
{


 char port_buf[6];
    struct evutil_addrinfo hints;
    struct evutil_addrinfo *answer = NULL;
    int err;
    evutil_socket_t sock;

    /* no usamos el port */
     strcpy(port_buf,"");

    /* Build the hints to tell getaddrinfo how to act. */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; /* v4 or v6 is fine. */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; /* We want a TCP socket */
    /* Only return addresses we can use. */
    hints.ai_flags = EVUTIL_AI_ADDRCONFIG;

    /* Look up the hostname. */
    err = evutil_getaddrinfo(hostname, port_buf, &hints, &answer);
    if (err != 0) {
          fprintf(stderr, "Error while resolving '%s': %s",
                  hostname, evutil_gai_strerror(err));
          return -1;
    }
}
