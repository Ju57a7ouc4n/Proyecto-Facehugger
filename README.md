# Proyecto: Facehugger

**Facehugger** es un proyecto de malware educativo orientado tanto a la investigación y formación en ciberseguridad ofensiva como al análisis de ejecutables y la puesta en práctica de los conocimientos que fui adquiriendo en los últimos años.

Su objetivo principal es la infección de archivos ejecutables a través de accesos directos (*shortcuts*), incorporando técnicas de *code caving* y evasión de antivirus mediante mecanismos de ofuscación y autodestrucción.

Este proyecto ha sido desarrollado con fines **exclusivamente académicos y experimentales**, como parte de mi portafolio personal en ciberseguridad ofensiva, y está destinado únicamente a ser ejecutado en entornos de laboratorio controlados.

---

## ⚖️ Uso Responsable y Legal

Este software **NO** debe ser utilizado fuera de entornos aislados y controlados, como máquinas virtuales o laboratorios de práctica sin conexión. El propósito del proyecto es puramente formativo y académico.

**El uso inadecuado de este código podría infringir leyes locales o internacionales sobre ciberseguridad, y conllevar consecuencias penales graves.**  
El autor **no se hace responsable** por cualquier daño, pérdida de información o actividad ilegal derivada del uso inapropiado del código.

---

## 📍 Roadmap

Actualmente, Facehugger está en desarrollo, por lo que en el siguiente *roadmap* marco las metas alcanzadas y por alcanzar:

Facehugger:
    - ✅ Reconocimiento de accesos directos y ejecutables en una carpeta.  
    - ✅ Gestión y análisis de accesos directos.  
    - ✅ Obtención de rutas absolutas de ejecutables a través de sus accesos directos.  
    - ✅ Autodestrucción luego de finalizada la infección.  
    - ⬜ Creación de *code caves* en archivos ejecutables.

Mother:
    - ✅ Embeber mother con facehuggers.  
    - ✅ Capacidad de ejecutar facehuggers en el escritorio de la victima. 
    - ⬜ Creación de un proceso keylogger en .../drivers/colors.
    - ⬜ Creación de "beacon", similar a un servidor SMB en la ubicacion de madre.
    
## 🎯 Propósitos del Proyecto

- Concientizar sobre el peligro de descargar y ejecutar archivos de los que no se esté seguro de donde provienen.

- Desarrollar habilidades prácticas en:
  - Análisis de formatos ejecutables y accesos directos (.exe, .lnk, ELF).
  - Redirección del flujo de ejecución.
  - Técnicas de evasión y persistencia.
  - Seguridad ofensiva desde una perspectiva defensiva.

- Poner en práctica mis conocimientos en:
  - Programación en C y ensamblador.
  - Manejo de estructuras de datos.
  - Sistemas operativos Windows.

- Generar material profesional para ser incluido en mi portfolio de ciberseguridad.

---

## ⚙️ Uso

Dentro de las carpetas del repositorio puede encontrarse el ejecutable `mother.exe` ya compilado. Una vez ejecutado, realizará paso por paso las siguientes acciones:
  1. Intentará obtener la ruta al escritorio del usuario víctima.
  2. En caso de hacerlo, generará un facehugger en el mismo e intentará ejecutarlo.
Luego, `facehugger.exe`:
  1. Listará automáticamente los archivos de la carpeta en la que se encuentre.  
  2. Generará una lista doblemente enlazada con los archivos ejecutables y accesos directos. En caso de no encontrar nada, pasará directamente al siguiente paso.  
  3. Accederá a la lista y, por cada acceso directo que encuentre:
     - Abrirá el archivo.
     - Intentará encontrar la ruta al ejecutable al que apunta.
     - En caso de encontrar una ruta válida, reemplazará la ruta del acceso directo en el nodo por la encontrada.
     - En caso de no encontrar una ruta válida o que el archivo no sea válido, eliminará el nodo de la lista para evitar errores.
  4. Por cada elemento de la lista, intentará acceder al ejecutable correspondiente para crear *code caves*.
  5. Al finalizar, creará un proceso en el sistema que, luego de un breve retardo, eliminará el ejecutable, borrando sus rastros.

---

## 📝 Algunos Comentarios Finales

Como bien antes mencioné, facehugger está en proceso, por lo que utilizaré esta sección para comunicar cosas relevantes al mismo.

- Actualmente los avances los realizo en mi tiempo libre, por lo que el tiempo entre commits es (muy) variable.

- El código que subo no se filtra de ninguna manera, es decir, todos los comentarios y "cosas raras" que se encuentren tienen que ver con mi forma de razonar.

- Es muy probable que a medida que vaya avanzando en el proyecto me surjan ideas de funciones, así que lo más probable es que las notifique en esta sección.

- Todavía no tengo una idea clara de qué hará el proyecto una vez terminada la infección, pero ahora mismo la idea es que las aplicaciones se conviertan en keyloggers y, eventualmente, repliquen facehuggers en todo el equipo infectado.

- Cualquier comentario, crítica o idea para mejorar el proyecto siempre es bienvenida.

- Estuve revisando (muy por encima), el compoprtamiento de Astaroth para pasar desapercibido, en proximas versiones puede que lo replique de alguna manera: Cuando me refiero a replicar comportamiento es en el sentido "amplio", es decir, me gustaria ocultar al .exe madre a traves de sucesivas descargas y ejecuciones de archivos que hagan pequeñas llamadas al sistema, para que cuando mother.exe caiga al sistema no se sospechoso.
