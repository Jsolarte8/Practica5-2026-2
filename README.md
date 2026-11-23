# Práctica 5 — Informática II (2026-2)

Proyecto en C++17 de la Universidad de Antioquia, organizado en dos actividades.

## Actividad 1: simulación de colisiones
Abrir `parte1/Parte1.pro` en Qt Creator o compilar desde `parte1/`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp vector2d.cpp particula.cpp obstaculo.cpp simulador.cpp -o simulacion
./simulacion
python graficar_simulacion.py
```

El simulador modela cuatro partículas circulares y cuatro obstáculos cuadrados. Registra en `salida_simulacion.txt` posiciones, velocidades, tiempo y eventos, y en `obstaculos_simulacion.txt` la geometría de los obstáculos. La gráfica requiere `pandas` y `matplotlib` (`python -m pip install pandas matplotlib`); marca las colisiones con cruces y muestra los tiempos de cada evento por consola.

Se aplican rebotes elásticos contra paredes, restitución inelástica contra obstáculos y fusiones de partículas mediante conservación del momento lineal.

## Actividad 2: juego por turnos con Qt Widgets
Abrir `parte2/Parte2.pro` en Qt Creator. Dos jugadores seleccionan ángulo y rapidez para lanzar proyectiles. La infraestructura recibe daño proporcional a masa por rapidez del proyectil. El juego administra los turnos y la condición de victoria.

## Pruebas recomendadas
1. Ejecutar ambas aplicaciones desde Qt Creator.
2. Verificar cuatro partículas y cuatro obstáculos y sus eventos exportados.
3. Probar choques contra esquinas y caras (sin rebotes artificiales).
4. Comprobar fusiones, turnos, daño de infraestructura y victoria.

Este repositorio contiene correcciones sobre `Jsolarte8/Practica5-Lab`. El diagrama PNG original no se ha podido transferir por la limitación de acceso a archivos binarios; hay un resumen en `DIAGRAMA_CLASES.md`.
