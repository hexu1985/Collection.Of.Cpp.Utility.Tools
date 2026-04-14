### sub端

```
2025-09-11 10:40:29.648 [RTPS_READER Warning] Reader 01.0f.05.0a.04.ad.2b.ba.01.00.00.00|0.0.1.4 was configured to have a large history (5000 max samples), but the history size used with writer 01.0f.05.0a.f7.ac.f4.9c.01.00.00.00|0.0.1.3 will be 3 max samples. (/home/hexu/fast_dds/Fast-DDS/src/cpp/rtps/DataSharing/DataSharingListener.cpp:226) -> Function add_datasharing_writer
```


### pub端

```
2025-09-11 10:40:17.847 [RTPS_UTILS Info] Creating CacheChangePool of size: 3 (/home/hexu/fast_dds/Fast-DDS/src/cpp/rtps/history/CacheChangePool.cpp:54) -> Function init
2025-09-11 10:40:17.847 [RTPS_UTILS Info] Static Mode is active, preallocating memory for pool_size elements (/home/hexu/fast_dds/Fast-DDS/src/cpp/rtps/history/CacheChangePool.cpp:76) -> Function init
2025-09-11 10:40:17.847 [RTPS_UTILS Info] Allocating group of cache changes of size: 3 (/home/hexu/fast_dds/Fast-DDS/src/cpp/rtps/history/CacheChangePool.cpp:120) -> Function allocateGroup
```
