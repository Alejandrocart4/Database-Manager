```md
# Database Manager Desktop – Qt & MariaDB

## Descripción del proyecto

Este proyecto consiste en el desarrollo de una **aplicación de escritorio (Desktop)** para la **gestión y administración de bases de datos MariaDB**, implementada utilizando **Qt con C++**.  
La aplicación está diseñada para funcionar de forma **visual e interactiva**, permitiendo al usuario conectarse a una base de datos existente, explorar su estructura y ejecutar sentencias SQL directamente desde una interfaz gráfica, sin recurrir a tecnologías web.

El objetivo principal del proyecto es aplicar conceptos de programación estructurada y modular, manejo de estructuras de datos, y conexión a bases de datos, integrándolos en una herramienta funcional similar a un gestor de bases de datos, pero desarrollada completamente desde cero con fines académicos.

---

## Base de datos

La base de datos utilizada en este proyecto está implementada en **MariaDB**, un sistema de gestión de bases de datos relacional.  
La aplicación se conecta directamente a una base de datos ya existente (proveniente de un proyecto anterior), lo que permite trabajar sobre estructuras reales como:

- Bases de datos
- Tablas
- Columnas
- Llaves primarias y relaciones
- Vistas y otros objetos según los permisos del usuario

La comunicación entre la aplicación y MariaDB se realiza mediante **consultas SQL directas**, ejecutadas desde la propia aplicación.  
Por lineamientos académicos, **no se utiliza el esquema `information_schema`**, por lo que la información estructural de la base de datos se obtiene mediante tablas del sistema o comandos SQL permitidos por el motor MariaDB.

---

## Qt y aplicación de escritorio

La aplicación está desarrollada utilizando **Qt**, un framework multiplataforma para la creación de interfaces gráficas de usuario.  
Se emplea **C++ como lenguaje de programación**, junto con los módulos de Qt orientados a aplicaciones de escritorio, lo que permite:

- Construir una interfaz gráfica visual e intuitiva.
- Gestionar ventanas, formularios y componentes gráficos.
- Conectarse a MariaDB mediante los módulos de acceso a bases de datos de Qt.
- Mostrar resultados de consultas SQL en tablas visuales.
- Mantener una arquitectura modular y organizada.

Al tratarse de una aplicación **Desktop**, el sistema se ejecuta localmente en el equipo del usuario y no depende de navegadores, servidores web ni tecnologías basadas en internet.

---

## Autor

**Nombre:** Jonny Alejandro Gómez Cartagena  
**Carrera:** Ingeniería en Sistemas Computacionales  
**Institución:** UNITEC SPS, Honduras  
**Materia:** Estructura de Datos II / Proyecto Integrador  
**Año:** 2026
```
