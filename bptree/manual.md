# 计划
目前还不确定是否需要指向父亲的指针（所以保留了指向父亲的指针）

## bptree结构如下：
```hpp
template <class key_t, class value_t>
class bptree {
    struct Node {

    };
    
    class iterator {
    
    };
};

```
## 我的接口：

### 1、查询：
* 询问是否有某个key

  `int count(const key_t &Key)`

* 根据key查找元素

  `value_t find(const key_t &Key)`

  返回key所对应的value，若key不存在，返回default
  
* 返回一个指向大于等于key的迭代器

	`iterator lower_bound(const key_t &Key)`

### 2、更新：
* 插入

  `bool insert(const key_t &Key, const value_t &Value)`
  
* 删除

  `bool erase(const key_t &Key)`
  
* 修改

	`bool modify(const key_t &Key, const value_t &Value)`
	
* 初始化
	`void clear()`

## 3、关于iterator的操作

#### 迭代器类里的函数

* 检查迭代器指向是否有效

	`bool check()`
#### B+树类里的函数

* 下一个迭代器
	`void next_iterator(iterator &it)`
	
* 返回迭代器指向的key

	`key_t retKey(iterator it)`
	
* 返回迭代器指向的value

	`value_t retValue(iterator it)`
