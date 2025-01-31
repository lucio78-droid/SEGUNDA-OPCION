#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"

#define MAX_ZONAS 100
//FUNCION SOLO PARA VALIDAR NUMEROS POSITIVOS
int validarEnteroPositivo(int num) {
    return num > 0;
}
//FUNCION PARA SOLO VALIDAR LETRAS
int validarSoloLetras(const char* cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isalpha(cadena[i]) && cadena[i] != ' ') {
            return 0;
        }
    }
    return 1;
}
//FUNCION PARA LA VALIDACION DE IDS EXISTENTES
int idExiste(Zona zonas[], int numZonas, int id) {
    for (int i = 0; i < numZonas; i++) {
        if (zonas[i].id == id) {
            return 1;
        }
    }
    return 0; // ID único
}
//FUNCION PARA CARGAR LOS DATOS Y AUTOGURADADO EN EL DOCUMENTO TXT
void cargarDatos(Zona zonas[], int *numZonas, const char* archivoNombres, const char* archivoDatos) {
    if (*numZonas >= MAX_ZONAS) {
        printf("Error: Se ha alcanzado el maximo de zonas permitidas (%d).\n", MAX_ZONAS);
        return;
    }

    Zona nuevaZona;
    do {
        printf("Ingrese un ID para la zona (solo enteros positivos y unico): ");
        scanf("%d", &nuevaZona.id);
        if (idExiste(zonas, *numZonas, nuevaZona.id)) {
            printf("❌ Error: El ID ingresado ya está en uso. Intente con otro.\n");
        }
    } while (!validarEnteroPositivo(nuevaZona.id) || idExiste(zonas, *numZonas, nuevaZona.id));
    getchar();

    do {
        printf("Nombre de la zona: ");
        fgets(nuevaZona.nombreZona, sizeof(nuevaZona.nombreZona), stdin);
        nuevaZona.nombreZona[strcspn(nuevaZona.nombreZona, "\n")] = 0;
    } while (!validarSoloLetras(nuevaZona.nombreZona));

    do {
        printf("Nivel de CO2 (ppm): ");
        scanf("%f", &nuevaZona.CO2);
    } while (nuevaZona.CO2 < 0);

    do {
        printf("Nivel de SO2 (ppm): ");
        scanf("%f", &nuevaZona.SO2);
    } while (nuevaZona.SO2 < 0);

    do {
        printf("Nivel de NO2 (ppm): ");
        scanf("%f", &nuevaZona.NO2);
    } while (nuevaZona.NO2 < 0);

    do {
        printf("Nivel de PM2.5 (µg/m³): ");
        scanf("%f", &nuevaZona.PM2_5);
    } while (nuevaZona.PM2_5 < 0);

    zonas[*numZonas] = nuevaZona;
    (*numZonas)++;
    exportarDatos(zonas, *numZonas, archivoDatos);
}
//FUNCION PARA LEER LOS DATOS Y QUE ESTE TODO BIEN.
void leerDatos(Zona zonas[], int *numZonas, const char* archivoNombres, const char* archivoDatos) {
    FILE *file = fopen(archivoDatos, "r");
    if (!file) {
        printf("No se encontro el archivo de datos. Iniciando sin datos previos.\n");
        return;
    }
    *numZonas = 0;
    while (fscanf(file, "%d %99s %f %f %f %f", &zonas[*numZonas].id, zonas[*numZonas].nombreZona,
                  &zonas[*numZonas].CO2, &zonas[*numZonas].SO2, &zonas[*numZonas].NO2, &zonas[*numZonas].PM2_5) == 6) {
        (*numZonas)++;
        if (*numZonas >= MAX_ZONAS) break;
    }
    fclose(file);
}
//FUNCION PARA MONITORIAR LA CONTAMINACION ACTUAL
void monitorearContaminacion(Zona zonas[], int *numZonas, const char* archivoDatos) {
    FILE *file = fopen(archivoDatos, "r");
    if (!file) {
        printf("❌ Error: No se pudo abrir el archivo de datos.\n");
        return;
    }

    *numZonas = 0;
    int idTemp;
    char nombreTemp[100];
    float CO2Temp, SO2Temp, NO2Temp, PM2_5Temp;
    while (fscanf(file, "%d %99s %f %f %f %f", &idTemp, nombreTemp, &CO2Temp, &SO2Temp, &NO2Temp, &PM2_5Temp) == 6) {
        zonas[*numZonas].id = idTemp;
        strcpy(zonas[*numZonas].nombreZona, nombreTemp);
        zonas[*numZonas].CO2 = CO2Temp;
        zonas[*numZonas].SO2 = SO2Temp;
        zonas[*numZonas].NO2 = NO2Temp;
        zonas[*numZonas].PM2_5 = PM2_5Temp;
        (*numZonas)++;

        if (*numZonas >= MAX_ZONAS) break;
    }
    fclose(file);

    if (*numZonas == 0) {
        printf("❌ No hay datos registrados para mostrar.\n");
        return;
    }

    printf("\n--- Monitoreo de Contaminacion Actual ---\n");
    printf("%-5s %-20s %-10s %-10s %-10s %-10s\n", 
           "ID", "Zona", "CO2 (ppm)", "SO2 (ppm)", "NO2 (ppm)", "PM2.5 (µg/m³)");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < *numZonas; i++) {
        printf("%-5d %-20s %-10.2f %-10.2f %-10.2f %-10.2f\n", 
               zonas[i].id, zonas[i].nombreZona, zonas[i].CO2, 
               zonas[i].SO2, zonas[i].NO2, zonas[i].PM2_5);
    }

    printf("✅ Datos mostrados correctamente.\n");
}
//FUNCION PARA CALCULAR LOS PROMEDIOS HISTORICOS
void calcularPromedios(Zona zonas[], int numZonas, const char* archivoDatos, const char* archivoPromedios) {
    FILE *file = fopen(archivoPromedios, "w");
    if (!file) {
        printf("❌ Error al abrir archivo de promedios.\n");
        return;
    }

    printf("\n--- Promedios Historicos de Contaminacion ---\n");
    printf("%-5s %-20s %-10s\n", "ID", "Zona", "Promedio");
    printf("--------------------------------------------\n");

    for (int i = 0; i < numZonas; i++) {
        float promedio = (zonas[i].CO2 + zonas[i].SO2 + zonas[i].NO2 + zonas[i].PM2_5) / 4.0;
        fprintf(file, "%d %.2f\n", zonas[i].id, promedio);
        printf("%-5d %-20s %-10.2f\n", zonas[i].id, zonas[i].nombreZona, promedio);
    }
    fclose(file);
    printf("✅ Promedios calculados correctamente.\n");
}
//FUNCION PARA GENERAR RECOMENDACIONES CLIMATICAS.
void generarRecomendaciones(Zona zonas[], int numZonas) {
    printf("\n--- Recomendaciones para Mitigar la Contaminacion ---\n");
    int hayRecomendaciones = 0;
    for (int i = 0; i < numZonas; i++) {
        if (zonas[i].PM2_5 > 25) {
            printf("Zona: %s (ID: %d)\n", zonas[i].nombreZona, zonas[i].id);
            printf("  - Reducir el trafico vehicular y promover transporte publico.\n");
            printf("  - Fomentar el uso de mascarillas en exteriores.\n");
            hayRecomendaciones = 1;
        }
    }
    if (!hayRecomendaciones) {
        printf("✅ No hay recomendaciones para este caso.\n");
    }
}
//FUNCION DE PREDICCION DE LA CONTAMINACION
void predecirContaminacion(Zona zonas[], int numZonas, const char* archivoDatos, const char* archivoPredicciones) {
    FILE *filePred = fopen(archivoPredicciones, "w");
    if (!filePred) {
        printf("Error al abrir el archivo de predicciones.\n");
        return;
    }
    printf("\n--- Predicciones de Contaminacion ---\n");
    
    for (int i = 0; i < numZonas; i++) {
        float predPM25 = zonas[i].PM2_5 * 1.1; 
        printf("Zona %s - PM2.5 Predicho: %.2f µg/m³\n", zonas[i].nombreZona, predPM25);
        fprintf(filePred, "%d %.2f\n", zonas[i].id, predPM25);
    }
    
    fclose(filePred);
}
//FUNCION PARA GENERAR ALERTAS PREVENTIVAS
void generarAlertas(Zona zonas[], int numZonas) {
    printf("\n--- Alertas Preventivas ---\n");
    int hayAlertas = 0;

    for (int i = 0; i < numZonas; i++) {
        if (zonas[i].PM2_5 > 25) {
            printf("⚠️ Alerta: Zona %s (ID: %d) tiene un nivel alto de PM2.5 (%.2f µg/m³).\n", 
                   zonas[i].nombreZona, zonas[i].id, zonas[i].PM2_5);
            hayAlertas = 1;
        }
    }

    if (!hayAlertas) {
        printf("✅ No hay alertas activas. Todos los niveles están dentro del rango seguro.\n");
    }
}
//FUNCION PARA ELIMINAR ZONAS INECESARIAS
void eliminarZona(Zona zonas[], int *numZonas, const char* archivoDatos) {
    if (*numZonas == 0) {
        printf("No hay zonas registradas para eliminar.\n");
        return;
    }
    int idEliminar;
    printf("Ingrese el ID de la zona que desea eliminar: ");
    scanf("%d", &idEliminar);
    int index = -1;
    for (int i = 0; i < *numZonas; i++) {
        if (zonas[i].id == idEliminar) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("No se encontro una zona con el ID %d.\n", idEliminar);
        return;
    }
    for (int i = index; i < *numZonas - 1; i++) {
        zonas[i] = zonas[i + 1];
    }
    (*numZonas)--;
}
//FUNCION PARA EXPORTACION DE DATOS A LOS ARCHIVOS TXT.
void exportarDatos(Zona zonas[], int numZonas, const char* archivoDatos) {
    FILE *file = fopen(archivoDatos, "w");
    if (!file) {
        printf("Error al abrir el archivo para exportar datos.\n");
        return;
    }
    for (int i = 0; i < numZonas; i++) {
        fprintf(file, "%d %s %.2f %.2f %.2f %.2f\n", zonas[i].id, zonas[i].nombreZona,
                zonas[i].CO2, zonas[i].SO2, zonas[i].NO2, zonas[i].PM2_5);
    }
    fclose(file);
    printf("✅ Datos exportados correctamente.\n");
}
