/*
   Assignment V - Searching, Sorting and Recursion (Blue Team)
   Exercise 2: MOVIE LOG

   System to log the movies a user has watched: title, duration in
   minutes and genre. It covers the four requirements of the
   exercise: registering movies (Movie class), sorting them by
   duration (bubble sort), searching by title (linear search, case
   insensitive) and adding up the total time watched with a
   RECURSIVE function.

   The log always keeps the movies in registration order; the
   sorting option works on a copy, so it shows the ordered view
   without altering the list. As extras, a movie can be deleted by
   its number in the list and the full log can be displayed. All
   input is validated, including a clean exit if the input stream
   is closed (EOF).
*/

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

const string LINE  = string(50, '-');
const string DLINE = string(50, '=');

// =================================================================
//  MOVIE CLASS
//  Models a movie with its three attributes: title, duration
//  (in minutes) and genre, plus a fixed registration number that
//  works as a unique ID (shown in the search detail card).
// =================================================================
class Movie
{
public:
    string title;
    int    duration;   // in minutes
    string genre;
    int    regNum;     // fixed registration number (1, 2, 3, ...)

    // Empty constructor (needed to create the array)
    Movie()
    {
        title    = "";
        duration = 0;
        genre    = "";
        regNum   = 0;
    }

    Movie(string t, int d, string g, int r)
    {
        title    = t;
        duration = d;
        genre    = g;
        regNum   = r;
    }
};


// =================================================================
//  MOVIELOG CLASS
//  Manages the set of movies using an array. The sorting, the
//  searching and the recursion all live here.
// =================================================================
class MovieLog
{
private:
    static const int MAX = 100;   // Maximum capacity of the log
    Movie movies[MAX];            // array of movies
    int count;                    // how many movies are registered
    int nextReg;                  // next registration number; it only
                                  // grows, so numbers are never reused
                                  // (like invoice numbers).

    // Converts a text to uppercase to compare ignoring case.
    string toUpper(string text)
    {
        for (int i = 0; i < (int)text.length(); i++)
            text[i] = toupper((unsigned char)text[i]);
        return text;
    }

    // RECURSIVE function that adds up the durations from a given
    // position to the end of the array.
    //   Base case: when the index reaches the count there are no
    //              more movies to add, so it returns 0.
    //   Recursive case: the duration of the current movie plus the
    //              time of the remaining movies.
    int sumTime(int index)
    {
        if (index == count)             // base case
            return 0;
        return movies[index].duration   // current movie
             + sumTime(index + 1);      // plus the rest (recursion)
    }

    // Prints any list of movies with the standard table format.
    // Shared by show() and sortByDuration().
    void printList(Movie list[], int n)
    {
        cout << LINE << endl;
        cout << " No.  TITULO                   DURACION  GENERO" << endl;
        cout << LINE << endl;
        for (int i = 0; i < n; i++)
        {
            cout << " ";
            cout.width(5); cout << left << (to_string(i + 1) + ".");
            cout.width(24); cout << left << list[i].title;
            cout << " ";
            cout.width(10); cout << list[i].duration;
            cout << list[i].genre << endl;
        }
        cout << LINE << endl;
        cout << " Total de peliculas registradas: " << n << endl;
    }

public:
    MovieLog()
    {
        count = 0;
        nextReg = 1;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    bool isFull()
    {
        return count == MAX;
    }

    int countMovies()
    {
        return count;
    }

    // a) Register a movie
    // The new movie takes its fixed registration number from nextReg
    // (count + 1 would repeat numbers after a deletion).
    void registerMovie(string title, int duration, string genre)
    {
        movies[count] = Movie(title, duration, genre, nextReg);
        count++;
        nextReg++;
    }

    // b) Sort by duration (bubble sort)
    // Neighboring movies are compared and swapped until everything
    // goes from shortest to longest. It works on a COPY, so the log
    // keeps its registration order.
    void sortByDuration()
    {
        // Copy of the log to sort without altering the original
        Movie sorted[MAX];
        for (int i = 0; i < count; i++)
            sorted[i] = movies[i];

        // Bubble sort over the copy
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - 1 - i; j++)
            {
                if (sorted[j].duration > sorted[j + 1].duration)
                {
                    Movie temp    = sorted[j];
                    sorted[j]     = sorted[j + 1];
                    sorted[j + 1] = temp;
                }
            }
        }

        printList(sorted, count);
    }

    // c) Search a movie by title (linear search)
    // Goes through the array comparing titles; returns the position
    // where it was found, or -1 if it does not exist.
    int searchByTitle(string title)
    {
        string target = toUpper(title);
        for (int i = 0; i < count; i++)
        {
            if (toUpper(movies[i].title) == target)
                return i;       // found at position i
        }
        return -1;              // not found
    }

    // d) Total time watched (recursively)
    // Public function that fires the recursion from position 0.
    int totalTime()
    {
        return sumTime(0);
    }

    // Extra: delete a movie by its position in the list
    // Receives the position shown in the "No." column (1 to count)
    // and shifts the elements after it one place to the left.
    // Returns true if the position was valid.
    bool removeAt(int pos)
    {
        if (pos < 1 || pos > count)
            return false;          // position out of range

        for (int i = pos - 1; i < count - 1; i++)
            movies[i] = movies[i + 1];

        count--;
        return true;
    }

    // Display the full list
    // Shows the movies in REGISTRATION order, the order the array
    // always keeps.
    void show()
    {
        printList(movies, count);
    }

    // Displays the data of a single movie (used by the search)
    void showOne(int i)
    {
        cout << "\n   Titulo  : " << movies[i].title << endl;
        cout << "   Duracion: " << movies[i].duration << " minutos" << endl;
        cout << "   Genero  : " << movies[i].genre << endl;
        cout << "   Registro: No. " << movies[i].regNum << endl;
    }
};


// =================================================================
//  INPUT HELPER FUNCTIONS (with validation)
// =================================================================

// Reads an integer, validating that the user does not type letters.
// If the input stream is closed (EOF) it returns -1 so the caller
// can end in an orderly way.
int readInt(string message)
{
    int value;
    while (true)
    {
        cout << message;
        cin >> value;
        if (cin.fail())
        {
            // If the input was closed (EOF) there is nothing left to read.
            if (cin.eof())
                return -1;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Entrada invalida. Escriba un numero." << endl;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Reads a full line of text (allows titles with spaces) and does
// not accept empty fields. If the input is closed (EOF) it returns
// "0", which the program interprets as cancel.
string readText(string message)
{
    string text;
    cout << message;
    while (true)
    {
        if (!getline(cin, text))
            return "0";              // input closed: cancel
        if (!text.empty())
            return text;
        cout << " El campo no puede quedar vacio. Intente de nuevo." << endl;
        cout << message;
    }
}


// =================================================================
//  MAIN PROGRAM
// =================================================================
int main()
{
    MovieLog log;

    // Preloaded data to make testing easier
    log.registerMovie("El Padrino", 175, "Drama");
    log.registerMovie("Buscando a Nemo", 100, "Animacion");
    log.registerMovie("Titanic", 195, "Romance");
    log.registerMovie("Toy Story", 81, "Animacion");
    log.registerMovie("Gladiador", 155, "Accion");

    int option = -1;

    do
    {
        cout << "\n" << DLINE << endl;
        cout << "        REGISTRO DE PELICULAS (MOVIE LOG)" << endl;
        cout << DLINE << endl;
        cout << "   Peliculas registradas: " << log.countMovies() << endl;
        cout << LINE << endl;
        cout << "   [1] Registrar una pelicula" << endl;
        cout << "   [2] Eliminar una pelicula" << endl;
        cout << "   [3] Ordenar peliculas por duracion" << endl;
        cout << "   [4] Buscar una pelicula por titulo" << endl;
        cout << "   [5] Calcular el tiempo total visto (recursivo)" << endl;
        cout << "   [6] Mostrar todas las peliculas" << endl;
        cout << "   [0] Salir" << endl;
        cout << DLINE << endl;

        option = readInt(" Que desea hacer? ");

        // If the input was closed (EOF), exit in an orderly way.
        if (option == -1 && cin.eof())
        {
            cout << "\n Saliendo del sistema... Hasta luego." << endl;
            break;
        }

        switch (option)
        {
            // 1) Register
            case 1:
            {
                cout << "\n >> REGISTRAR UNA PELICULA" << endl;
                cout << LINE << endl;
                if (log.isFull())
                {
                    cout << "\n El registro esta lleno. No se pueden agregar mas." << endl;
                    break;
                }
                cout << endl;
                string title = readText(" Titulo (0 para volver al menu): ");
                if (title == "0")
                {
                    cout << "\n Registro cancelado. Volviendo al menu." << endl;
                    break;
                }
                int duration;
                do
                {
                    duration = readInt(" Duracion (minutos): ");
                    if (duration == -1 && cin.eof())
                    {
                        cout << "\n Registro cancelado. Volviendo al menu." << endl;
                        break;
                    }
                    if (duration <= 0)
                        cout << " La duracion debe ser mayor que 0." << endl;
                } while (duration <= 0);
                if (duration <= 0)   // canceled because of EOF
                    break;

                string genre = readText(" Genero  : ");
                if (genre == "0")
                {
                    cout << "\n Registro cancelado. Volviendo al menu." << endl;
                    break;
                }

                log.registerMovie(title, duration, genre);
                cout << "\n Pelicula registrada correctamente." << endl;
                break;
            }

            // 2) Delete a movie
            case 2:
            {
                cout << "\n >> ELIMINAR UNA PELICULA" << endl;
                if (log.isEmpty())
                {
                    cout << LINE << endl;
                    cout << "\n No hay peliculas registradas." << endl;
                    break;
                }

                log.show();
                cout << endl;
                int pos = readInt(" No. de la pelicula a eliminar (0 para volver al menu): ");

                if (pos == 0 || (pos == -1 && cin.eof()))
                {
                    cout << "\n Volviendo al menu." << endl;
                    break;
                }

                if (log.removeAt(pos))
                    cout << "\n Pelicula eliminada del registro." << endl;
                else
                    cout << "\n Ese numero no esta en la lista." << endl;
                break;
            }

            // 3) Sort by duration
            case 3:
            {
                cout << "\n >> ORDENAR POR DURACION" << endl;
                if (log.isEmpty())
                {
                    cout << LINE << endl;
                    cout << "\n No hay peliculas registradas." << endl;
                    break;
                }
                log.sortByDuration();
                cout << "\n Vista ordenada de la mas corta a la mas larga." << endl;
                break;
            }

            // 4) Search by title
            case 4:
            {
                cout << "\n >> BUSCAR UNA PELICULA" << endl;
                cout << LINE << endl;
                if (log.isEmpty())
                {
                    cout << "\n No hay peliculas registradas." << endl;
                    break;
                }
                cout << endl;

                string title = readText(" Titulo a buscar (0 para volver al menu): ");
                if (title == "0")
                {
                    cout << "\n Volviendo al menu." << endl;
                    break;
                }
                int pos = log.searchByTitle(title);
                if (pos == -1)
                    cout << "\n No se encontro ninguna pelicula con ese titulo." << endl;
                else
                {
                    cout << "\n Pelicula encontrada en la posicion " << pos + 1 << ":";
                    log.showOne(pos);
                }
                break;
            }

            // 5) Total time (recursive)
            case 5:
            {
                cout << "\n >> TIEMPO TOTAL VIENDO PELICULAS" << endl;
                cout << LINE << endl;
                if (log.isEmpty())
                {
                    cout << "\n No hay peliculas registradas." << endl;
                    break;
                }
                int total = log.totalTime();
                int hours = total / 60;
                int minutes = total % 60;
                cout << "\n En total ha visto " << total << " minutos de cine." << endl;
                cout << " Eso equivale a " << hours << " horas y "
                     << minutes << " minutos." << endl;
                break;
            }

            // 6) Show all
            case 6:
            {
                cout << "\n >> LISTADO COMPLETO" << endl;
                if (log.isEmpty())
                {
                    cout << LINE << endl;
                    cout << "\n No hay peliculas registradas." << endl;
                    break;
                }
                log.show();
                break;
            }

            // 0) Exit
            case 0:
                cout << "\n Saliendo del sistema... Hasta luego." << endl;
                break;

            default:
                cout << "\n Opcion no valida. Intente de nuevo." << endl;
        }

    } while (option != 0);

    return 0;
}


/*
   ANALISIS DE COMPLEJIDAD ALGORITMICA (Big O)

   Aqui n es la cantidad de peliculas registradas. Las funciones de
   una sola operacion directa son O(1), las que recorren el arreglo
   una vez son O(n) y el ordenamiento con sus dos ciclos anidados
   es O(n^2).

   | Funcion             | Complejidad | Por que                                |
   |---------------------|-------------|----------------------------------------|
   | isEmpty / isFull    | O(1)        | Solo comparan el contador.             |
   | countMovies         | O(1)        | Devuelve el contador ya guardado.      |
   | registerMovie       | O(1)        | Inserta al final usando el contador.   |
   | toUpper             | O(1)        | Depende del titulo, no de n.           |
   | searchByTitle       | O(n)        | Recorre el arreglo comparando titulos. |
   | removeAt            | O(n)        | Desplaza los elementos posteriores.    |
   | sumTime (recursiva) | O(n)        | Una llamada por cada pelicula.         |
   | totalTime           | O(n)        | Dispara la recursion sobre las n.      |
   | show / printList    | O(n)        | Recorren la lista para imprimirla.     |
   | sortByDuration      | O(n^2)      | Copia O(n) + burbuja O(n^2);           |
   |                     |             | domina el termino cuadratico.          |

   En general, la operacion dominante del programa es el ordenamiento
   O(n^2); frente a ella, las demas se vuelven despreciables al crecer
   n. Como el menu se repite, si el usuario realiza k operaciones el
   tiempo total queda en O(k * n^2).
*/