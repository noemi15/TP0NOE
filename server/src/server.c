#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	if (cliente_fd==-1)
	{
		log_info(logger,"No se pudo conectar al cliente");
		terminar_programa(server_fd,cliente_fd, logger);
		exit (1);
	}

	if (cliente_fd==-2)
	{
		log_info(logger,"Error Handshake");
		terminar_programa(server_fd,cliente_fd, logger);
		exit (2);
	}
	log_info(logger,"Conexión exitosa al cliente");

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	terminar_programa(server_fd,cliente_fd, logger);
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(int conexion,int conexion2, t_log* logger)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if (logger!=NULL)
		log_destroy(logger);
	if (conexion!=-1)
		close (conexion);
	if (conexion2!=-1)
			close (conexion);

}

