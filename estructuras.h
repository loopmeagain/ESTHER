#ifndef _estructuras_h
#include <parser/metadata_program.h>
#include <parser/parser.h>
#include <stdbool.h>
#define _estructuras_h




typedef struct {
				int ocupado; // 1=ocupado ; 0=libre
				int bytes;
				}__attribute__((packed))
				  hashMemoria;
typedef struct 
				{ 	int frame;
					int pid;
					void * contenido;
					int antiguedad;
					int modificado;
				}t_estructuraCache;
typedef struct { 
				
				void * numeroPagina;
				}__attribute__((packed))
				 t_marco;

typedef struct {
				int fd;
				char * path;
				int tamanioPath;
				int vecesAbierto;
				}__attribute__((packed))
				  t_tablaGlobalArchivos;
typedef struct  {
				int size;
				bool isFree;
				}t_heapMetaData;

typedef struct {
				int pagina;
				int espacioLibre;
				int cantidadMetadatas;
				t_heapMetaData * contenido;
				}__attribute__((packed))
				  t_heapProceso;
typedef struct {
				int pid;
				int cantPaginas;
				t_heapProceso * paginas;
				}__attribute__((packed))
				  t_heapGlobal;

typedef struct {
				t_banderas flags;
				int cursor;
				int globalFd;
				int abierto;
				}__attribute__((packed))
				  t_tablaArchivosDeProcesos;

typedef struct {
				char ** unPrograma; // el programa en si
				int  dimension; // cantidad de lineas
				}__attribute__((packed))
				  t_programa;

typedef struct { 
				char * elPrograma; 
				int tamanio; // bytes del programa
				}__attribute__((packed))
				t_programaSalida;

typedef struct { 
				int tamanio; // bytes del programa
				char * path; 
				}__attribute__((packed))
				t_path;

typedef struct { 
				int tamanio; // bytes del programa
				char * mensaje; 
				}__attribute__((packed))
				t_mensaje;

typedef struct { 
				int socket;
				int interfaz;				
				}__attribute__((packed))
				  dataParaComunicarse;

typedef struct { 
				int etiquetas_size;
				char * etiquetas;			
				}__attribute__((packed))
				  t_etiquetas;

typedef struct { 
				char id;
				int pagina;	
				int offset;	
				int size;				
				}__attribute__((packed))
				  t_argumento;

typedef t_argumento t_variable;

typedef struct {
				int pagina;
				int offset;
				int size;
				}__attribute__((packed))
					t_posMemoria;

typedef struct { 
				int cantidadArgumentos;
				t_argumento * argumentos;
				int cantidadVariables;
				t_variable * variables;
				int posRetorno;
				t_posMemoria varRetorno;		
				}__attribute__((packed))
				  t_stack;


typedef struct {
				int pid;
				int programCounter;
				int estado;
				int rafagasEjecutadas;
				int privilegiadasEjecutadas;
				int paginasHeap;
				int alocaciones;
				int bytesAlocados;
				int liberaciones;
				int bytesLiberados;
				int cantidadArchivos;
				t_tablaArchivosDeProcesos * referenciaATabla;
				int paginasCodigo; // estaba en el TP pero no lo habiamos puesto
				int posicionStack;
				int cantidadInstrucciones;
				t_intructions * indiceCodigo;
				t_etiquetas indiceEtiquetas; 
				int cantidadStack;
				t_stack * indiceStack;
				int exitCode;
				}__attribute__((packed))
				 t_pcb;

typedef struct { 
				
				int pid;
				int pagina;
				int estado;
				}t_estructuraADM;


typedef struct { 
				int cantidad;
				int * pids;
				}__attribute__((packed))
				  t_arrayPids;

typedef struct {
				int pid;
				int paginasAAsignar;
				}__attribute__((packed))
				t_solicitudAsignar;
typedef struct { 
				int unaInterfaz;
				int tipoPaquete;

				}__attribute__((packed))
				  t_seleccionador;
typedef struct {
				
				int tamanioCodigo;
				char * codigo;
				int cantidadPaginasCodigo;
				int cantidadPaginasStack;
				int pid;
				int respuesta; 

				}__attribute__((packed))
				 t_solicitudMemoria;

typedef struct {
				
				int pid;
				int pagina;
				int offset;
				int size; 

				}__attribute__((packed))
				 t_peticionBytes;

typedef struct {
				int pid;
				int pagina;
				}__attribute__((packed))
				t_liberarPagina;

typedef struct {
				
				int pid;
				int pagina;
				int offset;
				int size;
				void * valor; 

				}__attribute__((packed))
				 t_almacenarBytes;

typedef struct {
				
				char * linea;
				int tamanio;

				}__attribute__((packed))
				 t_linea;


typedef struct 
				{
				int pid;
				int programCounter;
				int indiceStack;	
				}__attribute__((packed))
				 t_actualizacion;
typedef struct {
				int pid;
				int resultado; // 0 NO HAY MEMORIA - 1 TODO PIOLA
				}__attribute__((packed))
				  t_resultadoIniciarPrograma;

typedef struct {
				int tamanioIdentificador;
				char * identificadorSemaforo;
				int estadoSemaforo;
				
				}__attribute__((packed))
					t_solicitudSemaforo;

typedef struct {
				int espacio;
				
				}__attribute__((packed))
					t_reservarEspacioMemoria;

typedef struct {
				int direccionMemoria;
				}__attribute__((packed))
					t_liberarMemoria;

typedef struct {
				int tamanio;
				char* direccionArchivo;
				t_banderas flags;
				
				}__attribute__((packed))
					t_abrirArchivo;


typedef struct {
				int fdABorrar;
				
				
				}__attribute__((packed))
					t_borrarArchivo;

typedef struct {
				int descriptorArchivo;
				
			}__attribute__((packed))
					t_cerrarArchivo;

typedef struct {
				int fdArchivo;
				void * informacion;
				int tamanio;
				
				}__attribute__((packed))
					t_escribirArchivo;

typedef struct {
				int descriptor;
				int tamanio;
				
				}__attribute__((packed))
					t_leerArchivo;

typedef struct {
				int descriptorArchivo;
				int posicion;
				}__attribute__((packed))
					t_moverCursor;

typedef struct {
				int tamanioNombre;
				char* variable;
				}__attribute__((packed))
					t_solicitudValorVariable;

typedef struct {
				int tamanioNombre;
				char* variable;
				int valor;
				}__attribute__((packed))
					t_asignarVariableCompartida;

typedef struct {
				int estadoSemaforo;
				}__attribute__((packed))
					t_semaforo;

typedef struct {
				int puntero;
				}__attribute__((packed))
					t_reservar;

typedef struct {
				int fd;
				}__attribute__((packed))
					t_fdParaLeer;

typedef struct {
				int tamanioPath;
				char * path;
				int offset;
				int size;
				void * buffer;
				}__attribute__((packed))
					t_escribirArchivoFS;

typedef struct {
				int tamanioPath;
				char * path;
				int offset;
				int size;
				}__attribute__((packed))
					t_leerArchivoFS;

typedef struct {
				int tamanio;
				void * paquete;
				}__attribute__((packed))
					t_paqueteFS;

typedef struct {
				int tamanioBloques;
				int cantBloques;
				char * magicNumber;
				}__attribute__((packed))
					t_metadataFS;


//funcion que retorna ok/exception al kernel ante el pedido de memoria



#endif