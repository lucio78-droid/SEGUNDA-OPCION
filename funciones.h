#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_ZONAS 100

// Estructura para almacenar los datos de una zona
typedef struct {
    int id;
    char nombreZona[100];
    float CO2, SO2, NO2, PM2_5;
} Zona;

// Declaraciones de funciones
int validarSoloLetras(const char* cadena);
void cargarDatos(Zona zonas[], int *numZonas, const char* archivoNombres, const char* archivoDatos);
void leerDatos(Zona zonas[], int *numZonas, const char* archivoNombres, const char* archivoDatos);
void eliminarZona(Zona zonas[], int *numZonas, const char* archivoDatos);
void monitorearContaminacion(Zona zonas[], int *numZonas, const char* archivoDatos);
void predecirContaminacion(Zona zonas[], int numZonas, const char* archivoDatos, const char* archivoPredicciones);
void calcularPromedios(Zona zonas[], int numZonas, const char* archivoDatos, const char* archivoPromedios);
void generarAlertas(Zona zonas[], int numZonas);
void generarRecomendaciones(Zona zonas[], int numZonas);
void exportarDatos(Zona zonas[], int numZonas, const char* archivoDatos);

#endif // FUNCIONES_H
