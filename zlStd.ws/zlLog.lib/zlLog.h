/* zLog include    */


void PanicInit(char *FileName,char *ProgramName);
int LgInit(char *FileName, int Nivel);
int LogInit(char *FileName, int Nivel);
void LgClose(int NumLog);
void LogClose();
void Panic(int nivel_pedido,char *fmtx, ...);
void LgW(int NumLog,int nivel_pedido,char *fmtx, ...);
void LogW(int nivel_pedido,char *fmtx, ...);
unsigned int zlLogCrea();
void LogInitStd(int Nivel);
void zlLogFlushOff();
void zlLogFlushOn();

