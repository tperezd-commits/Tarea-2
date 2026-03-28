#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> // remove, rename
using namespace std;

struct Alumno {
    int id;
    char nombre[50]; // tamaño fijo para binario
    float nota;
};

// 1. Crear y guardar en archivo de texto
void crearAlumnoTexto() {
    ofstream archivo("alumnos.txt", ios::app);
    Alumno a;
    cout << "ID: "; cin >> a.id;
    cin.ignore();
    cout << "Nombre: "; cin.getline(a.nombre, 50);
    cout << "Nota: "; cin >> a.nota;
    archivo << a.id << "|" << a.nombre << "|" << a.nota << endl;
    archivo.close();
}

// 2. Listar registros en texto
void listarTexto() {
    ifstream archivo("alumnos.txt");
    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
    archivo.close();
}

// 3. Buscar alumno por id en texto
void buscarTexto(int idBusca) {
    ifstream archivo("alumnos.txt");
    string linea;
    bool encontrado = false;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue; // saltar líneas vacías

        size_t p1 = linea.find("|");
        size_t p2 = linea.find("|", p1+1);

        if (p1 == string::npos || p2 == string::npos) {
            cout << "Formato invalido en linea: " << linea << endl;
            continue;
        }

        try {
            int id = stoi(linea.substr(0,p1));
            string nombre = linea.substr(p1+1, p2-p1-1);
            float nota = stof(linea.substr(p2+1));

            if (id == idBusca) {
                cout << "ID: " << id << " | Nombre: " << nombre << " | Nota: " << nota << endl;
                encontrado = true;
            }
        } catch (const exception& e) {
            cout << "Error al procesar linea: " << linea << endl;
        }
    }

    if (!encontrado) cout << "Alumno no encontrado." << endl;
    archivo.close();
}
// 4. Actualizar nota usando archivo temporal
void actualizarNotaTexto(int idBusca, float nuevaNota) {
    ifstream original("alumnos.txt");
    ofstream temporal("temp.txt");
    string linea;
    bool encontrado = false;

    while (getline(original, linea)) {
        size_t p1 = linea.find("|");
        size_t p2 = linea.find("|", p1+1);
        int id = stoi(linea.substr(0,p1));
        string nombre = linea.substr(p1+1, p2-p1-1);
        float nota = stof(linea.substr(p2+1));

        if (id == idBusca) {
            temporal << id << "|" << nombre << "|" << nuevaNota << endl;
            encontrado = true;
        } else {
            temporal << id << "|" << nombre << "|" << nota << endl;
        }
    }
    original.close();
    temporal.close();
    remove("alumnos.txt");
    rename("temp.txt","alumnos.txt");

    if (encontrado) cout << "Nota actualizada con exito." << endl;
    else cout << "Alumno no encontrado." << endl;
}

// 5. Crear archivo binario
void crearAlumnoBinario() {
    ofstream archivo("alumnos.dat", ios::binary | ios::app);
    Alumno a;
    cout << "ID: "; cin >> a.id;
    cin.ignore();
    cout << "Nombre: "; cin.getline(a.nombre, 50);
    cout << "Nota: "; cin >> a.nota;
    archivo.write(reinterpret_cast<char*>(&a), sizeof(Alumno));
    archivo.close();
}

// 6. Leer registro específico (acceso aleatorio)
void leerRegistroBinario(int pos) {
    ifstream archivo("alumnos.dat", ios::binary);
    Alumno a;
    archivo.seekg(pos * sizeof(Alumno));
    archivo.read(reinterpret_cast<char*>(&a), sizeof(Alumno));
    cout << "ID: " << a.id << " | Nombre: " << a.nombre << " | Nota: " << a.nota << endl;
    archivo.close();
}

// 7. Recorrer archivo binario
void listarBinario() {
    ifstream archivo("alumnos.dat", ios::binary);
    Alumno a;
    while (archivo.read(reinterpret_cast<char*>(&a), sizeof(Alumno))) {
        cout << "ID: " << a.id << " | Nombre: " << a.nombre << " | Nota: " << a.nota << endl;
    }
    archivo.close();
}

// Menú principal
int main() {
    int opcion;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Crear alumno (texto)\n";
        cout << "2. Listar alumnos (texto)\n";
        cout << "3. Buscar alumno por ID (texto)\n";
        cout << "4. Actualizar nota (texto)\n";
        cout << "5. Crear alumno (binario)\n";
        cout << "6. Leer registro especifico (binario)\n";
        cout << "7. Listar alumnos (binario)\n";
        cout << "0. Salir\n";
        cout << "Opcion: "; cin >> opcion;

        switch(opcion) {
            case 1: crearAlumnoTexto(); break;
            case 2: listarTexto(); break;
            case 3: {int id; cout<<"ID a buscar: ";cin>>id; buscarTexto(id);} break;
            case 4: {int id; float nota; cout<<"ID: ";cin>>id; cout<<"Nueva nota: ";cin>>nota; actualizarNotaTexto(id,nota);} break;
            case 5: crearAlumnoBinario(); break;
            case 6: {int pos; cout<<"Posicion (0=primero): ";cin>>pos; leerRegistroBinario(pos);} break;
            case 7: listarBinario(); break;
        }
    } while(opcion!=0);
    return 0;
}
