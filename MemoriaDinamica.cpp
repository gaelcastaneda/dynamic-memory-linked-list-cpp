#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

struct modulo {
    int dato;
    struct modulo *seguimiento;
};

void Alta(struct modulo **, int);
void Baja(struct modulo *, int);
void Medio(struct modulo *, int, int);
void Dimensionar(struct modulo **, int *);
void MenuPrincipal();
void SubMenu();
struct modulo *nuevoModulo(void);
void DibujarLista(HDC hdc, struct modulo *lista);
int posicion = 1, rer = 0, con, con2, num, pos, dim;
int i;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main() {
    struct modulo *lista = NULL;
    posicion = 1, rer = 0, con, con2, num, pos, dim;

    Dimensionar(&lista, &dim);

    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ListaEnlazada";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        "ListaEnlazada",
        "Visualización de Lista Enlazada",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, lista);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd); // Asegura que el contenido se actualice en pantalla

    // Espera 5 segundos antes de cerrar la ventana gráfica
    Sleep(5000);
    SendMessage(hwnd, WM_CLOSE, 0, 0);

    // Menú principal después de cerrar la ventana gráfica
    int opcion;
    do {
        MenuPrincipal();
        printf("\nSeleccione una opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                printf("\nIngrese el valor para agregar a la lista: ");
                int valor;
                scanf("%d", &valor);
                Alta(&lista, valor);
                break;
            case 2:
                hwnd = CreateWindowEx(
                    0,
                    "ListaEnlazada",
                    "Visualización de Lista Enlazada",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                    NULL, NULL, hInstance, lista);
                ShowWindow(hwnd, SW_SHOW);
                UpdateWindow(hwnd); // Asegura que el contenido se actualice en pantalla
                Sleep(5000);
                SendMessage(hwnd, WM_CLOSE, 0, 0);
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida, intente nuevamente.\n");
        }
    } while (opcion != 3);

    free(lista);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static struct modulo *lista;
    
    if (uMsg == WM_CREATE) {
        lista = (struct modulo *)((LPCREATESTRUCT)lParam)->lpCreateParams;
    } else if (uMsg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DibujarLista(hdc, lista);
        EndPaint(hwnd, &ps);
    } else if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void MenuPrincipal() {
    printf("\n\t\t\tMenu\n");
    printf("\n\t1. Altas");
    printf("\n\t2. Ver la lista en modo grafico");
    printf("\n\t3. Salir");
}

void SubMenu() {
    printf("\n\t\t\t¿Dónde desea guardar su dato?\n");
    printf("\n\t1. Al principio");
    printf("\n\t2. En medio");
    printf("\n\t3. Al final");
}

void Alta(struct modulo **lista, int num) {
    int posicion;
    SubMenu();
    printf("\nSeleccione la posición: ");
    scanf("%d", &posicion);

    switch (posicion) {
        case 1:
            // Insertar al principio
            {
                struct modulo *aux = nuevoModulo();
                if (aux != NULL) {
                    aux->dato = num;
                    aux->seguimiento = *lista;
                    *lista = aux;
                } else {
                    printf("No hay suficiente memoria\n");
                }
            }
            break;
        case 2:
            // Insertar en el medio
            {
                int med;
                printf("Ingrese la posición (índice) en la que desea insertar: ");
                scanf("%d", &med);
                Medio(*lista, num, med);
            }
            break;
        case 3:
            // Insertar al final
            Baja(*lista, num);
            break;
        default:
            printf("Posición no válida\n");
            break;
    }
}

void Baja(struct modulo *lista, int num) {
    struct modulo *aux = nuevoModulo();
    aux->dato = num;
    if (aux != NULL) {
        while (lista->seguimiento != NULL) {
            lista = lista->seguimiento;
        }
        lista->seguimiento = aux;
    } else {
        printf("No hay suficiente memoria\n");
    }
}

void Medio(struct modulo *lista, int num, int med) {
    struct modulo *aux1, *aux = nuevoModulo();
    aux->dato = num;
    if (aux != NULL) {
        for (i = 1; i < med; i++) {
            lista = lista->seguimiento;
        }
        aux1 = lista->seguimiento;
        lista->seguimiento = aux;
        aux->seguimiento = aux1;
    } else {
        printf("No hay suficiente memoria\n");
    }
}

struct modulo *nuevoModulo(void) {
    struct modulo *aux = (struct modulo*)malloc(sizeof(struct modulo));
    aux->seguimiento = NULL;
    return aux;
}

void Dimensionar(struct modulo **lista, int *dim) {
    printf("\t\tMEMORIA DINAMICA\t\n\n\nIngrese el numero de elementos de la lista: ");
    scanf("%i", dim);

    int valor;
    for (int i = 0; i < *dim; i++) {
        printf("Ingrese el valor para el elemento %d: ", i + 1);
        scanf("%d", &valor);
        if (*lista == NULL) {
            *lista = nuevoModulo();
            (*lista)->dato = valor;
        } else {
            Baja(*lista, valor);
        }
    }
    system("cls");
}

void DibujarLista(HDC hdc, struct modulo *lista) {
    int x = 50, y = 100; // Posición inicial de los nodos
    struct modulo *aux = lista;
    char buffer[10];
    
    // Texto "Lista" y su flecha al primer nodo
    TextOut(hdc, x - 30, y - 50, "Lista", 5);
    MoveToEx(hdc, x - 10, y - 30, NULL); // Posición inicial de la flecha
    LineTo(hdc, x + 10, y); // Línea de la flecha hacia el primer nodo
    LineTo(hdc, x + 5, y - 5); // Extremo de la flecha (arriba)
    MoveToEx(hdc, x + 10, y, NULL);
    LineTo(hdc, x + 5, y + 5); // Extremo de la flecha (abajo)

    while (aux != NULL) {
        // Dibuja el nodo con un rectángulo
        Rectangle(hdc, x, y, x + 70, y + 50);
        
        // Muestra el valor del nodo
        sprintf(buffer, "%d", aux->dato);
        TextOut(hdc, x + 15, y + 15, buffer, strlen(buffer));

        // Dibuja una flecha hacia el siguiente nodo si existe
        if (aux->seguimiento != NULL) {
            MoveToEx(hdc, x + 70, y + 25, NULL);
            LineTo(hdc, x + 90, y + 25);
            LineTo(hdc, x + 85, y + 20);
            MoveToEx(hdc, x + 90, y + 25, NULL);
            LineTo(hdc, x + 85, y + 30);
        }

        // Avanza la posición para el siguiente nodo
        x += 150;
        aux = aux->seguimiento;
    }

    // Texto "Ultimo" en el último nodo
    TextOut(hdc, x - 100, y + 100, "Ultimo", 6);
}


