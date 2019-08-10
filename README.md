# Huffmanzip

### 函数调用组织结构图:
    ![](http://github.com/dengyapeng/Huffmanzip/tree/master/Image/compress.png)
采用哈夫曼编码实现的压缩文件程序

压缩过程
1.通过fstream流操作读取要压缩的文件的所有字符，
使用哈希表(ordered_map)加快了哈夫曼树的构建
