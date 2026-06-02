### 使用xml配置文件

启动sub端
```bash
$ export FASTRTPS_DEFAULT_PROFILES_FILE=/xxx/default_profile.xml
$ DDSHelloWorldSubscriber --env
```

启动pub端
```bash
$ export FASTRTPS_DEFAULT_PROFILES_FILE=/xxx/default_profile.xml
$ DDSHelloWorldPublisher --env --samples 10
```
