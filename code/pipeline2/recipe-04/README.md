### 带类层次的管道和过滤器模式

Pipeline相关基类和SimplePipeline子类实现
- SimplePipeline子类支持多类型的Pipe
- 增加DataFilterAny类 
- 增加CompositeDataFilter类和SimpleCompositeDataFilter类
- 删除make_pipe()接口，要求创建Pipe时必须指定capacity
