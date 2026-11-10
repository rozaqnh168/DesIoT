# Tugas Individu IoT — ESP32, Blynk, dan ThingSpeak

Proyek tugas individu mata kuliah **Desain dan Aplikasi IoT** untuk membaca suhu dan kelembapan menggunakan ESP32-S3 dan sensor DHT11, menampilkan data melalui aplikasi Blynk, serta melaporkan data ke ThingSpeak.

## Ketentuan tugas

Berdasarkan materi tugas pada foto:

1. Node ESP32 terhubung ke sensor.
2. Aplikasi Blynk terhubung ke ESP32.
3. ESP32 melaporkan nilai sensornya ke ThingSpeak.

## Implementasi proyek

| Bagian | Konfigurasi |
| --- | --- |
| Board | ESP32-S3 DevKitC-1 |
| Framework | Arduino melalui PlatformIO |
| Sensor | DHT11 |
| Pin data sensor | GPIO 4 |
| Data yang dibaca | Suhu (°C) dan kelembapan relatif (%) |
| Interval pembacaan dan pengiriman | 20 detik |
| Serial Monitor | 115200 baud |
| Blynk | V0 untuk suhu, V1 untuk kelembapan |
| ThingSpeak | Field 1 untuk suhu, Field 2 untuk kelembapan |

Alur data:

```text
DHT11 → ESP32-S3 → Wi-Fi → Blynk → Aplikasi pemantauan
                       └→ ThingSpeak → Penyimpanan dan grafik data
```

ESP32 membaca sensor secara berkala. Jika pembacaan valid, data ditampilkan pada Serial Monitor dan dikirim ke kedua layanan. Jika pembacaan menghasilkan `NaN`, pengiriman pada siklus tersebut dilewati.

## Peralatan

- ESP32-S3 DevKitC-1.
- Sensor atau modul DHT11.
- Kabel USB yang mendukung transfer data.
- Kabel jumper dan breadboard sesuai kebutuhan.
- Jaringan Wi-Fi dengan akses internet yang kompatibel dengan board.
- Komputer dengan PlatformIO, misalnya melalui ekstensi PlatformIO IDE di Visual Studio Code.
- Akun Blynk dan ThingSpeak, serta aplikasi Blynk untuk pemantauan.

## Rangkaian sensor

Rangkaian berikut mengasumsikan modul DHT11 yang mendukung catu dan sinyal 3,3 V. Periksa label serta spesifikasi modul sebelum merangkai.

| Pin DHT11 | Pin ESP32-S3 |
| --- | --- |
| VCC / + | 3V3 |
| DATA / OUT / S | GPIO 4 |
| GND / − | GND |

Ikuti label pin modul karena urutan pin dapat berbeda. Untuk sensor DHT11 tanpa modul, gunakan resistor pull-up pada jalur DATA sesuai datasheet sensor. Lepaskan daya saat memasang kabel.

## Struktur folder

```text
Tugas_iot/
├── include/         # Direktori header proyek
├── lib/             # Direktori library lokal
├── src/
│   └── main.cpp     # Pembacaan DHT11 dan pengiriman data
├── test/            # Direktori pengujian PlatformIO
├── platformio.ini   # Board, framework, dependensi, dan Serial Monitor
└── README.md        # Dokumentasi proyek
```

Dependensi yang tercantum di `platformio.ini`:

- `blynkkk/Blynk @ ^1.4.0`
- `mathworks/ThingSpeak @ ^2.1.1`
- `adafruit/DHT sensor library @ ^1.4.7`
- `adafruit/Adafruit Unified Sensor @ ^1.1.15`

## Konfigurasi

### 1. Blynk

Siapkan template dan perangkat untuk ESP32 dengan koneksi Wi-Fi. Buat datastream dan hubungkan widget pemantauan dengan pemetaan berikut:

| Datastream | Virtual pin | Tipe data | Satuan |
| --- | --- | --- | --- |
| Suhu | V0 | Double | °C |
| Kelembapan | V1 | Double | % |

Sesuaikan rentang datastream dengan pengukuran sensor. Masukkan identitas template dan token perangkat pada `src/main.cpp`:

```cpp
#define BLYNK_TEMPLATE_ID "ISI_TEMPLATE_ID_ANDA_DISINI"
#define BLYNK_TEMPLATE_NAME "ISI_NAMA_TEMPLATE_ANDA_DISINI"
#define BLYNK_AUTH_TOKEN "ISI_AUTH_TOKEN_ANDA_DISINI"
```

### 2. ThingSpeak

Siapkan channel dengan **Field 1** untuk suhu dan **Field 2** untuk kelembapan. Salin Channel ID dan Write API Key ke `src/main.cpp`:

```cpp
unsigned long myChannelNumber = 1234567; // Ganti dengan Channel ID milik Anda
const char * myWriteAPIKey = "ISI_WRITE_API_KEY_ANDA";
```

### 3. Wi-Fi

Isi nama jaringan dan kata sandinya pada `src/main.cpp`:

```cpp
char ssid[] = "NAMA_WIFI_ANDA";
char pass[] = "PASSWORD_WIFI_ANDA";
```

Konfigurasi awal masih berupa placeholder dan harus diganti sebelum perangkat digunakan. Jangan unggah kata sandi Wi-Fi, Auth Token Blynk, atau Write API Key asli ke repositori publik maupun dokumentasi pengumpulan.

## Cara menjalankan

1. Rangkai DHT11 sesuai tabel koneksi.
2. Buka folder proyek di PlatformIO IDE.
3. Lengkapi konfigurasi Blynk, ThingSpeak, dan Wi-Fi.
4. Hubungkan ESP32-S3 ke komputer melalui kabel USB data.
5. Jalankan **Build**, lalu **Upload** melalui PlatformIO.
6. Buka **Serial Monitor** pada 115200 baud.
7. Setelah koneksi berhasil dan timer berjalan, tunggu siklus pembacaan setiap 20 detik. Periksa nilai di Serial Monitor, aplikasi Blynk, dan channel ThingSpeak.

Jika PlatformIO CLI tersedia di terminal, gunakan perintah berikut dari folder proyek:

```sh
pio run
pio run --target upload
pio device monitor --baud 115200
```

Environment yang digunakan adalah `esp32-s3-devkitc-1`. Jika port tidak terdeteksi otomatis, pilih port perangkat yang sesuai pada PlatformIO.

Contoh format keluaran Serial Monitor, bukan hasil pengukuran aktual:

```text
Suhu: 28.00 *C
Kelembapan: 65.00 %
```

## Pemecahan masalah

| Gejala | Pemeriksaan |
| --- | --- |
| Upload gagal | Periksa kabel USB data, pilihan board, port, dan apakah port sedang digunakan aplikasi lain. |
| Tidak ada nilai sensor | Periksa daya, GND, jalur DATA ke GPIO 4, dan jenis sensor DHT11 pada kode. Pembacaan gagal dilewati tanpa pesan kesalahan. |
| Blynk tidak terhubung | Periksa koneksi internet, SSID, kata sandi, identitas template, dan Auth Token perangkat. |
| Blynk terhubung tetapi widget kosong | Pastikan datastream dan widget menggunakan V0 untuk suhu serta V1 untuk kelembapan. |
| ThingSpeak tidak menerima data | Periksa Channel ID, Write API Key, Field 1 dan Field 2, koneksi internet, serta batas interval layanan pada akun yang digunakan. |

