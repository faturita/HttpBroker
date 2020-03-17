#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#define DP_PORT 5504
#define TAM_LISTEN 3
#define MAXHOSTNAME 32



	printf ("Esperando conexion de algun cliente que oficiara de fantasma...\n");
	tams=sizeof(remtsa);
	if ( (est[i].sock=accept(sock,(void *)&remtsa,&tams))<0 )
	    {
	    printf ("Se ha producido un error al intentar conectarse al cliente...\n");
	    //printf ("Error devuelto por el kernel:%s",strerror(errno));
	    exit(-1);
	    }


	printf ("Conexion exitosa !!!\n");
	/* Lectura del nick..... */
	tams=read(est[i].sock,est[i].nick,30);
	est[i].nick[tams]='\0';
	est[i].CONTROL=2;

	printf ("Conectado con %s.\n",est[i].nick);

	/* Desbloqueo del read */
	fcntl (est[i].sock,F_SETFL,O_NONBLOCK);


	close(sock);


	/* Archivo de cabecera con funciones para la conexion a un socket. */


int abrirSocketS()
{
struct hostent *hp;

/* Estructura que corresponde al protocolo de TCP/IP.
    Hay dos, porque en una se van a albergar los datos correspondientes al host,
    en tanto que en la otra, se guardan los datos que corresponden al cliente */
struct sockaddr_in hostsa;
struct sockaddr_in remtsa;

/* Descriptor del socket */
int sock;

char hst[MAXHOSTNAME];


printf ("\nConexion como SERVIDOR PRINCIPAL.\n");
/* Lee el nombre del host donde estamos y lo guarda en hst */

gethostname(hst,MAXHOSTNAME);
printf ("Nombre del host:%s\n",hst);


/* Lee el archivo /etc/hosts y devuelve los datos que encuentra alli en una
estructura, segun el nombre que se le especifique en hst */
hp=gethostbyname(hst);

printf ("Datos de la red....\n");
printf ("-------------------\n");
printf ("Nombre host completo:%s\n",hp->h_name);
printf ("Alias :\n");
printf ("Familia :%d\n",hp->h_addrtype);
printf ("Tamanio :%d\n",hp->h_length);
printf ("Direccion :%ld\n",hp->h_addr_list[0]);

/* A partir de 5000 se pueden usar puertos libres que el sistema no tiene
asignado y que tampoco va a asignar.  de 0 a 1024 son solo reservados para
super usuario, y de 1024 hasta 5000 los puede gestionar automaticamente */

hostsa.sin_port=DP_PORT;
printf ("Puerto del servicio: %d\n",hostsa.sin_port);

hostsa.sin_family=AF_INET;

/* AF_INET especifica el protocolo de internet */

bcopy(hp->h_addr,(char *)&hostsa.sin_addr,hp->h_length);

/* bcopy es una funcion que copia cadenas...  origen  destino  cant a copiar.
se guarda en la estructura correspondiente a este host el dato de la direccion
que tiene asignado este host que leimos antes con gethostbyname */


if ((sock=socket(AF_INET,SOCK_STREAM,0))<0)
    {
    printf ("\a\nError en la creacion del socket....\n");
    printf ("Verifique la instalacion y vuelvalo a intentar....\n");
    //printf (strerror(errno));
    exit(-1);
    }
    

/* Se crea el socket.  El 0 es porque en ese campo va el protocolo especifico
que se va a usar, y poniendo 0 se daja para que se encargue el sistema */


/* bind hace la publicacion en la red de los datos que se fueron guardando 
en la estructura hostsa.  Con esto se habilita la posibilidad de que los
demas se puedan conectar */

if (bind(sock,(void *)&hostsa,sizeof (hostsa))<0)
    {
    printf ("\a\nSe produjo un error en el BIND...\n");
    printf ("Verifique que el puerto definido no sea un puerto restrinjido, o que la configuracion de la red es la correcta.\n");
//    printf (strerror(errno));
    exit(-1);
    }
        
/* bind modifica la estructura y coloca una direccion que es la que se le 
asigna a este host dentro de la red  */


//printf ("Datos de HOST....\n");
//printf ("Puerto asignado:%d\n",hostsa.sin_port);    
printf ("Direccion del host:%ld....\n",hostsa.sin_addr);
printf ("Familia de protocolos:%d....\n",hostsa.sin_family);


/* listen marca la cantidad de elementos que va a tener la cola de espera para
la conexion */

listen(sock,TAM_LISTEN);

/* accept acepta una conexion connect (para el caso de SOCK_STREAM). 
lo que se le manda es una estructura del mismo tipo que la del hostsa, donde 
se van a guardar los datos del cliente, como direccion, puerto del proceso 
que esta ejecutando como cliente en el otro nodo, etc. */
/* NOTA:  Este puerto puede no coincidir con el anterior, porque corresponde
al proceso que oficia de cliente en la otra maquina */

/*tams=sizeof(remtsa);
if ( (socki=accept(sock,(void *)&remtsa,&tams))<0 )
    {
    printf ("Se produjo un error en el accept....\n");
    exit(1);
    }

printf ("Conexion exitosa !!!!!!\n\n");
    
printf ("Datos del CLIENTE...\n");
printf ("Puerto asignado:%d\n",remtsa.sin_port);    
printf ("Address:%ld\n",remtsa.sin_addr);
printf ("Familia de protocolos:%d\n",remtsa.sin_family);
*/
printf ("\nSocket abierto correctamente !!!\n");

return sock;
}




int abrirSocketC(struct sockaddr_in *hosts,char hst[MAXHOSTNAME])
{
/* Recibe hostsa que lo llena con los datos del server. */
struct hostent *hp;
struct servent *sp;
struct sockaddr_in remtsa;
struct sockaddr_in hostsa;
int sock;

/* El nombre del host como del puerto se deben especificar al intentar la conexion */

/* O al menos el nombre del host si se va a usar un servicio especifico del host */

/* La direccion del socket remoto la averiguamos del fichero /etc/host    */


hp=gethostbyname(hst);
printf ("Lectura del archivo /etc/hosts.....\n\n");

printf ("Informacion del host especificado.  Host remoto ......\n");
printf ("Nombre host completo:%s\n",hp->h_name);
printf ("Familia :%d\n",hp->h_addrtype);
printf ("Tamanio :%d\n",hp->h_length);
printf ("Direccion :%ld\n",hp->h_addr_list[0]);


hostsa.sin_port=DP_PORT; 
hostsa.sin_family=AF_INET;

bcopy(hp->h_addr,(char *)&hostsa.sin_addr,hp->h_length);

/* Cliente abre el socket */
if ( (sock=socket(hostsa.sin_family,SOCK_STREAM,0))<0 )
    {
    printf ("\nError en la creacion del socket....\n");
    exit(0);
    }
    
printf ("\nSocket abierto satisfactoriamente.....\n");

/*if ( (connect(sock,(void *)&hostsa,sizeof(hostsa)))<0 )
    {
    printf ("Fallo en el connect ....\n");
    exit(1);
    }


printf ("Conexion exitosa !!!!!!\n\n");

printf ("Datos del HOST REMOTO....\n");
printf ("Puerto asignado:%d\n",hostsa.sin_port);
printf ("Address:%ld\n",hostsa.sin_addr);
printf ("Familia de protocolos:%d\n",hostsa.sin_family);
*/


*(hosts)=hostsa;
return sock;
}



