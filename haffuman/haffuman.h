#ifndef _HADDUMAN_H
#define _HADDUMAN_H
#include<map>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<queue>
#include<algorithm>
#include<cstring>
#include<sstream>
using namespace std;

const int MAXCHARS =  256;
const int CHAR_BITS = 8;
const int BUF_SIZE  = 4096;
const unsigned char mask[CHAR_BITS] = {
    0x80,
    0x40,
    0x20,
    0x10,
    0x08,
    0x04,
    0x02,
    0x01
};

struct HaffumanNode;
typedef struct HaffumanNode HFN;

struct KeyValue {
    
    int key;
    void *value;
  //  KeyValue& operator=(const KeyValue &rhs) {
    //    key = rhs.key;
      //  value = rhs.value;
        //return *this;
    //}
};

bool operator < (const KeyValue& lhs, const KeyValue& rhs)  {
    return lhs.key > rhs.key;
}

typedef KeyValue* kv_ptr;

struct HaffumanNode {

    char _ch;
    int _count;
    struct HaffumanNode *_left;
    struct HaffumanNode *_right;

};

KeyValue 
*getKeyValue(int key, void *value) {
    
    KeyValue *kv = new KeyValue;
    kv->key = key;
    kv->value = value;
    return kv;
}

HaffumanNode
*getHFN(char ch, int count) {
    
    HaffumanNode *hfn = new HaffumanNode;
    hfn->_ch = ch;
    hfn->_count = count;
    hfn->_left = hfn->_right = NULL;
    return hfn;
}

class compare {

public:
    bool operator() (const kv_ptr& l, const kv_ptr& r) {
        return (*l).key > (*r).key;
    }
};

class HaffumanTree {

public:
    explicit HaffumanTree(string &);
    HaffumanTree(string&, int);
    ~HaffumanTree(){
        //haffuman_free(haffuman);
    };  // just test   
    void compress(string&);
    void decompress();
private:
    //std::vector<std::string> file;
    void _generate_statistic(string&);
    void _generate_haffuman_tree();
    void _generate_dictionary(HFN *, char *, int);
   
    void _read_statistic();
//    void haffuman_free(HFN *);
    int statistic[MAXCHARS];
    //map<int, char> dictionary;
    int chars_sum;
    long chars_total;
    HFN *haffuman;
    priority_queue<kv_ptr, vector<kv_ptr>, compare > pq;
    string dictionary[MAXCHARS];
    fstream in_file;
    ofstream out_file;
    string in_file_name;
    string out_file_name;
};


HaffumanTree::HaffumanTree(string &in):in_file_name(in) {
   
    //haffuman = new HaffumanNode;
    chars_sum = chars_total = 0;
    _generate_statistic(in_file_name);
    _generate_haffuman_tree();
    char path[25];
    _generate_dictionary(haffuman, path, 0);
    compress(in_file_name);
}

HaffumanTree::HaffumanTree(string &in, int i) : in_file_name(in) {
    
    chars_sum = chars_total = 0;
    _read_statistic();
    _generate_haffuman_tree();
    char path[25];
    _generate_dictionary(haffuman, path, 0);
    decompress();
}

void HaffumanTree::_generate_statistic(string &is) {
    
    memset(statistic, 0 , sizeof(statistic));
    unsigned char ch;
    ifstream in(is.c_str(), ifstream::in|ifstream::binary);
    string word;
    while (getline(in, word)) {
        size_t len = word.length();
        for (size_t i = 0; i < len; ++i) {
            if (statistic[word[i]] == 0)
                chars_sum++;
            statistic[word[i]] += 1;
            chars_total++;
        }
    }
    in.close();
}

void HaffumanTree::_generate_haffuman_tree() {
    
    HaffumanNode *hn, *parent, *left, *right;
    kv_ptr kv, kv1, kv2;

    for (int i = 0; i < MAXCHARS; ++i) {
        if (statistic[i]) {
            hn = getHFN((char)i, statistic[i]);
            kv = getKeyValue(statistic[i], hn);
            pq.push(kv);
        }
    }

    while (!pq.empty()) {

        kv_ptr tmp;
        kv1 = pq.top();
        //kv1 = getKeyValue(tmp->key, getHFN(tmp->key, 
         //           static_cast<HaffumanNode *>(tmp->value)->_count));
        pq.pop();

        if (pq.empty()) {
            //haffuman = getHFN(0, 0);
            haffuman = static_cast<HaffumanNode *>(kv1->value);
            break;
        }
        else {
            
            kv2 = pq.top();
            //kv2 = getKeyValue(tmp->key, getHFN(tmp->key, 
            //static_cast<HaffumanNode *>(tmp->value)->_count));
            pq.pop();
            
            left = static_cast<HaffumanNode *>(kv1->value);
            right = static_cast<HaffumanNode *>(kv2->value);
            int count = left->_count + right->_count;
            //delete left;
            //delete right;
            parent = getHFN(0, count);
            parent->_left = left;
            parent->_right = right;
            kv = getKeyValue(count, parent);
            pq.push(kv);
        }
    } // end while

}

void HaffumanTree::_generate_dictionary(HFN *hf, char path[], int depth) {
   
    char *code = NULL;
    if (hf) {
        if (hf->_left == NULL && hf->_right == NULL) {
            //path[depth] = '1';
            code = (char *)malloc(sizeof(char)*(depth + 1));
            memset(code, 0 , sizeof(char)*(depth + 1));
            memcpy(code, path, depth);
            dictionary[static_cast<unsigned char>(hf->_ch)] = code; 
            cout << "code: " << code << " hf->_ch " << hf->_ch << endl;
        }
        if (hf->_left) {
            path[depth] = '0';
            cout << "0" << " " << endl;
            _generate_dictionary(hf->_left, path, depth + 1);
        }
        if (hf->_right) {
            path[depth] = '1';
            cout << "1" << " " << endl;
            _generate_dictionary(hf->_right, path, depth + 1);

        }

    }
}

/*
void HaffumanTree::haffuman_free(HFN *hfn) {
 
    if (hfn) {
        if (hfn->_left) {
            HFN *tmp = hfn->_left;
            haffuman_free(hfn->_left);
            delete tmp;
        }
        if (hfn->_right) {
            HFN *tmp = hfn->_right;
            haffuman_free(hfn->_right);
            delete tmp;
        }
    delete hfn;
    }   
}
*/

void HaffumanTree::compress(string& in) {

    ofstream out((in_file_name+".haf").c_str());    
    if (out) {
        out << chars_sum << endl;
        for (int i = 0; i < MAXCHARS; ++i) {
            if (statistic[i] != 0)
                out << i << ":" << statistic[i] << " ";
            }
        out << endl << '~' << endl;
        unsigned char bitBuf = 0x00;
        int bitPos = 0;

        ifstream inFile(in.c_str(), ifstream::binary|ifstream::in);
        //in_file.open(in_file_name.c_str(), ios::in|ios::binary);
        string text;
        while(getline(inFile, text)) {
            //in_file.read(inBuf, BUF_SIZE);
            
            size_t len = text.length();
            for (size_t i = 0; i < len; ++i) {    
                string code = dictionary[text[i]];
                size_t l = code.length();
                for (int j = 0; j < l; ++j) {
                    bitBuf <<= 1;   // 1
                    if (code[j] == '1')
                        //bitBuf |= mask[bitPos++];
                        bitBuf |= 1; // 1
                   // else
                    bitPos++;

                    if (bitPos == CHAR_BITS) {
                        out << bitBuf;
                        bitBuf = 0x00;
                        bitPos = 0;
                    }
                }
            } // end for
            //memset(inBuf, 0 , sizeof(inBuf));
            //in_file.read(inBuf, BUF_SIZE);
        } // end while 
        if (bitPos != 0)
        {
            bitBuf <<= (8 - bitPos);
            out << bitBuf;
        } 
        //   out << bitBuf;

    } else {
        cout << "open file error!";
        return;
    }
    out.close();
}

void HaffumanTree::_read_statistic() {
    
    memset(statistic, 0, sizeof(statistic));
    ifstream in(in_file_name.c_str());
    string line, word;
    getline(in, line);
    chars_sum = atoi(line.c_str());
    getline(in, line);
    istringstream is(line);
    while (is >> word) {
        size_t pos = word.find(":");
        int i = atoi(word.substr(0, pos).c_str());
        int j = atoi(word.substr(pos + 1, word.length() - pos - 1).c_str());
        chars_total += j;
        statistic[i] = j;
        //cout << i << ":" << j << " ";
    }
}

void HaffumanTree::decompress() {
    
    size_t i  = in_file_name.rfind("haf");    
    out_file_name = in_file_name.substr(0 , i - 1);
    ofstream out(out_file_name.c_str());
    ifstream in(in_file_name.c_str(), ifstream::binary|ifstream::in);
    HFN *hfn = haffuman;
    unsigned char bitCode = 0x00, ch;
    int bitPos = 0;
    string word;
    for (size_t i = 0; i < 3; ++i)
    {
        getline(in, word);
    }
    while (getline(in, word)) {
        
        int len = word.length();
        for (size_t j = 0; j < len; ++j) {
            
            ch = word[j];
            for (size_t m = CHAR_BITS; m>0; --m ){
                int t = (ch >> (m - 1)) & 1;
                if (hfn->_left == NULL && hfn->_right == NULL) {
                    if (chars_total > 0) {
                        out << hfn->_ch;
                        chars_total--;
                    }
                    hfn = haffuman;
                } 
                if (t == 1)
                    hfn = hfn->_right;
                else 
                    hfn = hfn->_left;
            }
        }
    }
    in.close();
    out.close();
}
#endif
