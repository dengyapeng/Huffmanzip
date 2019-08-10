# Huffmanzip

### 函数调用组织结构图:

    
    
采用哈夫曼编码实现的压缩文件程序

压缩过程
1.通过fstream流操作读取要压缩的文件的所有字符，
使用哈希表(ordered_map)加快了哈夫曼树的构建

![](https://github.com/dengyapeng/Huffmanzip/raw/master/Image/compress.png)
create_pq()根据输入文件构造包含字符及其频率的数组

![](https://github.com/dengyapeng/Huffmanzip/raw/master/Image/decompress.png)



#### 使用方式
      1. git clone 完成后 直接make 生成可执行文件 test
      2. 压缩方法: ./test -c filename1 filename2
          filename1 为被压缩的路径+文件名 filenam2是压缩后的文件路径及其文件名 (不加路径即为当前文件夹)
      3. 解压方法: ./test -x filename2 filename1
          filename2 为之前被压缩过的路径+文件名 filenam1为解压后的文件路径及其文件名 (不添加路径为当前目录)
