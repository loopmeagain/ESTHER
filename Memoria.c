


#include "estructuras.h"

#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/txt.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include <commons/config.h>
#include <signal.h> 
#define BACKLOG 5
#define LIBRE 0
#define OCUPADO 1
#define KERNEL 0
	#define ARRAYPIDS 5
	#define PIDFINALIZACION 2
	#define PATH 3
	#define PIDINFO 4
	#define RESPUESTAOKMEMORIA 1

	#define SOLICITUDMEMORIA 70
	#define SOLICITUDINFOPROG 1
	#define ESCRIBIRMEMORIA 2
	#define LIBERARMEMORIA 3
 	#define ACTUALIZARPCB 4
	#define SOLICITUDBYTES 31
	#define LINEA 19
	#define ALMACENARBYTES 36
	//-------------------------------
	#define DELAY 0
	#define DUMP 1
	#define FLUSH 2
	#define SIZE 3
#define CONSOLA 2
	#define INICIARPROGRAMA 0
	#define FINALIZARPROGRAMA 104
	#define DESCONECTARCONSOLA 2
	#define LIMPIARMENSAJES 3
	//------------------------------	
	#define MENSAJES 0
	#define PIDNUEVO 1
#define LIBERARPAGINA 71
#define CPU 3
#define FS 4
#define TRUE 1
#define FALSE 0
#define OK 1
#define FAIL 0
#define BLOQUE 20
#define ASIGNARPAGINAS 63
#define CACHE 0
#define MEMORIA 1 
#define TABLA 2
#define PID 3
#define PCB 3
#define PIDSIZE 1
#define MEMORIASIZE 0
int retardo=0;
int HAB=1;
pthread_mutex_t controlMemoria;

t_estructuraADM * bloquesAdmin;
int tamPagina;
t_list * paginasLiberadas;
t_marco * asignador;
unsigned int tamanioAdministrativas;
char * mensajeError="la instruccion solicitada no existe, gracias vuelvas prontos";
char * mensajeFinalizacionHilo="el hilo no esta, el hilo se fue, el hilo se escapa de mi vida";
char *IP_KERNEL;
char *PUERTO_KERNEL;
char *PUERTO;
int MARCOS;
int MARCO_SIZE;
int ENTRADAS_CACHE;
int CACHE_X_PROC;
char *REEMPLAZO_CACHE;
int RETARDO_MEMORIA;

t_marco * marcos;
t_estructuraCache* memoriaCache;
int socketKernel;

t_list**overflow;
FILE *MemoriaLog;
char * horaActual;
char* nombreLog;
int len;

void stripLog(char** string){
	int i ;
	for(i=0;i<string_length(*string); i++){
		if((*string)[i]==' ' || (*string)[i]=='/' )
			(*string)[i]='-';
	}
}
void cortar(){
	fclose(MemoriaLog);
	int unMarco=0;
		int unaLista=0;

	
		free(bloquesAdmin); 

for(unMarco=0;unMarco<MARCOS;unMarco++)
{
	free(marcos[unMarco].numeroPagina);
	
}
free(marcos);
for(unMarco=0;unMarco<ENTRADAS_CACHE;unMarco++)
{
	free(memoriaCache[unMarco].contenido);
}
free(memoriaCache); 
for ( unaLista = 0; unaLista < MARCOS; unaLista++)
{
	list_destroy(overflow[unaLista]);
}
free(overflow); 
	exit(0);
}
void escribirEnArchivoLog(char * contenidoAEscribir, FILE ** archivoDeLog,char * direccionDelArchivo){
	
	fseek(*archivoDeLog,0,SEEK_END);
	len=ftell(*archivoDeLog);
	fwrite(contenidoAEscribir,strlen(contenidoAEscribir),1,*archivoDeLog);
	fwrite("\n",1,1,*archivoDeLog);
	}
	
/* LEER CONFIGURACION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
t_config *CFG;
/* LEER CONFIGURACION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
void consolaMemoria();
void comunicarse(int * socket);
void aceptar(dataParaComunicarse * unData);
int main(){

CFG = config_create("memoriaCFG.txt");
IP_KERNEL= config_get_string_value(CFG ,"IP_KERNEL");
PUERTO_KERNEL= config_get_string_value(CFG ,"PUERTO_KERNEL");
PUERTO= config_get_string_value(CFG ,"PUERTO");
MARCOS= config_get_int_value(CFG ,"MARCOS");
MARCO_SIZE= config_get_int_value(CFG ,"MARCO_SIZE");
ENTRADAS_CACHE= config_get_int_value(CFG ,"ENTRADAS_CACHE");
CACHE_X_PROC= config_get_int_value(CFG ,"CACHE_X_PROC");
REEMPLAZO_CACHE= config_get_string_value(CFG ,"REEMPLAZO_CACHE");
RETARDO_MEMORIA= config_get_int_value(CFG ,"RETARDO_MEMORIA");
fflush(stdout);
printf("Configuración:\nPUERTO = %s,\nIP_KERNEL = %s,\nPUERTO_KERNEL = %s,\nMARCOS = %i,\nMARCO_SIZE = %i,\nENTRADAS_CACHE = %i,\nCACHE_X_PROC = %i,\nREEMPLAZO_CACHE = %s,\nRETARDO_MEMORIA = %i.\n"
		,PUERTO,IP_KERNEL,PUERTO_KERNEL,MARCOS,MARCO_SIZE,ENTRADAS_CACHE,CACHE_X_PROC,REEMPLAZO_CACHE,RETARDO_MEMORIA);
retardo=RETARDO_MEMORIA;

char * nombreLog=calloc(1,200);
	strcpy(nombreLog,"logMemoria-");
	horaActual=calloc(1,200);
	horaYFechaActual(&horaActual);
	stripLog(&horaActual);
	strcat(horaActual,".txt");
	strcat(nombreLog,horaActual);

int nbytes;

int a,listenningSocket,rv,i;
struct addrinfo hints;
struct addrinfo *serverInfo;


memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_flags = AI_PASSIVE;
hints.ai_socktype = SOCK_STREAM;

if ((rv =getaddrinfo(NULL, PUERTO, &hints, &serverInfo)) != 0) fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

fflush(stdout);
printf("%s \n", "El Servidor esta configurado.\n");
listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
fflush(stdout);
printf("%s \n", "Socket Listo.");

if(bind(listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen)==-1) {perror("Error en el bind."); exit(1);}

fflush(stdout);
printf("%s \n", "Bind Listo.\n");

MemoriaLog=fopen(nombreLog,"w+");
signal(SIGINT,cortar);
freeaddrinfo(serverInfo);
fflush(stdout);
config_destroy(CFG);
system("clear");
printf("%s \n", "El Servidor se encuentra listo para escuchar conexiones.");
pthread_t hiloAceptador;
dataParaComunicarse *unData;
unData=malloc(sizeof(dataParaComunicarse));
unData->socket = listenningSocket;
pthread_mutex_init(&controlMemoria,NULL);

 ////////////////////////////////////// INICIALIZAMOS EL BLOQUE DE MEMORIA/////////////////////////////////////

bloquesAdmin=calloc(1,MARCOS*sizeof(t_estructuraADM)+MARCOS*sizeof(t_marco));
tamanioAdministrativas=MARCOS*sizeof(t_estructuraADM);
overflow = calloc(1,sizeof(t_list*) * MARCOS);
inicializarOverflow(MARCOS,overflow);
int unaAdmin;
for (unaAdmin = 0; unaAdmin < MARCOS; unaAdmin++) //inicializo los bloques de admin con pid-1 libre y num de pag y el hash
{
bloquesAdmin[unaAdmin].estado=0;
bloquesAdmin[unaAdmin].pid=-1;
bloquesAdmin[unaAdmin].pagina=-1;
	
}
 
marcos=(t_marco*)calloc(1,MARCOS*sizeof(t_marco));
int unMarco=0;






memoriaCache=(t_estructuraCache*)calloc(1,sizeof(t_estructuraCache)*ENTRADAS_CACHE);
 // marcos es un bloque de punteros a void porque el tipo void en c no existe y aca quiero pegar lo que me de la gana.
for(unMarco=0;unMarco<MARCOS; unMarco++) //asignar su numero de marco a cada region de memoria
	{ 
		marcos[unMarco].numeroPagina=calloc(1,MARCO_SIZE);
		
	}
	
	
for(unMarco=0;unMarco<ENTRADAS_CACHE; unMarco++) //asignar su numero de marco a cada region de memoria
	{
		memoriaCache[unMarco].contenido=calloc(1,MARCO_SIZE);
		memoriaCache[unMarco].frame=-666; //para distinguir  una pagina real de una virgen
		memoriaCache[unMarco].pid=-1;
		memoriaCache[unMarco].antiguedad=-1;
		memoriaCache[unMarco].modificado=0;
			}
	/*for(unMarco=0;unMarco<MARCOS;unMarco++)
	{	 printf("el contenido de la pag numero %i es :\n",unMarco );
		 DumpHex(memoriaCache[unMarco].contenido,MARCO_SIZE);	
		
		sleep(1);}*/
	asignador=&marcos[0];

	int adminPorPagina=MARCO_SIZE/sizeof(t_estructuraADM); 
	int marcoParaAdmin;
	int offsetAdmin=0;
	t_estructuraADM * copiaAdmins=calloc(1,adminPorPagina*sizeof(t_estructuraADM)+1);
	if(MARCOS<2){printf("%s\n", "imposible gestionar la memoria");return -1;}
	int unaPagina=0;
	int paginasParaAdmin=(MARCOS/adminPorPagina)+1;
	for(unaPagina=0;unaPagina<paginasParaAdmin;unaPagina++)
	{	
		for (unaAdmin = 0; unaAdmin < adminPorPagina; unaAdmin++)
		{
		copiaAdmins[unaAdmin].pid=bloquesAdmin[unaAdmin+(unaPagina*adminPorPagina)].pid;
		copiaAdmins[unaAdmin].pagina=bloquesAdmin[unaAdmin+(unaPagina*adminPorPagina)].pagina;
		copiaAdmins[unaAdmin].estado=bloquesAdmin[unaAdmin+(unaPagina*adminPorPagina)].estado;

		}
		memcpy(marcos[unaPagina].numeroPagina,copiaAdmins,adminPorPagina*sizeof(t_estructuraADM));
		bloquesAdmin[unaPagina].estado=1;
		
	}
	free(copiaAdmins);
////////////////////////////////INICIAMOS HILOS DE COMINICACION/////////////////////////////////////////////



pthread_create(&hiloAceptador,NULL,(void *)aceptar,unData);
pthread_t hiloConsolaMemoria;
pthread_create(&hiloConsolaMemoria,NULL,(void *)consolaMemoria,NULL);
pthread_join(hiloAceptador,NULL);
pthread_join(hiloConsolaMemoria,NULL);
free(unData);



} 




void comunicarse(int  * socket){ // aca tenemos que agregar toda la wea que es "global"
fflush(stdout); 
escribirEnArchivoLog("en comunicarse", &MemoriaLog,nombreLog);
int * buffer;
int unData=*socket;
void * auxiliar;
int a=1;
t_seleccionador * seleccionador;
int entrada;
char * paquete;
int paginasRequeridas;
int stackRequeridas;
int indice;
int test;
t_peticionBytes * peticionBytes;
t_almacenarBytes * bytesAAlmacenar;
t_solicitudMemoria * solicitud;
int  * pidALiberar;
int confirmacion;
int * marco;
int pedidos;
t_solicitudAsignar * pedidoAsignacion;
t_liberarPagina * liberarPagina;
int ultimaPagina;
int mLibre=0;
int flagHilo=1,rv;
while(flagHilo) {
		seleccionador=calloc(1,8);
			while(0>(rv=recv(unData,seleccionador,sizeof(t_seleccionador),0)));
			if (rv==0)
			{
				flagHilo=0;
			}else{

				switch (seleccionador->tipoPaquete)
				{
						case SOLICITUDMEMORIA: // [Identificador del Programa] // paginas necesarias para guardar el programa y el stack
								 //esto lo vi en stack overflow no me peguen
								escribirEnArchivoLog("en case solicitud memoria", &MemoriaLog,nombreLog);
							solicitud=malloc(sizeof(t_solicitudMemoria));
								recibirDinamico(SOLICITUDMEMORIA,unData,solicitud);
								escribirEnArchivoLog("recibo solicitud memoria", &MemoriaLog,nombreLog);
								fflush(stdout);
								
	 							
	 							paginasRequeridas=solicitud->cantidadPaginasCodigo;
	 							stackRequeridas=solicitud->cantidadPaginasStack;
	 							pthread_mutex_lock(&controlMemoria);
	 							usleep(retardo*1000);
	 							if(hayPaginasLibres(paginasRequeridas+stackRequeridas,bloquesAdmin,MARCOS)==-1) 
	 							{ pthread_mutex_unlock(&controlMemoria);
	 							solicitud->respuesta=FAIL;
	 							send(unData,&(solicitud->respuesta),sizeof(int),0);
	 							escribirEnArchivoLog("envio respuesta", &MemoriaLog,nombreLog);
	 							}

	 							else
	 							{pthread_mutex_unlock(&controlMemoria); //mandarOK memoria
	 							solicitud->respuesta=OK;
	 						
	 							send(unData,&(solicitud->respuesta),sizeof(int),0);
	 							escribirEnArchivoLog("envio respuesta", &MemoriaLog,nombreLog);
	 							pthread_mutex_lock(&controlMemoria);
	 							usleep(retardo*1000);
								test=reservarYCargarPaginas(paginasRequeridas,stackRequeridas,MARCOS,bloquesAdmin,&marcos,solicitud->tamanioCodigo, solicitud->pid,&(solicitud->codigo),MARCO_SIZE,overflow,ENTRADAS_CACHE,memoriaCache,CACHE_X_PROC,retardo);
	 							pthread_mutex_unlock(&controlMemoria);
								if(test==1){}
	 							else printf("%s\n","Error en la reserva" );
	 							free(solicitud->codigo);
	 							free(solicitud);
	 							
	 							
	 							 }	
	 							
	 							
	 							
	 							

	 					break;
	 		 
	 					case SOLICITUDBYTES:
	 										escribirEnArchivoLog("en case solicitud bytes", &MemoriaLog,nombreLog);
	 										peticionBytes=calloc(1,sizeof(t_peticionBytes));
	 										recibirDinamico(SOLICITUDBYTES,unData,peticionBytes);
	 										escribirEnArchivoLog("recibo solicitud bytes", &MemoriaLog,nombreLog);
	 										
	 										
	 										pthread_mutex_lock(&controlMemoria);
	 										
	 										if(-1==existePagina(peticionBytes->pid,peticionBytes->pagina ,bloquesAdmin,MARCOS))
											{	
												pthread_mutex_unlock(&controlMemoria);confirmacion=-1;
												send(unData, &confirmacion, sizeof(int),0);
												escribirEnArchivoLog("envio confirmacion", &MemoriaLog,nombreLog);
											}
											else{fflush(stdout);
												pthread_mutex_unlock(&controlMemoria);
												confirmacion=1;
												send(unData, &confirmacion, sizeof(int),0);
												escribirEnArchivoLog("envio informancion", &MemoriaLog,nombreLog);
											pthread_mutex_lock(&controlMemoria);

	 										if((entrada=estaEnCache(peticionBytes->pid,peticionBytes->pagina,memoriaCache,ENTRADAS_CACHE))!=-1)
	 										{pthread_mutex_unlock(&controlMemoria);confirmacion=-1;//lo busco en cache
	 											paquete=calloc(1,peticionBytes->size);
	 											auxiliar=(void*)solicitarBytesCache(peticionBytes->pid,peticionBytes->pagina,memoriaCache,ENTRADAS_CACHE,peticionBytes->offset,peticionBytes->size);
	 											memcpy(paquete,auxiliar,peticionBytes->size);
	 											free(auxiliar);
	 										}
	 										else
	 										{
	 										pthread_mutex_unlock(&controlMemoria);confirmacion=-1;//lo busco en memoria
	 											
	 											indice=calcularPosicion(peticionBytes->pid,peticionBytes->pagina,MARCOS); 
	 											entrada=buscarEnOverflow(indice,peticionBytes->pid,peticionBytes->pagina,bloquesAdmin,MARCOS,overflow);
	 											pthread_mutex_lock(&controlMemoria);
	 											usleep(retardo*1000);
	 											paquete=calloc(1,peticionBytes->size+1);
	 											memcpy(paquete,marcos[entrada].numeroPagina+peticionBytes->offset,peticionBytes->size);
	 											escribirEnCache(peticionBytes->pid,peticionBytes->pagina,marcos[entrada].numeroPagina,memoriaCache,ENTRADAS_CACHE,0,MARCO_SIZE,0,MARCOS,overflow,bloquesAdmin,marcos,MARCO_SIZE,CACHE_X_PROC,retardo);
	 											pthread_mutex_unlock(&controlMemoria);confirmacion=-1;//uso escribirEnCache para guardar una pagina entera en cache que esta en memoria
	 										}	
	 											buffer=calloc(1,sizeof(int));
												memcpy(buffer,&a,sizeof(int));
												
	 											free(buffer);
												
	 											send(unData,paquete,peticionBytes->size,0);
	 											
	 											escribirEnArchivoLog("envio paquete", &MemoriaLog,nombreLog);
	 											// printf("paquete: %i\n",*(int*) paquete);
	 											// strcat(paquete,"\0");
	 											// printf("paquete: %s\n",(char*) paquete);
	 											free(paquete);
	 										}
	 											free(peticionBytes);
	 					break;
	 					case ASIGNARPAGINAS: //: [Identificador del Programa] [Páginas requeridas]
	 										escribirEnArchivoLog("en case asignar paginas", &MemoriaLog,nombreLog);
	 										pedidoAsignacion=calloc(1,sizeof(t_solicitudAsignar));
	 										recibirDinamico(ASIGNARPAGINAS,unData,pedidoAsignacion);
	 										escribirEnArchivoLog("recibo asignar paginas", &MemoriaLog,nombreLog);
	 										
	 										pthread_mutex_lock(&controlMemoria);
	 										usleep(retardo*1000);
	 										if(hayPaginasLibres(pedidoAsignacion->paginasAAsignar,bloquesAdmin,MARCOS)==-1) 
	 										{ pthread_mutex_unlock(&controlMemoria);
	 											confirmacion=-1;
												send(unData, &confirmacion, sizeof(int),0);
												escribirEnArchivoLog("envio confirmacion", &MemoriaLog,nombreLog);
	 										}
	 										else{pthread_mutex_unlock(&controlMemoria);
	 											confirmacion=0;
	 											fflush(stdout);
	 											marco=calloc(1,sizeof(int));
	 											for(pedidos=0;pedidos<pedidoAsignacion->paginasAAsignar;pedidos++)
	 											{pthread_mutex_lock(&controlMemoria);
	 												usleep(retardo*1000);
												ultimaPagina=buscarUltimaPaginaAsignada(pedidoAsignacion->pid,bloquesAdmin,MARCOS);
												pthread_mutex_unlock(&controlMemoria);
	        									ultimaPagina++;
												indice=calcularPosicion(pedidoAsignacion->pid,ultimaPagina,MARCOS);
												
												pthread_mutex_lock(&controlMemoria);
												usleep(retardo*1000);
												mLibre=buscarMarcoLibre(marcos,MARCOS,bloquesAdmin);
	         									memcpy(marco,&mLibre,sizeof(int)); 
	         									
	         									if(*marco!=-1)
	         									{
	         									agregarSiguienteEnOverflow(indice,&marco,overflow);
	        									 bloquesAdmin[*marco].estado=1;
	        									bloquesAdmin[*marco].pid=pedidoAsignacion->pid;
	        									bloquesAdmin[*marco].pagina=ultimaPagina;
	        									pthread_mutex_unlock(&controlMemoria);
	 											send(unData, &confirmacion, sizeof(int),0);
	 											escribirEnArchivoLog("envio confirmacion", &MemoriaLog,nombreLog);
	 											}else{pthread_mutex_unlock(&controlMemoria);printf("se rompio algo\n");} 
												} free(marco);
											}free(pedidoAsignacion);
								 			
	 					break;
	 					case ALMACENARBYTES:	
	 								escribirEnArchivoLog("en case almacenar bytes", &MemoriaLog,nombreLog);
	 								bytesAAlmacenar=calloc(1,sizeof(t_almacenarBytes));
									// bytesAAlmacenar->valor=calloc(1,20); dserial_void ya le hace malloc
	 								recibirDinamico(ALMACENARBYTES,unData,bytesAAlmacenar);
	 								escribirEnArchivoLog("recibo almacenar bytes", &MemoriaLog,nombreLog);
	 								
	 								pthread_mutex_lock(&controlMemoria);
	 								usleep(retardo*1000);
									if(existePagina(bytesAAlmacenar->pid,bytesAAlmacenar->pagina,bloquesAdmin,MARCOS)==-1)
									{	pthread_mutex_unlock(&controlMemoria);
										confirmacion=-1;
										send(unData, &confirmacion, sizeof(int),0);
										escribirEnArchivoLog("envio confirmacion", &MemoriaLog,nombreLog);
									}
									else
									{usleep(retardo*1000);
									almacenarBytes(bytesAAlmacenar->pid,bytesAAlmacenar->pagina,bytesAAlmacenar->valor, marcos,MARCOS, bytesAAlmacenar->offset,bytesAAlmacenar->size,bloquesAdmin,overflow,memoriaCache,ENTRADAS_CACHE,MARCO_SIZE,CACHE_X_PROC,retardo);
									confirmacion=0;pthread_mutex_unlock(&controlMemoria);
									send(unData, &confirmacion, sizeof(int),0);
									escribirEnArchivoLog("envio confirmacion", &MemoriaLog,nombreLog);
									}free(bytesAAlmacenar->valor);
									free(bytesAAlmacenar);
	 					break;
	 					case LIBERARMEMORIA: 	
	 											escribirEnArchivoLog("en case liberar memoria", &MemoriaLog,nombreLog);
	 											pidALiberar=calloc(1,sizeof(int));
	 											buffer=calloc(1,sizeof(int));
												memcpy(buffer,&a,sizeof(int));
	 											while(0>recv(unData,pidALiberar,sizeof(int),0));
	 											
	 											pthread_mutex_lock(&controlMemoria);
	 											usleep(retardo*1000);
	 											liberarPaginas(pidALiberar,bloquesAdmin,marcos,MARCOS,overflow,MARCO_SIZE);
	 											pthread_mutex_unlock(&controlMemoria);
	 											free(buffer);free(pidALiberar);
	 							 
	 					break;
	 					case LIBERARPAGINA:
	 								liberarPagina=calloc(1,sizeof(t_liberarPagina));
	 								recibirDinamico(LIBERARPAGINA,unData,liberarPagina);
	 								pthread_mutex_lock(&controlMemoria);
	 								usleep(retardo*1000);
	 								if(-1==existePagina(liberarPagina->pid,liberarPagina->pagina ,bloquesAdmin,MARCOS))
									{	
										pthread_mutex_unlock(&controlMemoria);confirmacion=-1;
										send(unData, &confirmacion, sizeof(int),0);
										escribirEnArchivoLog("envio confirmacion", &MemoriaLog,nombreLog);
										break;
									}
									else{
										pthread_mutex_unlock(&controlMemoria);confirmacion=1;
										send(unData, &confirmacion, sizeof(int),0);
		 								indice=calcularPosicion(liberarPagina->pid,liberarPagina->pagina,MARCOS);
										entrada=buscarEnOverflow(indice,liberarPagina->pid,liberarPagina->pagina,bloquesAdmin,MARCOS,overflow); 
										memset(marcos[entrada].numeroPagina,0,MARCO_SIZE);
										bloquesAdmin[entrada].estado=0;
										bloquesAdmin[entrada].pid=-1;
										bloquesAdmin[entrada].pagina=-1;
										borrarDeOverflow(indice,entrada,overflow);
									}
									free(liberarPagina);

	 					break;
	 				}
			}
 				free(seleccionador);
}//while
//free(*marco);

}	





void consolaMemoria()
{	printf("%s\n", "bienvenido a la consola de memoria");
	int pid;
	int unFrame;
	int unaAdmin;
	char * dumpEntero;
	FILE* dumpLog;
	char * bufferLog;
	int unProceso=0;
	int encontrados=0;
	int unMarco=0;
	int cancelarThread=0;
	char * aEscribir; char* datoAdmin;
	while(cancelarThread==0)
	{ system("clear");
	int * instruccionConsola=malloc(sizeof(int));
	int bloquesOcupados, bloquesLibre;
	printf("%s\n","**********************");
	printf("%s\n","ingrese 0 para modificar el retardo de acceso de memoria" );
	printf("%s\n","**********************");
	printf("%s\n","ingrese 1 para realizar acciones de tipo dump" );
	printf("%s\n","**********************");
	printf("%s\n", "ingrese 2 para realizar un flush de cache");
	printf("%s\n","**********************");
	printf("%s\n", "ingrese 3 para obtener informacion acerca del tamaño");
	printf("%s\n","**********************");

	
	scanf("%d",instruccionConsola);
	switch(*instruccionConsola){

	case DELAY: 		
						system("clear");
						printf("%s\n","Ingrese el nuevo valor de retardo: " );
						getchar();
						scanf("%d",&retardo);
						
							
						retardo=retardo;
						printf("El nuevo retardo ha sido actualizado a: %i %s\n", retardo,"milisegundos" );printf("%s\n", "presione una tecla para volver al menu de la consola");getchar(); getchar();
		break;
	case DUMP:		system("clear");
					fflush(stdout);printf("%s\n", "Para realizar un dump de cache ingrese 0"); 
					fflush(stdout);printf("%s\n", "Para realizar un dump de memoria ingrese 1"); 
					fflush(stdout);printf("%s\n", "Para realizar un dump de administrativas ingrese 2"); 
					
					fflush(stdout);printf("%s\n","para realizar un dump de overflow ingrese 4" );


					scanf("%d",instruccionConsola);//preguntar si en disco quiere decir en filesystem o en disco posta
					switch(*instruccionConsola)
					{
						case CACHE:	
										system("clear");
										
										dumpLog=fopen("cacheDump.bin","wb");
									for(unFrame=0;unFrame<ENTRADAS_CACHE;unFrame++)
									{
										printf("el numero de pagina  es:%i\n",memoriaCache[unFrame].frame);
										printf("el pid del proceso es:%i\n",memoriaCache[unFrame].pid);
										printf("su antiguedad es :%i\n", memoriaCache[unFrame].antiguedad);
										printf("%s\n","el contenido en cache es: " );
										DumpHex(memoriaCache[unFrame].contenido,MARCO_SIZE);
										fwrite(memoriaCache[unFrame].contenido,MARCO_SIZE,1,dumpLog);
										
									}
									
									
									fclose(dumpLog);
									printf("%s\n", "presione una tecla para volver al menu de la consola");getchar();getchar();
									break;
						case MEMORIA:
										system("clear");
										usleep(retardo*1000);
										
										
										dumpLog=fopen("memoriaDump.bin","wb");
										if(dumpLog!=NULL)
										{	printf("dumpLog valor %p\n",dumpLog );perror ("Error opening threshold file");
										for(unMarco=0;unMarco<MARCOS;unMarco++)
										{	
											printf("el numero de frame es: %i\n", unMarco);
											printf("%s\n","el contenido del frame es :");
											DumpHex(marcos[unMarco].numeroPagina,MARCO_SIZE);

											fwrite(marcos[unMarco].numeroPagina,MARCO_SIZE,1,dumpLog);
										
										}
										
									
										fclose(dumpLog);}
										else{perror ("Error opening threshold file");}
										printf("%s\n", "presione una tecla para volver al menu de la consola");getchar();getchar();
										
						break;
						case TABLA:
										system("clear");
										usleep(retardo*1000);
										datoAdmin=calloc(1,9);
										aEscribir=calloc(1,1000);
										dumpLog=fopen("tabla.bin","wb");
										for (unaAdmin= 0; unaAdmin < MARCOS; unaAdmin++)
										{fflush(stdout);printf("%s\n","tabla de paginas");
	
											fflush(stdout);printf("%i\n",bloquesAdmin[unaAdmin].pid);
											fflush(stdout);printf("%i\n",bloquesAdmin[unaAdmin].pagina);
											fflush(stdout);printf("%i\n",bloquesAdmin[unaAdmin].estado);
											
											
											strcpy(aEscribir, "administrativa nummero:");
											sprintf(datoAdmin,"%d",unaAdmin);
											strcat(aEscribir,datoAdmin);
											fseek(dumpLog,0,SEEK_END);
											fwrite(aEscribir,strlen(aEscribir),1,dumpLog);
											fwrite("\n",1,1,dumpLog);

											strcpy(aEscribir, "pid asociado a la admministrativa:");
											sprintf(datoAdmin,"%d",bloquesAdmin[unaAdmin].pid);
											strcat(aEscribir,datoAdmin);
											fseek(dumpLog,0,SEEK_END);
											fwrite(aEscribir,strlen(aEscribir),1,dumpLog);
											fwrite("\n",1,1,dumpLog);

											strcpy(aEscribir, "pagina asociada a la admministrativa:");
											sprintf(datoAdmin,"%d",bloquesAdmin[unaAdmin].pagina);
											strcat(aEscribir,datoAdmin);
											fseek(dumpLog,0,SEEK_END);
											fwrite(aEscribir,strlen(aEscribir),1,dumpLog);
											fwrite("\n",1,1,dumpLog);

											strcpy(aEscribir, "estado asociado a la admministrativa:");
											sprintf(datoAdmin,"%d",bloquesAdmin[unaAdmin].estado);
											strcat(aEscribir,datoAdmin);
											fseek(dumpLog,0,SEEK_END);
											fwrite(aEscribir,strlen(aEscribir),1,dumpLog);
											fwrite("\n",1,1,dumpLog);


										}
										printf("%s\n","lista de procesos activos:" ); 
										for (unProceso = 0; unProceso < MARCOS; unProceso++)
										{	
											if(bloquesAdmin[unProceso].pid != -1)
											{
											fflush(stdout);printf("proceso: %i\n",bloquesAdmin[unProceso].pid);

											/*strcat(aEscribir, "********************Listado de procesos Activos********************");
											
											strcat(aEscribir,datoAdmin);
											fseek(dumpLog,0,SEEK_END);
											fwrite(aEscribir,strlen(aEscribir),1,dumpLog);
											fwrite("\n",1,1,dumpLog);

											strcat(aEscribir, "Proceso:");
											sprintf(datoAdmin,"%d",bloquesAdmin[unProceso].pid);
											strcat(aEscribir,datoAdmin);
											fseek(dumpLog,0,SEEK_END);
											fwrite(aEscribir,strlen(aEscribir),1,dumpLog);
											fwrite("\n",1,1,dumpLog);*/

											}
		
		
										}
											printf("%s\n", "presione una tecla para volver al menu de la consola");getchar();getchar();
											
											fclose(dumpLog);
											free(datoAdmin);
											free(aEscribir);
						break;
						
						case 4 : system("clear");
									generarDumpOverflow(overflow,MARCOS);printf("%s\n", "presione una tecla para volver al menu de la consola");getchar();getchar();
									break;
						default: break;
						
					}
							
							
	
		break;
	case FLUSH:	system("clear");
					
					for(unMarco=0;unMarco<ENTRADAS_CACHE; unMarco++) //asignar su numero de marco a cada region de memoria
					{
					memset(memoriaCache[unMarco].contenido,0,MARCO_SIZE);
					memoriaCache[unMarco].frame=-666; //para distinguir  una pagina real de una virgen
					memoriaCache[unMarco].pid=-1;
					memoriaCache[unMarco].antiguedad=-1;
					}	printf("%s\n","la memoria cache ha sido reinicializada" );	
						printf("%s\n", "presione una tecla para volver al menu de la consola");getchar(); getchar();
		break;
	case SIZE:	system("clear");		fflush(stdout);printf("%s\n", "para conocer el tamaño de la memoria ingrese 0");

								fflush(stdout);printf("%s\n", "para conocer el tamaño de un proceso ingrese 1");getchar();
			 					scanf("%d",instruccionConsola);
			switch(*instruccionConsola)
			{
				case MEMORIASIZE:	system("clear");
								fflush(stdout);printf("el tamaño de la memoria es de  %i bytes\n", MARCOS*MARCO_SIZE);
								bloquesLibre=cantidadBloquesLibres(MARCOS,bloquesAdmin);
								fflush(stdout);printf("cantidad paginas libres: %i\n", bloquesLibre);
								bloquesOcupados=cantidadBloquesOcupados(MARCOS,bloquesAdmin);
								fflush(stdout);printf("cantidad paginas ocupadas: %i\n", bloquesOcupados);
								printf("%s\n", "presione una tecla para volver al menu de la consola");
								fflush(stdout);
								getchar();getchar();
				break;
				case PIDSIZE:system("clear"); printf("%s\n","ingrese un pid" );
				getchar(); scanf("%d",&pid);
				 calcularTamanioProceso(pid,bloquesAdmin,MARCOS,MARCO_SIZE);printf("%s\n", "presione una tecla para volver al menu de la consola"); getchar();getchar();
				break;
				default: pagaraprata();
				break;
			}	
							
		break;

	default:	
	break;
	}
	
	free(instruccionConsola);
	}free(dumpLog);
}




void aceptar(dataParaComunicarse * unData){

int socketNuevaConexion,rv;
pthread_t hiloComunicador;
struct sockaddr_in addr;
int addrlen = sizeof(addr);
if(rv=listen(unData->socket,BACKLOG)==-1) perror("Error en el listen");
while(HAB){
	int * unBuffer=calloc(1,sizeof(int));
	if ((socketNuevaConexion = accept(unData->socket, (struct sockaddr *)&addr,&addrlen)) == -1) perror("Error con conexion entrante");
	else {
		
		handshakeServer(socketNuevaConexion,MEMORIA,(void*)unBuffer);
		
		if(*unBuffer==KERNEL)
			{	tamPagina=MARCO_SIZE;
				printf("MARCO SIZE: %i\n",tamPagina );
				memcpy(unBuffer,&tamPagina, sizeof(int));
				send(socketNuevaConexion, unBuffer,sizeof(int),0);
				escribirEnArchivoLog("envio un buffer", &MemoriaLog,nombreLog);
				socketKernel=socketNuevaConexion;	
			}
			if(*unBuffer==CPU)
			{	tamPagina=MARCO_SIZE;
				printf("MARCO SIZE: %i\n",tamPagina );
				memcpy(unBuffer,&tamPagina, sizeof(int));
				send(socketNuevaConexion, unBuffer,sizeof(int),0);
				escribirEnArchivoLog("envio un buffer", &MemoriaLog,nombreLog);
					
			}
			free(unBuffer);
		pthread_create(&hiloComunicador,NULL,(void *) comunicarse,(void*)&socketNuevaConexion);
	}
} pthread_join(hiloComunicador,NULL);
}

