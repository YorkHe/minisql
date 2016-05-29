# Class IndexManager

`typedef INDEX_CODE int;`

```
struct Index{
  String index_name,
  int type_id,
  String value,
  long offset  
}
```

## 公用成员函数

根据cond中指定的条件， 搜索给定的表, 返回找到的第一个值的索引号
- `INDEX_CODE searchOne(String DB_name, String table_name, Condition cond)`

根据cond中指定的条件， 搜索给定的表，返回所有符合条件的值的索引号
- `INDEX_CODE[] searchMany(String DB_name, String table_name, Condition cond)`

删除一个特定索引值
- `CError deleteOne(String DB_name, String table_name, Condition cond)`

删除多个特定的索引值
- `CError deleteMany(String DB_name, String table_name, Condition cond)`


获取特定索引值相对应的索引信息
- `Index getIndex(INDEX_CODE index_code)`
