# 计划
目前还不确定是否需要指向父亲的指针

## bptree结构如下：
```hpp
template <class key_t, class value_t>
class bptree {
    struct Node{

    };
};

```
> blockSize = n, 每个节点有n - 1个搜索码key， n个指针，用vector存储
>
> n - 1个搜索码值$K_0, K_1, \dots, K_{n - 2}$
>
> n个指针$P_0, P_1, \dots, P_{n - 1}$

## 我的接口：

### 1、查询：
* 询问是否有某个key

  `int count(const key_t &Key)`

* 根据key查找元素

  `value_t find(const key_t &Key)`

  返回key所对应的value，若key不存在，返回default

### 2、更新：
* 插入(insert_in_parent() unfinished）

  `void insert(const key_t &Key, const value_t &Value)`
* 删除(erase_entry_unfinished)
 
  `void erase(const key_t &Key)`


## 苏起冬的B+树接口$\Downarrow$
- 询问是否有某个key

  `int count(const key_t &key)`

  如果有返回1，否则返回0

- 根据key查找元素

  `value_t find(const key_t &key, const value_t & d = value_t())`

  返回key所对应的value，若key不存在，返回d

- 判断是否为空

  `inline bool empty()`

- 插入元素

  `insert(const key_t &key, const value_t &v)`

  若key已存在，则什么事情都不会做

- 修改

  `void set(const key_t &key, const value_t &v)`

  如果不存在key，会报错，返回not_found

- 删除

  `void remove(const key_t &key)`

  如果不存在key，会报错，返回not_found

- 区间查找

  `void search(array_t & arr, const key_t & key, std::function<bool(const key_t &, const key_t &)> compar)`