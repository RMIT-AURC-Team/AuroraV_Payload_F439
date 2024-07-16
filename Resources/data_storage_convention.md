# Data Storage Conventions
Each read contains the following 21 byte breakdown

| Bytes     | Size (Bytes) | Size (Bits) | Contents                | Endian |
|-----------|--------------|-------------|-------------------------|--------|
| [0:1]     | 2            | 16          | Timestamp               | Little |
| [2:3]     | 2            | 16          | X Accel (Raw)           | Little |
| [4:5]     | 2            | 16          | Y Accel (Raw)           | Little |
| [6:7]     | 2            | 16          | Z Accel (Raw)           | Little |
| [8:9]     | 2            | 16          | Pressure (Sensor 0)     | Little |
| [10:11]   | 2            | 16          | Temperature ((Sensor 0) | Little |
| [12:13]   | 2            | 16          | Humidity (Sensor 0)     | Little |
| [14:15]   | 2            | 16          | Pressure (Sensor 1)     | Little |
| [16:17]   | 2            | 16          | Temperature (Sensor 1)  | Little |
| [18:19]   | 2            | 16          | Humidity (Sensor 1)     | Little |
| [20 [7]]   | -            | 1           | End of Flash                 |  |
| [20 [6:5]] | -            | 2           | State                   |  |
| [20 [4:0]] | -            | 5           | Status Code             |  |