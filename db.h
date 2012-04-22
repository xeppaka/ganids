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
    bool insert(const char *action, const char *message, const char *rule);
    bool is_opened();

private:
    sqlite3 *db;
};

#endif // DB_H
