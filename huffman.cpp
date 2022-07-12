#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

Node* root = NULL;

Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

struct comp
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};

void encode(Node* root, string str,
			unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}

	if (!root->left && !root->right)
	{
		cout << root->ch;
		return;
	}

	index++;

	if (str[index] =='0')
		decode(root->left, index, str);
	else
		decode(root->right, index, str);
}

void buildHuffmanTree(string text)
{
	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	priority_queue<Node*, vector<Node*>, comp> pq;

	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}
	root = pq.top();
}

int main()
{
    string text = "";
	int char_count = 0, code_count = 0;

	cout << "Enter Text to Compress: ";
    getline(cin, text);

    buildHuffmanTree(text);
    unordered_map<char, string> huffmanCode;

    cout << "\nOriginal Text:\n" << endl << text << endl;

    cout << "\nHuffman Codes:\n" << endl;
    encode(root, "", huffmanCode);
    for (auto pair: huffmanCode)
    {
        cout << pair.first << " : " << pair.second << '\n';
    }


    cout << "\nEncoded Text: \n" << endl;
    string str = "";
    for (char ch: text)
    {
        str += huffmanCode[ch];
    }
    cout << str << endl;

    cout << "\nDecoded Text: \n" << endl;
    int index = -1;
    while (index < (int)str.size() - 2)
    {
        decode(root, index, str);
    }

    cout << "\n\nInfo: \n" << endl;
    for (char ch: str)
    {
        code_count += 1;
    }
    for (char ch: text)
    {
        char_count += 1;
    }
    cout << "Original Text Size: " << char_count*8 << " bits" << endl;
    cout << "Encoded Text Size: " << code_count << " bits" << endl;
    float c_ratio = (float)(char_count*8)/(float)code_count;
    cout << "Compression Ratio: " << c_ratio << endl;
    float percentage = ((float)((char_count*8) - code_count)/(float)(char_count*8)) * 100;
    cout << "Compression Percentage: " << percentage << endl;

    return 0;
}
