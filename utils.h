#pragma once

#include "crypting.h"
#include "termios.h"
#include "unistd.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

inline optional<vector<string>> searchInVec(vector<string> &urls,
                                            const string &toSearch) {
  vector<string> resultToReturn;

  for (const string &nom : urls) {
    if (nom.find(toSearch) != string::npos) {
      resultToReturn.push_back(nom);
    }
  }
  if (resultToReturn.size() == 0)
    return nullopt;
  else
    return resultToReturn;
}

inline void prettyCout(vector<string> results) {
  int offset = 0;
  int idx = 0;
  string website;
  string email;
  string passwd;

  // search for the biggest offset
  for (string &nom : results) {
    size_t firstPos = nom.find('|');
    website = nom.substr(0, firstPos);

    size_t secondPos = nom.find('|', firstPos + 1);
    email = nom.substr(firstPos + 1, secondPos - firstPos - 1);

    while (website.size() + email.size() + 3 >= offset)
      offset++;
  }

  // for each result we print website and stars to hide passwords
  for (string &nom : results) {
    size_t firstPos = nom.find('|');
    website = nom.substr(0, firstPos);

    size_t secondPos = nom.find('|', firstPos + 1);
    email = nom.substr(firstPos + 1, secondPos - firstPos - 1);

    string separator(offset - (website.size() + email.size() + 3), ' ');

    cout << idx++ << ". " << website << " (" << email << ")" << separator
         << " -> " << " : **********" << endl;
  }
}

inline bool putInClipboard(string &toCopy) {
  FILE *pipe = popen("xclip -selection clipboard", "w");
  if (pipe == NULL) {
    std::cerr << "Failed to open pipe to xclip." << std::endl;
    return 0;
  }

  fprintf(pipe, "%s", toCopy.c_str());
  pclose(pipe);
  return 1;
}

inline void disableEcho() {
  termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

inline void enableEcho() {
  termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

inline string getPassphrase(bool passph = true) {
  string passphrase = "";
  if (passph)
    cout << "Enter your passphrase : ";
  disableEcho();
  getline(cin, passphrase);
  enableEcho();
  cout << endl;
  return passphrase;
}

inline bool checkValidPassphrase(string &passphrase) {
  ifstream file("database.passman");
  string line;

  while (getline(file, line)) {
    string decrypt;
    try {
      decrypt = aesDecrypt(line, passphrase);
    } catch (runtime_error &e) {
      cerr << "Bad passphrase !" << endl;
    }
    if (decrypt != "passwordmanagerfile") {
      return false;
    } else {
      return true;
    }
  }
  return false;
}
