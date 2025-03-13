#pragma once

#include "crypting.h"
#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

inline string readFile(vector<string> &urls) {
  ifstream file_pass("database.passman");
  string passphrase = getPassphrase();
  if (!file_pass) {
    cerr << "Bad file ! If you don't have data file use : ./passman init"
         << endl;
    exit(EXIT_FAILURE);
  }

  if (!checkValidPassphrase(passphrase)) {
    cerr << "Bad passphrase ! " << endl;
    exit(EXIT_FAILURE);
  }

  string line;
  int idx = 0;
  while (getline(file_pass, line)) {
    if (idx != 0) {
      urls.push_back(aesDecrypt(line, passphrase));
    }
    idx++;
  }
  return passphrase;
}

inline void createFile() {
  ifstream file_pass("database.passman");
  string passphrase;

  if (file_pass) {
    string choice;
    cout << "Caution, you choose 'init' but a data file was found ! This will "
            "destroy your old file !"
         << endl;
    cout << "Do you want to continue ? (y/N)";
    getline(cin, choice);
    if (choice != "y") {
      exit(EXIT_FAILURE);
    }
  }

  passphrase = getPassphrase();

  fstream cfile_pass;
  cfile_pass.open("database.passman", ios::out);
  cfile_pass << aesEncrypt("passwordmanagerfile", passphrase);
  cfile_pass.close();

  cout << "Creation of your data file completed !";
  exit(EXIT_SUCCESS);
}

inline void addEntry() {
  ofstream file("database.passman", ios::app);
  string passphrase = getPassphrase();
  string website;
  string email;
  string password;

  if (!checkValidPassphrase(passphrase)) {
    exit(EXIT_FAILURE);
  }

  cout << "Website : ";
  getline(cin, website);

  cout << "Email / Pseudo : ";
  getline(cin, email);

  cout << "Password : ";
  password = getPassphrase(false);

  string toWrite = toLowerCase(website) + "|" + email + "|" + password;

  file << "\n" << aesEncrypt(toWrite, passphrase);
}

inline void deleteEntry(const string &entryToDelete, int indexToDelete) {
  vector<string> urls;
  string passphrase = readFile(urls);
  
  optional<vector<string>> resultsVec = searchInVec(urls, toLowerCase(entryToDelete));
  if (!resultsVec.has_value()) {
      cerr << "No entry found for deletion!" << endl;
      exit(EXIT_FAILURE);
  }
  
  vector<string> entries = resultsVec.value();
  if (indexToDelete < 0 || indexToDelete >= entries.size()) {
      cerr << "Invalid index!" << endl;
      exit(EXIT_FAILURE);
  }
  
  string entryToRemove = entries[indexToDelete];
  
  ofstream file("database.passman", ios::out);
  
  if (!file) {
      cerr << "Error opening file!" << endl;
      exit(EXIT_FAILURE);
  }
  
  file << aesEncrypt("passwordmanagerfile", passphrase);
  
  for (const auto &entry : urls) {
      if (entry != entryToRemove) {
          file << "\n" << aesEncrypt(entry, passphrase);
      }
  }
  
  file.close();
  cout << "Entry deleted successfully!" << endl;
}

