#include "obstaculo.h"

#include <algorithm>
#include <cmath>
#include <sstream>

Obstaculo::Obstaculo()
    : id(0), pos(), ancho(10.0), alto(10.0), restitucion(0.6) {}

Obstaculo::Obstaculo(int id, const Vector2D& posicion, double ancho, double alto,
                     double coeficienteRestitucion)
    : id(id), pos(posicion), ancho(ancho), alto(alto),
      restitucion(coeficienteRestitucion) {}

bool Obstaculo::colisionaCon(const Particula& particula) const {
    if (!particula.estaActiva()) {
        return false;
    }

    const Vector2D& centro = particula.obtenerPosicion();
    const double cercanoX = limitar(centro.x, pos.x, pos.x + ancho);
    const double cercanoY = limitar(centro.y, pos.y, pos.y + alto);
    const double dx = centro.x - cercanoX;
    const double dy = centro.y - cercanoY;
    const double r = particula.obtenerRadio();

    return dx * dx + dy * dy <= r * r;
}

LadoImpacto Obstaculo::determinarLadoImpacto(const Particula& particula) const {
    const Vector2D& centro = particula.obtenerPosicion();
    const double r = particula.obtenerRadio();

    const double penetracionIzquierda = (centro.x + r) - pos.x;
    const double penetracionDerecha = (pos.x + ancho) - (centro.x - r);
    const double penetracionArriba = (centro.y + r) - pos.y;
    const double penetracionAbajo = (pos.y + alto) - (centro.y - r);

    double menor = penetracionIzquierda;
    LadoImpacto lado = LadoImpacto::Izquierda;

    if (penetracionDerecha < menor) {
        menor = penetracionDerecha;
        lado = LadoImpacto::Derecha;
    }
    if (penetracionArriba < menor) {
        menor = penetracionArriba;
        lado = LadoImpacto::Arriba;
    }
    if (penetracionAbajo < menor) {
        lado = LadoImpacto::Abajo;
    }

    return lado;
}

std::string Obstaculo::resolverColision(Particula& particula) const {
    if (!colisionaCon(particula)) return "";
    Vector2D posicion = particula.obtenerPosicion();
    Vector2D velocidad = particula.obtenerVelocidad();
    const double r = particula.obtenerRadio();
    const double cx = limitar(posicion.x, pos.x, pos.x + ancho);
    const double cy = limitar(posicion.y, pos.y, pos.y + alto);
    Vector2D normal(posicion.x - cx, posicion.y - cy);
    const double d = normal.magnitud();
    LadoImpacto lado = LadoImpacto::Ninguno;
    if (d > 1e-9) {
        normal = normal / d;
        if (std::abs(normal.x) >= std::abs(normal.y))
            lado = normal.x < 0 ? LadoImpacto::Izquierda : LadoImpacto::Derecha;
        else
            lado = normal.y < 0 ? LadoImpacto::Arriba : LadoImpacto::Abajo;
        posicion += normal * (r - d + 1e-6);
    } else {
        lado = determinarLadoImpacto(particula);
        switch (lado) {
        case LadoImpacto::Izquierda: normal = Vector2D(-1, 0); posicion.x = pos.x - r - 1e-6; break;
        case LadoImpacto::Derecha: normal = Vector2D(1, 0); posicion.x = pos.x + ancho + r + 1e-6; break;
        case LadoImpacto::Arriba: normal = Vector2D(0, -1); posicion.y = pos.y - r - 1e-6; break;
        case LadoImpacto::Abajo: normal = Vector2D(0, 1); posicion.y = pos.y + alto + r + 1e-6; break;
        default: break;
        }
    }
    const double vn = velocidad.x * normal.x + velocidad.y * normal.y;
    if (vn < 0) velocidad = velocidad - normal * ((1 + restitucion) * vn);
    particula.asignarPosicion(posicion);
    particula.asignarVelocidad(velocidad);
    return "obstaculo_" + std::to_string(id) + "_" + nombreLado(lado);
}

int Obstaculo::obtenerId() const {
    return id;
}

const Vector2D& Obstaculo::obtenerPosicion() const {
    return pos;
}

double Obstaculo::obtenerAncho() const {
    return ancho;
}

double Obstaculo::obtenerAlto() const {
    return alto;
}

double Obstaculo::obtenerRestitucion() const {
    return restitucion;
}

double Obstaculo::limitar(double valor, double minimo, double maximo) {
    return std::max(minimo, std::min(valor, maximo));
}

std::string Obstaculo::nombreLado(LadoImpacto lado) {
    switch (lado) {
    case LadoImpacto::Izquierda:
        return "izquierda";
    case LadoImpacto::Derecha:
        return "derecha";
    case LadoImpacto::Arriba:
        return "arriba";
    case LadoImpacto::Abajo:
        return "abajo";
    case LadoImpacto::Ninguno:
        return "ninguno";
    }
    return "ninguno";
}
