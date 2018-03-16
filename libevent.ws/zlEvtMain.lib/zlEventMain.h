void zlEventDescBevEvent (int evento );
struct sockaddr *zlEventSockaddr ( struct sockaddr_in *adr ,char * ip, int puerto );
int eventoInicia(struct event_base *base,int argc,char **argv);
void eventoIniciaPrevio(int argc,char **argv);
struct event_base *zlEventMainBase();


