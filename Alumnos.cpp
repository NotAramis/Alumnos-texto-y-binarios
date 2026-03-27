#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <limits> // Necesario para limpiar el buffer de forma segura

using namespace std;

// 1. Estructura para Texto puede ser flexible (string para nombre)
struct Alumno {
    int id;
    string nombre;
    float nota;
};

// 2. Estructura para Binario debe ser fija (char[] para nombre)
struct AlumnoBinario {
    int id;
    char nombre[50];
    float nota;
};

// modulo 1: ARCHIVOS DE TEXTO

void crearAlumno() {
    ofstream archivo("alumnos.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de texto.\n";
        return;
    }

    Alumno a;
    cout << "Ingrese ID: ";
    cin >> a.id;
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia el buffer en VS para evitar problemas con getline

    cout << "Ingrese Nombre: ";
    getline(cin, a.nombre);

    cout << "Ingrese Nota: ";
    cin >> a.nota;

    archivo << a.id << " " << a.nombre << " " << a.nota << endl;
    archivo.close();
    cout << "-> Registro de texto guardado exitosamente.\n";
}

void listarAlumnos() {
    ifstream archivo("alumnos.txt");
    if (!archivo.is_open()) {
        cout << "El archivo de texto aun no existe o esta vacio.\n";
        return;
    }

    Alumno a;
    cout << "\n--- LISTA DE ALUMNOS (TEXTO) ---\n";
    while (archivo >> a.id >> a.nombre >> a.nota) {
        cout << "ID: " << a.id << "\tNombre: " << a.nombre << "\tNota: " << a.nota << endl;
    }
    archivo.close();
}

void buscarAlumno() {
    ifstream archivo("alumnos.txt");
    if (!archivo.is_open()) {
        cout << "El archivo no existe.\n";
        return;
    }

    int idBusqueda;
    bool encontrado = false;
    Alumno a;

    cout << "Ingrese el ID a buscar: ";
    cin >> idBusqueda;

    while (archivo >> a.id >> a.nombre >> a.nota) {
        if (a.id == idBusqueda) {
            cout << "\n-> Alumno Encontrado:\n";
            cout << "Nombre: " << a.nombre << " | Nota: " << a.nota << endl;
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout << "-> No se encontro ningun alumno con ese ID.\n";
    archivo.close();
}

void actualizarNota() {
    ifstream archivo("alumnos.txt");
    ofstream temporal("temporal.txt");

    if (!archivo.is_open() || !temporal.is_open()) {
        cout << "Error al procesar los archivos.\n";
        return;
    }

    int idBusqueda;
    float nuevaNota;
    bool encontrado = false;
    Alumno a;

    cout << "Ingrese el ID del alumno a actualizar: ";
    cin >> idBusqueda;

    while (archivo >> a.id >> a.nombre >> a.nota) {
        if (a.id == idBusqueda) {
            cout << "Alumno: " << a.nombre << " (Nota actual: " << a.nota << ")\n";
            cout << "Ingrese la NUEVA nota: ";
            cin >> nuevaNota;

			// Escribimos el dato modificado en el temporal y el resto sin cambios
            temporal << a.id << " " << a.nombre << " " << nuevaNota << endl;
            encontrado = true;
        }
        else {
			// Copiamos el dato intacto al temporal si no es el que queremos modificar
            temporal << a.id << " " << a.nombre << " " << a.nota << endl;
        }
    }

    archivo.close();
    temporal.close();

	// Proceso de sustitucion del archivo original por el temporal modificado
    if (encontrado) {
        remove("alumnos.txt");
        rename("temporal.txt", "alumnos.txt");
        cout << "-> Nota actualizada correctamente.\n";
    }
    else {
        remove("temporal.txt");
        cout << "-> Error: ID no encontrado en el sistema.\n";
    }
}
// modulo 2: ARCHIVOS BINARIOS

void crearArchivoBinario() {
    ofstream archivo("alumnos.dat", ios::binary | ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo binario.\n";
        return;
    }

    AlumnoBinario ab;
    cout << "Ingrese ID: ";
    cin >> ab.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese Nombre (Max 50 caracteres): ";
    cin.getline(ab.nombre, 50);

    cout << "Ingrese Nota: ";
    cin >> ab.nota;

	// la magia del binario: inyectamos la memoria directamente en el archivo sin preocuparnos por el formato
    archivo.write(reinterpret_cast<char*>(&ab), sizeof(AlumnoBinario));
    archivo.close();
    cout << "-> Registro binario guardado exitosamente.\n";
}

void leerRegistroAleatorio() {
    ifstream archivo("alumnos.dat", ios::binary);
    if (!archivo.is_open()) {
        cout << "El archivo binario no existe.\n";
        return;
    }

    int n;
    cout << "Ingrese el numero de registro (1, 2, 3...): ";
    cin >> n;

    if (n <= 0) {
        cout << "El registro debe ser un numero positivo.\n";
        return;
    }

	// Calculo del offset matematico para posicionar el puntero de lectura en el registro deseado
    streampos posicion = (n - 1) * sizeof(AlumnoBinario);
    archivo.seekg(posicion, ios::beg);

    AlumnoBinario ab;
    if (archivo.read(reinterpret_cast<char*>(&ab), sizeof(AlumnoBinario))) {
        cout << "\n--- DATOS DEL REGISTRO " << n << " ---\n";
        cout << "ID: " << ab.id << "\nNombre: " << ab.nombre << "\nNota: " << ab.nota << endl;
    }
    else {
        cout << "-> Error: El registro esta vacio o fuera de rango.\n";
    }
    archivo.close();
}

void recorrerArchivoBinario() {
    ifstream archivo("alumnos.dat", ios::binary);
    if (!archivo.is_open()) {
        cout << "El archivo binario no existe.\n";
        return;
    }

    AlumnoBinario ab;
    cout << "\n--- LISTADO COMPLETO (BINARIO) ---\n";
    while (archivo.read(reinterpret_cast<char*>(&ab), sizeof(AlumnoBinario))) {
        cout << "ID: " << ab.id << "\tNombre: " << ab.nombre << "\tNota: " << ab.nota << endl;
    }
    archivo.close();
}

// funciones principales dentro del main para mostrar el menu y manejar las opciones del usuario

int main() {
    int opcion;
    do {
        cout << "      SISTEMA DE GESTION DE ALUMNOS     \n";
        cout << "archivos de texto\n";
        cout << "1. Agregar Alumno\n";
        cout << "2. Listar Todos\n";
        cout << "3. Buscar por ID\n";
        cout << "4. Actualizar Nota\n";
        cout << "archivos binarios\n";
        cout << "5. Guardar Nuevo Registro\n";
        cout << "6. Leer por Acceso Aleatorio\n";
        cout << "7. Mostrar Todo el Archivo\n";
        cout << "0. Salir del Programa\n";
        cout << "Seleccione una opcion: ";

		if (!(cin >> opcion)) { // Validacion si el usuario ingresa una letra en vez de un numero
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
        }

        cout << "\n";
        switch (opcion) {
        case 1: crearAlumno(); break;
        case 2: listarAlumnos(); break;
        case 3: buscarAlumno(); break;
        case 4: actualizarNota(); break;
        case 5: crearArchivoBinario(); break;
        case 6: leerRegistroAleatorio(); break;
        case 7: recorrerArchivoBinario(); break;
        case 0: cout << "Saliendo del sistema...\n"; break;
        default: cout << "Opcion no valida. Intente de nuevo.\n"; break;
        }
    } while (opcion != 0);

	system("pause"); // Evita que la consola de Visual Studio se cierre de golpe al terminar el programa, permitiendo al usuario ver los mensajes finales antes de salir.
    return 0;
}