#ifndef BYTE_SEQUENCE_SIGNATURE_H
#define BYTE_SEQUENCE_SIGNATURE_H

// Attributes
static const char NONCE_EQ[] = { 'n', 'o', 'n', 'c', 'e', '=' };
#define NONCE_EQ_LEN 6

static const char SRC_EQ[] = { 's', 'r', 'c', '=' };
#define SRC_EQ_LEN 4


static const char INTEGRITY_EQ[] = { 'i', 'n', 't', 'e', 'g', 'r', 'i', 't', 'y', '=' };
#define INTEGRITY_EQ_LEN 10

static const char TYPE_EQ[] = { 't', 'y', 'p', 'e', '=' };
#define TYPE_EQ_LEN 5

static const char TYPE_EQ_APPLICATION[] = { 't','y','p','e','=','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n' };
#define TYPE_EQ_APPLICATION_LEN 21

// Tags 
static const char SCRIPT_OPEN[] = { '<', 's', 'c', 'r', 'i', 'p', 't' };
#define SCRIPT_OPEN_LEN 7

// WORDS
static const char MODULE[] = {'m', 'o', 'd','u','l','e'};
#define MODULE_LEN 6

static const char CROSSORIGIN[] = { 'c', 'r', 'o', 's', 's', 'o', 'r', 'i', 'g', 'i', 'n' };
#define CROSSORIGIN_LEN 11

static const char APPLICATION[] = { 'a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n' };
#define APPLICATION_LEN 16

static const char HTTPS[] = {'h','t','t','p', 's', ':', '/', '/'};
#define HTTPS_LEN 8

static const char HTTP[] = {'h','t','t','p', ':', '/', '/'};
#define HTTP_LEN 7

static const char DATA_URI[] = {'d','a','t','a',':'};
#define DATA_URI_LEN 5

static const char PROT_REL[] = {'/','/'};
#define PROT_REL_LEN 2

#endif