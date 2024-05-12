#pragma once

#include <iostream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <string>

inline std::string aesEncrypt(const std::string &plaintext,
                              const std::string &passphrase) {
  OpenSSL_add_all_algorithms();

  // Générer une clé à partir de la passphrase
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  if (EVP_BytesToKey(
          EVP_aes_256_cbc(), EVP_sha256(), nullptr,
          reinterpret_cast<const unsigned char *>(passphrase.c_str()),
          passphrase.length(), 1, key, iv) != 32) {
    throw std::runtime_error("Error generating key");
  }

  // Initialiser le contexte de chiffrement
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Error creating cipher context");
  }

  // Initialiser le chiffrement
  if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Error initializing encryption");
  }

  // Chiffrement du texte
  std::string ciphertext;
  int len;
  unsigned char buffer[4096];
  if (EVP_EncryptUpdate(
          ctx, buffer, &len,
          reinterpret_cast<const unsigned char *>(plaintext.c_str()),
          plaintext.length()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Error encrypting data");
  }
  ciphertext.append(reinterpret_cast<char *>(buffer), len);

  // Finaliser le chiffrement
  if (EVP_EncryptFinal_ex(ctx, buffer, &len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Bad passphrase !");
  }
  ciphertext.append(reinterpret_cast<char *>(buffer), len);

  // Libérer le contexte de chiffrement
  EVP_CIPHER_CTX_free(ctx);

  // Retourner le texte chiffré
  return ciphertext;
}

inline std::string aesDecrypt(const std::string &ciphertext,
                              const std::string &passphrase) {
  OpenSSL_add_all_algorithms();

  // Générer une clé à partir de la passphrase
  unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
  if (EVP_BytesToKey(
          EVP_aes_256_cbc(), EVP_sha256(), nullptr,
          reinterpret_cast<const unsigned char *>(passphrase.c_str()),
          passphrase.length(), 1, key, iv) != 32) {
    throw std::runtime_error("Error generating key");
  }

  // Initialiser le contexte de déchiffrement
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    throw std::runtime_error("Error creating cipher context");
  }

  // Initialiser le déchiffrement
  if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Error initializing decryption");
  }

  // Déchiffrement du texte
  std::string plaintext;
  int len;
  unsigned char buffer[4096];
  if (EVP_DecryptUpdate(
          ctx, buffer, &len,
          reinterpret_cast<const unsigned char *>(ciphertext.c_str()),
          ciphertext.length()) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Error decrypting data");
  }
  plaintext.append(reinterpret_cast<char *>(buffer), len);

  // Finaliser le déchiffrement
  if (EVP_DecryptFinal_ex(ctx, buffer, &len) != 1) {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("Error ending decrypting ! ");
  }
  plaintext.append(reinterpret_cast<char *>(buffer), len);

  // Libérer le contexte de déchiffrement
  EVP_CIPHER_CTX_free(ctx);

  // Retourner le texte déchiffré
  return plaintext;
}
