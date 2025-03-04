#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freqMap) pq.push(new Node(pair.first, pair.second));
    
    while (pq.size() > 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();
        Node *merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }
    return pq.top();
}

void generateCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) huffmanCode[root->ch] = str;
    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

string encodeString(const string& input, const unordered_map<char, string>& huffmanCode) {
    string encodedStr;
    for (char ch : input) encodedStr += huffmanCode.at(ch);
    return encodedStr;
}

// Function to decode the encoded string using the Huffman Tree
string decodeHuffman(Node* root, const string& encodedStr) {
    string decodedStr;
    Node* current = root;
    for (char bit : encodedStr) {
        current = (bit == '0') ? current->left : current->right;
        // If a leaf node is reached, add the character and reset to the root
        if (!current->left && !current->right) {
            decodedStr += current->ch;
            current = root;
        }
    }
    return decodedStr;
}

void freeTree(Node* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " \"string to encode\"" << endl;
        return 1;
    }
    
    string input = argv[1];
    unordered_map<char, int> freqMap;
    for (char ch : input) freqMap[ch]++;
    
    Node* root = buildHuffmanTree(freqMap);
    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);
    
    string encodedStr = encodeString(input, huffmanCode);
    int encodedSize = encodedStr.size() / 8 + 1;
    cout << endl << "Encoded string: " << encodedStr << endl;
    cout << "Size: " << encodedSize << endl << endl;
    
    string decodedStr = decodeHuffman(root, encodedStr);
    int decodedSize = decodedStr.size();
    cout << "Decoded string: " << decodedStr << endl;
    cout << "Size: " << decodedSize << endl;
    
    freeTree(root);
    return 0;
}
