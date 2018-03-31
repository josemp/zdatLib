
typedef void (*zlEvtTimer_i)(void *tag);
typedef struct {
zlEvtTimer_i function;
struct event *evtimeout;
void *tag;
} zlEvtTimer_t;


void zlEvtTimerFree(zlEvtTimer_t *defered);
zlEvtTimer_t *zlEvtTimerNuevo(struct event_base *base,zlEvtTimer_i function,void *arg);
zlEvtTimer_t *zlEvtTimerNuevoLibera(struct event_base *base,zlEvtTimer_i function,void *arg);
void zlEvtTimerActiva(zlEvtTimer_t *defered,int sec,int usec);
zlEvtTimer_t *zlEvtTimerNuevoActiva(struct event_base *base,int sec,int usec,zlEvtTimer_i function,void *arg);
zlEvtTimer_t *zlEvtTimerNuevoActivaLibera(struct event_base *base,int sec,int usec,zlEvtTimer_i function,void *arg);
void zlEvtTimerDesactiva(zlEvtTimer_t *defered);
/* Función rapida que libera y con 0 de timer */
zlEvtTimer_t * zlEvtTimerFunction(struct event_base *base,zlEvtTimer_i funcion, void *arg);
void *zlEventSelfPuntero();// Función para poner el timer en el callback


