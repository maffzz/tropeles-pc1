
#include <iostream>
using namespace std;

class Tropel {
protected:
    string nombre;
    // iniciando felicidad en 0 [0-100]
    int felicidad = 0;
public:
    // constructor con nombre de 3 letras aleatorio
    Tropel() {
        string nombreAleatorio;
        for (int i = 0; i < 3; i++) {
            nombreAleatorio += 'a' + (rand() % 26);}
        this->nombre = nombreAleatorio;}
    // destructor virtual
    virtual ~Tropel() = default;
    // getters y setters
    string getNombre() const {
        return nombre;}
    int getFelicidad() const {
        return felicidad;}
    void setFelicidad(int fel) {
        felicidad = fel;}
    // metodo para disminuir felicidad
    void disminuirFelicidad(int cantidad) {
        felicidad -= cantidad;
        if (felicidad < 0) {
            felicidad = 0;}}
    // metodos virtuales puros
    virtual Tropel* clonar() const = 0;
    virtual void alimentar(int alimento) = 0;
    virtual void limpiar(int limpieza) = 0;};

class TropelAlegre : public Tropel {
public:
    TropelAlegre() : Tropel() {}
    TropelAlegre(const TropelAlegre& otro) : Tropel() {
        felicidad = otro.felicidad;}
    Tropel* clonar() const override {
        return new TropelAlegre(*this);}
    void alimentar(int alimento) override {
        if (alimento > 5) {
            felicidad += 25;}
        cout << "ñam :3 ";}
    void limpiar(int limpieza) override {
        if (limpieza > 5) {
            felicidad += 50;}
        cout << "fresh :3 ";}};
class TropelGloton : public Tropel {
public:
    TropelGloton() : Tropel() {}
    TropelGloton(const TropelGloton& otro) : Tropel() {
        felicidad = otro.felicidad;}
    Tropel* clonar() const override {
        return new TropelGloton(*this);}
    void alimentar(int alimento) override {
        if (alimento > 8) {
            felicidad += 20;}
        cout << "ñam :3 ";}
    void limpiar(int limpieza) override {
        if (limpieza > 5) {
            felicidad += 50;}
        cout << "fresh :3 ";}};
class TropelExigente : public Tropel {
public:
    TropelExigente() : Tropel() {}
    TropelExigente(const TropelExigente& otro) : Tropel() {
        felicidad = otro.felicidad;}
    Tropel* clonar() const override {
        return new TropelExigente(*this);}
    void alimentar(int alimento) override {
        if (alimento > 5) {
            felicidad += 25;}
        cout << "ñam :3 ";}
    void limpiar(int limpieza) override {
        if (limpieza > 8) {
            felicidad += 35;}
        cout << "fresh :3 ";}};

template<typename T = Tropel>
class Poblacion {
private:
    vector<T*> poblacion;
    size_t capacidad;
    void inicializar(size_t) {}
    template<typename G, typename... Args>
    void inicializar(size_t index, G* primerito, Args*... args) {
        if (index < capacidad) {
            poblacion[index] = primerito->clonar();
            inicializar(index + 1, args...);}}
public:
    ~Poblacion() {
        for (auto tropel : poblacion) {
            delete tropel;}}
    template<typename... Args>
    Poblacion(size_t cap, Args*... args) : capacidad(cap) {
        poblacion.resize(capacidad, nullptr);
        inicializar(0, args...);}
    // sobrecarga del operador []
    T* operator[](size_t i) const {
        if (i >= capacidad) {
            throw out_of_range("índice fuera de rango");}
        return poblacion[i];}
    // sobrecarga del operador ++ (aumentar la capacidad en 1)
    Poblacion& operator++() {
        capacidad++;
        poblacion.push_back(nullptr);
        for (auto& tropel : poblacion) {
            if (tropel != nullptr) {
                tropel->disminuirFelicidad(20);}}
        return *this;}
    // función para trasladar tropeles
    string trasladar(int i, int j) {
        if (i >= capacidad || j >= capacidad || i < 0 || j < 0) {
            return "esa posición no existe en la población | no se pudo trasladar al tropel :c";}
        if (poblacion[i] == nullptr || poblacion[j] != nullptr) {
            return "no hay tropel o la posición nueva está ocupada | no se pudo trasladar al tropel :c";}
        poblacion[j] = poblacion[i];
        poblacion[i] = nullptr;
        return "el tropel fue trasladado exitosamente :D";}
    // función para multiplicar tropeles
    void multiplicar() {
        vector<size_t> muyfelices;
        for (size_t i = 0; i < poblacion.size(); i++) {
            if (poblacion[i] != nullptr && poblacion[i]->getFelicidad() > 100) {
                muyfelices.push_back(i);}}
        for (size_t pos : muyfelices) {
            for (size_t i = 0; i < poblacion.size(); i++) {
                if (poblacion[i] == nullptr) {
                    poblacion[i] = poblacion[pos]->clonar();
                    poblacion[pos]->setFelicidad(0);
                    poblacion[i]->setFelicidad(0);
                    break;}}}}
    friend ostream& operator<<(ostream& os, const Poblacion<T>& pob) {
        for (size_t i = 0; i < pob.capacidad; i++) {
            os << "posición " << i << ": ";
            if (pob.poblacion[i] != nullptr) {
                os << pob.poblacion[i]->getNombre()
                   << " (felicidad: " << pob.poblacion[i]->getFelicidad() << ")";}
            else {
                os << "disponible";}
            os << endl;}
        return os;}};

int main() {
    srand(time(nullptr));
    TropelAlegre alegre;
    TropelGloton gloton;
    TropelExigente exigente;
    Poblacion<Tropel> poblacion(10, &alegre, &gloton, &exigente);
    cout << endl << "población inicial:" << endl;
    cout << poblacion << endl;
    // alimentar y limpiar para aumentar felicidad
    if (poblacion[0]) {
        for (int i = 0; i < 10; i++) {
            poblacion[0]->alimentar(10);}
        poblacion[0]->limpiar(10);}
    if (poblacion[1]) {
        poblacion[1]->alimentar(12);
        poblacion[1]->limpiar(7);}
    if (poblacion[2]) {
        poblacion[2]->alimentar(8);
        poblacion[2]->limpiar(10);}
    poblacion.multiplicar();
    cout << endl << endl << "después de multiplicar:" << endl;
    cout << poblacion;
    ++poblacion;
    cout << endl << "después de aumentar capacidad:" << endl;
    cout << poblacion;
    return 0;}
