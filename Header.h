#pragma once
#include <string>
#include <stdio.h>
#include <limits.h>
#include <vector>

using namespace std;

#pragma warning(disable: 4996)

// Para almacenar la información leida del archivo de datos del grafo
struct dato_de_un_enlace_del_grafo
{
	string nodo1 = ""; // Primer nodo
	string nodo2 = ""; // Segundo nodo
	float costo_nodo1 = 0.0; // Costo del nodo 1
	float costo_nodo2 = 0.0; // Costo del nodo 2
	float costo = 0.0; // Costo del arco
	float x_nodo1 = 0.0; // Coordenada x del nodo uno
	float y_nodo1 = 0.0; // Coordenada y del nodo uno
	float x_nodo2 = 0.0; // Coordenada x del nodo dos
	float y_nodo2 = 0.0; // Coordenada y del nodo dos
};

#define MAX_DATOS 85 // Cantidad de enlaces en el archivo de texto del grafo

#define MAX_HIJOS 8

struct nodo_de_arbol // Nodo del arbol
{
	string nombre = "";
	unsigned int padre = 0;
	unsigned int hijos[MAX_HIJOS] = { 0 };
	unsigned int cantidad_real_de_hijos = 0;
	float costo_acumulado = 0;
	float costo_acumulado_con_heuristica = 0;
};

#define MAX_ELEMENTOS_EN_EL_ARBOL 1000
nodo_de_arbol Arbol[MAX_ELEMENTOS_EN_EL_ARBOL];
unsigned int cantidad_real_del_arbol;

dato_de_un_enlace_del_grafo grafo[MAX_DATOS]; // Estos son los datos de mi grafo que lei
unsigned int cantidad_real_de_enlaces_del_grafo = 0; // La cantidad maxima es MAX_DATOS pero esto es lo que realmente tiene

#define MAX_AGENDA 1000
unsigned int Agenda[MAX_AGENDA];
unsigned int cantidad_real_en_agenda;

void crea_agenda(unsigned int Agenda[],
	unsigned int& cantidad_real_en_agenda)
{
	cantidad_real_en_agenda = 0;
}


bool agregar_al_final_de_la_agenda_un_nodo(unsigned int nodo_a_agregar,
	unsigned int Agenda[],
	unsigned int& cantidad_real_en_agenda)
{
	if (cantidad_real_en_agenda == MAX_AGENDA - 1)
		return false;
	cantidad_real_en_agenda++;
	Agenda[cantidad_real_en_agenda] = nodo_a_agregar;
	return true;
}

float devuelve_heuristica(string nodo_actual, string meta,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int& cantidad_real_datos)
{
	float x1 = 0.0;
	float y1 = 0.0;
	for (unsigned int i = 1; i <= cantidad_real_datos; i++)
	{
		if (nodo_actual.compare(datos[i].nodo1) == 0)
		{
			x1 = datos[i].x_nodo1;
			y1 = datos[i].y_nodo1;
			break;
		}
		else if (nodo_actual.compare(datos[i].nodo2) == 0)
		{
			x1 = datos[i].x_nodo2;
			y1 = datos[i].y_nodo2;
			break;
		}
	}
	float x2 = 0.0;
	float y2 = 0.0;
	for (unsigned int i = 1; i <= cantidad_real_datos; i++)
	{
		if (meta.compare(datos[i].nodo1) == 0)
		{
			x2 = datos[i].x_nodo1;
			y2 = datos[i].y_nodo1;
			break;
		}
		else if (meta.compare(datos[i].nodo2) == 0)
		{
			x2 = datos[i].x_nodo2;
			y2 = datos[i].y_nodo2;
			break;
		}
	}
	return sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
}

bool agregar_al_inicio_de_la_agenda_un_nodo(unsigned int nodo_a_agregar,
	unsigned int Agenda[],
	unsigned int& cantidad_real_en_agenda)
{
	if (cantidad_real_en_agenda == MAX_AGENDA - 1)
		return false;
	if (cantidad_real_en_agenda == 0)
	{
		Agenda[1] = nodo_a_agregar;
		cantidad_real_en_agenda = 1;
		return true;
	}
	for (unsigned int i = cantidad_real_en_agenda; i >= 1; i--)
		Agenda[i + 1] = Agenda[i];
	cantidad_real_en_agenda++;
	Agenda[1] = nodo_a_agregar;
	return true;
}

bool saca_el_primer_nodo_de_la_agenda(unsigned int Agenda[],
	unsigned int& cantidad_real_en_agenda,
	unsigned int& nodo_sacado)
{
	if (cantidad_real_en_agenda == 0)
		return false;
	nodo_sacado = Agenda[1];
	for (unsigned int i = 1; i <= cantidad_real_en_agenda - 1; i++)
		Agenda[i] = Agenda[i + 1];
	cantidad_real_en_agenda--;
	return true;
}


void crea_arbol(string nodo_raiz,
	nodo_de_arbol Arbol[],
	unsigned int& cantidad_real_del_arbol)
{
	cantidad_real_del_arbol = 1;
	Arbol[1].nombre = nodo_raiz;
	Arbol[1].cantidad_real_de_hijos = 0;
	Arbol[1].padre = 0;
}

float devuelve_costo_de_padre_a_hijo(string hijo,
	string padre,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int cantidad_real_datos)
{
	for (unsigned int i = 1; i <= cantidad_real_datos; i++)
	{
		if (hijo.compare(datos[i].nodo1) == 0 && padre.compare(datos[i].nodo2) == 0)
			return datos[i].costo;
		if (hijo.compare(datos[i].nodo2) == 0 && padre.compare(datos[i].nodo1) == 0)
			return datos[i].costo;

	}
}

float devuelve_costo_del_nodo_hijo(string hijo,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int cantidad_real_datos)
{
	for (unsigned int i = 1; i <= cantidad_real_datos; i++)
	{
		if (hijo.compare(datos[i].nodo1) == 0)
			return datos[i].costo_nodo1;
		if (hijo.compare(datos[i].nodo2) == 0)
			return datos[i].costo_nodo2;

	}
}

bool anade_un_hijo_a_un_padre(string nombre_del_hijo,
	unsigned int padre,
	nodo_de_arbol Arbol[],
	unsigned int& cantidad_real_del_arbol,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int cantidad_real_datos,
	string meta)
{
	if (cantidad_real_del_arbol == MAX_ELEMENTOS_EN_EL_ARBOL - 1)
		return false;
	cantidad_real_del_arbol++;
	Arbol[cantidad_real_del_arbol].nombre = nombre_del_hijo;
	Arbol[cantidad_real_del_arbol].costo_acumulado = Arbol[padre].costo_acumulado +
		devuelve_costo_de_padre_a_hijo(nombre_del_hijo, Arbol[padre].nombre, datos, cantidad_real_datos) +
		devuelve_costo_del_nodo_hijo(nombre_del_hijo, datos, cantidad_real_datos);
	Arbol[cantidad_real_del_arbol].costo_acumulado_con_heuristica = Arbol[cantidad_real_del_arbol].costo_acumulado +
		devuelve_heuristica(nombre_del_hijo, meta, datos, cantidad_real_datos);
	Arbol[cantidad_real_del_arbol].padre = padre;
	Arbol[cantidad_real_del_arbol].cantidad_real_de_hijos = 0;
	Arbol[padre].cantidad_real_de_hijos++;
	Arbol[padre].hijos[Arbol[padre].cantidad_real_de_hijos] = cantidad_real_del_arbol;
}


// Esta función lee los datos del archivo del grafo
// Hay que dar la carpeta donde está el archivo
// Que devuelve esos datos leidos para el programa
bool lee_datos(string nombre_archivo, dato_de_un_enlace_del_grafo datos[],
	unsigned int& cantidad_real_datos)
{
	FILE* manipulador;
	if (!(manipulador = fopen(nombre_archivo.c_str(), "rt"))) // abrir un archivo, rt r que es para lectura
		return(false);						// t que es un archivo de texto
	cantidad_real_datos = 0;
	unsigned int leidos;
	char temporal_nodo1[100] = "";
	char temporal_nodo2[100] = "";
	while (!feof(manipulador)) // feof devuelve False si no esta en el fin delrchi y True en caso contrario
	{
		cantidad_real_datos++;
		leidos = fscanf(manipulador, "%s %f %s %f %f %f %f %f %f",
			temporal_nodo1,
			&(datos[cantidad_real_datos].costo_nodo1),
			temporal_nodo2,
			&(datos[cantidad_real_datos].costo_nodo2),
			&(datos[cantidad_real_datos].costo),
			&(datos[cantidad_real_datos].x_nodo1),
			&(datos[cantidad_real_datos].y_nodo1),
			&(datos[cantidad_real_datos].x_nodo2),
			&(datos[cantidad_real_datos].y_nodo2));
		datos[cantidad_real_datos].nodo1 = temporal_nodo1;
		datos[cantidad_real_datos].nodo2 = temporal_nodo2;
		if (leidos != 9)
		{
			fclose(manipulador);
			return(false);
		}
	}
	fclose(manipulador);
	return(true);
}



void devuelve_vecinos_de_un_nodo_grafo_no_dirigido(string nodo_para_ver_vecinos,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int cantidad_real_datos,
	string lista_de_vecinos[],
	unsigned int& cantidad_de_vecinos)
{
	cantidad_de_vecinos = 0;
	for (unsigned int i = 1; i <= cantidad_real_datos; i++)
	{
		if (nodo_para_ver_vecinos.compare(datos[i].nodo1) == 0)
		{
			cantidad_de_vecinos++;
			lista_de_vecinos[cantidad_de_vecinos] = datos[i].nodo2;
		}
		else if (nodo_para_ver_vecinos.compare(datos[i].nodo2) == 0)
		{
			cantidad_de_vecinos++;
			lista_de_vecinos[cantidad_de_vecinos] = datos[i].nodo1;
		}
	}
}

bool es_ancestro(unsigned int nodo_actual, string vecino, nodo_de_arbol Arbol[], unsigned int cantidad_real_del_arbol)
{
	// Si vecino es ancestro de nodo_actual devuelve true sino devuelve false
	nodo_de_arbol padre;
	padre = Arbol[nodo_actual];
	while (padre.padre != 0)
	{
		if (padre.nombre.compare(vecino) == 0)
			return true;
		padre = Arbol[padre.padre];
	}
	if (vecino.compare(padre.nombre) == 0)
		return true;
	else
		return false;
}

void elimina_de_vecinos_ancestros(unsigned int nodo_actual, string lista_de_vecinos[], unsigned int& cantidad_de_vecinos, nodo_de_arbol Arbol[], unsigned int cantidad_real_del_arbol)
{
	// Eliminar de la lista de vecinos a todos los ancestros
	for (unsigned int i = 1; i <= cantidad_de_vecinos; i++)
	{
		if (es_ancestro(nodo_actual, lista_de_vecinos[i], Arbol, cantidad_real_del_arbol))
		{
			for (unsigned int j = i; j <= cantidad_de_vecinos - 1; j++)
				lista_de_vecinos[j] = lista_de_vecinos[j + 1];
			cantidad_de_vecinos--;
			i--;
		}
	}
}



bool encuentra_camino_a_lo_profundo(string nodo_inicio, string nodo_destino,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int& cantidad_real_datos,
	string& camino)
{

	unsigned int Agenda[MAX_AGENDA] = { 0 };
	unsigned int cantidad_real_en_agenda;
	nodo_de_arbol Arbol[MAX_ELEMENTOS_EN_EL_ARBOL];
	unsigned int cantidad_real_del_arbol;
	string lista_de_vecinos[MAX_DATOS];
	unsigned int cantidad_de_vecinos;
	crea_agenda(Agenda, cantidad_real_en_agenda);
	crea_arbol(nodo_inicio, Arbol, cantidad_real_del_arbol);
	agregar_al_inicio_de_la_agenda_un_nodo(1, Agenda, cantidad_real_en_agenda);
	unsigned int nodo_actual = 1;
	bool camino_encontrado = false;
	unsigned int nodo_donde_se_encontro_camino = 0;


	do
	{
		saca_el_primer_nodo_de_la_agenda(Agenda, cantidad_real_en_agenda, nodo_actual);
		if (Arbol[nodo_actual].nombre.compare(nodo_destino) == 0)
		{
			nodo_donde_se_encontro_camino = nodo_actual;
			camino_encontrado = true;
			break;

		}
		devuelve_vecinos_de_un_nodo_grafo_no_dirigido(Arbol[nodo_actual].nombre, datos, cantidad_real_datos,
			lista_de_vecinos, cantidad_de_vecinos);
		elimina_de_vecinos_ancestros(nodo_actual, lista_de_vecinos, cantidad_de_vecinos, Arbol, cantidad_real_del_arbol);
		if (cantidad_de_vecinos != 0)
			for (unsigned int j = 1; j <= cantidad_de_vecinos; j++)
			{
				anade_un_hijo_a_un_padre(lista_de_vecinos[j], nodo_actual,
					Arbol, cantidad_real_del_arbol, datos,
					cantidad_real_datos, nodo_destino);
				agregar_al_inicio_de_la_agenda_un_nodo(cantidad_real_del_arbol, Agenda, cantidad_real_en_agenda);
			}
		else if (cantidad_real_en_agenda == 0)
			break;
	} while (camino_encontrado != true);
	if (camino_encontrado)
	{
		camino = Arbol[nodo_donde_se_encontro_camino].nombre + " ";
		do
		{
			nodo_donde_se_encontro_camino = Arbol[nodo_donde_se_encontro_camino].padre;
			camino = Arbol[nodo_donde_se_encontro_camino].nombre + " " + camino;
		} while (Arbol[nodo_donde_se_encontro_camino].padre != 0);
		return true;
	}
	return false;
}

bool encuentra_camino_a_lo_ancho(string nodo_inicio, string nodo_destino,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int& cantidad_real_datos,
	string& camino)
{

	unsigned int Agenda[MAX_AGENDA] = { 0 };
	unsigned int cantidad_real_en_agenda;
	nodo_de_arbol Arbol[MAX_ELEMENTOS_EN_EL_ARBOL];
	unsigned int cantidad_real_del_arbol;
	string lista_de_vecinos[MAX_DATOS];
	unsigned int cantidad_de_vecinos;
	crea_agenda(Agenda, cantidad_real_en_agenda);
	crea_arbol(nodo_inicio, Arbol, cantidad_real_del_arbol);
	agregar_al_inicio_de_la_agenda_un_nodo(1, Agenda, cantidad_real_en_agenda);
	unsigned int nodo_actual = 1;
	bool camino_encontrado = false;
	unsigned int nodo_donde_se_encontro_camino = 0;


	do
	{
		saca_el_primer_nodo_de_la_agenda(Agenda, cantidad_real_en_agenda, nodo_actual);
		if (Arbol[nodo_actual].nombre.compare(nodo_destino) == 0)
		{
			nodo_donde_se_encontro_camino = nodo_actual;
			camino_encontrado = true;
			break;
		}
		devuelve_vecinos_de_un_nodo_grafo_no_dirigido(Arbol[nodo_actual].nombre, datos, cantidad_real_datos,
			lista_de_vecinos, cantidad_de_vecinos);
		elimina_de_vecinos_ancestros(nodo_actual, lista_de_vecinos, cantidad_de_vecinos, Arbol, cantidad_real_del_arbol);
		if (cantidad_de_vecinos != 0)
			for (unsigned int j = 1; j <= cantidad_de_vecinos; j++)
			{
				anade_un_hijo_a_un_padre(lista_de_vecinos[j], nodo_actual,
					Arbol, cantidad_real_del_arbol, datos,
					cantidad_real_datos, nodo_destino);
				agregar_al_final_de_la_agenda_un_nodo(cantidad_real_del_arbol, Agenda, cantidad_real_en_agenda);
			}
		else if (cantidad_real_en_agenda == 0)
			break;
	} while (camino_encontrado != true);
	if (camino_encontrado)
	{
		camino = Arbol[nodo_donde_se_encontro_camino].nombre + " ";
		do
		{
			nodo_donde_se_encontro_camino = Arbol[nodo_donde_se_encontro_camino].padre;
			camino = Arbol[nodo_donde_se_encontro_camino].nombre + " " + camino;
		} while (Arbol[nodo_donde_se_encontro_camino].padre != 0);
		return true;
	}
	return false;
}



bool encuentra_mejor_camino(string nodo_inicio, string nodo_destino,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int& cantidad_real_datos,
	string& camino)
{

	unsigned int Agenda[MAX_AGENDA] = { 0 };
	unsigned int cantidad_real_en_agenda;
	nodo_de_arbol Arbol[MAX_ELEMENTOS_EN_EL_ARBOL];
	unsigned int cantidad_real_del_arbol;
	string lista_de_vecinos[MAX_DATOS];
	unsigned int cantidad_de_vecinos;
	crea_agenda(Agenda, cantidad_real_en_agenda);
	crea_arbol(nodo_inicio, Arbol, cantidad_real_del_arbol);
	agregar_al_inicio_de_la_agenda_un_nodo(1, Agenda, cantidad_real_en_agenda);
	unsigned int nodo_actual = 1;
	bool camino_encontrado = false;
	unsigned int nodo_donde_se_encontro_camino = 0;
	float costo_del_camino_encontrado = FLT_MAX;

	do
	{
		if (cantidad_real_en_agenda == 0)
		{
			camino = "";
			return false;
		}
		saca_el_primer_nodo_de_la_agenda(Agenda, cantidad_real_en_agenda, nodo_actual);
		if (Arbol[nodo_actual].nombre.compare(nodo_destino) == 0)
			if (Arbol[nodo_actual].costo_acumulado < costo_del_camino_encontrado)
			{
				costo_del_camino_encontrado = Arbol[nodo_actual].costo_acumulado;
				nodo_donde_se_encontro_camino = nodo_actual;
				camino_encontrado = true;
				// Eliminando los nodos con costos mayores o igual al costo del nodo encontrado
				for (unsigned int i = 1; i <= cantidad_real_en_agenda; i++)
					if (Arbol[Agenda[i]].costo_acumulado >= costo_del_camino_encontrado)
					{
						for (unsigned j = i; j <= cantidad_real_en_agenda - 1; j++)
							Agenda[j] = Agenda[j + 1];
						cantidad_real_en_agenda--;
						i--;
					}
				if (cantidad_real_en_agenda == 0)
				{
					camino_encontrado = true;
					break;
				}
				else
					saca_el_primer_nodo_de_la_agenda(Agenda, cantidad_real_en_agenda, nodo_actual);
			}
		devuelve_vecinos_de_un_nodo_grafo_no_dirigido(Arbol[nodo_actual].nombre, datos, cantidad_real_datos,
			lista_de_vecinos, cantidad_de_vecinos);
		elimina_de_vecinos_ancestros(nodo_actual, lista_de_vecinos, cantidad_de_vecinos, Arbol, cantidad_real_del_arbol);
		if (cantidad_de_vecinos != 0)
			for (unsigned int j = 1; j <= cantidad_de_vecinos; j++)
			{

				anade_un_hijo_a_un_padre(lista_de_vecinos[j], nodo_actual,
					Arbol, cantidad_real_del_arbol, datos,
					cantidad_real_datos, nodo_destino);
				agregar_al_final_de_la_agenda_un_nodo(cantidad_real_del_arbol, Agenda, cantidad_real_en_agenda);
			}
		else if (cantidad_real_en_agenda == 0)
			break;

	} while (true);
	if (camino_encontrado)
	{
		camino = Arbol[nodo_donde_se_encontro_camino].nombre + " ";
		do
		{
			nodo_donde_se_encontro_camino = Arbol[nodo_donde_se_encontro_camino].padre;
			camino = Arbol[nodo_donde_se_encontro_camino].nombre + " " + camino;
		} while (Arbol[nodo_donde_se_encontro_camino].padre != 0);
		return true;
	}
}

void devuelve_datos_de_un_nodo_grafo_no_dirigido(string nodo_para_ver_info,
	dato_de_un_enlace_del_grafo datos[],
	unsigned int cantidad_real_datos,
	float& posx,
	float& posy)
{
	for (unsigned int i = 1; i <= cantidad_real_datos; i++)
	{
		if (nodo_para_ver_info.compare(datos[i].nodo1) == 0)
		{
			posx = datos[i].x_nodo1;
			posy = datos[i].y_nodo1;
			return;
		}
		else if (nodo_para_ver_info.compare(datos[i].nodo2) == 0)
		{
			posx = datos[i].x_nodo2;
			posy = datos[i].y_nodo2;
			return;
		}
	}
}

void calcula_distancia_y_angulo(float pos_x_nodo_1, float pos_y_nodo_1,
	float pos_x_nodo_2, float pos_y_nodo_2,
	float& distancia,
	float& angulo) {
	distancia = sqrt(pow(pos_x_nodo_1 - pos_x_nodo_2, 2) + pow(pos_y_nodo_1 - pos_y_nodo_2, 2));
	angulo = atan2(pos_y_nodo_2 - pos_y_nodo_1, pos_x_nodo_2 - pos_x_nodo_1);
}
float calcula_distancia(float pos_x_nodo_1, float pos_y_nodo_1,
	float pos_x_nodo_2, float pos_y_nodo_2,
	float& distancia,
	float& angulo) {
	return sqrt(pow(pos_x_nodo_1 - pos_x_nodo_2, 2) + pow(pos_y_nodo_1 - pos_y_nodo_2, 2));
}
float calcula_angulo(float pos_x_nodo_1, float pos_y_nodo_1,
	float pos_x_nodo_2, float pos_y_nodo_2,
	float& distancia,
	float& angulo) {
	return angulo = atan2(pos_y_nodo_2 - pos_y_nodo_1, pos_x_nodo_2 - pos_x_nodo_1);
}