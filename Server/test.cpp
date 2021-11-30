#include <iostream>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <vector>
// #include <algorithm>
#define INVALID_CHARACTER '#'

template<typename K_T, typename V_T>
std::ostream& operator<<(std::ostream& stream, std::unordered_map<K_T, V_T>& map)
{
    for (auto& pair : map)
        std::cout << pair.first << ": " << pair.second << std::endl;
    return stream;
}

class BSTNode
{
    BSTNode* parent = NULL, *left = NULL, *right = NULL;
    size_t total_frequency = 0;
    char character = INVALID_CHARACTER;
public:
    BSTNode() = default;
    BSTNode(const BSTNode& other) = default;
    BSTNode& operator=(const BSTNode& other) = default;

    char getCharacter() const
        { return this->character; }

    size_t getTotalFreq() const
        { return this->total_frequency; }

    void setCharacter(const char& chr)
        { this->character = chr; }
    
    void setFrequency(const size_t& freq)
        { this->total_frequency = freq; }

    BSTNode* getLeft() const
        { return this->left; }

    BSTNode* getRight() const
        { return this->right; }

    void setLeft(BSTNode* child)
    {
        this->left = child;
        this->total_frequency += child->getTotalFreq();
    }

    void setRight(BSTNode* child)
    {
        this->right = child;
        this->total_frequency += child->getTotalFreq();
    }
    
    bool operator<(const BSTNode &other) const
        { return this->total_frequency < other.getTotalFreq(); }

    bool operator>(const BSTNode &other) const
        { return this->total_frequency > other.getTotalFreq(); }

    bool operator<=(const BSTNode &other) const
    {
        return this->total_frequency < other.getTotalFreq() ||
            this->total_frequency == other.getTotalFreq();
    }

    bool operator>=(const BSTNode &other) const
    {
        return this->total_frequency > other.getTotalFreq() ||
            this->total_frequency == other.getTotalFreq();
    }

    bool operator==(const BSTNode &other) const
        { return this->total_frequency == other.getTotalFreq(); }

    friend std::ostream& operator<< (std::ostream& stream, const BSTNode* other)
    {
        stream << other->getCharacter() << ": " << other->getTotalFreq();
        return stream;
    } 
};


class MinHeap
{
public:
    
    BSTNode** arrayOfHeap;
    MinHeap (const int& size)
    {
        arrayOfHeap = new BSTNode*[size + 1];
        lastNodeIndex = 0;
    }

    void enqueue (BSTNode* value)
    {
        arrayOfHeap[++lastNodeIndex] = value;
        fix_increased_node();
    }

    BSTNode* dequeue ()
    {
        auto rootValue = arrayOfHeap[1];
        std::swap (arrayOfHeap[1], arrayOfHeap[lastNodeIndex]);
        lastNodeIndex--;
        fix_subtree_root ();
        return rootValue;
    }

    void fix_subtree_root ()
    {
        int movingNodeIndex = 1; // root index
        auto leftChildIndex = movingNodeIndex*2, rightChildIndex = movingNodeIndex*2 + 1; 
        while (leftChildIndex <= lastNodeIndex && rightChildIndex <= lastNodeIndex) // while right child and left child exist
        {
            if (is_in_right_place(movingNodeIndex)) // if movingNode is in the place it must be
            {
                return;
            }
            else if (arrayOfHeap[leftChildIndex]->getTotalFreq() < arrayOfHeap[rightChildIndex]->getTotalFreq())
            {
                std::swap (arrayOfHeap[leftChildIndex], arrayOfHeap[movingNodeIndex]);
                movingNodeIndex = leftChildIndex;
            }
            else
            {
                std::swap (arrayOfHeap[rightChildIndex], arrayOfHeap[movingNodeIndex]);
                movingNodeIndex = rightChildIndex;
            }
            leftChildIndex = movingNodeIndex*2; rightChildIndex = movingNodeIndex*2 + 1;
        }

        if (movingNodeIndex*2 == lastNodeIndex && (arrayOfHeap[movingNodeIndex*2]->getTotalFreq() < arrayOfHeap[movingNodeIndex]->getTotalFreq()))
        {
            std::swap (arrayOfHeap[movingNodeIndex], arrayOfHeap[movingNodeIndex*2]);
        }
    };

    BSTNode* get_root ()
    {
        return arrayOfHeap[1];
    };

private:
    int lastNodeIndex;

    void fix_increased_node()
    {
        auto movingNodeIndex = lastNodeIndex;
        while (movingNodeIndex != 1 && (arrayOfHeap[movingNodeIndex]->getTotalFreq() < arrayOfHeap[movingNodeIndex/2]->getTotalFreq()))
        {
            std::swap (arrayOfHeap[movingNodeIndex], arrayOfHeap[movingNodeIndex/2]);
            movingNodeIndex = movingNodeIndex/2;
        }
    }

    bool is_in_right_place(const int& index)
    {
        return (arrayOfHeap[index] < arrayOfHeap[index*2])
            && (arrayOfHeap[index] < arrayOfHeap[index*2 + 1]);
    }
};

// completely static class
class HuffmanBST
{
public:
    static BSTNode* merge(BSTNode* first, BSTNode* second)
    {
        auto newRoot = new BSTNode;
        if (*first < *second)
        {
            newRoot->setLeft(first);
            newRoot->setRight(second);
        }
        else
        {
            newRoot->setLeft(second);
            newRoot->setRight(first);
        }
        return newRoot;
    }
    static BSTNode* createNew(size_t frequncy, const char& character)
    {
        auto newTree = new BSTNode;
        newTree->setCharacter(character);
        newTree->setFrequency(frequncy);
        return newTree;
    }

    static void printTree(const BSTNode* root)
    {
        using namespace std;
        if (root)
        {
            // int debug;
            // std::cin >> debug;
            cout << root << endl;
            if(root->getLeft())
            {
                cout << "going left: ";
                printTree(root->getLeft());
            }
            if(root->getRight())
            {
                cout << "going right: ";
                printTree(root->getRight());
            }
            cout << "backing ..." << endl;
        }
    }
};


class HuffmanCompressor
{
    std::string text_to_compress;
    std::unordered_map<char, size_t> frequencies;
    std::unordered_map<size_t, std::string> codes;
    // std::priority_queue<BSTNode, std::vector<BSTNode>, std::greater<BSTNode>> queue;
    // MinHeap queue;
public:
    HuffmanCompressor(const std::string& text)
        :text_to_compress(text)
    {}
public:
    std::unordered_map<char, size_t> getFrequencies() const
        { return this->frequencies; }

    std::string compress()
    {
        findFrequencies();
        auto compression_tree = buildTree();
        // generateCodes(&compression_tree);

        return std::string(" ");
    }
    
private:

    void generateCodes(BSTNode* root, std::string code_so_far = "")
    {
        if (!root->getLeft()) // no child -> leaf
        {
            codes[root->getCharacter()] = code_so_far;
        }
        else
        {
            generateCodes(root->getLeft(), code_so_far + '0');
            generateCodes(root->getRight(), code_so_far + '1');
        }
    }

    BSTNode* buildTree()
    {
        MinHeap queue(frequencies.size() + 1);
        for (auto& pair : this->frequencies)
            queue.enqueue(HuffmanBST::createNew(pair.second, pair.first));
        
        using namespace std;
        cout << frequencies << endl;
        
        for (int i = 1; i < frequencies.size() + 1 ;i++)
            cout << queue.arrayOfHeap[i] << endl;
        for (size_t i = 1; i < frequencies.size(); i++)
        {
            auto less = queue.dequeue();
            auto greater = queue.dequeue();
            auto merged = HuffmanBST::merge(less, greater);
            HuffmanBST::printTree(merged);
            queue.enqueue(merged);
        }
        // cout << queue.size() << endl;
        return queue.dequeue();
    }
    
    void findFrequencies()
    {
        for (char chr : this->text_to_compress)
            if (frequencies.find(chr) == frequencies.end())
                frequencies[chr] = 1;
            else
                frequencies[chr]++;
    
    }

};



int main()
{

    std::string input_string;
    std::cout << "Enter the text to compress: " << std::endl;
    std::getline(std::cin, input_string);

    HuffmanCompressor compressor(input_string);
    auto compressed_string = compressor.compress();
    std::cout << "The compressed string:\n" << compressed_string << std::endl;
    

}