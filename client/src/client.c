#include "client.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	int size;
	char* ip;
	char* puerto;
	char* valor;
	char* mensaje;
	op_code cod_oper;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger,"Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip=config_get_string_value (config,"IP");
	puerto=config_get_string_value (config,"PUERTO");
	valor=config_get_string_value (config,"CLAVE");

	log_info(logger,"IP: %s, PUERTO: %s, VALOR: %s\n",ip,puerto,valor);



	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	if (conexion==-1)
	{
		log_info(logger,"No se pudo conectar al servidor");
		terminar_programa(conexion, logger, config);
		exit (3);
	}

	if (conexion==-2)
	{
		log_info(logger,"Error Handshake");
		terminar_programa(conexion, logger, config);
		exit (4);
	}
	log_info(logger,"Conexion exitosa al servidor");

	// Enviamos al servidor el valor de CLAVE como mensaje

	cod_oper=MENSAJE;

	send(conexion, &cod_oper, sizeof(op_code), NULL);

	log_info(logger,"Se envio el codigo OP, %d/n",cod_oper);

	size = sizeof("No entiendo nada")+1;
	mensaje = malloc (size);
	mensaje = "No entiendo nada";
	send(conexion, &size, sizeof(int), NULL);

	send(conexion, mensaje, size, NULL);



	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	if((nuevo_logger=log_create("tp0.log","Cliente",1,LOG_LEVEL_TRACE))==NULL)
	{
		printf ("No logger\n");
		exit (1);
	}


	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if((nuevo_config=config_create("./cliente.config"))==NULL)
	{
		printf ("No config\n");
		exit (2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (1) {
		leido = readline(">");
	    if (leido) {
	    	add_history(leido);
	    }
	    if (!strncmp(leido, "exit", 4)) {
	        free(leido);
	        break;
	    }

	    if (!strncmp(leido, "man readline", 8)) {
	    	printf("readline returns the text of the line read.  A blank line returns the empty string.  If EOF is encountered while reading a line, and the line is empty, NULL is returned.  If an EOF is read with a non-empty line, it is treated as a newline.");
	        free(leido);
	    }

	    //Comando (leido);
	    printf("%s\n", leido);
	    free(leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!

}

void Comando (char *comando)
{
	char ** comandos;
	char * token = strtok(comando, " ");

	while( token != NULL ) {
	   printf( " %s\n", token ); //printing each token
	   token = strtok(NULL, " ");
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if (logger!=NULL)
		log_destroy(logger);
	if (config!=NULL)
		config_destroy(logger);
	if (conexion!=-1)
		close (conexion);

}
