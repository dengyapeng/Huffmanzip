#ifndef _Huffman_h
#define _Huffman_h

#include<cstdio>
#include<string.h>
#include<queue>
#include<map>
#include<fstream>
#include<iostream>
#include<cstdlib>
using namespace std;

#define MAX_SIZE 270
#define WRITE_BUFF_SIZE 10
#define PSEUDO_EOF 256


//定义哈夫曼数节点
struct Huffman_node
{
    int id;
    unsigned int freq;
    string code;
    Huffman_node *left,*right,*parent;
    /*参数
      id 采用int变量来表示所有出现的字符
      code 用来存储该字符代表的哈夫曼编码
      左孩子指针,右孩子指针，父亲节点指针
     */
};

typedef Huffman_node* Node_ptr;

class Huffman{
    public:
        //根据输入和输出初始化流对象
        Huffman(string in_file_name,string out_file_name);

        //析构函数
        ~Huffman();

        //压缩文件
        void compress();

        //解压文件
        void decompress();

    private:
        Node_ptr node_array[MAX_SIZE];//哈夫曼节点指针数组
        Node_ptr root;//根节点指针
        int size;//叶子节点数
        fstream in_file,out_file;//输入，输出文件流
        map<int,string>table; //字符->huffman编码映射表

        //定义优先队列比较器
        class Compare{
            public:
                //出现频率少的放在堆顶
                bool operator()(const Node_ptr&a,const Node_ptr&b)const
                {
                    return (*a).freq>(*b).freq;
                }
        };

        //用于比较优选队列元素间的顺序 小根堆p
        priority_queue<Node_ptr,vector<Node_ptr>,Compare> pq;

        /* <--压缩所用到的函数-->*/
        
        //根据输入文件构造包含字符及其频率的数组
        void create_node_array();

        //构造优先队列
        void create_pq();

        //构造Huffman数
        void create_huffman_tree();

        //根据构造好的Huffman树建立映射表
        void create_map_table(const Node_ptr node,bool left);
        //计算Huffman编码
        void calculate_huffman_codes();

        //开始压缩过程
        void do_compress();

        /*<--解压所用到的函数-->*/

        //从huffman编码文件重建huffman树
        void rebuild_huffman_tree();

        //根据重构好的huffman树解码文件
        void decode_huffman();
};

#endif

