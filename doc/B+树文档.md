# B+树文档

## 一、B+树：
```hpp
template <class key_t, class value_t>
class bptree {
    struct Node {

    };
    
    class iterator {
    
    };
};
```

## 二、B+树实现方式
参考《数据库系统概念》中的实现方法。

### 1. B+树的性质

叶节点的key的数量不少于$\lceil \text{leaf_max} \rceil$，不多于leaf_max。

内部结点child的数量不少于$\lceil \text{nonleaf_max} \rceil$，不多于non_leafmax(根节点除外)。

### 2.插入和删除操作
#### 1、 插入
从根开始往叶子递归，到了叶子便将key和value插入叶节点。（如果已经存在了便插入失败，一直返回false。）。

将节点信息写入文件，之后回溯到上一层，如果发现孩子（叶节点）不满足性质了，就将孩子分裂成两个叶节点，current节点增加一个孩子和一个key（key值为newLeaf的第一个key）。

将节点信息写入文件，之后继续回溯到上一层，如果发现孩子(内部结点)不满足性质了，就将孩子分裂成两个内部结点，并上升一个key(key是孩子分裂前的midKey，也就是孩子的第$\lceil \text{nonleaf_max} \rceil$个key)到current节点。current节点增加一个key和一个child。

将节点信息写入文件，一直回溯到根节点。如果根节点不满足性质，根节点分裂，产生一个新的节点newRoot作为根节点，这是使B+树变高的唯一情况。

#### 2、删除
从根开始往叶子递归，到了叶子便将对应的key和value删除。(如果不存在便删除失败，便一直返回false)。

将节点信息写入文件，之后回溯到上一层，如果发现孩子（叶节点）不满足性质了，这时候如果孩子能和相邻兄弟（左兄弟右兄弟都行，随便，只要存在就行）合并就合并，不能合并就从相邻兄弟那里借一对key和value。

将节点信息写入文件，之后回溯到上一层。如果发现孩子（内部节点）不满足性质了，这时候如果孩子能和相邻兄弟合并就合并，不能合并就从相邻兄弟那里借一个key（具体过程其实是兄弟节点把要借的key给current节点对应的位置，current节点再把原有的key给孩子节点）和一个child。

将节点信息写入文件，一直回溯到根。如果根最后因为孩子合并而只剩下一个孩子了，删除根，将根的唯一孩子作为新的根，这是B+树变矮的唯一情况。

### 3、具体实现

#### 1、B+树的信息

文件最前面记录三个关于B+树的基本信息（root_off, end_off, trash_off)，root_off是根的位置，end_off是文件的结尾位置，trash_off是最新的一个被删除节点原本的位置(通过维护一个链接结构可以循环利用空间)。

#### 2、节点的信息
文件之后记录B+树节点的信息。

```hpp
 struct Node {
            offset addr; //这个信息不会写入文件中
            offset next; //如果节点是叶子，next指的是它的后继叶子节点的位置，如果是被删除节点，next指的是比它之前先删除的一个节点原本的位置，如果节点是内部节点，next则没有意义

            Rank keySize;  //number of key
            Rank pointSize; //number of value or child

            bool isLeaf;	//节点类型

            char info[blockSize - node_byte + sizeof(key_t) + sizeof(value_t) + 4];
            //char数组用来存放节点数据，具体存法是交替存
            //叶节点是[value][key][...][value[key]
            //内部节点是[child][key][child][key][...][child][key][child][key][child]
}
```

在文件中，每个节点的信息存在一个块block中，块大小为4kb。首先写入next，然后是keySize和pointSize，最后写入节点具体数据。

#### 3、迭代器(用于区间查询)

```hpp
class iterator {
        private:
            offset addr;
            Rank idx;
}
```

迭代器只支持后移操作，可以返回指向的key和value。addr是指向节点的位置，idx是指向叶子的key或value的下标（秩）。

具体实现是通过一个char数组存储从文件中读取迭代器指向的节点的信息。获得迭代器的方式有调用函数next_iterator或者函数lower_bound。函数check用来判断迭代器是否有效。通过char数组中的信息返回迭代器指向的key或value。

## 三、具体接口：

* 初始化B+树(类似于构造函数)

  `void init(const char *fname)`

  fname:用于存储B+树数据的文件

  如果名为fname的文件已经存在，则打开文件，从文件中读取B+树的基本信息。

  如果文件不存在，则创建一个名为fname的文件，并打开文件。

* 关闭并保存文件
  `void write_back()`
  将B+树的基本信息写入文件，然后关闭文件

* 询问是否有某个key

  `int count(const key_t &Key)`
  如果key存在返回1，不存在返回0

* 根据key查找元素

  `value_t find(const key_t &Key)`

  返回key所对应的value，若key不存在，返回default，即value_t()

* 插入

  `bool insert(const key_t &Key, const value_t &Value)`

  如果插入成功则返回true，插入失败返回false

* 删除

  `bool erase(const key_t &Key)`

  如果删除成功则返回true，插入失败返回false

* 修改

  `bool modify(const key_t &Key, const value_t &Value)`

  如果修改成功则返回true，插入失败返回false

* 删库
  `void clear_file()`

* 返回一个指向大于等于key的迭代器

  `iterator lower_bound(const key_t &Key)`

* 获得下一个迭代器
  `void next_iterator(iterator &it)`

* 返回迭代器指向的key

  `key_t retKey(iterator it)`

* 返回迭代器指向的value

  `value_t retValue(iterator it)`

  

  

  

