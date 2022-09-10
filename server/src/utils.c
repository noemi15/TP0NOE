#include"utils.h"

t_log* logger;

int iniciar_servidor(void)
{

	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	// Asociamos el socket a un puerto
	if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen)==-1)
	{
		return -1;
	}
	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{

	// Aceptamos un nuevo cliente
	int socket_cliente;

	uint32_t handshake;
	uint32_t result;

	char * buffer;

	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof (struct sockaddr_in);

	socket_cliente = accept(socket_servidor, (void * ) &dir_cliente, &tam_direccion);

	if (socket_cliente < 0) return -1;

	// Handshake

	//buffer = (char *)&handshake;

	recv(socket_servidor,  &handshake, sizeof(uint32_t), MSG_WAITALL);

	printf("Recibimos:%d\n",handshake);

	//handshake = ntohl (handshake);

	//printf("Recibimos:%d\n",handshake);

	if(handshake == 55)
		result=10;
	else
		result=11;

	printf("Enviamos:%d\n",result);
	//result = htons (result);
	//printf("Enviamos:%d\n",result);
	//buffer = (char *)&result;
	send(socket_servidor, &result, sizeof(uint32_t), 0);

	if (result == 0) return -2;

	///////////////////

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
	{
		log_info(logger, "Codigo recibido: %s/n",cod_op);
		return cod_op;
	}
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
