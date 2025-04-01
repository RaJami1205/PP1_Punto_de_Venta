<img src="https://upload.wikimedia.org/wikipedia/commons/1/19/C_Logo.png" alt="C" width="50" height="55">   <img src="https://upload.wikimedia.org/wikipedia/commons/0/0a/MySQL_textlogo.svg" alt="MySQL" width="100" height="50">  
 
# 🛒 Sistema de Punto de Venta
### Lenguajes de Programación 
**Profesor:** Ing. Allan Rodríguez Dávila  

## 👥 Integrantes
- Eyden Su Díaz‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ | Carnet: 2023025837
- Raul Alfaro Rodríguez | Carnet: 2023060456

## 📅 Entrega
📦 **Estado de entrega:** [Deplorable | Regular | Buena | Muy Buena | Excelente | Superior]  
📅 **Fecha límite:** 31 de marzo de 2025  

## 📖 Descripción
Este proyecto consiste en el desarrollo de un **sistema de punto de venta en C**, el cual se ejecuta en la consola y almacena la información en una base de datos PostgreSQL o MySQL. Permite gestionar inventario, ventas y facturación de productos de forma eficiente.

## 🛠️ Instalación
1. Instala el compilador GCC:
   ```bash
   sudo apt install gcc
   ```
2. Instala PostgreSQL o MySQL:
   Para los scripts de sql se puede acceder a la carpeta de [Scripts](/Code/db)
   ```bash
   sudo apt install mysql-server # Para MySQL
   ```
4. Clona el repositorio:
   ```bash
   git clone https://github.com/RaJami1205/PP1_Punto_de_Venta.git
   cd repositorio
   ```

## ▶️ Compilación y Ejecución
Para compilar y ejecutar el programa, usa:
```bash
cd Code/
make
./program
```

## 📝 Documentación
Para más información sobre la instalación, ejecución y uso, consulta la [Documentación](/documents/).

---

## 🚀 Tecnologías Utilizadas
- **C** (Lenguaje de Programación)
- **MySQL** (Base de Datos)
- **Linux** (Sistema Operativo Recomendado)
- **GitHub** (Control de Versiones)

## 📂 Funcionalidades Principales
### 🔹 Opciones Administrativas (Requiere autenticación)
- 📂 Registro de familias de productos
- 📦 Registro de productos
- 📥 Carga de inventario
- 🧾 Consulta de facturas
- 📊 Estadísticas de ventas

### 🔹 Opciones Generales (Acceso sin contraseña)
- 🔍 Consulta de catálogo
- 🛒 Creación y modificación de cotizaciones
- 💳 Facturación
- ❌ Salir del sistema

## 📊 Estadísticas del Sistema
- 📌 Cantidad de cotizaciones pendientes  
- 📌 Cantidad de cotizaciones facturadas  
- 📌 Promedio de compra  
- 📌 Top 5 de productos más vendidos  
- 📌 Producto más vendido por familia  
- 📌 Monto vendido por familia  

---
