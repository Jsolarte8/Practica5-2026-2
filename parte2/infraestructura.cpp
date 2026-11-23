#include "infraestructura.h"

#include <algorithm>

Infraestructura::Infraestructura()
    : id(0), dueno(0), pos(), ancho(20.0), alto(20.0), resistencia(100.0),
      resistenciaInicial(100.0), restitucion(0.55) {}

Infraestructura::Infraestructura(int id, int dueno, const Vector2D& posicion,
                                 double ancho, double alto, double resistencia,
                                 double restitucion)
    : id(id), dueno(dueno), pos(posicion), ancho(ancho), alto(alto),
      resistencia(resistencia), resistenciaInicial(resistencia),
      restitucion(restitucion) {}

bool Infraestructura::colisionaCon(const Proyectil& proyectil) const {
    if (!proyectil.estaActiva() || estaDestruida()) {
        return false;
    }

    const Vector2D& centro = proyectil.obtenerPosicion();
    const double cercanoX = limitar(centro.x, pos.x, pos.x + ancho);
    const double cercanoY = limitar(centro.y, pos.y, pos.y + alto);
    const double dx = centro.x - cercanoX;
    const double dy = centro.y - cercanoY;
    const double r = proyectil.obtenerRadio();

    return dx * dx + dy * dy <= r * r;
}

double Infraestructura::resolverColision(Proyectil& proyectil, double factorDano,
                                         bool aplicarDano) {
    if (!colisionaCon(proyectil)) return 0.0;
    Vector2D posicion = proyectil.obtenerPosicion();
    Vector2D velocidad = proyectil.obtenerVelocidad();
    const double rapidez = velocidad.magnitud();
    const double r = proyectil.obtenerRadio();
    const double cx = limitar(posicion.x, pos.x, pos.x + ancho);
    const double cy = limitar(posicion.y, pos.y, pos.y + alto);
    Vector2D normal(posicion.x - cx, posicion.y - cy);
    const double d = normal.magnitud();
    if (d > 1e-9) {
        normal = normal / d;
        posicion += normal * (r - d + 1e-6);
    } else {
        switch (determinarLadoImpacto(proyectil)) {
        case LadoInfraestructura::Izquierda: normal = Vector2D(-1, 0); posicion.x = pos.x - r - 1e-6; break;
        case LadoInfraestructura::Derecha: normal = Vector2D(1, 0); posicion.x = pos.x + ancho + r + 1e-6; break;
        case LadoInfraestructura::Arriba: normal = Vector2D(0, -1); posicion.y = pos.y - r - 1e-6; break;
        case LadoInfraestructura::Abajo: normal = Vector2D(0, 1); posicion.y = pos.y + alto + r + 1e-6; break;
        default: break;
        }
    }
    const double vn = velocidad.x * normal.x + velocidad.y * normal.y;
    proyectil.asignarPosicion(posicion);
    if (vn >= 0.0) return 0.0;
    velocidad = velocidad - normal * ((1.0 + restitucion) * vn);
    proyectil.asignarVelocidad(velocidad);
    const double dano = factorDano * proyectil.obtenerMasa() * rapidez;
    if (aplicarDano) recibirDano(dano);
    return aplicarDano ? dano : 0.0;
}

void Infraestructura::recibirDano(double dano) {
    resistencia -= dano;
    if (resistencia < 0.0) {
        resistencia = 0.0;
    }
}

bool Infraestructura::estaDestruida() const {
    return resistencia <= 0.0;
}

int Infraestructura::obtenerId() const {
    return id;
}

int Infraestructura::obtenerDueno() const {
    return dueno;
}

const Vector2D& Infraestructura::obtenerPosicion() const {
    return pos;
}

double Infraestructura::obtenerAncho() const {
    return ancho;
}

double Infraestructura::obtenerAlto() const {
    return alto;
}

double Infraestructura::obtenerResistencia() const {
    return resistencia;
}

double Infraestructura::obtenerResistenciaInicial() const {
    return resistenciaInicial;
}

double Infraestructura::obtenerRestitucion() const {
    return restitucion;
}

LadoInfraestructura Infraestructura::determinarLadoImpacto(const Proyectil& proyectil) const {
    const Vector2D& centro = proyectil.obtenerPosicion();
    const double r = proyectil.obtenerRadio();

    const double penetracionIzquierda = (centro.x + r) - pos.x;
    const double penetracionDerecha = (pos.x + ancho) - (centro.x - r);
    const double penetracionArriba = (centro.y + r) - pos.y;
    const double penetracionAbajo = (pos.y + alto) - (centro.y - r);

    double menor = penetracionIzquierda;
    LadoInfraestructura lado = LadoInfraestructura::Izquierda;

    if (penetracionDerecha < menor) {
        menor = penetracionDerecha;
        lado = LadoInfraestructura::Derecha;
    }
    if (penetracionArriba < menor) {
        menor = penetracionArriba;
        lado = LadoInfraestructura::Arriba;
    }
    if (penetracionAbajo < menor) {
        lado = LadoInfraestructura::Abajo;
    }

    return lado;
}

double Infraestructura::limitar(double valor, double minimo, double maximo) {
    return std::max(minimo, std::min(valor, maximo));
}
