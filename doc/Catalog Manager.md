# Class CatalogManager

## 公用成员函数：

### 创建

创建数据库
- `CError createDatabase (String DB_name)`

创建表
- `CError createTable(String DB_name, String table_name, String attr_name)`

创建索引
- `CError createIndex(String DB_name, String table_name, String attr_name, String index_name, int length, int offset, int type)`

### 删除

删除数据库
- `CError dropDatabase (String DB_name)`

删除表
- `CError dropTable(String DB_name, String table_name, )`

删除索引
- `CError dropIndex(String DB_name, String index_name)`

删除表内所有元组
- `CError emptyDable(String DB_name, String table_name, String index_name, count)`

### 获取

获取给定表内所有属性
- `String[] getAllAttributes(String DB_name, String table_name)`

获取给定表内所有详细属性
- `Attr[] getAllAttributesDetail(String DB_name, String table_name)`

获取特定属性详细信息
- `String getAttributeDetail(String DB_name, String table_name, String attr_name)`

获取特定数据库中所有索引信息
- `Index[] getAllIndexes(String DB_name)`

### 管理

指定主键
- `CError denotePrimeKey(String DB_name, String table_name, String index_name)`

指定外键
- `CError denoteForeignKey(String DB_name, String table_name, String attr_name, String DB_name_ref, String table_name_ref, String attr_name_ref)`

指定



### 使用数据库

- `CError useDatabase(String DB_name)`
