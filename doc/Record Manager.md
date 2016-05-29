# Class RecordManager

## 公用成员函数

构造器

RecordManager(String DB_name, String table_name)

### 选择语句

`CError select(Condition cond)`

### 删除语句

`CError delete(Condition cond)`

### 插入语句

`CError insert(Tuple tuple)`

### 打印语句

打印全部
`void printAll(&outstream out)`

打印部分
`void printLimit(&outstream out, int from, int count)`

在执行完select语句之后， 搜索的结果会被保存在RecordManager的私有成员变量之中， 直接调用print函数就可以将其打印出来。





## 结构体

```
struct Condition{
  bool with_where;
  Formula* where_clause;
}
```

```
struct Formula{
  String attr_1;
  String attr_2;
  RELATION relationship;
}
```

```
enum RELATION{
  EQUAL,
  GREATER,
  GREATE_AND_EQUAL,
  LESS,
  LESS_AND_EQUAL
}
```

例： id < 3表示为

Condition{
  with_where: true,
  where_clause:[
    {
      attr_1: "id",
      attr_2: "3",
      relationship: RELATION.LESS
    }
  ]
}

```
struct Tuple{
  Element[] element;
}
```

```
struct Element{
  String attr_name;
  String value;
}
```
