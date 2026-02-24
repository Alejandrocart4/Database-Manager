# Instalación del Proyecto Database Manager

MariaDB + Qt + ODBC


## Instalación de Qt

Para ejecutar el proyecto Database Manager es necesario instalar primero el entorno de desarrollo Qt.

Se debe ingresar al sitio oficial:

https://www.qt.io/download

Descargar el instalador en línea e iniciar sesión con una cuenta gratuita.

Durante la instalación se recomienda:

Seleccionar una versión estable de Qt 6.x

Elegir Desktop 64-bit

Instalar MinGW 64-bit o MSVC 64-bit

Asegurarse de incluir el módulo Qt SQL

Qt Creator se instalará automáticamente junto con el entorno.

Una vez finalizada la instalación, Qt quedará listo para compilar el proyecto.


## Instalación de MariaDB

El proyecto utiliza MariaDB como sistema gestor de base de datos.

Descargar MariaDB desde:

https://mariadb.org/download/

Seleccionar la versión para Windows 64 bits.

Durante la instalación configurar:

Puerto: 3306

Usuario: root

Definir una contraseña segura

Finalizar la instalación.

Para verificar que MariaDB funciona correctamente, abrir un cliente SQL y ejecutar:

SELECT VERSION();

Luego crear la base de datos que utilizará el proyecto:

CREATE DATABASE `database-manager`;


## Instalación del MariaDB ODBC Connector

El proyecto no utiliza conexión directa QMYSQL, sino conexión mediante ODBC.
Por lo tanto es obligatorio instalar el conector oficial de MariaDB para ODBC.

Ingresar a:

https://mariadb.com/downloads/connectors/

Descargar:

MariaDB Connector/ODBC 64-bit

Es importante que la arquitectura coincida con:

El sistema operativo (64 bits)

El Qt instalado (64 bits)

Ejecutar el instalador y completar el proceso con las opciones por defecto.

Este conector será el intermediario entre Qt y MariaDB.


## Configuración del ODBC y creación del DSN

Después de instalar el conector ODBC, se debe crear un DSN (Data Source Name) que permitirá que la aplicación se conecte a la base de datos.

Abrir el administrador ODBC escribiendo en Ejecutar:

odbcad32.exe

Asegurarse de abrir la versión de 64 bits si Qt es 64 bits.

En el administrador ODBC:

Ir a System DSN

Click en Add

Seleccionar MariaDB ODBC Driver

Configurar los siguientes datos:

Data Source Name: database-manager

Server: localhost

Port: 3306

User: root

Password: (contraseña configurada)

Database: database-manager

Presionar Test para verificar que la conexión sea exitosa.

Si la prueba es correcta, el DSN está correctamente configurado.


## Configuración del Driver en el Código

El proyecto utiliza el driver QODBC.

La conexión debe configurarse de la siguiente manera:

QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("database-manager");
db.setUserName("root");
db.setPassword("contraseña");
db.open();

Para verificar que el driver está disponible, se puede ejecutar:

qDebug() << QSqlDatabase::drivers();

Debe aparecer:

QODBC

Si no aparece, el kit de Qt seleccionado no tiene soporte ODBC o existe conflicto de arquitectura (32/64 bits).


## Arquitectura de Funcionamiento

El flujo de conexión del sistema funciona de la siguiente manera:

Aplicación Desktop (Qt)
↓
Driver QODBC
↓
MariaDB ODBC Connector
↓
MariaDB Server

Todos estos componentes deben estar correctamente instalados para que el proyecto funcione sin errores.


## Verificación Final

Una vez completados todos los pasos:

Qt instalado correctamente

MariaDB ejecutándose

ODBC Connector instalado

DSN configurado

Base de datos creada

Se puede abrir el proyecto en Qt Creator, compilarlo y ejecutarlo.

Si todo está correctamente configurado, la aplicación permitirá:

Iniciar sesión

Conectarse a la base de datos

Listar objetos

Ejecutar consultas SELECT

Generar y exportar DDL

## Autor

**Nombre:** Jonny Alejandro Gómez Cartagena  
**Carrera:** Ingeniería en Sistemas Computacionales  
**Institución:** UNITEC SPS, Honduras  
**Materia:** Teoría a la base de datos II / Proyecto Integrador  
**Año:** 2026
