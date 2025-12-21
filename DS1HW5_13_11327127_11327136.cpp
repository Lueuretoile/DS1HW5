// 資訊二甲 11327136 陳彥均 11327127 林煒博
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct Pokemon {
  int id;
  string name;
  string type1;
  string type2;
  int total;
  int HP;
  int attack;
  int defense;
  int spAtk;
  int spDef;
  int speed;
  int generation;
  bool legendary;
};
extern vector<Pokemon> pokemonDatabase;
struct BSTNode {
  int hp;
  vector<int> pokemonIDs;
  BSTNode* left;
  BSTNode* right;

  BSTNode(int h, int id) : hp(h), left(nullptr), right(nullptr) {
    pokemonIDs.push_back(id);
  }
};

class BST {
 public:
  BST() : root_m(nullptr) {}
  ~BST() {deleteSubtree(root_m);}

  void clear();   // 清空BST
  void insert(int hp, int id);    // public插入函數
  int getHeight() const;
  void rangeSearch(int min, int max, vector<int>& results, int& visitedCount) const;


  vector<int> deleteMin();
  vector<int> deleteMax();
  bool isEmpty() const {return root_m == nullptr;};   // BST是否為空

  void buildBalancedTree();   // public build balanced tree
  void printLevelOrder();
 private:
  BSTNode* root_m;
  void deleteSubtree(BSTNode* t);   // Recursively delete all nodes in the subtree
  BSTNode* insert(BSTNode* t, int hp, int id);    // private插入函數
  void rangeSearch(BSTNode* t, int min, int max, int &count, vector<int>& results) const;   // private範圍搜尋函數

  BSTNode* findMin(BSTNode* t) const;   // 尋找最小節點(left)
  BSTNode* findMax(BSTNode* t) const;   // 尋找最大節點（right）

  BSTNode* removeMin(BSTNode* t, vector<int>& deletedIDs);    // 移除最小節點
  BSTNode* removeMax(BSTNode* t, vector<int>& deletedIDs);    // 移除最大節點
  int height(BSTNode* t) const;
  void inorderCollect(BSTNode* t);   // 走訪收集所有節點
  BSTNode* buildBalanced(int left, int right);    // build balanced tree
  vector<BSTNode*> inorderNodes;
};

void BST::deleteSubtree(BSTNode* t) {
  if (t != nullptr) {
    deleteSubtree(t->left);
    deleteSubtree(t->right);
    delete t;
  }
}

void BST::clear() {
  deleteSubtree(root_m);
  root_m = nullptr;
}

BSTNode* BST::insert(BSTNode* t, int hp, int id) {
  if (t == nullptr) {
    return new BSTNode(hp, id);
  }

  if (hp < t->hp) {
    t->left = insert(t->left, hp, id);
  } else if (hp > t->hp) {
    t->right = insert(t->right, hp, id);
  } else {
    // HP相同,加入到同一節點的pokemonIDs中
    t->pokemonIDs.push_back(id);
  }
  return t;
}

void BST::insert(int hp, int id) {
  root_m = insert(root_m, hp, id);
};

int BST::height(BSTNode* t) const {
  if (t == nullptr) {
    return 0;
  }
  int leftHeight = height(t->left);
  int rightHeight = height(t->right);
  return 1 + max(leftHeight, rightHeight);
}

  int BST::getHeight() const {
    return height(root_m);
  };

BSTNode* BST::findMin(BSTNode* t) const {
  if (t == nullptr) {
    return nullptr;
  }
  if (t->left == nullptr) {
    return t;
  }
  return findMin(t->left);
}

BSTNode* BST::findMax(BSTNode* t) const {
  if (t == nullptr) {
    return nullptr;
  }
  if (t->right == nullptr) {
    return t;
  }
  return findMax(t->right);
}

BSTNode* BST::removeMin(BSTNode* t, vector<int>& deletedIDs) {
  if (t == nullptr) {
    return nullptr;
  }
  
  if (t->left == nullptr) {
    // 找到最小
    deletedIDs = t->pokemonIDs;
    BSTNode* rightChild = t->right;
    delete t;
    return rightChild;
  }
  
  t->left = removeMin(t->left, deletedIDs);
  return t;
}

BSTNode* BST::removeMax(BSTNode* t, vector<int>& deletedIDs) {
  if (t == nullptr) {
    return nullptr;
  }
  
  if (t->right == nullptr) {
    // 找到最大
    deletedIDs = t->pokemonIDs;
    BSTNode* leftChild = t->left;
    delete t;
    return leftChild;
  }
  
  t->right = removeMax(t->right, deletedIDs);
  return t;
}

vector<int> BST::deleteMin() {
  vector<int> deletedIDs;
  if (root_m != nullptr) {
    root_m = removeMin(root_m, deletedIDs);
  }
  return deletedIDs;
}

vector<int> BST::deleteMax() {
  vector<int> deletedIDs;
  if (root_m != nullptr) {
    root_m = removeMax(root_m, deletedIDs);
  }
  return deletedIDs;
}

void BST::rangeSearch(BSTNode* t, int min, int max, int &count, vector<int>& results) const {
  if (t == nullptr) {
    return;
  }
  count++;
  // if current node HP > min, search left
  if (t->hp > min) {
    rangeSearch(t->left, min, max, count, results);
  }
  
  // 如果當前節點在範圍內,加入結果
  if (t->hp >= min && t->hp <= max) {
    for (int id : t->pokemonIDs) {
      results.push_back(id);
    }
  }
  
  // if current node HP < max,search right
  if (t->hp < max) {
    rangeSearch(t->right, min, max, count, results);
  }
}

void BST::rangeSearch(int min, int max, vector<int>& results, int& visitedCount) const {
  visitedCount = 0;
  results.clear();
  rangeSearch(root_m, min, max, visitedCount, results);
}

vector<BSTNode*> inorderNodes;
void BST::inorderCollect(BSTNode* t) {
  if (t == nullptr) return;
  inorderCollect(t->left);
  inorderNodes.push_back(t);
  inorderCollect(t->right);
}

BSTNode* BST::buildBalanced(int left, int right) {
  if (left > right) return nullptr;

  int mid = (left + right) / 2;
  BSTNode* root = inorderNodes[mid];

  root->left = buildBalanced(left, mid - 1);
  root->right = buildBalanced(mid + 1, right);

  return root;
}

void BST::buildBalancedTree() {
  inorderNodes.clear();
  inorderCollect(root_m);

  if (inorderNodes.empty()) return;

  root_m = buildBalanced(0, inorderNodes.size() - 1);
}

void BST::printLevelOrder() {
  if (root_m == nullptr) return;

  cout << "HP tree:" << endl;

  vector<BSTNode*> q;
  q.push_back(root_m);

  int level = 1;

  while (!q.empty()) {
    int size = q.size();
    cout << "<level " << level << "> ";

    for (int i = 0; i < size; i++) {
      BSTNode* cur = q[i];

      // (HP, id|id|id)
      cout << "(" << cur->hp << ", ";
      for (size_t k = 0; k < cur->pokemonIDs.size(); k++) {
        int idx = cur->pokemonIDs[k];
        cout << pokemonDatabase[idx].id;
        if (k + 1 < cur->pokemonIDs.size()) cout << "|";
      }
      cout << ") ";

      if (cur->left) q.push_back(cur->left);
      if (cur->right) q.push_back(cur->right);
    }

    cout << endl;
    q.erase(q.begin(), q.begin() + size);
    level++;
  }
}

void task1();
void task2();
void task3();
void task4();

void ReadInput(int &command) {
  cout << "Input a choice(0, 1, 2, 3, 4): ";
  if (!(cin >> command)) {
    cin.clear();
    string dummy;
    getline(cin, dummy);
    return;
  }

  if (command < 0 || command > 4) {
    cout << endl << "Command does not exist!" << endl << endl;
    return;
  }
};
string getInputFileID() {  
  string id;
  cout << endl << "Input a file number [0: quit]: ";
  cin >> id;
  if (id == "0") {
    return "";
  }
  
  return id;
}

void showMenu() {
  cout << "*** (^_^) Data Structure (^o^) ***" << endl;
  cout << "** Binary Search Tree on Pokemon *" << endl;
  cout << "* 0. QUIT                        *" << endl;
  cout << "* 1. Read a file to build HP BST *" << endl;
  cout << "* 2. Range search on HP field    *" << endl;
  cout << "* 3. Delete the min on HP field  *" << endl;
  cout << "* 4. Rebuild the balanced HP BST *" << endl;
  cout << "**********************************" << endl;
};
// 全域變數
vector<Pokemon> pokemonDatabase;
BST bst;
bool deleteMinFlag = true;

int main() {
  int command = 0;
  do {
    showMenu();
    ReadInput(command);
    switch (command) {
      case 1: task1(); break;
      case 2: task2(); break;
      case 3: task3(); break;
      case 4: task4(); break;
      case 0: return 0;
    }
  } while (command != 0);  
  return 0;
}

void task1() {
  string fileID = getInputFileID();
  if (fileID.empty()) {
    return;
  }
  string inputFileName = "input" + fileID + ".txt";
  ifstream inFile(inputFileName);
  if (!inFile) {
    cout << endl << "### " << inputFileName << " does not exist! ###" << endl << endl;
    return;
  }

  // 清空之前的資料
  pokemonDatabase.clear();
  bst.clear();
  string headerLine;
  getline(inFile, headerLine);
  
  // 讀取所有寶可夢資料
  string line;
  int index = 0;
  while (getline(inFile, line)) {
    if (line.empty()) continue;
    Pokemon pokemon;
    int fieldCount = 0;
    size_t start = 0;
    size_t pos = 0;
    
    while (pos < line.length() && fieldCount < 13) {
      pos = line.find('\t', start);
      if (pos == string::npos) {
        pos = line.length();
      }
      
      string field = line.substr(start, pos - start);
      
      switch (fieldCount) {
        case 0: pokemon.id = stoi(field); break;
        case 1: pokemon.name = field; break;
        case 2: pokemon.type1 = field; break;
        case 3: pokemon.type2 = field; break;
        case 4: pokemon.total = stoi(field); break;
        case 5: pokemon.HP = stoi(field); break;
        case 6: pokemon.attack = stoi(field); break;
        case 7: pokemon.defense = stoi(field); break;
        case 8: pokemon.spAtk = stoi(field); break;
        case 9: pokemon.spDef = stoi(field); break;
        case 10: pokemon.speed = stoi(field); break;
        case 11: pokemon.generation = stoi(field); break;
        case 12: pokemon.legendary = (field == "TRUE"); break;
      }
      fieldCount++;
      start = pos + 1;
    }
    
    pokemonDatabase.push_back(pokemon);
    // 插入到 BST
    bst.insert(pokemon.HP, index);
    index++;
  }
  inFile.close();
  cout << "\t#\tName\t\t\tType 1\t\tHP" << endl;
  for (size_t i = 0; i < pokemonDatabase.size(); i++) {
    cout << "[" << setw(3) << (i+1) << "]\t"
         << pokemonDatabase[i].id << "\t"
         << pokemonDatabase[i].name;
    if (pokemonDatabase[i].name.length() < 8) cout << "\t\t\t";
    else if (pokemonDatabase[i].name.length() > 16) cout << "\t";
    else cout << "\t\t";
    cout << pokemonDatabase[i].type1;
    if (pokemonDatabase[i].type1.length() >= 8) cout << "\t";
  else cout << "\t\t";
    cout << pokemonDatabase[i].HP << "\t" << endl;
  }
  cout << "HP tree height = " << bst.getHeight() << endl << endl;
};

int task2_RangeInput() {
  int inputHP = -1;
  while (true) {
    cout <<  endl << "Input a non-negative integer: ";
    if (cin >> inputHP) {
      if (inputHP >= 0) {
        return inputHP;
        break;
      }
      cout << endl << "### It is NOT a non-negative integer. ###" << endl;
      cout << "Try again: ";
    } else {
      cin.clear();
      string dummy;
      getline(cin, dummy);
      cout << endl << "### It is NOT a non-negative integer. ###" << endl;
      cout << "Try again: ";
    }
  }
};

void task2() {
  if (pokemonDatabase.empty()) {
    cout << endl << "----- Execute Mission 1 first! -----" << endl << endl;
    return;
  }
  int minHP, maxHP;
  minHP = task2_RangeInput();
  maxHP = task2_RangeInput();
  // 範圍檢索
  vector<int> results;
  int visitedCount = 0;
  bst.rangeSearch(minHP, maxHP, results, visitedCount);

  if (results.empty()) {
    cout << "No record was found in the specified range." << endl;
    cout << "Number of visited nodes = " << visitedCount << endl << endl;
    return;
  }
  // 冒泡排序 HP 遞減排序
  for (size_t i = 0; i < results.size(); i++) {
    for (size_t j = 0; j < results.size() - 1 - i; j++) {
      int hpJ = pokemonDatabase[results[j]].HP;
      int hpJPlus1 = pokemonDatabase[results[j + 1]].HP;
      
      // 如果後面的 HP 大於前面的，交換（遞減）
      if (hpJPlus1 > hpJ) {
        int temp = results[j];
        results[j] = results[j + 1];
        results[j + 1] = temp;
      }
    }
  }
  // output :
  cout << "\t#\tName\t\t\tType 1\t\tTotal\tHP\tAttack\tDefense" << endl;
  for (size_t co = 0; co < results.size(); co++) {
    int idx = results[co];
    Pokemon& p = pokemonDatabase[idx];
    cout << "[" << setw(3) << (co + 1) << "]\t"
         << p.id << "\t" << p.name;
    if (p.name.length() < 8) cout << "\t\t\t";
    else if (p.name.length() > 16) cout << "\t";
    else cout << "\t\t";
    cout << p.type1;
    if (p.type1.length() >= 8) cout << "\t";
    else cout << "\t\t";
    cout << p.total << "\t" << p.HP << "\t"
         << p.attack << "\t" << p.defense << endl;
  }
  cout << "Number of visited nodes = " << visitedCount << endl << endl;
};

void task3() {
  if (pokemonDatabase.empty() || bst.isEmpty()) {
    cout << endl << "----- Execute Mission 1 first! -----" << endl << endl;
    return;
  }

  // 根據 deleteMinFlag 決定刪除最小或最大節點
  vector<int> deletedIDs;
  if (deleteMinFlag) {
    deletedIDs = bst.deleteMin();
  } else {
    deletedIDs = bst.deleteMax();
  }

  // 切換刪除標記
  deleteMinFlag = !deleteMinFlag;
  cout << "HP tree height = " << bst.getHeight() << endl << endl;
};

void task4() {
  if (bst.isEmpty()) {
    cout << endl
         << "----- Execute Mission 1 first! -----"
         << endl << endl;
    return;
  }

  bst.buildBalancedTree();
  bst.printLevelOrder();
  cout << endl;
}