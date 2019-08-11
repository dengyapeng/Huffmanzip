# Huffmanzip
### 采用哈夫曼编码实现的压缩文件程序

### 函数调用组织结构图:

![](https://github.com/dengyapeng/Huffmanzip/raw/master/Image/compress.png)


![](https://github.com/dengyapeng/Huffmanzip/raw/master/Image/decompress.png)  


压缩过程
1.通过fstream流操作读取要压缩的文件的所有字符，使用哈希表(ordered_map)加快了哈夫曼树的构建。

2.create_pq()根据输入文件，进行**字符频数统计**，读文件一般按字节读入。每个字节是8bit，2^8 =256种可能，将**char(表示范围-128至127)转为unsigned char(0-255)** ，记录至**频率数组freq中**。

3.构造过程可以使用**优先队列(priority_queue)辅助节点的选择**，可以**自定义排序规则**，每次从优先队列中取出的值都是**权值最小的节点**，之后new一个新的节点作为两个最小节点的父亲节点，然后新的节点再次push进入优先队列。

4.之后得到**Huffman树后**，根据其得到**对应的Huffman编码表**，使用**unordered_map容器存取**，加快搜索速度。

5.最后根据编码表将文件编码，**写入到输出文件**。
但是**仅有这些信息，解压时无法获取哈夫曼树信息，因此无法进行Huffman编码的decode**。所以我们不仅需要把Huffman编码写入文件，也需要将Huffman树也存入到压缩文件中。这样我们在解压缩将根据这段信息重构出huffman树。

在输出文件中添加生成Huffman树的数据信息-所有字符及其对应编码，和字符的种类数
为了告知解压程序何时结束解码工作，除了在输出文件中添加上述信息外，可以再加入一个**pseudo-eof伪结束符。**


#### 使用方式
      1. git clone 完成后 直接make 生成可执行文件 test
      2. 压缩方法: ./test -c filename1 filename2
          filename1 为被压缩的路径+文件名 filenam2是压缩后的文件路径及其文件名 (不加路径即为当前文件夹)
      3. 解压方法: ./test -x filename2 filename1
          filename2 为之前被压缩过的路径+文件名 filenam1为解压后的文件路径及其文件名 (不添加路径为当前目录)
[参考博客](https://blog.csdn.net/Small_Hacker/article/details/52843738 )
