### posix共享内存 示例

参考《UNIX网络编程 卷2》，第13章 Posix共享内存

**创建共享内存**

源码：shmcreate.cpp

使用示例：

```
$ ./shmcreate --helpshort
shmcreate:
usage: ./shmcreate [--check_exists] [--name NAME] [--length LENGTH]

create shared memory


  Flags from shmcreate.cpp:
    -check_exists (check shared memory already exists) type: bool
      default: false
    -length (shared memory length) type: uint32 default: 1024
    -name (shared memory name) type: string default: "shm_test"
$ ./shmcreate --name "test" --length 128
$ ls /dev/shm/
test
$ ./shmcreate --name "test" --check_exists
terminate called after throwing an instance of 'std::system_error'
  what():  shm_open error for test: File exists
已放弃 (核心已转储)
$
```

**删除共享内存**

源码：shmunlink.cpp

```
$ ./shmunlink --helpshort
shmunlink:
usage: ./shmunlink [--name NAME]

remove shared memory


  Flags from shmunlink.cpp:
    -name (shared memory name) type: string default: "shm_test"
$ ./shmcreate --name "test" --length 128
$ ls /dev/shm/
test
$ ./shmunlink --name "test"
$ ls /dev/shm/
$ ./shmunlink --name "test"
terminate called after throwing an instance of 'std::system_error'
  what():  shm_unlink error: No such file or directory
已放弃 (核心已转储)
```

**读写共享内存**

源码：shmread.cpp，shmwrite.cpp

```
$ ./shmread --helpshort
shmread:
usage: ./shmread [--name NAME]

create shared memory


  Flags from shmread.cpp:
    -name (shared memory name) type: string default: "shm_test"
$ ./shmwrite --helpshort
shmwrite:
usage: ./shmwrite [--name NAME]

create shared memory


  Flags from shmwrite.cpp:
    -name (shared memory name) type: string default: "shm_test"
$ ./shmcreate --name "test" --length 512
$ ./shmread --name "test"
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
$ ./shmwrite --name "test"
$ ./shmread --name "test"
00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
10 11 12 13 14 15 16 17  18 19 1A 1B 1C 1D 1E 1F
20 21 22 23 24 25 26 27  28 29 2A 2B 2C 2D 2E 2F
30 31 32 33 34 35 36 37  38 39 3A 3B 3C 3D 3E 3F
40 41 42 43 44 45 46 47  48 49 4A 4B 4C 4D 4E 4F
50 51 52 53 54 55 56 57  58 59 5A 5B 5C 5D 5E 5F
60 61 62 63 64 65 66 67  68 69 6A 6B 6C 6D 6E 6F
70 71 72 73 74 75 76 77  78 79 7A 7B 7C 7D 7E 7F
80 81 82 83 84 85 86 87  88 89 8A 8B 8C 8D 8E 8F
90 91 92 93 94 95 96 97  98 99 9A 9B 9C 9D 9E 9F
A0 A1 A2 A3 A4 A5 A6 A7  A8 A9 AA AB AC AD AE AF
B0 B1 B2 B3 B4 B5 B6 B7  B8 B9 BA BB BC BD BE BF
C0 C1 C2 C3 C4 C5 C6 C7  C8 C9 CA CB CC CD CE CF
D0 D1 D2 D3 D4 D5 D6 D7  D8 D9 DA DB DC DD DE DF
E0 E1 E2 E3 E4 E5 E6 E7  E8 E9 EA EB EC ED EE EF
F0 F1 F2 F3 F4 F5 F6 F7  F8 F9 FA FB FC FD FE FF
00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
10 11 12 13 14 15 16 17  18 19 1A 1B 1C 1D 1E 1F
20 21 22 23 24 25 26 27  28 29 2A 2B 2C 2D 2E 2F
30 31 32 33 34 35 36 37  38 39 3A 3B 3C 3D 3E 3F
40 41 42 43 44 45 46 47  48 49 4A 4B 4C 4D 4E 4F
50 51 52 53 54 55 56 57  58 59 5A 5B 5C 5D 5E 5F
60 61 62 63 64 65 66 67  68 69 6A 6B 6C 6D 6E 6F
70 71 72 73 74 75 76 77  78 79 7A 7B 7C 7D 7E 7F
80 81 82 83 84 85 86 87  88 89 8A 8B 8C 8D 8E 8F
90 91 92 93 94 95 96 97  98 99 9A 9B 9C 9D 9E 9F
A0 A1 A2 A3 A4 A5 A6 A7  A8 A9 AA AB AC AD AE AF
B0 B1 B2 B3 B4 B5 B6 B7  B8 B9 BA BB BC BD BE BF
C0 C1 C2 C3 C4 C5 C6 C7  C8 C9 CA CB CC CD CE CF
D0 D1 D2 D3 D4 D5 D6 D7  D8 D9 DA DB DC DD DE DF
E0 E1 E2 E3 E4 E5 E6 E7  E8 E9 EA EB EC ED EE EF
F0 F1 F2 F3 F4 F5 F6 F7  F8 F9 FA FB FC FD FE FF
```

或者可以用hexdump命令查看共享内存的内容：

```
$ hexdump -C /dev/shm/shm_test
00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
00000010  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
00000020  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
00000030  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
00000040  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFGHIJKLMNO|
00000050  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVWXYZ[\]^_|
00000060  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  |`abcdefghijklmno|
00000070  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  |pqrstuvwxyz{|}~.|
00000080  80 81 82 83 84 85 86 87  88 89 8a 8b 8c 8d 8e 8f  |................|
00000090  90 91 92 93 94 95 96 97  98 99 9a 9b 9c 9d 9e 9f  |................|
000000a0  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  |................|
000000b0  b0 b1 b2 b3 b4 b5 b6 b7  b8 b9 ba bb bc bd be bf  |................|
000000c0  c0 c1 c2 c3 c4 c5 c6 c7  c8 c9 ca cb cc cd ce cf  |................|
000000d0  d0 d1 d2 d3 d4 d5 d6 d7  d8 d9 da db dc dd de df  |................|
000000e0  e0 e1 e2 e3 e4 e5 e6 e7  e8 e9 ea eb ec ed ee ef  |................|
000000f0  f0 f1 f2 f3 f4 f5 f6 f7  f8 f9 fa fb fc fd fe ff  |................|
00000100  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
00000110  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
00000120  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
00000130  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
00000140  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFGHIJKLMNO|
00000150  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVWXYZ[\]^_|
00000160  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  |`abcdefghijklmno|
00000170  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  |pqrstuvwxyz{|}~.|
00000180  80 81 82 83 84 85 86 87  88 89 8a 8b 8c 8d 8e 8f  |................|
00000190  90 91 92 93 94 95 96 97  98 99 9a 9b 9c 9d 9e 9f  |................|
000001a0  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  |................|
000001b0  b0 b1 b2 b3 b4 b5 b6 b7  b8 b9 ba bb bc bd be bf  |................|
000001c0  c0 c1 c2 c3 c4 c5 c6 c7  c8 c9 ca cb cc cd ce cf  |................|
000001d0  d0 d1 d2 d3 d4 d5 d6 d7  d8 d9 da db dc dd de df  |................|
000001e0  e0 e1 e2 e3 e4 e5 e6 e7  e8 e9 ea eb ec ed ee ef  |................|
000001f0  f0 f1 f2 f3 f4 f5 f6 f7  f8 f9 fa fb fc fd fe ff  |................|
00000200  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
00000210  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
00000220  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
00000230  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
00000240  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFGHIJKLMNO|
00000250  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVWXYZ[\]^_|
00000260  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  |`abcdefghijklmno|
00000270  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  |pqrstuvwxyz{|}~.|
00000280  80 81 82 83 84 85 86 87  88 89 8a 8b 8c 8d 8e 8f  |................|
00000290  90 91 92 93 94 95 96 97  98 99 9a 9b 9c 9d 9e 9f  |................|
000002a0  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  |................|
000002b0  b0 b1 b2 b3 b4 b5 b6 b7  b8 b9 ba bb bc bd be bf  |................|
000002c0  c0 c1 c2 c3 c4 c5 c6 c7  c8 c9 ca cb cc cd ce cf  |................|
000002d0  d0 d1 d2 d3 d4 d5 d6 d7  d8 d9 da db dc dd de df  |................|
000002e0  e0 e1 e2 e3 e4 e5 e6 e7  e8 e9 ea eb ec ed ee ef  |................|
000002f0  f0 f1 f2 f3 f4 f5 f6 f7  f8 f9 fa fb fc fd fe ff  |................|
00000300  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
00000310  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
00000320  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
00000330  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
00000340  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFGHIJKLMNO|
00000350  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVWXYZ[\]^_|
00000360  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  |`abcdefghijklmno|
00000370  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  |pqrstuvwxyz{|}~.|
00000380  80 81 82 83 84 85 86 87  88 89 8a 8b 8c 8d 8e 8f  |................|
00000390  90 91 92 93 94 95 96 97  98 99 9a 9b 9c 9d 9e 9f  |................|
000003a0  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  |................|
000003b0  b0 b1 b2 b3 b4 b5 b6 b7  b8 b9 ba bb bc bd be bf  |................|
000003c0  c0 c1 c2 c3 c4 c5 c6 c7  c8 c9 ca cb cc cd ce cf  |................|
000003d0  d0 d1 d2 d3 d4 d5 d6 d7  d8 d9 da db dc dd de df  |................|
000003e0  e0 e1 e2 e3 e4 e5 e6 e7  e8 e9 ea eb ec ed ee ef  |................|
000003f0  f0 f1 f2 f3 f4 f5 f6 f7  f8 f9 fa fb fc fd fe ff  |................|
00000400
```
