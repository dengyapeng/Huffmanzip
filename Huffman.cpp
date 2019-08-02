#include"Huffman.h"

Huffman::Huffman(std::string in_file_name,std::string out_file_name)
{
    //打开文件用于读取数据
    in_file.open(in_file_name.c_str(),ios::in);

    if(!in_file)
    {
       printf("open error,path is%s\n",in_file_name.c_str());
       exit(1);
    }

    //打开文件用于写入
    out_file.open(out_file_name.c_str(),ios::out);
    if(!out_file)
    {
        printf("Open file error,path is:%s\n",out_file_name.c_str());
        exit(1);
    }
}

Huffman::~Huffman()
{
    in_file.close();
    out_file.close();
}

void Huffman::compress()
{
    create_pq();
    create_huffman_tree();
    calculate_huffman_codes();
    do_compress();
}

void Huffman::decompress()
{
    rebuild_huffman_tree();
    decode_huffman();
}


void Huffman::create_node_array()
{
    int i,count;
    int freq[MAX_SIZE]={0};//频数统计数组
    char in_char;

    //依次读入字符，统计数据 一个字符为单位
    while(!in_file.eof())
    {
        in_file.get(in_char);

        //消除最后一行的影响
        if(in_file.eof())
        {
            break;
        }

        //char 转换为unsigned char作为数组下标
        freq[(unsigned char)in_char]++;
    }

    count=0;
    for(i=0;i<MAX_SIZE;++i)
    {
        if(freq[i]<=0)
        {
            continue;
        }

        //创建一个哈夫曼树节点赋值后放入节点数组中
        Node_ptr node  = new Huffman_node();
        node->id=i;
        node->freq=freq[i];
        node->code="";
        node->left=NULL;
        node->right=NULL;
        node->parent=NULL;
        node_array[count++]=node;
    }
    //seudo-eof也需要Huffman编码，这样我们解码时在遇到这个“字符”时就知道解码可以结束了
    Node_ptr node = new Huffman_node();
    node->id=PSEUDO_EOF;
    node->freq=1;
    node->code="";
    node->left=NULL;
    node->right=NULL;
    node_array[count++]=node;

    size=count;
}

void Huffman::create_pq()
{
    int i;
    create_node_array();
    for(i=0;i<size;++i)
    {
        pq.push(node_array[i]);
    }
}

void Huffman::create_huffman_tree()
{
    root =NULL;
    while(!pq.empty())
    {
        Node_ptr first =pq.top();
        pq.pop();

        //如果优先队列中取出一个节点后为空,那个root节点为firsst
        if(pq.empty())
        {
            root=first;
            break;
        }
        
        //合并两个较小节点为一个节点后压入优先队列
        Node_ptr second =pq.top();
        pq.pop();
        Node_ptr new_node = new Huffman_node();
        new_node->freq=first->freq+second->freq;
       
        //较小的作为左子树
        if(first->freq<=second->freq)
        {
            new_node->left=first;
            new_node->right=second;
        }

        else{
            new_node->left=second;
            new_node->right=first;
        }
        
        first->parent=new_node;
        second->parent=new_node;
        pq.push(new_node);
    }
}

void Huffman::create_map_table(const Node_ptr node,bool left)
{
    if(left)
        node->code=node->parent->code+"0";
    else
        node->code=node->parent->code+"1";

    //如果递归到叶子节点，code完整,将其加入编码表
    if(node->left==NULL&&node->right==NULL)
        table[node->id]=node->code;
    else{
        if(node->left!=NULL)
            create_map_table(node->left,true);
        if(node->right!=NULL)
            create_map_table(node->right,false);
    }
}

void Huffman::calculate_huffman_codes()
{
    if(root==NULL)
    {
        printf("Build huffman tree failed \n");
        exit(1);
    }
    if(root->left!=NULL)
    {
        create_map_table(root->left,true);
    }
    if(root->right!=NULL)
    {
        create_map_table(root->right,false);
    }
}

void Huffman::do_compress()
{
    int i,j;
    int length;
    char in_char;
    unsigned char out_c,tmp_c;
    string code,out_string;
    map<int,string>::iterator table_it;

    //按节点总数(包括pseudo-EOF)+哈夫曼树映射关系 +哈夫曼编码来写入文件
    
    //第一行写入节点总数
    out_file<<size<<endl;
    
    //第2-(size+1) 行写入huffman树，即每行写入字符+huffman编码 ,如"43 00100"
    for(table_it=table.begin();table_it!=table.end();++table_it)
    {
        out_file<<table_it->first<<" "<<table_it->second<<endl;
    }

    //第size+2行写入huffman编码
    code.clear();//code 设置为空
    
    /*fstream当读取完整个文件之后如果再想读取一遍,必须先clear清除状态,seekg()函数把读指针重定位到文件开头。*/
    in_file.clear();
    in_file.seekg(ios::beg);
    
    while(!in_file.eof())
    {
        in_file.get(in_char);
        //消除最后一行回车的影响
        if(in_file.eof())
        {
            break;
        }

        //找到每个字符所对应的huffman编码
        table_it = table.find((unsigned char)in_char);
        if(table_it!=table.end())
            code+=table_it->second;
        else{
            printf("Can't find the huffman code of character %X\n",in_char);
            exit(1);
        }

        //当总编码长度大于预设的WRITE_BUFF_SIZE 时再写入文件
        length = code.length();
        if(length>WRITE_BUFF_SIZE)
        {
            out_string.clear();
            //将huffman的01编码以二进制流写入输出文件
            for(i=0;i+7<length;i+=8)
            {
                //每八位01 转化为一个unsignded char 输出
                //不使用char，如果使用char,在移位操作的时候符号位会影响结果
                //另外char 和unsigned char 相互转化二进制位并不变
                out_c =0;
                for(j=0;j<8;j++)
                {
                    if('0'==code[i+j])
                        tmp_c=0;
                    else
                        tmp_c=1;
                    out_c +=tmp_c<<(7-j);
                }
                out_string+=out_c;
            }
            out_file<<out_string;
            code=code.substr(i,length-i);
        }
    }

    //已经读完所有文件,先插入pseudo-EOF
    table_it = table.find(PSEUDO_EOF);
    if(table_it!=table.end())
    {
        code+=table_it->second;
    }
    else{
        printf("Can't find huffman code of pseudo-EOF\n");
        exit(1);
    }

    //再处理尾部剩余的huffman编码
    length = code.length();
    out_c =0;
    for(i=0;i<length;++i)
    {
        if('0'==code[i])
            tmp_c=0;
        else
            tmp_c=1;
        out_c +=tmp_c<<(7-(i%8));
        if(0==(i+1)%8||i==length-1)
        {
            //每8位写入一次文件
            out_file<<out_c;
            out_c=0;
        }
    }
}

void Huffman::rebuild_huffman_tree()
{
    int i,j;
    int id,length;
    string code;
    Node_ptr node,tmp,new_node;
    
    root = new Huffman_node();
    root->left=NULL;
    root->right=NULL;
    root->parent=NULL;
    
    in_file.clear();
    in_file.seekg(ios::beg);
    //读入size 即叶子节点的总个数
    in_file>>size;
    if(size>MAX_SIZE)
    {
        printf("The number of nodes is not valid,maybe the compressed file has been broken.\n");
        exit(1);
    }
   
    for(i=0;i<size;++i)
    {
        in_file>>id;
        in_file>>code;

        length=code.length();
        node=root;
        for(j=0;j<length;++j)
        {
            if('0'==code[j])
            tmp=node->left;
            else if('1'==code[j])
            tmp=node->right;
            else{
                printf("Decode error \n");
                exit(1);
            }

            //如果到了空，则新建一个节点
            if(tmp==NULL)
            {
                new_node = new Huffman_node();
                new_node->left=NULL;
                new_node->right=NULL;
                new_node->parent=node;

                //如果是最后一个0或1，说明到了叶子节点，给叶子节点赋予相关的值
                if(j==length-1)
                {
                    new_node->id =id;
                    new_node->code=code;
                }
                if('0'==code[j])
                    node->left=new_node;
                else
                    node->right=new_node;
                
                tmp  = new_node;
            }

            //如果不为空，切到了该huffman编码的最后一位，这里却已经存在了一个节点
            //说明之前的huffman编码有问题
            else if(j==length-1)
            {
                printf("Huffman code is not valid, maybe the compressed file has been broken.\n");
                exit(1);
            }
            //如果不为空，但节点却已经是叶子节点，huffman编码也不对
            else if(tmp->left==NULL&&tmp->right==NULL)
            {
                printf("Huffman code is not valid, maybe the compressed file has been broken.\n");
                exit(1);
            }
            node= tmp;
        }
    }
}

void Huffman::decode_huffman()
{
        int i,id;
        bool pseudo_eof;
        char in_char;
        string out_string;
        unsigned char u_char,flag;
        Node_ptr node;

        out_string.clear();
        node =root;
        pseudo_eof=false;
        in_file.get(in_char);//跳过编码行上一行的换行
        while(!in_file.eof())
        {
            in_file.get(in_char);
            u_char=(unsigned char)in_char;
            flag=0x80;//10000000
             for(i = 0; i < 8; ++i)
             {
                    if(u_char&flag)
                    {
                        node=node->right;
                    }
                    else{
                        node=node->left;
                    }

                    //当遍历到叶子节点
                    if(node->left==NULL&&node->right==NULL)
                    {
                        id = node->id;
                        if(id==PSEUDO_EOF)
                        {
                            pseudo_eof=true;
                            break;
                        }
                        else{
                            //int to char 是安全的高位会被截断
                            out_string+=(char)(node->id);
                            node = root;
                        }
                    }
                    flag = flag>>1;
             }
             if(pseudo_eof)
                 break;
                
             if(WRITE_BUFF_SIZE<out_string.length())
             {
                 out_file<<out_string;
                 out_string.clear();
             }
        }
        if(!out_string.empty())
        {
            out_file<<out_string;
        }
        
}
