#include "database.h"
#include "utils.h"
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

  if ((argc != 2) && (argc != 3) && (argc != 4)) {
    cout << "Usage : " << endl;
    cout << "./passman 'website' <index to copy> [default is index 0]" << endl;
    cout << "./passman init" << endl;
    cout << "./passman add" << endl;
    cout << "./passman delete 'website' <index to delete> [default is index 0]" << endl;

    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "init") == 0) {
    cout << "Create file" << endl;
    createFile();
  } else if (strcmp(argv[1], "add") == 0) {
    cout << "Add entry to the file" << endl;
    addEntry();
  } else if (strcmp(argv[1], "delete") == 0) {
    cout << "Delete entry to the file" << endl;
    if (argc == 3) {
      deleteEntry(argv[2], 0);
    } else {
      deleteEntry(argv[2], stoi(argv[3]));
    }
  } else {
    cout << "Read file" << endl;
    const string toSearch = toLowerCase(argv[1]);

    vector<string> urls;
    string _ = readFile(urls);

    optional<vector<string>> resultsVec = searchInVec(urls, toSearch);

    if (!resultsVec.has_value()) {
      cerr << "No result !" << endl;
      exit(EXIT_FAILURE);
    }

    int idx = 0;
    if (argc == 2) {
      prettyCout(resultsVec.value());
    } else {
      idx = stoi(argv[2]);
    }

    string toCopy = resultsVec.value().at(idx);

    size_t firstPos = toCopy.find('|');
    size_t secondPos = toCopy.find('|', firstPos + 1);
    toCopy = toCopy.substr(secondPos + 1);

    if (putInClipboard(toCopy)) {
      cout << "Password " << idx << " copied in clipboard !" << endl;
    } else {
      cerr << "Couldn't copy in clipboard..." << endl;
    }
  }
  return 0;
}
