#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

#define ARCHIVO_NOMBRES "zonas_nombres.txt"
#define ARCHIVO_DATOS "zonas_datos.txt"
#define ARCHIVO_PROMEDIOS "zonas_promedios.txt"
#define ARCHIVO_PREDICCIONES "zonas_predicciones.txt"

int main() {
    Zona zonas[MAX_ZONAS];
    int numZonas = 0;
    int opcion;

    leerDatos(zonas, &numZonas, ARCHIVO_NOMBRES, ARCHIVO_DATOS);

    do {
        printf("\n--- Sistema de Gestion de Contaminacion del Aire ---\n");
        printf("1. Monitorear contaminacion actual\n");
        printf("2. Agregar zona\n");
        printf("3. Eliminar zona\n");
        printf("4. Predecir niveles futuros\n");
        printf("5. Generar alertas preventivas\n");
        printf("6. Calcular promedios historicos\n");
        printf("7. Generar recomendaciones\n");
        printf("8. Exportar datos\n");
        printf("9. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); 

        switch (opcion) {
case 1:
    printf("\n--- Monitoreo de contaminación actual ---\n");
    monitorearContaminacion(zonas, &numZonas, ARCHIVO_DATOS);
    break;
            case 2:
                cargarDatos(zonas, &numZonas, ARCHIVO_NOMBRES, ARCHIVO_DATOS);
                break;
            case 3:
                eliminarZona(zonas, &numZonas, ARCHIVO_DATOS);
                break;
            case 4:
predecirContaminacion(zonas, numZonas, ARCHIVO_DATOS, ARCHIVO_PREDICCIONES);
                break;
            case 5:
                generarAlertas(zonas, numZonas);
                break;
            case 6:
                calcularPromedios(zonas, numZonas, ARCHIVO_DATOS, ARCHIVO_PROMEDIOS);
                break;
            case 7:
                generarRecomendaciones(zonas, numZonas);
                break;
            case 8:
                exportarDatos(zonas, numZonas, ARCHIVO_DATOS);
                printf("✅ Datos exportados correctamente.\n");
                break;
            case 9:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (opcion != 9);

    return 0;
}