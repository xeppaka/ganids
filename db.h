#ifndef DB_H
#define DB_H

#include <stdlib.h>
#include <sqlite3.h>

class Db
{
public:
    Db();
    ~Db();

    bool open(const char *filename);
    void close();
    bool insert(const char *message, const char *rule, const char *payload, int payload_size);

private:
    sqlite3 *db;
};

#endif // DB_H
