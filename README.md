# FootCar Remote Control

## Building

### Prerequisites

Make sure ESP-IDF >= 4.0.0, NodeJS >= 11.4.0 are installed on your machine.

### Building the firmware

With the ESP-IDF environment variables set properly,

```bash
idf.py build
```

### Building the web interface

```bash
cd web
npm i -D
npm run build
```
